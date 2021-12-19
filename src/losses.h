/* [ file: losses.h ] */
/*******************************************************************************
*                                                                              *
*   Function body losses(*); HELIOS release v1.0r1                             *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function computes the dielectric and skin effect losses in a coaxial  *
*   line, given the frequency, [ reflectionless ] transmitted CW power, and    *
*   the outer and inner conductor temperatures.                                *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

ELECTRIC *\
losses( HELIOSSTATE *state )
{
/* allusions: */

/* declarations: */

   static HELIOSSTATE
     *stp;

   static FILE
     *errfle;

   static DIMENSIONS
     *dmp;

   static ELECTRIC
     *elp;

   static THERMDYN
     *thp;

   static const double
      my = MY_VAC_, /* vac. permeability [V*sec/(A*m)] */
      eps = EPS_VAC, /* vac. permittivity [A*sec/(V*m)] */
      gamma = GRAVIT_ACC; /* gravitation acceleration [m/s^2] */

   static short
      frc = null;

   static double
      freq = ZERO, /* operation frequency */
      cwpr = ZERO, /* transmitted CW power [W] */
      dout = ZERO, /* outer conductor diameter, coaxial line [m] */
      dinn = ZERO, /* inner conductor diameter, coaxial line [m] */
      winn = ZERO, /* rectangular waveguide inner width, 'a' [m] */
      hinn = ZERO, /* rectangular waveguide inner height, 'b' [m] */
      limp = ZERO, /* char line impedance [Ohm] */
      tout = ZERO, /* temperature, outer conductor [K] */
      tinn = ZERO, /* temperature, inner conductor [K] */
      tav = ZERO, /* any ['average'] temperature [K] */

      myo = ZERO, /* relative permeability, outer conductor */ 
      ffo = ZERO, /* form factor, outer conductor */
      rso = ZERO, /* resistivity, outer conductor [Ohm*m]*/
      dro = ZERO, /* temperature coeff of the latter [1/KELVIN] */
      rao = ZERO, /* actual resistivity, outer conductor [Ohm*m] */
      cao = ZERO, /* actual conductivity, outer conductor [S/m] */
      rfo = ZERO, /* surface resistance, outer conductor */
      rro = ZERO, /* outer loss resistance */

      myi = ZERO, /* relative permeability, inner conductor */ 
      ffi = ZERO, /* form factor, inner conductor */
      rsi = ZERO, /* resistivity, inner conductor [Ohm*m]*/
      dri = ZERO, /* temperature coeff of the latter [1/KELVIN] */
      rai = ZERO, /* actual resistivity, inner conductor [Ohm*m] */
      cai = ZERO, /* actual conductivity, inner conductor [S/m] */
      rfi = ZERO, /* surface resistance, inner conductor */
      rri = ZERO, /* inner loss resistance */

      epd = ZERO, /* relative permittivity */ 
      tge = ZERO, /* electric loss factor, tangent delta_E */
      myd = ZERO, /* relative permeability */
      hcd = ZERO, /* heat conductivity, dielectric */

      lssi = ZERO, /* losses, inner conductor */
      lsso = ZERO, /* losses, outer conductor */
      lssd = ZERO, /* dielectric losses */
      lsdo = ZERO, /* dielectric loss current into outer conductor */
      lsdi = ZERO, /* dielectric loss current into inner conductor */
      lsst = ZERO, /* total losses */

      grdo = ZERO, /* temperature gradient, outer conductor */
      grdi = ZERO, /* temperature cradient, inner conductor */

      dto = ZERO,
      dti = ZERO,

      kk0 = ZERO,
      kk1 = ZERO,
      cc0 = ZERO, /* light velocity in vacuum */
      cc1 = ZERO, /* light velocity in dielectric */
      wl1 = ZERO, /* wavelenghth in dielectric */
      wlc = ZERO, /* critical wavelength in wave guide */
      ss0 = ZERO,
      ss1 = ZERO,
      
      ny = ZERO, /* cinematic viscosity */
      gr = ZERO, /* Grashof number */
      pp = ZERO,
      qq = ZERO,
      zeta = ZERO;
/*............................................................................*/
/*
   static char
     ptr[STS_SIZE] = {null};
*/
/*............................................................................*/
/* prototyping: */

   double sqrt( double xx );
   double log(  double xx );
   double pow(  double xx, double yy );

   double grazet ( double gr, double ratio );

# ifndef _CCBUG
   char 
      *strcpy( char *ptr1, const char *ptr2 ),  
      *strcat( char *ptr1, const char *ptr2 ),
      *strncat( char *ptr1, const char *ptr2, size_t n );
# endif

   char
      *lotos( long mm, char cc );

   char
      *dotos( double x, char precision, char *format );
/*----------------------------------------------------------------------------*/
/* initialize: */

   stp = state;

   dmp = ( stp->dmp );
   elp = ( stp->elp );
   thp = ( stp->thp );

   frc = ( elp->frc ); /* the actual frequency label */

   freq = ( elp->freq );
   cwpr = ( elp->cwpr );

   ( stp->rsp->freq[frc] ) = freq;
   ( stp->rsp->cwpr[frc] ) = cwpr;

   if (( stp->opp->n[1] ) == null ) /* rectangular waveguide */
   {
      winn = ( dmp->winn );
      hinn = ( dmp->hinn );

      if (( stp->cyl[1] ) == ONE ) /* waveguide with separate mantle */
         tinn = ( thp->tinn );
      else /* waveguide and mantle not separated */
         tinn = ( thp->tjck );

      epd = ( elp->epd );
      tge = ( elp->tge );
      myd = ( elp->myd );
      rsi = ( elp->rsi );
      dri = ( elp->dri );
      myi = ( elp->myi );

      ffi = ( dmp->ffi );

      dti = tinn - CELSIUS_TO_KELVIN;
      rai = rsi*( 1. + dri*dti );

      if ( ZERO < fabs( rai ))
         cai = 1./rai;
      else
         cai = HUGE_VALF;

      rfi = ffi*sqrt( PI*freq*my*myi*rai ); /* skin effect surface resistance */

      ( stp->rsp->cai[frc] ) = cai;
      ( stp->rsp->rai[frc] ) = rai;
      ( stp->rsp->rfi[frc] ) = rfi;

      cc0 = 1. / sqrt( eps*my ); /* phase velocity in vacuum */
      cc1 = cc0 / sqrt( epd*myd ); /* phase velocity in dielectric */
      wl1 = cc1 / freq; /* wavelength in medium */
      wlc = 2.*winn;    /* cutoff wavelength, TE10 mode */
      qq = wl1 / wlc;

      if ( 1. <= qq )
      {
         errfle = fopen(( stp->errfle ), "a+" );

         fprintf( errfle, \
            "\nError message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); \
         fprintf( errfle,
            "\nOperating frequency %.12e Hz below", freq );
         fprintf( errfle,
            "\ncutoff frequency %.12e Hz !", ( cc1 / wlc ));
         fprintf( errfle,
            "\n[ Illegal frequency domain.]\n" );

         fclose( errfle );

         fprintf( stderr, \
            "\n Error message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); \
         fprintf( stderr,
            "\n Operating frequency %.12e Hz below", freq );
         fprintf( stderr,
            "\n cutoff frequency %.12e Hz !", ( cc1 / wlc ));
         fprintf( stderr,
            "\n [ Illegal frequency domain.]\n\n" );

	 exit( EXIT_FAILURE );
      };

      qq *= qq;
      pp = hinn / winn;
      ss0 = sqrt( 1. - qq );
      ss1 = sqrt(( my*myd ) / ( eps*epd ));
      limp = ss1/ss0; /* TE10 mode characteristic impedance */
      ( elp->limp ) = limp;

/* skin effect losses: */

      kk0 = rfi/( ss0*ss1*hinn );
      kk0 *= ( 2.*( 1. + 2.*pp*qq )); /* attenuation [ Neper ] */
      lssi = kk0*cwpr; /* waveguide skin effect losses [W/m] */

/* dielectric losses: */

      kk1 = 2.*PI*freq*eps*epd*tge*limp;
      lssd = kk1*cwpr;
      
/* [ hence, the total losses ] */

      lsst = lssi+lssd;
/*..........................................................................*/
      ( elp->lssi ) = lssi;
      ( elp->lssd ) = lssd;
      ( elp->lsst ) = lsst;
/*..........................................................................*/
/* results [ for debugging purposes, e.g.] : */

      ( stp->rsp->lssi[frc] ) = lssi;
      ( stp->rsp->lssd[frc] ) = lssd;
      ( stp->rsp->lsst[frc] ) = lsst;
   }
   else /* coaxial line */
   {
/*............................................................................*/
      dout = ( dmp->dout );
      dinn = ( dmp->dinn );

      tout = ( thp->tout );
      tinn = ( thp->tinn );

      myo = ( elp->myo );
      rso = ( elp->rso );
      dro = ( elp->dro );
      ffo = ( dmp->ffo );

      myi = ( elp->myi );
      rsi = ( elp->rsi );
      dri = ( elp->dri );
      ffi = ( dmp->ffi );

      epd = ( elp->epd );
      tge = ( elp->tge );
      myd = ( elp->myd );
/*...........................................................................*/
/* calculation: */
/*
      PRINTF( "dout", "=", dout );
      PRINTF( "dinn", "=", dinn );
      PRINTF( "char line impedance", "limp=", limp );
      PRINTF( "frequency", "f =", freq );
      PRINTF( "power", "p =", cwpr );
      PRINTF( "form factor, outer cond", "ffo=", ffo );
      PRINTF( "permeability, outer cond", "myo=", myo );
      PRINTF( "form factor, inner cond", "ffi=", ffi );
      PRINTF( "permeability, inner cond", "myi=", myi );
      PRINTF( "permittivity, dielectric", "epd=", epd );
*/
      qq = dout / dinn;

      limp = ( log( qq )*sqrt(( my*myd ) / ( eps*epd ))) / ( 2.*PI );
      ( elp->limp ) = limp;

/* surface conductivity, inner conductor */

      dti = tinn - CELSIUS_TO_KELVIN;
      rai = rsi*( 1. + dri*dti );
      cai = 1./rai;

/* surface resistance, inner conductor */

      rfi = ffi*sqrt( PI*freq*my*myi*rai );

/* inner loss resistance and conductor losses: */

      rri = rfi/dinn/PI;

      ( stp->rsp->cai[frc] ) = cai;
      ( stp->rsp->rai[frc] ) = rai;
      ( stp->rsp->rfi[frc] ) = rfi;
      ( stp->rsp->rri[frc] ) = rri;

      lssi = cwpr*rri/limp;

/* surface conductivity, outer conductor */

      dto = tout - CELSIUS_TO_KELVIN;
      rao = rso*( 1. + dro*dto );

      if ( ZERO < fabs( rao ))
         cao = 1./rao;
      else
         cao = HUGE_VALF;

/* surface resistance, outer conductor */

      rfo = ffo*sqrt( PI*freq*my*myo*rao );

/* outer loss resistance and conductor losses: */  

      rro = rfo/dout/PI;
      
      ( stp->rsp->cao[frc] ) = cao;
      ( stp->rsp->rao[frc] ) = rao;
      ( stp->rsp->rfo[frc] ) = rfo;
      ( stp->rsp->rro[frc] ) = rro;
      
      lsso = cwpr*rro/limp;

/* dielectric losses: */

      if ( SMALL_VAL < tge )
      {
         hcd = ( thp->hcd ); /* heat conductivity, dielectric */
                                    /* [W/(m*K)] */
         if ((( stp->opp->n[3] ) == ONE ) /* gas [or any fluid] dielectric */
           &&(( stp->opp->n[4] ) == null )) /* free convect between cndctrs */
         {
            tav = .5* ( tinn + tout );
            ny = ( thp->nyd ) + \
                 ( tav - CELSIUS_TO_KELVIN )*( thp->dnd );

            ny /= ( thp->prd ); /* divided through pressure */

            gr = gamma*pow( dinn, 3. ) / pow( ny, 2.);
            gr *= (( tinn - tout ) / tav );
            gr = fabs( gr ); /* only to make sure ...*/
/*............................................................................*/
            zeta = grazet( gr, qq );       /* essentialy the Nusselt number */
/*.......................................*/
            hcd *= zeta;
         };

         lssd = 2.*PI*freq*cwpr*tge*sqrt( eps*epd*my*myd );

/* temperature gradients: */

         if (( stp->opp->n[7] ) != null ) /* additional inner cndct cooling */
         {
            ss0 = log( .5*dout );
            ss1 = log( .5*dinn );

            kk0 = lssd / ( 2.*PI*(  ss0 - ss1 ));
            kk1 = ( tout - tinn )/( ss0 - ss1 );
            kk1 += ( kk0*( ss0 + ss1 )/2./hcd );

            grdo = 2.*( kk1 - kk0*ss0/hcd ) / dout;
            grdi = 2.*( kk1 - kk0*ss1/hcd ) / dinn;
         }
         else
         {
            grdo = - ( lssi + lssd )/( PI*dout*hcd );
            grdi = - lssi/( PI*dinn*hcd );
         };
/*
         PRINTF( "losses(*)", "grdo", grdo );
         PRINTF( "losses(*)", "grdi", grdi );
*/
         if (( stp->opp->n[3] ) == null ) /* solid dielectric */
         {
            if (( stp->opp->n[7] ) != null ) /* additional inner cond coolg */
               lsdi = - PI*dinn*hcd*grdi - lssi;
            else
               lsdi = ZERO;

            lsdo = lssd - lsdi;
         };
      }
      else /* lossless dielectric */
      {
         lsdi = ZERO;
         lsdo = ZERO;
         lssd = ZERO;

/* temperature gradients in dielectric */
/* at inner and outer conductors: */

         ss0 = log( dout/dinn );
         kk1 = ( tout - tinn )/ss0;

         grdi = 2.*kk1/dinn;
         grdo = 2.*kk1/dout;
      };
   
/* hence, the total losses: */

      lsst = lssi+lsso+lssd;
/*
      PRINTF( "outer surface resistance", "rfo=", rfo );
      PRINTF( "losses, outer conductor", "lsso=", lsso );
      PRINTF( "inner surface resistance", "rfi=", rfi );
      PRINTF( "losses, inner conductor", "lssi=", lssi );
      PRINTF( "losses, dielectric", "lssd=", lssd );
*/
/*..........................................................................*/
/* heat currents: */

      ( elp->lsso ) = lsso;
      ( elp->lssi ) = lssi;
      ( elp->lssd ) = lssd;
      ( elp->lsdi ) = lsdi;
      ( elp->lsdo ) = lsdo;
      ( elp->lsst ) = lsst;

      ( thp->grdo ) = grdo;
      ( thp->grdi ) = grdi;
/*..........................................................................*/
/* results [ for debugging purposes, e.g.] : */

      ( stp->rsp->lsso[frc] ) = lsso;
      ( stp->rsp->lssi[frc] ) = lssi;
      ( stp->rsp->lssd[frc] ) = lssd;
      ( stp->rsp->lsdi[frc] ) = lsdi;
      ( stp->rsp->lsdo[frc] ) = lsdo;
      ( stp->rsp->lsst[frc] ) = lsst;

      ( stp->rsp->grdo[frc] ) = grdo;
      ( stp->rsp->grdi[frc] ) = grdi;
   }; /* end else [ if opp->n[1] == 1: coaxial line */

   ( elp->rtn ) = null;

   return elp;
}  
/*============================================================================*/
/************************ end of function losses(*) ***************************/
