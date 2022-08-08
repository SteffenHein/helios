/* [ file: metals.h ] */
/*******************************************************************************
*                                                                              *
*   Function body metal(*); HELIOS release v1.0r1                              *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Returns physical parameters for metals commonly used in electric           *
*   waveguides.                                                                *
*   [ Parameters at ZERO degree Celsius ]                                      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef HLS_PARSET
   # define HLS_PARSET 1
# endif
/*============================================================================*/

METALS * \
metals( METALS *mpt )
{
   static METALS
      *rtp;
/*----------------------------------------------------------------------------*/
/* initialize: */

   rtp = mpt;

   ( rtp->md )  = ZERO; /* mass density [Kg/m^3] */
   ( rtp->er )  = ZERO; /* electric conductivity [S/m] */
   ( rtp->der ) = ZERO; /* its temp coefficient ( in general negative ) [1/K] */
   ( rtp->myr ) = ZERO; /* relative permeability */
   ( rtp->hc )  = ZERO; /* heat conductivity [W/(m*K)] */
   ( rtp->dhc ) = ZERO; /* its temperature coefficient [1/K] */
   ( rtp->cv )  = ZERO; /* heat capacity, cp [J/(Kg*K)] */
/*............................................................................*/
/* select ... */

   if (( null == strcmp(( mpt->shm ), "Cu" ))
     ||( null == strcmp(( mpt->shm ), "CU" ))
     ||( null == strcmp(( mpt->shm ), "cu" ))
     ||( null == strncmp(( mpt->shm ), "copper", 3 ))
     ||( null == strncmp(( mpt->shm ), "COPPER", 3 ))
     ||( null == strncmp(( mpt->shm ), "Copper", 3 ))
     ||( null == strncmp(( mpt->shm ), "Kupfer", 3 ))
     ||( null == strncmp(( mpt->shm ), "KUPFER", 3 )))
   {
      strcpy(( rtp->met ), "copper" );
      strcpy(( rtp->shm ), "Cu" );

      ( rtp->md )  = 8.9300e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 3.9300e+02; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.422 */
      ( rtp->cv )  = 3.9295e+02;
      ( rtp->er )  = 1.5500e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.3300e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "Al" ))
          ||( null == strcmp(( mpt->shm ), "AL" ))
          ||( null == strcmp(( mpt->shm ), "al" ))
          ||( null == strncmp(( mpt->shm ), "aluminium", 3 ))
          ||( null == strncmp(( mpt->shm ), "ALUMINIUM", 3 ))
          ||( null == strncmp(( mpt->shm ), "Aluminium", 3 )))
   {
      strcpy(( rtp->met ), "aluminium" );
      strcpy(( rtp->shm ), "Al" );

      ( rtp->md )  = 2.7000e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 2.3700e+02; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.421 */
      ( rtp->cv )  = 9.1903e+02;

# if HLS_PARSET == 1 /* secure value for some Aluminium alloys */
      ( rtp->er )  = 3.9950e-08;
      ( rtp->der ) = 4.0800e-03;
# else
      ( rtp->er )  = 2.5000e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.6000e-03; /* Handbook of Physical Quantities, p.550 */
# endif
   }
   else if (( null == strcmp(( mpt->shm ), "Me" ))
          ||( null == strcmp(( mpt->shm ), "ME" ))
          ||( null == strcmp(( mpt->shm ), "me" ))
          ||( null == strcmp(( mpt->shm ), "Ms" ))
          ||( null == strcmp(( mpt->shm ), "MS" ))
          ||( null == strcmp(( mpt->shm ), "ms" ))
          ||( null == strcmp(( mpt->shm ), "Br" ))
          ||( null == strcmp(( mpt->shm ), "BR" ))
          ||( null == strcmp(( mpt->shm ), "br" ))
          ||( null == strncmp(( mpt->shm ), "Messing", 5 ))
          ||( null == strncmp(( mpt->shm ), "messing", 5 ))
          ||( null == strncmp(( mpt->shm ), "MESSING", 5 ))
          ||( null == strncmp(( mpt->shm ), "Brass", 5 ))
          ||( null == strncmp(( mpt->shm ), "brass", 5 ))
          ||( null == strncmp(( mpt->shm ), "BRASS", 5 )))
   {
      strcpy(( rtp->met ), "brass" );
      strcpy(( rtp->shm ), "BR" );

      ( rtp->md )  = 6.7000e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 1.1000e+02;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 4.0000e+02;
      ( rtp->er )  = 6.3586e-08;
      ( rtp->der ) = 3.9000e-03;
   }
   else if (( null == strcmp(( mpt->shm ), "Zn" ))
          ||( null == strcmp(( mpt->shm ), "ZN" ))
          ||( null == strcmp(( mpt->shm ), "zn" ))
          ||( null == strncmp(( mpt->shm ), "zinc", 3 ))
          ||( null == strncmp(( mpt->shm ), "ZINC", 3 ))
          ||( null == strncmp(( mpt->shm ), "Zinc", 3 ))
          ||( null == strncmp(( mpt->shm ), "Zink", 3 ))
          ||( null == strncmp(( mpt->shm ), "ZINK", 3 )))
   {
      strcpy(( rtp->met ), "zinc" );
      strcpy(( rtp->shm ), "Zn" );

      ( rtp->md )  = 7.1330e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 1.1100e+02; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.426 */
      ( rtp->cv )  = 3.9510e+02;
      ( rtp->er )  = 5.6500e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.1700e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "Au" ))
          ||( null == strcmp(( mpt->shm ), "AU" ))
          ||( null == strcmp(( mpt->shm ), "au" ))
          ||( null == strncmp(( mpt->shm ), "gold", 3 ))
          ||( null == strncmp(( mpt->shm ), "GOLD", 3 ))
          ||( null == strncmp(( mpt->shm ), "Gold", 3 )))
   {
      strcpy(( rtp->met ), "gold" );
      strcpy(( rtp->shm ), "Au" );

      ( rtp->md )  = 1.9320e+04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 3.1100e+02; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.422 */
      ( rtp->cv )  = 1.3270e+02;
      ( rtp->er )  = 2.0600e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.0200e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "Ag" ))
          ||( null == strcmp(( mpt->shm ), "AG" ))
          ||( null == strcmp(( mpt->shm ), "ag" ))
          ||( null == strncmp(( mpt->shm ), "silver", 3 ))
          ||( null == strncmp(( mpt->shm ), "SILVER", 3 ))
          ||( null == strncmp(( mpt->shm ), "Silver", 3 ))
          ||( null == strncmp(( mpt->shm ), "Silber", 3 ))
          ||( null == strncmp(( mpt->shm ), "SILBER", 3 )))
   {
      strcpy(( rtp->met ), "silver" );
      strcpy(( rtp->shm ), "Ag" );

      ( rtp->md )  = 1.0497e+04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 4.2900e+02; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.425 */
      ( rtp->cv )  = 2.4006e+02;
      ( rtp->er )  = 1.4900e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.2800e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "Pt" ))
          ||( null == strcmp(( mpt->shm ), "PT" ))
          ||( null == strcmp(( mpt->shm ), "pt" ))
          ||( null == strncmp(( mpt->shm ), "platinum", 3 ))
          ||( null == strncmp(( mpt->shm ), "PLATINUM", 3 ))
          ||( null == strncmp(( mpt->shm ), "Platinum", 3 ))
          ||( null == strncmp(( mpt->shm ), "Platin", 3 ))
          ||( null == strncmp(( mpt->shm ), "PLATIN", 3 )))
   {
      strcpy(( rtp->met ), "platinum" );
      strcpy(( rtp->shm ), "Pt" );

      ( rtp->md )  = 2.1450e+04;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 7.2000e+01; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.424 */
      ( rtp->cv )  = 1.3657e+02;
      ( rtp->er )  = 9.8100e-08; /* I.S.Grigoriev, E.Z.Meilikhov: */
      ( rtp->der ) = 3.9600e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "Ti" ))
          ||( null == strcmp(( mpt->shm ), "TI" ))
          ||( null == strcmp(( mpt->shm ), "ti" ))
          ||( null == strncmp(( mpt->shm ), "titanium", 3 ))
          ||( null == strncmp(( mpt->shm ), "TITANIUM", 3 ))
          ||( null == strncmp(( mpt->shm ), "Titanium", 3 ))
          ||( null == strncmp(( mpt->shm ), "Titan", 3 ))
          ||( null == strncmp(( mpt->shm ), "TITAN", 3 )))
   {
      strcpy(( rtp->met ), "titanium" );
      strcpy(( rtp->shm ), "Ti" );

      ( rtp->md )  = 4.5050e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 2.0000e+01; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.426 */
      ( rtp->cv )  = 6.2700e+02;
      ( rtp->er )  = 4.2000e-07; /* I.S.Grigoriev, E.Z.Meilikhov: */
      ( rtp->der ) = 5.4600e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "Ni" ))
          ||( null == strcmp(( mpt->shm ), "NI" ))
          ||( null == strcmp(( mpt->shm ), "ni" ))
          ||( null == strncmp(( mpt->shm ), "nickel", 3 ))
          ||( null == strncmp(( mpt->shm ), "NICKEL", 3 ))
          ||( null == strncmp(( mpt->shm ), "Nickel", 3 )))
   {
      strcpy(( rtp->met ), "nickel" );
      strcpy(( rtp->shm ), "Ni" );

      ( rtp->md )  = 8.9100e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 8.0000e+01; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.423 */
      ( rtp->cv )  = 4.5737e+02;
      ( rtp->er )  = 6.1400e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 6.9200e-03; /* Handbook of Physical Quantities, p.551 */
   }
   else if (( null == strcmp(( mpt->shm ), "inv" ))
          ||( null == strcmp(( mpt->shm ), "INV" ))
          ||( null == strcmp(( mpt->shm ), "Inv" ))
          ||( null == strcmp(( mpt->shm ), "iv" ))
          ||( null == strcmp(( mpt->shm ), "IV" ))
          ||( null == strcmp(( mpt->shm ), "Iv" ))
          ||( null == strcmp(( mpt->shm ), "in" ))
          ||( null == strcmp(( mpt->shm ), "IN" ))
          ||( null == strcmp(( mpt->shm ), "In" ))
          ||( null == strncmp(( mpt->shm ), "invar", 3 ))
          ||( null == strncmp(( mpt->shm ), "INVAR", 3 ))
          ||( null == strncmp(( mpt->shm ), "Invar", 3 )))
   {
      strcpy(( rtp->met ), "invar" );
      strcpy(( rtp->shm ), "Inv" );

      ( rtp->md )  = 8.0500e+03;
      ( rtp->myr ) = 1.0000e+00;
      ( rtp->hc )  = 1.0153e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 5.1500e+02;
      ( rtp->er )  = 8.1960e-07;
      ( rtp->der ) = 6.7000e-03;
   }
   else if (( null == strcmp(( mpt->shm ), "N24" ))
	  ||( null == strcmp(( mpt->shm ), "n24" )))
   {
      strcpy(( rtp->met ), "N24" );
      strcpy(( rtp->shm ), "N24" );

      ( rtp->md )  = 8.1200e+03;
      ( rtp->myr ) = 1.0000e+00; /* TODO: n.a., but contains iron !!! */
      ( rtp->hc )  = 1.4600e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 5.0200e+02;
      ( rtp->er )  = 6.3000e-07;
      ( rtp->der ) = 0.0000e+00;
   }
   else if (( null == strcmp(( mpt->shm ), "DI" ))
          ||( null == strcmp(( mpt->shm ), "di" ))
          ||( null == strcmp(( mpt->shm ), "Di" ))
          ||( null == strncmp(( mpt->shm ), "dilver", 3 ))
          ||( null == strncmp(( mpt->shm ), "DILVER", 3 ))
          ||( null == strncmp(( mpt->shm ), "Dilver", 3 )))
   {
      strcpy(( rtp->met ), "dilver_p" );
      strcpy(( rtp->shm ), "Di" );

      ( rtp->md )  = 8.1500e+03;
      ( rtp->myr ) = 1.0000e+00; /* TODO: n.a., but contains iron !!! */
      ( rtp->hc )  = 1.7600e+01;
      ( rtp->dhc ) = 0.0000e+00;
      ( rtp->cv )  = 5.0200e+02; 
      ( rtp->er )  = 4.9000e-07;
      ( rtp->der ) = 0.0000e+00;
   }
   else
   {
      strcpy(( rtp->met ), "Else" );
      strcpy(( rtp->shm ), "ELSE" ); /* 'E'lse */

# if MET_DEFLT == 1 /* values for Silver */
      ( rtp->md )  = 1.04970e+04;
      ( rtp->myr ) = 1.00000e+00;
      ( rtp->hc )  = 4.2900e+02; /* [400 K] I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->dhc ) = 0.0000e+00; /* Handbook of Physical Quantities, p.425 */
      ( rtp->cv )  = 2.40060e+02;
      ( rtp->er )  = 1.49000e-08; /* I.S.Grigoriev, E.Z.Meilikhov; */
      ( rtp->der ) = 4.28000e-03; /* Handbook of Physical Quantities, p.551 */
# else /* if MET_DEFLT != 1 */
      ( rtp->md )  = ZERO;
      ( rtp->er )  = ZERO;
      ( rtp->der ) = ZERO;
      ( rtp->myr ) = ZERO;
      ( rtp->hc )  = ZERO;
      ( rtp->dhc ) = ZERO;
      ( rtp->cv )  = ZERO;
# endif /* MET_DEFLT != 1 */
  };
   return rtp;
}
/*============================================================================*/
/************************ end of function metal(*) ****************************/
