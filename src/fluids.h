/* [ file: fluids.h ] */
/*******************************************************************************
*                                                                              *
*   Function body fluids(*); HELIOS release v1.0r1                             *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns physical parameters for gases and cooling fluids     *
*   used in electric waveguides                                                *
*   [ Parameters at ZERO degree Celsius.]                                      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

FLUIDS * \
fluids( FLUIDS *flp )
{
   static FLUIDS
      *rtp;
/*----------------------------------------------------------------------------*/
/* initialize. */

   rtp = flp;
                        /* parameters at 0 Deg Celsius and 1 Bar abs. pressure*/
   ( rtp->md )  = ZERO; /* mass density [Kg/m^3] */
   ( rtp->ny )  = ZERO; /* cinematic viscosity [m^2/s] */
   ( rtp->dny ) = ZERO; /* its temperature coefficient [1/K] */
   ( rtp->epr ) = ZERO; /* relative permittivity */
   ( rtp->myr ) = ZERO; /* relative permeability */
   ( rtp->hc )  = ZERO; /* heat conductivity [W/(m*K)] */
   ( rtp->dhc ) = ZERO; /* its temperature coefficient [1/K] */
   ( rtp->cv )  = ZERO; /* heat capacity cp in [ J/(Kg*K) ] */
   ( rtp->pr )  = ZERO; /* the Prandtl number */
   ( rtp->dpr ) = ZERO; /* its temperature coefficient [1/K] */

   if (( null == strcmp(( flp->shf ), "AIR" ))
     ||( null == strcmp(( flp->shf ), "air" ))
     ||( null == strcmp(( flp->shf ), "Air" ))
     ||( null == strcmp(( flp->shf ), "LU" ))
     ||( null == strcmp(( flp->shf ), "lu" ))
     ||( null == strcmp(( flp->shf ), "Lu" ))
     ||( null == strcmp(( flp->shf ), "GAS" ))
     ||( null == strcmp(( flp->shf ), "gas" ))
     ||( null == strcmp(( flp->shf ), "Gas" ))
     ||( null == strncmp(( flp->shf ), "LUFT", 4 ))
     ||( null == strncmp(( flp->shf ), "luft", 4 ))
     ||( null == strncmp(( flp->shf ), "Luft", 4 )))
   {
      strcpy(( flp->fld ), "air" );
      strcpy(( flp->shf ), "AIR" );

/* values from Groeber, Erk, and Grigull: */
/* Die Grundgesetze der Waermeuebertragung */
/* Springer Verlag, Reprint 1981 */

      ( rtp->md )  = 1.28620e+00;
      ( rtp->ny )  = 1.33000e-05;
      ( rtp->dny ) = 9.76000e-08; /* i.e. increasing with temperature */
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 2.43000e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 1.00495e+03; /* cp in [ J/(Kg*K) ] */
      ( rtp->pr )  = 7.11000e-01;
      ( rtp->dpr ) = ZERO;
   }
   else if (( null == strcmp(( flp->shf ), "N2" ))
          ||( null == strcmp(( flp->shf ), "n2" ))
          ||( null == strncmp(( flp->shf ), "nitrogen", 7 ))
          ||( null == strncmp(( flp->shf ), "Nitrogen", 7 ))
          ||( null == strncmp(( flp->shf ), "Stickstoff", 7 ))
          ||( null == strncmp(( flp->shf ), "STICKSTOFF", 7 )))
   {
      strcpy(( flp->fld ), "nitrogen" ); 
      strcpy(( flp->shf ), "N2" ); 

      ( rtp->md )  = 1.28620e+00; /* [ values for air ] */
      ( rtp->ny )  = 1.33000e-05;
      ( rtp->dny ) = 9.76000e-08;
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 2.43000e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 1.00495e+03;
      ( rtp->pr )  = 7.11000e-01;
      ( rtp->dpr ) = ZERO;
   }
   else if (( null == strcmp(( flp->shf ), "O2" ))
          ||( null == strcmp(( flp->shf ), "o2" ))
          ||( null == strncmp(( flp->shf ), "oxygen", 6 ))
          ||( null == strncmp(( flp->shf ), "Oxygen", 6 ))
          ||( null == strncmp(( flp->shf ), "OXYGEN", 6 ))
          ||( null == strncmp(( flp->shf ), "Sauerstoff", 7 ))
          ||( null == strncmp(( flp->shf ), "sauerstoff", 7 ))
          ||( null == strncmp(( flp->shf ), "SAUERSTOFF", 7 )))
   {
      strcpy(( flp->fld ), "oxygen" ); 
      strcpy(( flp->shf ), "O2" ); 

      ( rtp->md )  = 1.42895e+00;
      ( rtp->ny )  = 1.34364e-05;
      ( rtp->dny ) = 9.23820e-08;
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 2.59490e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 9.95180e+02; /* cp in [ J/(Kg*K) ] */
      ( rtp->pr )  = 7.11000e-01;
      ( rtp->dpr ) = ZERO;
   }
   else if (( null == strcmp(( rtp->shf ), "He" ))
          ||( null == strcmp(( rtp->shf ), "he" ))
          ||( null == strncmp(( rtp->shf ), "HELIUM", 6 ))
          ||( null == strncmp(( rtp->shf ), "helium", 6 ))
          ||( null == strncmp(( rtp->shf ), "Helium", 6 )))
   {
      strcpy(( rtp->fld ), "helium" ); 
      strcpy(( rtp->shf ), "He" ); 

      ( rtp->md )  = 2.30800e-01;
      ( rtp->epr ) = 1.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 1.43600e-01;
      ( rtp->dhc ) = 7.10000e-05; /* ??? */
      ( rtp->cv )  = 5.22400e+02; /* cp in [ J/(Kg*K) ] */
      ( rtp->ny )  = 7.53200e-05;
      ( rtp->dny ) = 0.00000e+00; /* ??? */
      ( rtp->pr )  = 6.87000e-01; /* ??? */
   }
   else if (( null == strcmp(( rtp->shf ), "SF6" ))
          ||( null == strcmp(( rtp->shf ), "sf6" ))
          ||( null == strcmp(( rtp->shf ), "Sf6" ))
          ||( null == strncmp(( rtp->shf ), "SULFURHEXAFLUORIDE", 9 ))
          ||( null == strncmp(( rtp->shf ), "sulfurhexafluoride", 9 ))
          ||( null == strncmp(( rtp->shf ), "Sulfurhexafluoride", 9 ))
          ||( null == strncmp(( rtp->shf ), "SCHWEFELHEXAFLUORID", 9 ))
          ||( null == strncmp(( rtp->shf ), "schwefelhexafluorid", 9 ))
          ||( null == strncmp(( rtp->shf ), "Schwefelhexafluorid", 9 )))
   {
      strcpy(( rtp->fld ), "sulfurhexafluoride" ); 
      strcpy(( rtp->shf ), "SF6" ); 

      ( rtp->md )  = 5.94300e+00;
      ( rtp->epr ) = 1.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 1.20000e-02;
      ( rtp->dhc ) = 7.10000e-05; /* ??? */
      ( rtp->cv )  = 6.70000e+02; /* cp in [ J/(Kg*K) ] */
      ( rtp->ny )  = 2.37200e-06;
      ( rtp->dny ) = 0.00000e+00; /* ??? */
      ( rtp->pr )  = 7.11000e-01; /* ??? */
   }
   else if (( null == strcmp(( flp->shf ), "H2O" ))
          ||( null == strcmp(( flp->shf ), "h2o" ))
          ||( null == strncmp(( flp->shf ), "water", 5 ))
          ||( null == strncmp(( flp->shf ), "Water", 5 ))
          ||( null == strncmp(( flp->shf ), "WATER", 5 ))
          ||( null == strncmp(( flp->shf ), "Wasser", 6 ))
          ||( null == strncmp(( flp->shf ), "wasser", 6 ))
          ||( null == strncmp(( flp->shf ), "WASSER", 6 )))
   {
      strcpy(( flp->fld ), "water" );
      strcpy(( flp->shf ), "H2O" ); /* values from Groeber, Erk, and Grigull: */
                                    /* Die Grundgesetze der Waermeuebertragung*/
                                    /* Springer Verlag, Reprint 1981 */
      ( rtp->md )  =  9.98200e+02;  /* [ values for 0++ DEG Celsius ] */
      ( rtp->ny )  =  1.79200e-06;  /* ny(0) */
   /* ( rtp->dny ) = -2.47600e-08; */ /* ( ny(50)-ny(0) )/50 */
      ( rtp->dny ) = -1.49000e-08;    /* ( ny(100)-ny(0) )/100 */
      ( rtp->epr ) =  7.70000e+01;
      ( rtp->myr ) =  1.00000e+00;
      ( rtp->hc )  =  5.98000e-01;
      ( rtp->dhc ) =  1.30000e-03;
      ( rtp->cv )  =  4.18197e+03;
      ( rtp->pr )  =  7.01000e+00;
      ( rtp->dpr ) = ZERO;
   }
   else if (( null == strcmp(( flp->shf ), "else" ))
          ||( null == strcmp(( flp->shf ), "Else" ))
          ||( null == strcmp(( flp->shf ), "ELSE" )))
   {
      strcpy(( rtp->fld ), "else" );
      strcpy(( rtp->shf ), "ELSE" );

# if DEL_DEFLT == 1
      ( rtp->md )  =  9.98200e+02;  /* [ parameters for H2O ] */
      ( rtp->ny )  =  1.79200e-06; 
   /* ( rtp->dny ) = -2.47600e-08; */ /* ( ny(50)-ny(0) )/50 */
      ( rtp->dny ) = -1.49000e-08;    /* ( ny(100)-ny(0) )/100 */
      ( rtp->epr ) =  7.70000e+01;
      ( rtp->myr ) =  1.00000e+00;
      ( rtp->hc )  =  5.98000e-01;
      ( rtp->dhc ) =  1.30000e-03;
      ( rtp->cv )  =  4.18197e+06;
      ( rtp->pr )  =  7.01000e+00;
      ( rtp->dpr ) = ZERO;
# else
      ( rtp->md )  = ZERO; 
      ( rtp->ny )  = ZERO; 
      ( rtp->dny ) = ZERO;
      ( rtp->epr ) = ZERO;
      ( rtp->myr ) = ZERO;
      ( rtp->hc )  = ZERO;
      ( rtp->dhc ) = ZERO;
      ( rtp->cv )  = ZERO;
      ( rtp->pr )  = ZERO;
      ( rtp->dpr ) = ZERO;
# endif
   }
   else if (( null == strcmp(( flp->shf ), "void" ))
          ||( null == strcmp(( flp->shf ), "VOID" ))
          ||( null == strcmp(( flp->shf ), "Void" )))
   {
      strcpy(( rtp->fld ), "void" );
      strcpy(( rtp->shf ), "VOID" );

# if DEL_DEFLT == 1
      ( rtp->md )  =  9.98200e+02;  /* [ parameters for H2O ] */
      ( rtp->ny )  =  1.79200e-06; 
   /* ( rtp->dny ) = -2.47600e-08; */ /* ( ny(50)-ny(0) )/50 */
      ( rtp->dny ) = -1.49000e-08;    /* ( ny(100)-ny(0) )/100 */
      ( rtp->epr ) =  7.70000e+01;
      ( rtp->myr ) =  1.00000e+00;
      ( rtp->hc )  =  5.98000e-01;
      ( rtp->dhc ) =  1.30000e-03;
      ( rtp->cv )  =  4.18197e+06;
      ( rtp->pr )  =  7.01000e+00;
      ( rtp->dpr ) = ZERO;
# else
      ( rtp->md )  = ZERO; 
      ( rtp->ny )  = ZERO; 
      ( rtp->dny ) = ZERO;
      ( rtp->epr ) = ZERO;
      ( rtp->myr ) = ZERO;
      ( rtp->hc )  = ZERO;
      ( rtp->dhc ) = ZERO;
      ( rtp->cv )  = ZERO;
      ( rtp->pr )  = ZERO;
      ( rtp->dpr ) = ZERO;
# endif
   }
   else
   {
      strcpy(( flp->fld ), "air" ); 
      strcpy(( flp->shf ), "AIR" ); 

      ( rtp->md )  = 1.28620e+00; /* [ values for air ] */
      ( rtp->ny )  = 1.33000e-05;
      ( rtp->dny ) = 9.76000e-08;
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 2.43000e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 1.00495e+03;
      ( rtp->pr )  = 7.11000e-01;
      ( rtp->dpr ) = ZERO;
   };

   return rtp;
}
/*============================================================================*/
/*********************** end of function fluids(*) ****************************/
