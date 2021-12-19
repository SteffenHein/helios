/* [ file: solids.h ] */
/*******************************************************************************
*                                                                              *
*   Function body solids(*); HELIOS release v1.0r1                             *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns physical parameters for solid materials used in      *
*   electric waveguides.                                                       *
*   [ Parameters at ZERO degree Celsius.]                                      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef HLS_PARSET
   # define HLS_PARSET 1
# endif
/*============================================================================*/

SOLIDS * \
solids( SOLIDS *spt )
{
   static SOLIDS
      *rtp;
/*----------------------------------------------------------------------------*/
/* initialize: */

   rtp = spt;
                        /* parameters at ZERO Deg Celsius */
   ( rtp->md )  = ZERO; /* mass density [Kg/m^3] */
   ( rtp->er )  = ZERO; /* electric resistivity [Ohm*m] */
   ( rtp->der ) = ZERO; /* temperature coefficient of the former */
   ( rtp->epr ) = ZERO; /* relative permittivity */
   ( rtp->tge ) = ZERO; /* electric loss tangent */
   ( rtp->myr ) = ZERO; /* relative permeability */
   ( rtp->tgm ) = ZERO; /* magnetic loss tangent */
   ( rtp->hc )  = ZERO; /* heat conductivity [W/(m*K)] */
   ( rtp->dhc ) = ZERO; /* its temperature coefficient [1/K] */
   ( rtp->cv )  = ZERO; /* heat capacity, cp in [ J/(Kg*K) ] */
/*............................................................................*/
/* select... */

   if (( null == strcmp(( spt->shs ), "Cu" ))
     ||( null == strcmp(( spt->shs ), "CU" ))
     ||( null == strcmp(( spt->shs ), "cu" ))
     ||( null == strncmp(( spt->shs ), "copper", 3 ))
     ||( null == strncmp(( spt->shs ), "COPPER", 3 ))
     ||( null == strncmp(( spt->shs ), "Copper", 3 ))
     ||( null == strncmp(( spt->shs ), "Kupfer", 3 ))
     ||( null == strncmp(( spt->shs ), "KUPFER", 3 )))
   {
      strcpy(( rtp->sld ), "copper" );
      strcpy(( rtp->shs ), "Cu" );

      ( rtp->md )  = 8.9300e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 3.9300e+02; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.422 */
      ( rtp->cv )  = 3.9295e+02;
      ( rtp->er )  = 1.5500e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.3300e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "Al" ))
          ||( null == strcmp(( spt->shs ), "AL" ))
          ||( null == strcmp(( spt->shs ), "al" ))
          ||( null == strncmp(( spt->shs ), "aluminium", 3 ))
          ||( null == strncmp(( spt->shs ), "ALUMINIUM", 3 ))
          ||( null == strncmp(( spt->shs ), "Aluminium", 3 )))
   {
      strcpy(( rtp->sld ), "aluminium" );
      strcpy(( rtp->shs ), "Al" );

      ( rtp->md )  = 2.7000e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 2.4000e+02; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.421 */
      ( rtp->cv )  = 9.1903e+02;

# if HLS_PARSET == 1 /* secure value for some aluminium alloys */
      ( rtp->er )  = 3.9950e-08;
      ( rtp->der ) = 4.0800e-03;
# else
      ( rtp->er )  = 2.5000e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.6000e-03; /* Handbook of Physical Quantities, p.550 */
# endif
   }
   else if (( null == strcmp(( spt->shs ), "Zn" ))
          ||( null == strcmp(( spt->shs ), "ZN" ))
          ||( null == strcmp(( spt->shs ), "zn" ))
          ||( null == strncmp(( spt->shs ), "zinc", 3 ))
          ||( null == strncmp(( spt->shs ), "ZINC", 3 ))
          ||( null == strncmp(( spt->shs ), "Zinc", 3 ))
          ||( null == strncmp(( spt->shs ), "Zink", 3 ))
          ||( null == strncmp(( spt->shs ), "ZINK", 3 )))
   {
      strcpy(( rtp->sld ), "zinc" );
      strcpy(( rtp->shs ), "Zn" );

      ( rtp->md )  = 7.13300e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 1.1100e+02; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.426 */
      ( rtp->cv )  = 3.9510e+02;
      ( rtp->er )  = 5.6500e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.1700e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "Au" ))
          ||( null == strcmp(( spt->shs ), "AU" ))
          ||( null == strcmp(( spt->shs ), "au" ))
          ||( null == strncmp(( spt->shs ), "gold", 3 ))
          ||( null == strncmp(( spt->shs ), "GOLD", 3 ))
          ||( null == strncmp(( spt->shs ), "Gold", 3 )))
   {
      strcpy(( rtp->sld ), "gold" );
      strcpy(( rtp->shs ), "Au" );

      ( rtp->md )  = 1.9320e+04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 3.1100e+02; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.422 */
      ( rtp->cv )  = 1.3270e+02;
      ( rtp->er )  = 2.0600e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.0200e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "Ag" ))
          ||( null == strcmp(( spt->shs ), "AG" ))
          ||( null == strcmp(( spt->shs ), "ag" ))
          ||( null == strncmp(( spt->shs ), "silver", 3 ))
          ||( null == strncmp(( spt->shs ), "SILVER", 3 ))
          ||( null == strncmp(( spt->shs ), "Silver", 3 ))
          ||( null == strncmp(( spt->shs ), "Silber", 3 ))
          ||( null == strncmp(( spt->shs ), "SILBER", 3 )))
   {
      strcpy(( rtp->sld ), "silver" );
      strcpy(( rtp->shs ), "Ag" );

      ( rtp->md )  = 1.0497e+04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 4.2500e+02; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.425 */
      ( rtp->cv )  = 2.4006e+02;
      ( rtp->er )  = 1.4900e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.2800e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "Pt" ))
          ||( null == strcmp(( spt->shs ), "PT" ))
          ||( null == strcmp(( spt->shs ), "pt" ))
          ||( null == strncmp(( spt->shs ), "platinum", 3 ))
          ||( null == strncmp(( spt->shs ), "PLATINUM", 3 ))
          ||( null == strncmp(( spt->shs ), "Platinum", 3 ))
          ||( null == strncmp(( spt->shs ), "Platin", 3 ))
          ||( null == strncmp(( spt->shs ), "PLATIN", 3 )))
   {
      strcpy(( rtp->sld ), "platinum" );
      strcpy(( rtp->shs ), "Pt" );

      ( rtp->md )  = 2.1450e+04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 7.2000e+01; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.424 */
      ( rtp->cv )  = 1.3657e+02;
      ( rtp->er )  = 9.8100e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 3.9600e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "Ti" ))
          ||( null == strcmp(( spt->shs ), "TI" ))
          ||( null == strcmp(( spt->shs ), "ti" ))
          ||( null == strncmp(( spt->shs ), "titanium", 3 ))
          ||( null == strncmp(( spt->shs ), "TITANIUM", 3 ))
          ||( null == strncmp(( spt->shs ), "Titanium", 3 ))
          ||( null == strncmp(( spt->shs ), "Titan", 3 ))
          ||( null == strncmp(( spt->shs ), "TITAN", 3 )))
   {
      strcpy(( rtp->sld ), "titanium" );
      strcpy(( rtp->shs ), "Ti" );

      ( rtp->md )  = 4.5050e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 2.0000e+01; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.426 */
      ( rtp->cv )  = 6.2700e+02;
      ( rtp->er )  = 4.2000e-07; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 5.4600e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "Ni" ))
          ||( null == strcmp(( spt->shs ), "NI" ))
          ||( null == strcmp(( spt->shs ), "ni" ))
          ||( null == strncmp(( spt->shs ), "nickel", 3 ))
          ||( null == strncmp(( spt->shs ), "NICKEL", 3 ))
          ||( null == strncmp(( spt->shs ), "Nickel", 3 )))
   {
      strcpy(( rtp->sld ), "nickel" );
      strcpy(( rtp->shs ), "Ni" );

      ( rtp->md )  = 8.9100e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 8.0000e+01; /* [ 400K ] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.423 */
      ( rtp->cv )  = 4.5737e+02;
      ( rtp->er )  = 6.1400e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 6.9200e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( spt->shs ), "inv" ))
          ||( null == strcmp(( spt->shs ), "INV" ))
          ||( null == strcmp(( spt->shs ), "Inv" ))
          ||( null == strcmp(( spt->shs ), "iv" ))
          ||( null == strcmp(( spt->shs ), "IV" ))
          ||( null == strcmp(( spt->shs ), "Iv" ))
          ||( null == strcmp(( spt->shs ), "in" ))
          ||( null == strcmp(( spt->shs ), "IN" ))
          ||( null == strcmp(( spt->shs ), "In" ))
          ||( null == strncmp(( spt->shs ), "invar", 3 ))
          ||( null == strncmp(( spt->shs ), "INVAR", 3 ))
          ||( null == strncmp(( spt->shs ), "Invar", 3 )))
   {
      strcpy(( rtp->sld ), "invar" );
      strcpy(( rtp->shs ), "Inv" );

      ( rtp->md )  = 8.0500e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 1.0153e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 5.1500e+02;
      ( rtp->er )  = 8.1960e-07;
      ( rtp->der ) = 6.7000e-03;
   }
   else if ( ( null == strcmp(( spt->shs ), "n42" ))
	     ||( null == strcmp(( spt->shs ), "N42" )) )
   {
      strcpy(( rtp->sld ), "N24" );
      strcpy(( rtp->shs ), "N24" );

      ( rtp->md )  = 8.1200e+03;
      ( rtp->myr ) = 1.0000e+00; /* TODO: n.a., but contains iron !!! */
      ( rtp->hc )  = 1.4600e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 5.0200e+02;
      ( rtp->er )  = 6.3000e-07;
      ( rtp->der ) = 0.0000e+00;
   }
   else if (( null == strcmp(( spt->shs ), "DI" ))
          ||( null == strcmp(( spt->shs ), "di" ))
          ||( null == strcmp(( spt->shs ), "Di" ))
          ||( null == strncmp(( spt->shs ), "dilver", 3 ))
          ||( null == strncmp(( spt->shs ), "DILVER", 3 ))
          ||( null == strncmp(( spt->shs ), "Dilver", 3 )))
   {
      strcpy(( rtp->sld ), "dilver_p" );
      strcpy(( rtp->shs ), "Di" );

      ( rtp->md )  = 8.1500e+03;
      ( rtp->myr ) = 1.0000e+00; /* TODO: n.a., but contains iron !!! */
      ( rtp->hc )  = 1.7600e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 5.0200e+02; 
      ( rtp->er )  = 4.9000e-07;
      ( rtp->der ) = 0.0000e+00;
   }
   else if (( null == strcmp(( spt->shs ), "Me" ))
          ||( null == strcmp(( spt->shs ), "ME" ))
          ||( null == strcmp(( spt->shs ), "me" ))
          ||( null == strcmp(( spt->shs ), "Ms" ))
          ||( null == strcmp(( spt->shs ), "MS" ))
          ||( null == strcmp(( spt->shs ), "ms" ))
          ||( null == strcmp(( spt->shs ), "Br" ))
          ||( null == strcmp(( spt->shs ), "BR" ))
          ||( null == strcmp(( spt->shs ), "br" ))
          ||( null == strncmp(( spt->shs ), "Messing", 5 ))
          ||( null == strncmp(( spt->shs ), "messing", 5 ))
          ||( null == strncmp(( spt->shs ), "MESSING", 5 ))
          ||( null == strncmp(( spt->shs ), "Brass", 5 ))
          ||( null == strncmp(( spt->shs ), "brass", 5 ))
          ||( null == strncmp(( spt->shs ), "BRASS", 5 )))
   {
      strcpy(( rtp->sld ), "brass" );
      strcpy(( rtp->shs ), "BR" );

      ( rtp->md )  = 6.7000e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 1.1000e+02;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 4.0000e+02;
      ( rtp->er )  = 6.3586e-08;
      ( rtp->der ) = 3.9000e-03;
   }
   else if (( null == strcmp(( spt->shs ), "PTFE" ))
     ||( null == strcmp(( spt->shs ), "ptfe" ))
     ||( null == strcmp(( spt->shs ), "Ptfe" ))
     ||( null == strncmp(( spt->shs ), "Teflon", 6 ))
     ||( null == strncmp(( spt->shs ), "TEFLON", 6 ))
     ||( null == strncmp(( spt->shs ), "teflon", 6 ))
     ||( null == strncmp(( spt->shs ), "Teflon", 6 ))
     ||( null == strncmp(( spt->shs ), "POLYTETRAFLOUROETHYLENE", 12 ))
     ||( null == strncmp(( spt->shs ), "polytetraflouroethylene", 12 ))
     ||( null == strncmp(( spt->shs ), "Polytetraflouroethylene", 12 )))
   {
      strcpy(( spt->sld ), "teflon" ); 
      strcpy(( spt->shs ), "PTFE" ); 

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
   else if (( null == strcmp(( spt->shs ), "PU" ))
          ||( null == strcmp(( spt->shs ), "pu" ))
          ||( null == strcmp(( spt->shs ), "Pu" ))
          ||( null == strncmp(( spt->shs ), "POLYURETHANE", 7 ))
          ||( null == strncmp(( spt->shs ), "polyurethane", 7 ))
          ||( null == strncmp(( spt->shs ), "Polyurethane", 7 )))
   {
      strcpy(( spt->sld ), "polyurethane" ); 
      strcpy(( spt->shs ), "PU" ); 

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
   else if (( null == strcmp(( spt->shs ), "PI" ))
          ||( null == strcmp(( spt->shs ), "pi" ))
          ||( null == strcmp(( spt->shs ), "Pi" ))
          ||( null == strncmp(( spt->shs ), "POLYIMIDE", 7 ))
          ||( null == strncmp(( spt->shs ), "polyimide", 7 ))
          ||( null == strncmp(( spt->shs ), "Polyimide", 7 )))
   {
      strcpy(( spt->sld ), "polyimide" ); 
      strcpy(( spt->shs ), "PI" ); 

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
   else if (( null == strcmp(( spt->shs ), "PE" ))
          ||( null == strcmp(( spt->shs ), "pe" ))
          ||( null == strcmp(( spt->shs ), "Pe" ))
          ||( null == strcmp(( spt->shs ), "PET" ))
          ||( null == strcmp(( spt->shs ), "pet" ))
          ||( null == strcmp(( spt->shs ), "Pet" ))
          ||( null == strncmp(( spt->shs ), "POLYETHYLENE", 7 ))
          ||( null == strncmp(( spt->shs ), "polyethylene", 7 ))
          ||( null == strncmp(( spt->shs ), "Polyethylene", 7 )))
   {
      strcpy(( spt->sld ), "polyethylene" ); 
      strcpy(( spt->shs ), "PE" ); 

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
   else if (( null == strcmp(( spt->shs ), "PVC" ))
          ||( null == strcmp(( spt->shs ), "pvc" ))
          ||( null == strcmp(( spt->shs ), "Pvc" ))
          ||( null == strncmp(( spt->shs ), "POLYVINYLCHLORIDE", 9 ))
          ||( null == strncmp(( spt->shs ), "polyvinylchloride", 9 ))
          ||( null == strncmp(( spt->shs ), "Polyvinylchloride", 9 )))
   {
      strcpy(( spt->sld ), "polyvinylchloride" ); 
      strcpy(( spt->shs ), "PVC" ); 

      ( rtp->md )  = 1.3800e+03;
      ( rtp->epr ) = 3.3000e+00;
      ( rtp->tge ) = 3.6000e-02;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 1.6000e-01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.1e+03;
   }
   else if (( null == strcmp(( spt->shs ), "ALO95" ))
          ||( null == strcmp(( spt->shs ), "alo95" ))
          ||( null == strcmp(( spt->shs ), "Alo95" ))
          ||( null == strcmp(( spt->shs ), "ALO_95" ))
          ||( null == strcmp(( spt->shs ), "alo_95" ))
          ||( null == strcmp(( spt->shs ), "Alo_95" ))
          ||( null == strncmp(( spt->shs ), "ALO95", 5 ))
          ||( null == strncmp(( spt->shs ), "alo95", 5 ))
          ||( null == strncmp(( spt->shs ), "ALO_95", 6 ))
          ||( null == strncmp(( spt->shs ), "alo_95", 6 ))
          ||( null == strncmp(( spt->shs ), "ALOXID95", 8 ))
          ||( null == strncmp(( spt->shs ), "ALOXID95", 8 ))
          ||( null == strncmp(( spt->shs ), "ALUMINIUMOXIDE95", 16 ))
          ||( null == strncmp(( spt->shs ), "aluminiumoxide95", 16 ))
          ||( null == strncmp(( spt->shs ), "Aluminiumoxide95", 16 )))
   {
      strcpy(( spt->sld ), "aluminiumoxide_95%" ); 
      strcpy(( spt->shs ), "ALO95" ); 

      ( rtp->md )  = 3.7000e+03;
      ( rtp->epr ) = 9.1000e+00;
      ( rtp->tge ) = 2.7000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 2.0000e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 8.8e+02;
   }
   else if (( null == strcmp(( spt->shs ), "ALO99" ))
          ||( null == strcmp(( spt->shs ), "alo99" ))
          ||( null == strcmp(( spt->shs ), "Alo99" ))
          ||( null == strcmp(( spt->shs ), "ALO_99" ))
          ||( null == strcmp(( spt->shs ), "alo_99" ))
          ||( null == strcmp(( spt->shs ), "Alo_99" ))
          ||( null == strncmp(( spt->shs ), "ALO99", 5 ))
          ||( null == strncmp(( spt->shs ), "alo99", 5 ))
          ||( null == strncmp(( spt->shs ), "ALO_99", 6 ))
          ||( null == strncmp(( spt->shs ), "alo_99", 6 ))
          ||( null == strncmp(( spt->shs ), "ALOXID99", 8 ))
          ||( null == strncmp(( spt->shs ), "ALOXID99", 8 ))
          ||( null == strncmp(( spt->shs ), "ALUMINIUMOXIDE99", 16 ))
          ||( null == strncmp(( spt->shs ), "aluminiumoxide99", 16 ))
          ||( null == strncmp(( spt->shs ), "Aluminiumoxide99", 16 )))
   {
      strcpy(( spt->sld ), "aluminiumoxide_99%" );
      strcpy(( spt->shs ), "ALO99" ); 

      ( rtp->md )  = 3.9600e+03;
      ( rtp->epr ) = 9.9000e+00;
      ( rtp->tge ) = 2.7000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 3.0000e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 8.5000e+02;
   }
   else if (( null == strcmp(( spt->shs ), "BEO" ))
          ||( null == strcmp(( spt->shs ), "beO" ))
          ||( null == strcmp(( spt->shs ), "BeO" ))
          ||( null == strncmp(( spt->shs ), "BEOXIDE", 7 ))
          ||( null == strncmp(( spt->shs ), "beoxide", 7 ))
          ||( null == strncmp(( spt->shs ), "Beoxide", 7 ))
          ||( null == strncmp(( spt->shs ), "BE_OXIDE", 8 ))
          ||( null == strncmp(( spt->shs ), "Be_oxide", 8 ))
          ||( null == strncmp(( spt->shs ), "BERYLLIUMOXIDE", 12 ))
          ||( null == strncmp(( spt->shs ), "berylliumoxide", 12 ))
          ||( null == strncmp(( spt->shs ), "Berylliumoxide", 12 )))
   {
      strcpy(( spt->sld ), "berylliumoxide" ); 
      strcpy(( spt->shs ), "BEO" ); 

      ( rtp->md )  = 2.8500e+03;
      ( rtp->epr ) = 6.5000e+00;
      ( rtp->tge ) = 4.0000e-03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 2.4800e+02;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 1.0240e+03;
   }
   else if (( null == strcmp(( spt->shs ), "KER221" ))
          ||( null == strcmp(( spt->shs ), "ker221" ))
          ||( null == strcmp(( spt->shs ), "Ker221" ))
          ||( null == strcmp(( spt->shs ), "Ker_221" ))
          ||( null == strncmp(( spt->shs ), "ker221", 6 ))
          ||( null == strncmp(( spt->shs ), "KER221", 6 ))
          ||( null == strncmp(( spt->shs ), "STEATITE", 7 ))
          ||( null == strncmp(( spt->shs ), "steatite", 7 ))
          ||( null == strncmp(( spt->shs ), "Steatite", 7 )))
   {
      strcpy(( spt->sld ), "steatite" );
      strcpy(( spt->shs ), "KER221" ); 

      ( rtp->md )  = 2.7000e+03;
      ( rtp->epr ) = 6.7000e+00;
      ( rtp->tge ) = 5.0000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
      ( rtp->hc )  = 2.6000e+00;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 9.0000e+02;
   }
   else if (( null == strcmp(( spt->shs ), "else" ))
          ||( null == strcmp(( spt->shs ), "Else" ))
          ||( null == strcmp(( spt->shs ), "ELSE" )))
   {
      strcpy(( rtp->sld ), "else" );
      strcpy(( rtp->shs ), "ELSE" ); /* 'E'lse */

# if DEL_DEFLT == 1
      ( rtp->md )  = 2.7000e+03; /* [ parameters of KER221 ] */
      ( rtp->epr ) = 6.7000e+00;
      ( rtp->tge ) = 5.0000e-04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->tgm ) = 1.0000e+00;
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
   }
   else if (( null == strcmp(( rtp->shs ), "AIR" )) /* of course, under nor- */
          ||( null == strcmp(( rtp->shs ), "Air" )) /* mal conditions air is */
          ||( null == strcmp(( rtp->shs ), "air" )) /* not a solid material. */
          ||( null == strcmp(( rtp->shs ), "gas" )) /* Here it is neverthe- */
          ||( null == strcmp(( rtp->shs ), "GAS" )) /* less admitted for */
          ||( null == strcmp(( rtp->shs ), "Lu" ))  /* special calculations */
          ||( null == strcmp(( rtp->shs ), "lu" ))  /* [such as influence */
          ||( null == strcmp(( rtp->shs ), "LU" ))  /* of small air slits */
          ||( null == strcmp(( rtp->shs ), "gas" )) /* between dielectric */
          ||( null == strcmp(( rtp->shs ), "Gas" )) /* insulator and inner or */
          ||( null == strcmp(( rtp->shs ), "GAS" )) /* outer coax conductor]. */
          ||( null == strcmp(( rtp->shs ), "Luft" ))
          ||( null == strcmp(( rtp->shs ), "luft" ))
          ||( null == strcmp(( rtp->shs ), "LUFT" )))
   {
      strcpy(( rtp->sld ), "air" );
      strcpy(( rtp->shs ), "AIR" );

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
   }
   else
   {
      strcpy(( rtp->sld ), "void" );
      strcpy(( rtp->shs ), "VOID" );

      ( rtp->md )  = ZERO;
      ( rtp->epr ) = ZERO;
      ( rtp->tge ) = ZERO;
      ( rtp->myr ) = ZERO;
      ( rtp->tgm ) = ZERO;
      ( rtp->hc )  = ZERO;
      ( rtp->dhc ) = ZERO;
      ( rtp->cv )  = ZERO;
   };
   return rtp;
}
/*============================================================================*/
/*********************** end of function solids(*) ****************************/
