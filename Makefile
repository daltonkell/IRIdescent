# Makefile for the IRI-2016 model and corresponding C driver

# References: https://www.gnu.org/software/make/manual/make.html
#             https://makefiletutorial.com/#makefile-cookbook

FORTC     =/usr/bin/gfortran-8
FORTOPTS  =-g
CC        =/usr/bin/gcc-8
SRC_FORT  =./src/fort
SRC_C     =./src/c

# declare FORTRAN objects (since so few, explicit)
fort_objs=iritest.o irisub.o irifun.o iritec.o iridreg.o igrf.o cira.o iriflip.o
c_objs   =main.o

#
# Final targets
#
iri-exe: $(fort_objs)
	$(FORTC) $(FORTOPTS) -o $@ $(fort_objs)
libiri.so: $(fort_objs)
	$(FORTC) -shared -o $@ run_iri.o $(fort_objs)
c-iri: $(fort_objs) run_iri.o libiri.so $(c_objs)
	$(FORTC) $(FORTOPTS) -o $@ $(c_objs) -L./ -lgfortran -liri

#
# IRI FORTRAN build rules
#
# rule to compile the IRI shared objects for each module
# no fPIC results in errors, see err_compile_fPIC.txt
#
fort_o: $(fort_objs) run_iri.o
$(fort_objs): %.o : $(SRC_FORT)/%.for $(SRC_FORT)/run_iri.f08
	$(FORTC) $(FORTOPTS) $(FORTOPTS) -fPIC -c $< -o $@

#
# ISO_C_BINDING FORTRAN build
#
run_iri.o: $(SRC_FORT)/run_iri.f08
	$(FORTC) $(FORTOPTS) -c $<

#
# C build rules
#
$(c_objs): %.o : $(SRC_C)/%.c
	$(FORTC) $(FORTOPTS) -fPIC -c $< -o $@

clean:
	rm -rf *.o c-iri iri-exe

clean-c-iri:
	rm -rf main.o c-iri


