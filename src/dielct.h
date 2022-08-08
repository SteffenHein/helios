/* [ file: dielct.h ] */
/*******************************************************************************
*                                                                              *
*   Function body dielct(*); HELIOS, release v1.0r1                            *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns physical parameters for dielectric material commonly *
*   used in electric waveguides                                                *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

DIELCT * \
dielct( DIELCT *dpt )
{
   static DIELCT
      *rtp;
/*----------------------------------------------------------------------------*/
/* initialize: */

   rtp = dpt;
                        /* parameters at ZERO Deg Celsius */
   ( rtp->md )  = ZERO; /* mass density [Kg/m^3] */
   ( rtp->epr ) = ZERO; /* relative permittivity */
   ( rtp->tge ) = ZERO; /* electric loss tangent */
   ( rtp->myr ) = ZERO; /* relative permeability */
   ( rtp->tgm ) = ZERO; /* magnetic loss tangent */
   ( rtp->hc )  = ZERO; /* heat conductivity [W/(m*K)] */
   ( rtp->dhc ) = ZERO; /* its temperature coefficient [1/K] */
   ( rtp->cv )  = ZERO; /* heat capacity, cp in [ J/(Kg*K) ] */
   ( rtp->ny )  = ZERO; /* cinematic viscosity [m^2/s] */
   ( rtp->dny ) = ZERO; /* its temperature coefficient [1/K] */
   ( rtp->pr )  = ZERO; /* the Prandtl number [dimensionless] */
   ( rtp->dpr ) = ZERO; /* its temperature coefficient [1/K] */
/*............................................................................*/
/* select... */

   if (( null == strcmp(( dpt->shd ), "PTFE" ))
     ||( null == strcmp(( dpt->shd ), "ptfe" ))
     ||( null == strcmp(( dpt->shd ), "Ptfe" ))
     ||( null == strncmp(( dpt->shd ), "Teflon", 6 ))
     ||( null == strncmp(( dpt->shd ), "TEFLON", 6 ))
     ||( null == strncmp(( dpt->shd ), "teflon", 6 ))
     ||( null == strncmp(( dpt->shd ), "Teflon", 6 ))
     ||( null == strncmp(( dpt->shd ), "POLYTETRAFLOUROETHYLENE", 9 ))
     ||( null == strncmp(( dpt->shd ), "polytetraflouroethylene", 9 ))
     ||( null == strncmp(( dpt->shd ), "Polytetraflouroethylene", 9 )))
   {
      strcpy(( dpt->del ), "teflon" ); 
      strcpy(( dpt->shd ), "PTFE" ); 

# if HLS_PARSET == 1 /* SPINNER Parameters, Teflon PTFE, 9.5 GHz, 27 C */
      ( rtp->md )  = 1.0000e+03;
      ( rtp->epr ) = 2.0800e+00;
      ( rtp->tge ) = 7.0000e-05;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 2.3000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.0100e+03;
# else
      ( rtp->md )  = 1.0000e+03;
      ( rtp->epr ) = 2.1000e+00;
      ( rtp->tge ) = 1.5000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 2.3000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.0100e+03;
# endif
   }
   else if (( null == strcmp(( dpt->shd ), "PU" ))
          ||( null == strcmp(( dpt->shd ), "pu" ))
          ||( null == strcmp(( dpt->shd ), "Pu" ))
          ||( null == strncmp(( dpt->shd ), "POLYURETHANE", 7 ))
          ||( null == strncmp(( dpt->shd ), "polyurethane", 7 ))
          ||( null == strncmp(( dpt->shd ), "Polyurethane", 7 )))
   {
      strcpy(( dpt->del ), "polyurethane" ); 
      strcpy(( dpt->shd ), "PU" ); 

# if HLS_PARSET == 1 /* SPINNER Parameters */
      ( rtp->md )  = 1.0000e+03;
      ( rtp->epr ) = 3.0000e+00;
      ( rtp->tge ) = 1.0000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 1.9000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.5000e+03;
# else
      ( rtp->md )  = 1.0000e+03;
      ( rtp->epr ) = 3.0000e+00;
      ( rtp->tge ) = 1.0000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 1.9000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.5000e+03;
# endif
   }
   else if (( null == strcmp(( dpt->shd ), "PI" ))
          ||( null == strcmp(( dpt->shd ), "pi" ))
          ||( null == strcmp(( dpt->shd ), "Pi" ))
          ||( null == strncmp(( dpt->shd ), "POLYIMIDE", 7 ))
          ||( null == strncmp(( dpt->shd ), "polyimide", 7 ))
          ||( null == strncmp(( dpt->shd ), "Polyimide", 7 )))
   {
      strcpy(( dpt->del ), "polyimide" ); 
      strcpy(( dpt->shd ), "PI" ); 

# if HLS_PARSET == 1 /* SPINNER Parameters, Vespel SP-1(M); 8.7 GHz, 26 C */
      ( rtp->md )  = 1.4000e+03;
      ( rtp->epr ) = 3.3100e+00;
      ( rtp->tge ) = 1.4000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 3.5000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.1300e+03;
# else
      ( rtp->md )  = 1.4000e+03;
      ( rtp->epr ) = 3.5000e+00;
      ( rtp->tge ) = 4.1000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 2.5000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.2000e+03;
# endif
   }
   else if (( null == strcmp(( dpt->shd ), "PE" ))
          ||( null == strcmp(( dpt->shd ), "pe" ))
          ||( null == strcmp(( dpt->shd ), "Pe" ))
          ||( null == strcmp(( dpt->shd ), "PET" ))
          ||( null == strcmp(( dpt->shd ), "pet" ))
          ||( null == strcmp(( dpt->shd ), "Pet" ))
          ||( null == strncmp(( dpt->shd ), "POLYETHYLENE", 7 ))
          ||( null == strncmp(( dpt->shd ), "polyethylene", 7 ))
          ||( null == strncmp(( dpt->shd ), "Polyethylene", 7 )))
   {
      strcpy(( dpt->del ), "polyethylene" ); 
      strcpy(( dpt->shd ), "PE" ); 

# if HLS_PARSET == 1 /* SPINNER Parameters, Lupolen 5031L, 9.3 GHz, 26 C */
      ( rtp->md )  = 1.3200e+03;
      ( rtp->epr ) = 2.3100e+00;
      ( rtp->tge ) = 3.0000e-05;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 3.5000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 2.2000e+03;
# else
      ( rtp->md )  = 1.3200e+03;
      ( rtp->epr ) = 2.5000e+00;
      ( rtp->tge ) = 2.0000e-02;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 2.0000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 2.2000e+03;
# endif
   }
   else if (( null == strcmp(( dpt->shd ), "PVC" ))
          ||( null == strcmp(( dpt->shd ), "pvc" ))
          ||( null == strcmp(( dpt->shd ), "Pvc" ))
          ||( null == strncmp(( dpt->shd ), "POLYVINYLCHLORIDE", 9 ))
          ||( null == strncmp(( dpt->shd ), "polyvinylchloride", 9 ))
          ||( null == strncmp(( dpt->shd ), "Polyvinylchloride", 9 )))
   {
      strcpy(( dpt->del ), "polyvinylchloride" ); 
      strcpy(( dpt->shd ), "PVC" ); 

      ( rtp->md )  = 1.3800e+03;
      ( rtp->epr ) = 3.3000e+00;
      ( rtp->tge ) = 3.6000e-02;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 1.6000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.1e+03;
   }
   else if (( null == strcmp(( dpt->shd ), "ALO95" ))
          ||( null == strcmp(( dpt->shd ), "alo95" ))
          ||( null == strcmp(( dpt->shd ), "Alo95" ))
          ||( null == strcmp(( dpt->shd ), "ALO_95" ))
          ||( null == strcmp(( dpt->shd ), "alo_95" ))
          ||( null == strcmp(( dpt->shd ), "Alo_95" ))
          ||( null == strncmp(( dpt->shd ), "ALO95", 5 ))
          ||( null == strncmp(( dpt->shd ), "alo95", 5 ))
          ||( null == strncmp(( dpt->shd ), "ALO_95", 6 ))
          ||( null == strncmp(( dpt->shd ), "alo_95", 6 ))
          ||( null == strncmp(( dpt->shd ), "ALOXID95", 8 ))
          ||( null == strncmp(( dpt->shd ), "ALOXID95", 8 ))
          ||( null == strncmp(( dpt->shd ), "ALUMINIUMOXIDE95", 16 ))
          ||( null == strncmp(( dpt->shd ), "aluminiumoxide95", 16 ))
          ||( null == strncmp(( dpt->shd ), "Aluminiumoxide95", 16 )))
   {
      strcpy(( dpt->del ), "aluminiumoxid_95%" ); 
      strcpy(( dpt->shd ), "ALO95" ); 

      ( rtp->md )  = 3.7000e+03;
      ( rtp->epr ) = 9.1000e+00;
      ( rtp->tge ) = 2.7000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 2.0000e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 8.8e+02;
   }
   else if (( null == strcmp(( dpt->shd ), "ALO99" ))
          ||( null == strcmp(( dpt->shd ), "alo99" ))
          ||( null == strcmp(( dpt->shd ), "Alo99" ))
          ||( null == strcmp(( dpt->shd ), "ALO_99" ))
          ||( null == strcmp(( dpt->shd ), "alo_99" ))
          ||( null == strcmp(( dpt->shd ), "Alo_99" ))
          ||( null == strncmp(( dpt->shd ), "ALO99", 5 ))
          ||( null == strncmp(( dpt->shd ), "alo99", 5 ))
          ||( null == strncmp(( dpt->shd ), "ALO_99", 6 ))
          ||( null == strncmp(( dpt->shd ), "alo_99", 6 ))
          ||( null == strncmp(( dpt->shd ), "ALOXID99", 8 ))
          ||( null == strncmp(( dpt->shd ), "ALOXID99", 8 ))
          ||( null == strncmp(( dpt->shd ), "ALUMINIUMOXIDE99", 16 ))
          ||( null == strncmp(( dpt->shd ), "aluminiumoxide99", 16 ))
          ||( null == strncmp(( dpt->shd ), "Aluminiumoxide99", 16 )))
   {
      strcpy(( dpt->del ), "aluminiumoxide_99%" );
      strcpy(( dpt->shd ), "ALO99" ); 

      ( rtp->md )  = 3.9600e+03;
      ( rtp->epr ) = 9.9000e+00;
      ( rtp->tge ) = 2.7000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 3.0000e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 8.5000e+02;
   }
   else if (( null == strcmp(( dpt->shd ), "BEO" ))
          ||( null == strcmp(( dpt->shd ), "beO" ))
          ||( null == strcmp(( dpt->shd ), "BeO" ))
          ||( null == strncmp(( dpt->shd ), "BEOXIDE", 7 ))
          ||( null == strncmp(( dpt->shd ), "beoxide", 7 ))
          ||( null == strncmp(( dpt->shd ), "Beoxide", 7 ))
          ||( null == strncmp(( dpt->shd ), "BE_OXIDE", 8 ))
          ||( null == strncmp(( dpt->shd ), "Be_oxide", 8 ))
          ||( null == strncmp(( dpt->shd ), "BERYLLIUMOXIDE", 12 ))
          ||( null == strncmp(( dpt->shd ), "berylliumoxide", 12 ))
          ||( null == strncmp(( dpt->shd ), "Berylliumoxide", 12 )))
   {
      strcpy(( dpt->del ), "berylliumoxide" ); 
      strcpy(( dpt->shd ), "BEO" ); 

      ( rtp->md )  = 2.8500e+03;
      ( rtp->epr ) = 6.5000e+00;
      ( rtp->tge ) = 4.0000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 2.4800e+02;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.0240e+03;
   }
   else if (( null == strcmp(( dpt->shd ), "KER221" ))
          ||( null == strcmp(( dpt->shd ), "ker221" ))
          ||( null == strcmp(( dpt->shd ), "Ker221" ))
          ||( null == strcmp(( dpt->shd ), "Ker_221" ))
          ||( null == strncmp(( dpt->shd ), "ker221", 6 ))
          ||( null == strncmp(( dpt->shd ), "KER221", 6 ))
          ||( null == strncmp(( dpt->shd ), "STEATITE", 7 ))
          ||( null == strncmp(( dpt->shd ), "steatite", 7 ))
          ||( null == strncmp(( dpt->shd ), "Steatite", 7 )))
   {
      strcpy(( dpt->del ), "steatite" );
      strcpy(( dpt->shd ), "KER221" ); 

      ( rtp->md )  = 2.7000e+03;
      ( rtp->epr ) = 6.7000e+00;
      ( rtp->tge ) = 5.0000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 2.6000e+00;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 9.0000e+02;
   }
   else if (( null == strcmp(( rtp->shd ), "AIR" ))
          ||( null == strcmp(( rtp->shd ), "Air" ))
          ||( null == strcmp(( rtp->shd ), "air" ))
          ||( null == strcmp(( rtp->shd ), "gas" ))
          ||( null == strcmp(( rtp->shd ), "GAS" ))
          ||( null == strcmp(( rtp->shd ), "Lu" ))
          ||( null == strcmp(( rtp->shd ), "lu" ))
          ||( null == strcmp(( rtp->shd ), "LU" ))
          ||( null == strcmp(( rtp->shd ), "gas" ))
          ||( null == strcmp(( rtp->shd ), "Gas" ))
          ||( null == strcmp(( rtp->shd ), "GAS" ))
          ||( null == strcmp(( rtp->shd ), "Luft" ))
          ||( null == strcmp(( rtp->shd ), "luft" ))
          ||( null == strcmp(( rtp->shd ), "LUFT" )))
   {
      strcpy(( rtp->del ), "air" );
      strcpy(( rtp->shd ), "AIR" );

/* values from Groeber, Erk, and Grigull: */
/* Die Grundgesetze der Waermeuebertragung*/
/* Springer Verlag, Reprint 1981 */

      ( rtp->md )  = 1.28620e+00;
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->tge ) = 0.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->tgm ) = 0.00000e+00;
      ( rtp->hc )  = 2.43000e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 1.00495e+03; /* cp in [ J/(Kg*K) ] */
      ( rtp->ny )  = 1.33000e-05;
      ( rtp->dny ) = 9.76000e-08; /* i.e. increasing with temperature */
      ( rtp->pr )  = 7.11000e-01;
   }
   else if (( null == strcmp(( rtp->shd ), "N2" ))
          ||( null == strcmp(( rtp->shd ), "n2" ))
          ||( null == strncmp(( rtp->shd ), "nitrogen", 7 ))
          ||( null == strncmp(( rtp->shd ), "Nitrogen", 7 ))
          ||( null == strncmp(( rtp->shd ), "NITROGEN", 7 ))
          ||( null == strncmp(( rtp->shd ), "Stickstoff", 7 ))
          ||( null == strncmp(( rtp->shd ), "stickstoff", 7 ))
          ||( null == strncmp(( rtp->shd ), "STICKSTOFF", 7 )))
   {
      strcpy(( rtp->del ), "nitrogen" ); 
      strcpy(( rtp->shd ), "N2" ); 

      ( rtp->md )  = 1.28620e+00; /* [ values for air ] */
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->tge ) = 0.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->tgm ) = 0.00000e+00;
      ( rtp->hc )  = 2.43000e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 1.00495e+03;
      ( rtp->ny )  = 1.33000e-05;
      ( rtp->dny ) = 9.76000e-08; /* i.e. increasing with temperature */
      ( rtp->pr )  = 7.11000e-01;
   }
   else if (( null == strcmp(( rtp->shd ), "O2" ))
          ||( null == strcmp(( rtp->shd ), "o2" ))
          ||( null == strncmp(( rtp->shd ), "OXYGEN", 6 ))
          ||( null == strncmp(( rtp->shd ), "oxygen", 6 ))
          ||( null == strncmp(( rtp->shd ), "Oxygen", 6 ))
          ||( null == strncmp(( rtp->shd ), "SAUERSTOFF", 7 ))
          ||( null == strncmp(( rtp->shd ), "sauerstoff", 7 ))
          ||( null == strncmp(( rtp->shd ), "Sauerstoff", 7 )))
   {
      strcpy(( rtp->del ), "oxygen" ); 
      strcpy(( rtp->shd ), "O2" ); 

      ( rtp->md )  = 1.42895e+00;
      ( rtp->epr ) = 1.00060e+00;
      ( rtp->tge ) = 0.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->tgm ) = 0.00000e+00;
      ( rtp->hc )  = 2.59490e-02;
      ( rtp->dhc ) = 7.10000e-05;
      ( rtp->cv )  = 9.95180e+02; /* cp in [ J/(Kg*K) ] */
      ( rtp->ny )  = 1.33000e-05;
      ( rtp->dny ) = 9.76000e-08; /* i.e. increasing with temperature */
      ( rtp->pr )  = 7.11000e-01;
   }
   else if (( null == strcmp(( rtp->shd ), "He" ))
          ||( null == strcmp(( rtp->shd ), "he" ))
          ||( null == strncmp(( rtp->shd ), "HELIUM", 6 ))
          ||( null == strncmp(( rtp->shd ), "helium", 6 ))
          ||( null == strncmp(( rtp->shd ), "Helium", 6 )))
   {
      strcpy(( rtp->del ), "helium" ); 
      strcpy(( rtp->shd ), "He" ); 

      ( rtp->md )  = 2.30800e-01;
      ( rtp->epr ) = 1.00000e+00;
      ( rtp->tge ) = 0.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->tgm ) = 0.00000e+00;
      ( rtp->hc )  = 1.43600e-01;
      ( rtp->dhc ) = 7.10000e-05; /* ??? */
      ( rtp->cv )  = 5.22400e+02; /* cp in [ J/(Kg*K) ] */
      ( rtp->ny )  = 7.53200e-05;
      ( rtp->dny ) = 0.00000e+00; /* ??? */
      ( rtp->pr )  = 6.87000e-01; /* ??? */
   }
   else if (( null == strcmp(( rtp->shd ), "SF6" ))
          ||( null == strcmp(( rtp->shd ), "sf6" ))
          ||( null == strcmp(( rtp->shd ), "Sf6" ))
          ||( null == strncmp(( rtp->shd ), "SULFURHEXAFLUORIDE", 9 ))
          ||( null == strncmp(( rtp->shd ), "sulfurhexafluoride", 9 ))
          ||( null == strncmp(( rtp->shd ), "Sulfurhexafluoride", 9 ))
          ||( null == strncmp(( rtp->shd ), "SCHWEFELHEXAFLUORID", 9 ))
          ||( null == strncmp(( rtp->shd ), "schwefelhexafluorid", 9 ))
          ||( null == strncmp(( rtp->shd ), "Schwefelhexafluorid", 9 )))
   {
      strcpy(( rtp->del ), "sulfurhexafluoride" ); 
      strcpy(( rtp->shd ), "SF6" ); 

      ( rtp->md )  = 5.94300e+00;
      ( rtp->epr ) = 1.00000e+00;
      ( rtp->tge ) = 0.00000e+00;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->tgm ) = 0.00000e+00;
      ( rtp->hc )  = 1.20000e-02;
      ( rtp->dhc ) = 7.10000e-05; /* ??? */
      ( rtp->cv )  = 6.70000e+02; /* cp in [ J/(Kg*K) ] */
      ( rtp->ny )  = 2.37200e-06;
      ( rtp->dny ) = 0.00000e+00; /* ??? */
      ( rtp->pr )  = 7.11000e-01; /* ??? */
   }
   else if (( null == strcmp(( rtp->shd ), "H2O" ))
          ||( null == strcmp(( rtp->shd ), "h2o" ))
          ||( null == strncmp(( rtp->shd ), "WATER", 5 ))
          ||( null == strncmp(( rtp->shd ), "water", 5 ))
          ||( null == strncmp(( rtp->shd ), "Water", 5 ))
          ||( null == strncmp(( rtp->shd ), "WASSER", 6 ))
          ||( null == strncmp(( rtp->shd ), "wasser", 6 ))
          ||( null == strncmp(( rtp->shd ), "Wasser", 6 )))
   {
      strcpy(( rtp->del ), "water" );
      strcpy(( rtp->shd ), "H2O" ); /* values from Groeber, Erk, and Grigull: */
                                    /* Die Grundgesetze der Waermeuebertragung*/
                                    /* Springer Verlag, Reprint 1981 */
      ( rtp->md )  =  9.98200e+02;  /* [ values for 0++ DEG Celsius ] */
      ( rtp->epr ) =  7.70000e+01;
      ( rtp->tge ) =  1.00000e-02;
      ( rtp->myr ) =  1.00000e+00;
      ( rtp->tgm ) =  0.00000e+00;
      ( rtp->hc )  =  5.98000e-01;
      ( rtp->dhc ) =  1.30000e-03;
      ( rtp->cv )  =  4.18197e+03;
      ( rtp->ny )  =  1.79200e-06;  /* ny(0) */
   /* ( rtp->dny ) = -2.47600e-08; */ /* ( ny(50)-ny(0) )/50 */
      ( rtp->dny ) = -1.49000e-08;    /* ( ny(100)-ny(0) )/100 */
      ( rtp->pr )  =  7.01000e+00;
      ( rtp->dpr ) = ZERO;
   }
   else
   {
      strcpy(( rtp->del ), "Else" );
      strcpy(( rtp->shd ), "ELSE" ); /* 'E'lse */
# if DEL_DEFLT == 1
      ( rtp->md )  = 2.7000e+03; /* [ parameters of KER221 ] */
      ( rtp->epr ) = 6.7000e+00;
      ( rtp->tge ) = 5.0000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 0.0000e+00;
      ( rtp->hc )  = 2.6000e+00;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 9.0000e+02;
# else
      ( rtp->md )  = ZERO;
      ( rtp->epr ) = ZERO;
      ( rtp->tge ) = ZERO;
      ( rtp->myr ) = ZERO;
      ( rtp->tgm ) = ZERO;
      ( rtp->hc )  = ZERO;
      ( rtp->dhc ) = ZERO;
      ( rtp->cv )  = ZERO;
# endif
   };
   return rtp;
}
/*============================================================================*/
/*********************** end of function dielct(*) ****************************/
