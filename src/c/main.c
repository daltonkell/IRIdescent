#include <stdio.h>   /* printf, fprintf */

/**
* This program runs the IRI model using the iri_sub subroutine
* through a function call to the do_run_iri subroutine using
* the ISO_C_BINDING.
*/

/*
* declaration of do_run_iri function, defined in the
* run_iri.f08 file with ISO_C_BINDNIG
*/
void do_run_iri(int *jf, int *jmag, float *alati, float *along, int *iyyyy, int *_ddd, float *dhour, float *heibeg, float *heiend, float*heistp, float *outf, float *oarr);

int main(int argc, char *argv){

    /* declare pointer to OUTF data file, written at the end */
    FILE *outf_data_fp;

    /*
    * create boolean array for the "jf" parameter as to run the STANDARD
    * version of the model, as described in irisub.for;
    * FORTRAN LOGICAL type is translated as C int, according to gfortran manual
    */
    int jf[50] = {
        1,     /* 1    Ne computed            Ne not computed                     t*/
        1,     /* 2    Te, Ti computed        Te, Ti not computed                 t*/
        1,     /* 3    Ne & Ni computed       Ni not computed                     t*/
        0,     /* 4    B0,B1 - Bil-2000       B0,B1 - other models jf(31)         f*/
        0,     /* 5    foF2 - CCIR            foF2 - URSI                         f*/
        0,     /* 6    Ni - DS-1995 & DY-1985 Ni - RBV-2010 & TBT-2015            f*/
        1,     /* 7    Ne - Tops: f10.7<188   f10.7 unlimited                     t*/
        1,     /* 8    foF2 from model        foF2 or NmF2 - user input           t*/
        1,     /* 9    hmF2 from model        hmF2 or M3000F2 - user input        t*/
        1,     /*10    Te - Standard          Te - Using Te/Ne correlation        t*/
        1,     /*11    Ne - Standard Profile  Ne - Lay-function formalism         t*/
        1,     /*12    Messages to unit 6     to messages.txt on unit 11          t*/
        1,     /*13    foF1 from model        foF1 or NmF1 - user input           t*/
        1,     /*14    hmF1 from model        hmF1 - user input (only Lay version)t*/
        1,     /*15    foE  from model        foE or NmE - user input             t*/
        1,     /*16    hmE  from model        hmE - user input                    t*/
        1,     /*17    Rz12 from file         Rz12 - user input                   t*/
        1,     /*18    IGRF dip, magbr, modip old FIELDG using POGO68/10 for 1973 t*/
        1,     /*19    F1 probability model   only if foF1>0 and not NIGHT        t*/
        1,     /*20    standard F1            standard F1 plus L condition        t*/
        0,     /*21    ion drift computed     ion drift not computed              f*/
        1,     /*22    ion densities in %     ion densities in m-3                t*/
        0,     /*23    Te_tops (Bil-1985)     Te_topside (TBT-2012)               f*/
        1,     /*24    D-region: IRI-1990     FT-2001 and DRS-1995                t*/
        1,     /*25    F107D from APF107.DAT  F107D user input (oarr(41))         t*/  
        1,     /*26    foF2 storm model       no storm updating                   t*/
        1,     /*27    IG12 from file         IG12 - user                         t*/
        0,     /*28    spread-F probability 	 not computed                     f*/
        0,     /*29    IRI01-topside          new options as def. by JF(30)       f*/
        0,     /*30    IRI01-topside corr.    NeQuick topside model   	          f*/
        1,     /*31    B0,B1 ABT-2009	     B0 Gulyaeva-1987 h0.5                t*/
        1,     /*32    F10.7_81 from file     F10.7_81 - user input (oarr(46))    t*/
        0,     /*33    Auroral boundary model on/off  1/0	                  f*/
        1,     /*34    Messages on            Messages off                        t*/
        0,     /*35    foE storm model        no foE storm updating               f*/
        1,     /*36    hmF2 w/out foF2_storm  with foF2-storm                     t*/
        1,     /*37    topside w/out foF2-storm  with foF2-storm                  t*/
        1,     /*38    turn WRITEs off in IRIFLIP   turn WRITEs on                t*/
        0,     /*39    hmF2 (M3000F2)         new models                          f*/
        1,     /*40    hmF2 AMTB-model        Shubin-COSMIC model                 t*/
        1,     /*41    Use COV=F10.7_365      COV=f(IG12) (IRI before Oct 2015)   t*/
        1,     /*42    Te with PF10.7 dep.	 w/o PF10.7 dependance            t*/
        1,     /*43    B0 from model          B0 user input in OARR(10)           t*/
        1,     /*44    B1 from model          B1 user input in OARR(35)           t*/
        1,     /*45    HNEA=65/80km dya/night HNEA user input in OARR(89)         t*/
        1,     /*46    HNEE=2000km 	         HNEE user input in OARR(90)      t*/
        0,     /*47    CGM computation on 	 CGM computation off              f*/
        1,     /*  last three are 1 and not used */
        1,
        1
    };


    /*
    * variable JMAG, 0 indicates geographic coordinate system
    */
    int jmag = 0;

    /*
    * variables ALATI and ALONG are of REAL type; correspond to float
    * describe geographic coordinates in degrees (lat/lon, respesctively)
    * For the time being, these are hard-coded. It is trivial to get them
    * as command-line parameters.
    */
    float alati = 37.8;
    float along = 75.4;

    /*
    * datetime variables IYYYY (int, year), MMDD (date or day of year), DHOUR (hour of day)
    */
    int iyyyy    = 2021;
    int _ddd     = -62;  /* MMDD or -DDD in case of day of year, this is more convenient in C */
    float dhour  = 11.0; /* DHOUR indicates double, plus I saw IF(DHOUR.le.24.0) */

    /*
    * height variables, seem to be double
    * HEIBEG (start height), HEIEND (end height), HEISTP (step size)
    */
    float heibeg = 100.0;
    float heiend = 1000.0;
    float heistp = 100.0;

    /*
    * variable OUTF has dimension (20, 1000) as (cols, rows)
    * variable OARR has dimension (100)         (length);
    * within FORTRAN, initialized to -1.0
    */
    float outf[20*1000];
    float oarr[100]; 


    /* pass variables to ISO_C_BINDING FORTRAN routine created
    * FORTRAN passes by reference, so hand off pointers to the subroutine.
    */
    do_run_iri(jf,&jmag,&alati,&along,&iyyyy,&_ddd,&dhour,&heibeg,&heiend,&heistp,outf,oarr);

    /*
    * OUTF and OARR are filled, however, **contents are transposed** due to
    * column-major order in FORTRAN.
    * write OUTF contents of interest to data file
    *
    * NOTE this output isn't correct, so there's not much
    * use in printing it all out!
    */
    outf_data_fp = fopen("outf_data.txt", "w");
    for (int i=0; i<10; ++i){        /* column */
        for (int j=0; j<20; ++j) {   /* row */
            fprintf(outf_data_fp, "%-.3f    ", outf[i*1000 + j]);
        }
        fprintf(outf_data_fp, "\n");
    }

    /* always close open file handles */
    fclose(outf_data_fp);

    return 0;

}
