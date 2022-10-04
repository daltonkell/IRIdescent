module run_iri
use iso_c_binding
implicit none

contains

    SUBROUTINE do_run_iri(jf,jmag,alati,along,iyyyy,ddd,dhour,heibeg,heiend,heistp,outf,oarr) BIND (C)
        IMPLICIT NONE
    
        INTEGER (C_INT),   BIND(C), DIMENSION(50), INTENT(in)           :: jf
        INTEGER (C_INT),   BIND(C), INTENT(in)                          :: jmag
        REAL    (C_FLOAT), BIND(C), INTENT(in)                          :: alati
        REAL    (C_FLOAT), BIND(C), INTENT(in)                          :: along
        INTEGER (C_INT),   BIND(C), INTENT(in)                          :: iyyyy
        INTEGER (C_INT),   BIND(C), INTENT(in)                          :: ddd
        REAL    (C_FLOAT), BIND(C), INTENT(in)                          :: dhour
        REAL    (C_FLOAT), BIND(C), INTENT(in)                          :: heibeg 
        REAL    (C_FLOAT), BIND(C), INTENT(in)                          :: heiend
        REAL    (C_FLOAT), BIND(C), INTENT(in)                          :: heistp
        REAL    (C_FLOAT), BIND(C), DIMENSION(20, 1000), INTENT(inout)  :: outf
        REAL    (C_FLOAT), BIND(C), DIMENSION(100), INTENT(inout)       :: oarr
    
        ! read in the data into common blocks; this needs to be added according to
        ! irisub.for
        CALL read_ig_rz;
        CALL readapf107;

        ! invoke the IRI_SUB subroutine from here with the input args
        call iri_sub(jf,jmag,alati,along,iyyyy,ddd,dhour,heibeg,heiend,heistp,outf,oarr)
    END SUBROUTINE do_run_iri

end module RUN_IRI
