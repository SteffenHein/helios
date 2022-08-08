/* [ file: intlze.h ] */
/*******************************************************************************
*                                                                              *
*   Body of initialization function initlze(*); HELIOS, release v1.0r1         *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function Initializes waveguide temperatures for transferred           *
*   parameters, such as given materials, radio frequency or CW power.          *
*   It also determines the cylinder structure of the waveguide system,         *
*   viz. essentially the structure of its heat conductance matrix,             *
*   which is iterativeliy updated in function hlswrk(*) ].                     *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: January 31, 2022 ]                        <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef HLS_SDNODE
   # define HLS_SDNODE 0  /* [0] / 1 [don't] use side nodes */
# endif
/*.......................................................................*/
# if HLS_SDNODE == 1
   # include "./WRKMCS1.M"
# else
   # include "./WRKMCS0.M"
# endif
/*----------------------------------------------------------------------------*/
# ifndef INI_UPDWGT
   # define INI_UPDWGT WRK_UPDTMP
# endif
/*----------------------------------------------------------------------------*/
# define INI_DEBUG 0
/*============================================================================*/

HELIOSSTATE *\
initlze( HELIOSSTATE *state )
{
/* allusions: */

/* extern HELIOSSTATE helios; *//* state transfer structure */

/* declarations: */

   static HELIOSSTATE
     *stp;

   static OPERATIONS
     *opp;

   static DIMENSIONS
     *dmp;

   static ELECTRIC
     *elp;

   static THERMDYN
     *thp;

   static HCURRS
     *hcp;

   static HCNDCT
     *cdp;

   static DSPLAY
     *dsp = NULL;

   static FILE
     *logfle = NULL,
     *errfle = NULL;

   static char
      sgn = null,
      rtn = null;

   static short
      ii = null,
      jj = null,
      kk = null,
      frc = null,
      frn = null,
      nds_ = null,
      rank = null,
      nodes = null;

   static size_t
      bufsize =  null;

   static double
      upd = ZERO, /* update weight factor [ used in macro UMDTEMP( ) ] */
      hcm = ZERO, /* heat conductivity, mantle */
      hpp = ZERO, /* heat conductivity, cooling pipe */
      tge = ZERO, /* electric loss tangent */
      frq0 = ZERO, /* starting frequeny */
      frq1 = ZERO, /* stopping frequency */
      freq = ZERO, /* actual frequency */
      cwpr = ZERO, /* actual cw power */
      tfld = ZERO, /* cooling fluid temperature */
      ttub = ZERO, /* inner cooling tube temperature */
      tdel = ZERO, /* temperature, dielectric */
      tinn = ZERO, /* inner conductor temperature */
      tout = ZERO, /* outer conductor temperature */
      tjck = ZERO, /* mantle temperature */
      tenv = ZERO, /* environment temperature */
      tref = ZERO, /* reference temperature */
      tsdp = ZERO, /* side temperature, pipe */
      tsdm = ZERO, /* side temperature, mantle */
      trfm = ZERO, /* transferred side temperature, mantle */
      trfp = ZERO, /* transferred side temperature, pipe */
      tmpr = ZERO, /* any temperature */
      dtmp = ZERO, /* any temperature difference */
      quot = ZERO,
      rscl = 1.;   /* hcrr rescale factor */
      
   static const char /* dsplay(*) options */
      start = 's',   /* print 's'tart message */
      messge = 'm',  /* write 'm'essage under running cursor */
      cursor = 'c',  /* running 'c'ursor with percent display */
      percent = 'p'; /* 'p'ercent display */

/* time_t types etc.: */
   time_t
      nseconds = null,
     *timer = null;

   static char
     timeptr[STS_SIZE] = {null};

   double
      fabs( double xx ),
      sqrt( double xx ),
      pow( double xx, double yy );

   char
     *lotos( long mm, char cc );

   char
     *dotos( double x, char precision, char *format );

   DSPLAY 
     *dsplay( DSPLAY *dsp );

   HCNDCT
     *hcndct( HELIOSSTATE *stp );

   ELECTRIC
     *losses( HELIOSSTATE *stp );

   THERMDYN
     *kelvin( HELIOSSTATE *stp );

   RESULTS
     *values( HELIOSSTATE *stp );

   double
      granus( double gr );

   double
      grazet( double gr, double ratio );

   double
      trnorm( short ord, double ratio );
/*............................................................................*/
# ifndef _CCBUG
   char
     *strcpy( char *ptr1, const char *ptr2 ),  
     *strcat( char *ptr1, const char *ptr2 ),
     *strncat( char *ptr1, const char *ptr2, size_t n );
# endif
/*............................................................................*/
   rtn = null;
/*............................................................................*/
/* initialize pointers: */

   stp = state;

   opp = ( stp->opp );
   dmp = ( stp->dmp );
   elp = ( stp->elp );
   thp = ( stp->thp );
   hcp = ( stp->hcp );
   cdp = ( stp->cdp );
/*............................................................................*/
/* initialize some thermal variables: */

   ( thp->hcm ) = ( thp->hcj );
   ( thp->dhm ) = ( thp->dhj );

   ( thp->hcp ) = ( thp->hct );
   ( thp->dhp ) = ( thp->dht );
/*............................................................................*/
/* copy side temperatures, if such are fixed: */

   if (( opp->n[8] == ONE )\
     ||( opp->n[8] == THREE )) 
      ( thp->trfm ) = ( thp->tsdm ); /* fixed */
   else if (( opp->n[9] == ONE )\
          ||( opp->n[9] == THREE )) 
      ( thp->trfm ) = ( thp->tsdm ); /* initial value */
      
   if (( opp->n[8] == TWO )\
     ||( opp->n[8] == THREE ))
      ( thp->trfp ) = ( thp->tsdp ); /* fixed */
   else if (( opp->n[9] == TWO )\
          ||( opp->n[9] == THREE ))
      ( thp->trfp ) = ( thp->tsdp ); /* initial value */
/*............................................................................*/
/* reset update factors and exponents, */
/* and system topology parameters: */

   ( stp->uexp ) = ZERO;
   ( cdp->rank ) = null;
   ( cdp->nodes ) = null;
/*............................................................................*/
/* reset temperatures and cylinder marks: */

   nodes = NODES;
   rank = 2*NODES;

   ii = null; do
   {
      ( thp->tfix[ii] ) = ZERO;
      ( thp->temp[ii] ) = ZERO;
      ( thp->temp_init[ii] ) = ZERO;
      ( cdp->nde[ii] ) = -ONE;
      ( stp->cyl[ii] ) = null;
      ii++;
   } while ( ii < nodes );
   while ( ii < rank )
   {
      ( thp->temp[ii] ) = ZERO;
      ( thp->temp_init[ii] ) = ZERO;
      ii++;
   };
/*...........................................................................*/
/* reset mutual heat conductance matrix: */

   ii = null; do
   {
      jj = null; do
      {
         ( cdp->mhc[ii][jj] ) = ZERO;
         jj++;
      } while ( jj < rank );
      ii++;
   } while ( ii < rank );
/*............................................................................*/
/* heat currents rescale factor: */
# ifdef HLS_RESCLE
   rscl = HLS_RESCLE/( dmp->lwgd );
# endif
/*............................................................................*/
/* upd = weight factor for temperature updates: */ 
/* [ temp[i+1] := upd*new_temp + (1-upd)*temp[i] ] */

   upd = INI_UPDWGT;
/*............................................................................*/
/* reset operation marks: */

   kk = null; do
   {
      ( stp->opmrk[kk] ) = null;
   } while(( kk++ ) < OPERATS ); /* dimension opmrk[OPERATS+ONE] !!! */
/*............................................................................*/
/* initialize dsplay(*) [ structure dsp* DSPLAY ]: */

   dsp = dsplay( null );

   logfle = fopen(( stp->logfle ), "r+" );
   fseek( logfle, ( stp->fleps ), SEEK_SET );

   errfle = fopen(( stp->errfle ), "a+" );

   if (( stp->uif ) != 't' )
   {
      kk = setvbuf( logfle, null, _IONBF, bufsize ); /* set unbuffered */
      ( dsp->display ) = logfle;
   }
   else
      ( dsp->display ) = stdout;
/*............................................................................*/
/* HELIOS start message: */

   if ( stp->uif == 't' )
   {
      strcpy( dsp->messge, "\r Job no " );
      strcat( dsp->messge, lotos(( stp->job ), null ));
      strncat( dsp->messge, " launched", 9 );

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ) + 11 );
      strncat( dsp->messge, " at ", 4 );
      strncat( dsp->messge, timeptr, 8 );
      strncat( dsp->messge, " - please wait a moment ...", 28 );
/*............................................................................*/
/* write start message on screen: */

      dsp->option = start;
      dsplay( dsp );

      fprintf( stdout, "\n " );
   };
/*............................................................................*/
/* copy [ initial ] frequencies, CW power, temperatures etc. */
/* 1st stage - preparation: */

   stp->opmrk[0] = null;

   frn = ( elp->frn ); /* the number of frequency points */

   frq0 = ( elp->frq0 ); /* lower frequency [Hz] */
   frq1 = ( elp->frq1 ); /* upper frequency [Hz] */
   cwpr = ( elp->cwpr ); /* CW power [Watts] */
   freq = ( elp->freq ); /* CW power [Watts] */

   tenv = ( thp->tenv ); /* environment temperature */
   tfld = ( thp->tfld ); /* coolant fluid temperature */
   ttub = ( thp->ttub ); /* cooling pipe temperature */
   tdel = ( thp->tdel ); /* temperature, dielectric */
   tinn = ( thp->tinn ); /* inner conductor temperature */
   tout = ( thp->tout ); /* outer conductor temperature */
   tjck = ( thp->tjck ); /* mantle temperature */
   
   tsdp = ( thp->tsdp ); /* fixed side temperature, inner conductor */
   tsdm = ( thp->tsdm ); /* fixed side temperature, mantle */
   trfp = ( thp->trfp ); /* side temperature, inner conductor */
   trfm = ( thp->trfm ); /* side temperature, mantle */
   
   tge = ( elp->tge ); /* dielectric loss tangent */

   ( thp->mbe ) = null;
   ( thp->ppe ) = null;
   ( thp->tnorm ) = GIANT_VAL;
/*............................................................................*/
/* compute loss current factors for */
/* side temperatures tsdp and tsdm: */

   if ((( opp->n[8] ) != null )
     ||(( opp->n[9] ) != null ))
   {
      ( elp->cwpr ) = 1.; /* normalized CW power */
      
      if (( elp->frq0 ) <= ( elp->frq1 ))
      {
         frc = null;
         sgn = ONE;
      }
      else
      {
         frc = frn - ONE;
         sgn = - ONE;
      };
/*............................................................................*/
/* frequency sweep: */

      kk = null; freq = frq0; do
      {
         ( elp->frc ) = frc;

         if ( ONE < frn )
         {
            quot = (( double ) kk ) / ( frn - ONE );
            ( elp->freq ) = ( 1. - quot )*frq0 + quot*frq1;
         };

         if ( opp->n[1] == null ) /* rectangular waveguide */
         {
/*............................................................................*/
/* waveguide loss coefficient for fixed mantle side temperature tsdm */

            if (( opp->n[8] == ONE )
              ||( opp->n[8] == THREE ))
	    {
	       tmpr = ( thp->tsdm ); /* fixed side temperature, mantle [ input*/
                                     /* parameter s66, cf. file par.log ] */
               ( thp->tinn ) = tmpr;
               ( thp->tdel ) = tmpr;
/*............................................................................*/
               elp = losses( stp );        /*                                 */
/*.......................................*/
               ( elp->lssfm[frc] ) = ( elp->lssi ); /* since cwpr = 1. */

# if INI_ADDLSS == 1
               ( elp->lssfm[frc] ) += ( elp->lssd ); 
# endif
	    };
/*............................................................................*/
         }
         else /* if ( opp->n[1] == ONE ) i.e. coaxial line */
         {
/*............................................................................*/
/* loss coefficient for fixed inner conductor [ pipe ] side temperature tsdp */

            if (( opp->n[8] == TWO )
              ||( opp->n[8] == THREE ))
	    {
	       tmpr = ( thp->tsdp ); /* fixed side temperature, pipe [ input */
                                     /* parameter s69, cf. file par.log ] */
               ( thp->tinn ) = tmpr;
               ( thp->tdel ) = tmpr;
/*............................................................................*/
               elp = losses( stp );        /*                                 */
/*.......................................*/
               ( elp->lssfp[frc] ) = ( elp->lssi ); /* since cwpr = 1. */

# if INI_ADDLSS == 1
               ( elp->lssfp[frc] ) += ( .5*( elp->lssd )); 
# endif
            };
/*............................................................................*/
/* loss coefficient for fixed outer conductor temperature tsdm */

            if (( opp->n[8] == ONE )
              ||( opp->n[8] == THREE ))
	    {
	       tmpr = ( thp->tsdm ); /* fixed side temperature, mantle [ input*/
                                     /* parameter s66, cf. file par.log ] */
               ( thp->tout ) = tmpr;
               ( thp->tdel ) = tmpr;
/*............................................................................*/
               elp = losses( stp );        /*                                 */
/*.......................................*/
               ( elp->lssfm[frc] ) = ( elp->lsso ); /* since cwpr = 1. */

# if INI_ADDLSS == 1
               ( elp->lssfm[frc] ) += ( .5*( elp->lssd )); 
# endif
             };
/*............................................................................*/
         }; /* end if (( opp->n[1] ) == ONE ) - coaxial line */
         frc += sgn;
      } while( ++kk < frn );
   }; /* end if (( opp->n[8] ) != null )||(( opp->n[9] ) != null ) */
/*............................................................................*/
/* restore initial values: */

   tge = ( elp->tge ); /* dielectric loss tangent */

   ( thp->tinn ) = tinn;
   ( thp->tdel ) = tdel;
   ( thp->tout ) = tout;
   
   ( elp->frq0 ) = frq0; 
   ( elp->frq1 ) = frq1;
   ( elp->cwpr ) = cwpr;
   ( elp->freq ) = freq;
/*............................................................................*/
/* normalized temperature maximum of dielectric in rectangular waveguide: */

   if ((( opp->n[1] ) == null )  /* rectangular waveguide */
     &&(( opp->n[3] ) == null )) /* solid dielectric */
   {
      quot = ( dmp->winn ) / ( dmp->hinn );
      ( thp->tnrm ) = trnorm( INI_TRNORD, quot );
   };
/*............................................................................*/
/*............................................................................*/
/* CYLINDER STRUCTURE OF THERMAL NET [ INITIAL HEAT CONDUCTANCE MATRIX ] */
/*............................................................................*/
/*............................................................................*/
/* tentative approximation of resistive and dielectric losses ( elp->lss*): */
/* for given initial data [ frequency, cw power, and temperatures ] */

   ( stp->onerr ) = 's'; /* on error: don't exit [ 's'ilent return ] */
/*............................................................................*/
   elp = losses( stp );       /*                                           */
/*..........................*/
   rtn = ( elp->rtn );
/*............................................................................*/
/* RECTANGULAR WAVEGUIDE: */
/*............................................................................*/
   if ( opp->n[1] == null ) /* rectangular waveguide */
   {
/*............................................................................*/
/* approximate mantle temperature difference for initial data */
/* and finite mantle thickness [ shouldn't be too small ] */

      if (((( dmp->winn )*( dmp->hinn ))\
         < (( dmp->wout )*( dmp->hout )))
        &&(( dmp->hinn ) < ( dmp->hout ))
        &&(( thp->hcj ) < GIANT_VAL ))
      {
         hcm = ( thp->hcj );
         hcm += ( thp->dhj )*(( thp->tjck ) - CELSIUS_TO_KELVIN );

         if ( ZERO < hcm )
         {
            hcm *= (( dmp->winn )/(( dmp->hout ) - ( dmp->hinn )) + \
                    ( dmp->hinn )/(( dmp->wout ) - ( dmp->winn )));
	    hcm *=  4.;
	    dtmp = ( elp->lsst )/hcm;

	    if ( fabs( dtmp ) < INI_DTMNGL )
            {
               if (( opp->n[5] ) == null ) /* fixed mantle temperature */
               {
                  if (( stp->uif ) != 't' )
                  {
                     fprintf( errfle,
                        "\nWarning message from function %s:", __func__ );
                     fprintf( errfle,
                        "\nExtremely small temperature gradient "
                        "in waveguide mantle !" );

                     if (( opp->n[2] ) == null ) /* fixed CW power */
                        fprintf( errfle,
                           "\n[ CW power input "
                           "may be too low.]\n " );
                     else /* if (( opp->n[2] ) == ONE ) - fixed inn cdc temp */
                        fprintf( errfle,
                           "\n[ Initial CW power input "
                           "may be too low.]\n " );

                     fprintf( logfle, "\n%s",
                        "extremely small temperature gradient "
			"in waveguide mantle !" );

                     if (( opp->n[2] ) == null ) /* fixed CW power */
                        fprintf( logfle,
                           "\n[ CW power input "
                           "may be too low ]\n " );
                     else /* if (( opp->n[2] ) == ONE ) - fixed inn cdc temp */
                        fprintf( logfle,
                           "\n[ initial CW power input "
                           "may be too low ]\n " );

                     ( stp->fleps ) = ftell( logfle );
                     ( dsp->fleps ) = ftell( logfle );
                     ( dsp->option ) = percent;
                  }
                  else
                  {
                     strcpy(( dsp->messge ),
                        "[extremely_small_temperature_gradient"
			"_in_waveguide_mantle]" );
                     ( dsp->option ) = messge;
                     dsplay( dsp );
                     ( dsp->option ) = cursor;
                  };

                  fprintf( stderr,
                     "\n Warning message from function %s:", __func__ );
                  fprintf( stderr,
                     "\n Extremely small temperature gradient "
		     "in waveguide mantle !" );

                  if (( opp->n[2] ) == null ) /* fixed CW power */
                     fprintf( stderr,
                        "\n [ CW power input "
                        "may be too low.]\n " );
                  else /* if (( opp->n[2] ) == ONE ) - fixed inner temperature*/
                     fprintf( stderr,
                        "\n [ Initial CW power input "
                        "may be too low.]\n " );
               } /* end if (( opp->n[5] ) == null ) - fixed mantle temp */
/*............................................................................*/
/* mantle temperature difference estimates: */

	       else /* opp->n[5] == ONE */
               {
                  if (( stp->uif ) != 't' )
                  {
                     fprintf( logfle, "\n%s",
                        "very small temperature gradient "
			"in waveguide mantle !" );
                     fprintf( logfle, "\n%s",
                        "[ switching to mantle temperature estimates ]" );
                     
		     ( stp->fleps ) = ftell( logfle );
                     ( dsp->fleps ) = ftell( logfle );
                     ( dsp->option ) = percent;
                  }
                  else
                  {
                     strcpy(( dsp->messge ),
                        "[switching_to_mantle_temperature_estimates]" );
                     ( dsp->option ) = messge;
                     dsplay( dsp );
                     ( dsp->option ) = cursor;
                  };
	       }; /* end if (( opp->n[5] ) == ONE ) */
               ( thp->mbe ) = ONE;

               ( thp->hcj ) = GIANT_VAL;
               ( thp->dhj ) = ZERO;
            }
            else
               ( thp->mbe ) = null;
         };
      }; /* end if ( dmp->winn < dmp->wout ) etc. */
/*............................................................................*/
/* determine cylinder structure [ the thermal nodes ]: */

      if (( stp->uif ) != 't' )
      {
         fprintf( logfle, "\n%s", "determining cylinder structure ..." );
         ( stp->fleps ) = ftell( logfle );
         ( dsp->fleps ) = ftell( logfle );
         ( dsp->option ) = percent;
      }
      else
      {
         strcpy(( dsp->messge ), "[determining_cylinder_structure]" );
         ( dsp->option ) = messge;
         dsplay( dsp );
         ( dsp->option ) = cursor;
      };
/*............................................................................*/
/* cylinder-0 [ lossy dielectric ]: */

      if( SMALL_VAL < ( elp->tge ))
         ( stp->cyl[0] ) = ONE;
/*............................................................................*/
/* cylinder-1 [ inner ( conducting ) surface, in "finite" mantle ]: */

      if (((( dmp->winn )*( dmp->hinn )) \
         < (( dmp->wout )*( dmp->hout )))
        &&(( thp->hcj ) < GIANT_VAL ))
         ( stp->cyl[1] ) = ONE;
      else
         ( stp->cyl[1] ) = null;
/*............................................................................*/
/* cylinder-2 [ mantle in heat exchange with convective environment ]:*/

      if (( opp->n[5] ) == ONE ) /* "floating" mantle temperature, i.e. ...*/
      {
         ( stp->cyl[2] ) = ONE; /*...mantle under (free or forced) convection */
/*............................................................................*/
/* cylinder-3 - thermal contact to one side: */

         if (((( dmp->winn )*( dmp->hinn )) \
            < (( dmp->wout )*( dmp->hout )))
           &&(( thp->hcm ) < GIANT_VAL )
           &&((( opp->n[8] ) == ONE )
            ||(( opp->n[8] ) == THREE )
            ||(( opp->n[9] ) == ONE )
            ||(( opp->n[9] ) == THREE )))
         {
	    ( stp->opmrk[1] ) = ONE; /* side conditions mode indicator */
            ( stp->cyl[3] ) = ONE;   /* mantle with side conditions */

/* mantle heat conductance to side temperature trfm */

            ( thp->sdhcm ) = ( thp->hcm ) + \
                             ( trfm - CELSIUS_TO_KELVIN )*( thp->dhm );
            ( thp->sdhcm ) *= (( dmp->wout )*( dmp->hout ) - \
                              ( dmp->winn )*( dmp->hinn ));

/* reduce update factor to improve algorithm stability */

            upd = INI_UPDWGT/10.;
         };
      }; /* end if (( opp->n[5] ) == ONE ) - floating mantle temperature */
/*............................................................................*/
/* initialize the temperatures: */

      if ( opp->n[4] == ONE ) /* forced convection [ inner cooling ] */
      {
         if ( stp->cyl[2] == ONE )
            tjck = .80*tinn + .10*tenv;

         ( stp->uexp ) = INI_UDEXP2;
      }
      else if ( opp->n[6] == ONE ) /* forced convection in environment */
      {
         if ( stp->cyl[0] == ONE )
            tdel = 1.05*tinn -.05*tenv;

         if ( stp->cyl[2] == ONE )
            tjck = .95*tinn + .05*tenv;

         ( stp->uexp ) = INI_UDEXP1;
      }
      else
      {
         if ( stp->cyl[0] == ONE )
            tdel = 1.05*tinn -.05*tenv;

         if ( stp->cyl[2] == ONE )
            tjck = .95*tinn + .05*tenv;

         ( stp->uexp ) = INI_UDEXP0;
      };

      if ( stp->cyl[3] == ONE )
      {
         if (( opp->n[8] == ONE )
           ||( opp->n[8] == THREE ))   
         {
            tjck = .50*tjck + .50*tsdm;
         }
         else if (( opp->n[9] == ONE )
                ||( opp->n[9] == THREE ))
         {
            tjck = .50*tjck + .50*tsdm;
         };
      };
/*............................................................................*/
/* copy these approximate temperatures [ simultaneously counting the nodes ]: */

      ( cdp->ndm ) = -ONE;

      nds_ = null;

      if ( opp->n[4] == ONE ) /* inner waveguide cooling [forced convection] */
         ( thp->tfix[nds_] ) = tdel;

      if ( stp->cyl[0] == ONE ) /* lossy dielectric */
      {
         ( thp->temp[nds_] ) = tdel;
         ( cdp->nde[1] ) = nds_;      /* <- dielectric [ tdel ] */
         nds_++;
      };

      if ( stp->cyl[1] == ONE )      /* finite mantle thickness */
      {
         ( thp->temp[nds_] ) = tinn;

	 if ( stp->cyl[2] == null ) /* fixed mantle temperature */
            ( thp->tfix[nds_] ) = tjck;

         cdp->nde[2] = nds_;          /* <- temperature sensed node [ tinn ] */
         nds_++;
      };

      if ( stp->cyl[2] == ONE ) /* floating mantle temperature */
      {
         ( thp->temp[nds_] ) = tjck;
         ( thp->tfix[nds_] ) = tenv;

         if ( stp->cyl[1] == null ) /* "absent" mantle [wall thickness zero] */
         { 
            tjck = tinn;
            ( cdp->nde[2] ) = nds_;       /* <- mantle [ tinn ] */
         }
         ( cdp->nde[3] ) = nds_;       /* <- mantle [ tjck ] */
         nds_++;
      } /* [ end, floating mantle temperature ] */
      else if ( stp->cyl[2] == null ) /* fixed mantle temperature */
      {
         ( thp->tfix[nds_- ONE] ) = thp->tjck;
      };
/*...........................................................................*/
# if HLS_SDNODE == 1 /* use side nodes */

      if ( stp->cyl[3] == ONE ) /* side conditions */
      {
         cdp->ndm = cdp->nde[3];       /* <- mantle [ tjck ] */

         if (( opp->n[8] == ONE )\
	   ||( opp->n[8] == THREE ))
         {
            ( thp->temp[nds_] ) = .5*( tjck + tsdm );
            ( thp->tfix[nds_] ) = tsdm; /* fixed [ = tsdm ] */
            ( cdp->nde[4] ) = nds_;
            nds_++;
         }
	 else if (( opp->n[9] == ONE )\
                ||( opp->n[9] == THREE ))
         {
            ( thp->temp[nds_] ) = ( thp->trfm );
            ( cdp->nde[5] ) = nds_;
            nds_++;
         };
      }; /* end [ side conditions ] */
# endif /* if HLS_SDNODE == 1 */
/*...........................................................................*/
   } /* end [ rectangular waveguide ] */
/*............................................................................*/
/* COAXIAL WAVEGUIDE: */
/*............................................................................*/
   else /* ( opp->n[1] == ONE ): coaxial waveguide */
   {
/*............................................................................*/
/* approximate mantle temperature difference for initial data */
/* and finite mantle thickness */

      if ((( dmp->dout ) < ( dmp->djck ))
        &&(( thp->hcj ) < GIANT_VAL ))
      {
         hcm = ( thp->hcj );
         hcm += ( thp->dhj )*(( thp->tjck ) - CELSIUS_TO_KELVIN );

         if ( ZERO < hcm )
         {
            dtmp = ( elp->lsst )*log(( dmp->djck )/( dmp->dout ))/( 2.*PI*hcm );

            if ( fabs( dtmp ) < INI_DTMNGL )
            {
/* switch to mantle temperature difference estimation mode */

               if (( stp->uif ) != 't' )
               {
                  fprintf( logfle, "\n%s",
                     "switching to mantle temperature estimates" );
                  ( stp->fleps ) = ftell( logfle );
                  ( dsp->fleps ) = ftell( logfle );
                  ( dsp->option ) = percent;
               }
               else
               {
                  strcpy(( dsp->messge ),
                     "[switching_to_mantle_temperature_estimates]" );
                  ( dsp->option ) = messge;
                  dsplay( dsp );
                  ( dsp->option ) = cursor;
               };

               ( thp->mbe ) = ONE; /* "thermally small" mantle */

               ( thp->hcj ) = GIANT_VAL;
               ( thp->dhj ) = ZERO;
            }
            else
               ( thp->mbe ) = null;
         };
      }; /* end [ if ( dmp->dout ) < ( dmp->djck ) ] */

      if ((( opp->n[7] ) == ONE ) /* additional pipe cooling */
        &&(( dmp->dtub ) < ( dmp->dinn ))
         &&(( thp->hct ) < GIANT_VAL ))
      {
         hpp = ( thp->hct );
         hpp += ( thp->dht )*(( thp->tfld ) - CELSIUS_TO_KELVIN );

         if ( ZERO < hpp )
         {
            dtmp = ( elp->lsst )*log(( dmp->dinn )/( dmp->dtub ))/( 2.*PI*hpp );

            if ( fabs( dtmp ) < INI_DTMNGL )
            {
/* switch to mantle temperature difference estimation mode */

               if (( stp->uif ) != 't' )
               {
                  fprintf( logfle, "\n%s",
                     "switching to pipe temperature estimates" );
                  ( stp->fleps ) = ftell( logfle );
                  ( dsp->fleps ) = ftell( logfle );
                  ( dsp->option ) = percent;
               }
               else
               {
                  strcpy(( dsp->messge ),
                     "[switching_to_pipe_temperature_estimates]" );
                  ( dsp->option ) = messge;
                  dsplay( dsp );
                  ( dsp->option ) = cursor;
               };

               ( thp->ppe ) = ONE;

               ( thp->hcp ) = ( thp->hct );
               ( thp->dhp ) = ( thp->dht );

               ( thp->hct ) = GIANT_VAL;
               ( thp->dht ) = ZERO;
            }
            else
               ( thp->ppe ) = null;
         };
      }; /* end if ( dmp->dtub < dmp->dinn ) etc. */
/*............................................................................*/
/* determine cylinder structure [ the thermal nodes ]: */

      if (( stp->uif ) != 't' )
      {
         fprintf( logfle, "\n%s", "determining cylinder structure ..." );
         ( stp->fleps ) = ftell( logfle );
         ( dsp->fleps ) = ftell( logfle );
         ( dsp->option ) = percent;
      }
      else
      {
         strcpy(( dsp->messge ), "[determining_cylinder_structure]" );
         ( dsp->option ) = messge;
         dsplay( dsp );
         ( dsp->option ) = cursor;
      };
/*............................................................................*/
/* cylinder-0 [ pipe, cylinder-5: side conditions ]: */

      if (( dmp->dtub < dmp->dinn )
        &&( thp->hct < GIANT_VAL ))
      {
         if ( opp->n[7] == ONE ) /* inner pipe cooling */
            ( stp->cyl[0] ) = ONE;

         if ((( opp->n[8] == TWO )
            ||( opp->n[8] == THREE )
            ||( opp->n[9] == TWO )
            ||( opp->n[9] == THREE )))
         {
	    ( stp->opmrk[1] ) = ONE;
            ( stp->cyl[5] ) = ONE; /* side conditions, pipe */
	    
/* pipe heat conductance at side temperature trfp */

            ( thp->sdhcp ) = ( thp->hct ) + \
                             ( trfp - CELSIUS_TO_KELVIN )*( thp->dht );
            ( thp->sdhcp ) *= ( PI*(( dmp->dinn )*( dmp->dinn ) - \
                              ( dmp->dtub )*( dmp->dtub ))/4.);

/* reduce update factor to improve algorithm stability */

            upd = INI_UPDWGT/10.;
         };                     
      };
/*............................................................................*/
/* cylinder-1 - inner conductor [ always present, therefore ... ]: */

      ( stp->cyl[1] ) = ONE;
/*............................................................................*/
/* cylinder-2 [ lossy dielectric ]: */

      if (( opp->n[3] == null ) /* solid dielectric */
        &&( SMALL_VAL < ( elp->tge )))
         ( stp->cyl[2] ) = ONE;
/*............................................................................*/
/* cylinder-3 [ outer conductor ( mantle ) with finite wall thickness ]: */

      if (( dmp->dout <  dmp->djck )
        &&( thp->hcj < GIANT_VAL ))
         ( stp->cyl[3] ) = ONE;
/*............................................................................*/
/* cylinder-4 [ "floating" mantle ] */

      if ( opp->n[5] == ONE ) /* floating mantle temperature */
      {
         ( stp->cyl[4] ) = ONE;
/*............................................................................*/
/* cylinder-6 [ side conditions, mantle ]: */

         if (( dmp->dout < dmp->djck )
           &&( thp->hcm < GIANT_VAL ))
         {
            if (( opp->n[8] == ONE )
              ||( opp->n[8] == THREE )
              ||( opp->n[9] == ONE )
              ||( opp->n[9] == THREE ))
            {
               ( stp->cyl[6] ) = ONE;  /* side conditions, mantle */
	       ( stp->opmrk[1] ) = ONE;

/* mantle heat conductance to one side [ at temperature trfm ] */

               ( thp->sdhcm ) = ( thp->hcm ) + \
                             ( trfm - CELSIUS_TO_KELVIN )*( thp->dhm );
               ( thp->sdhcm ) *= ( PI*(( dmp->djck )*( dmp->djck ) - \
                              ( dmp->dout )*( dmp->dout ))/4.);

/* reduce update factor to improve algorithm stability */

               upd = INI_UPDWGT/10.;
            }; /* end [ side conditions, mantle ] */
         }; /* end [ finite wal thickness, mantle ] */
      } /* end if ( opp->n[5] == ONE ) - floating mantle temperature */
      else
         stp->cyl[6] = null;
/*............................................................................*/
/* initialize the temperatures: */

      if ( opp->n[4] == ONE ) /* forced convection within waveguide */
      {
         if ( stp->cyl[0] ) /* inner pipe cooling */
            ttub = .5*tinn + .5*tfld;

         if ( stp->cyl[4] == ONE ) /* floating mantle temperature */
            tjck = .80*tinn + .20*tdel;
         else
            tjck = thp->tjck;

         if ( stp->cyl[3] ) /* mantle with finite wall thickness */
            tout = .99*tinn + .01*tdel;
         else
            tout = tjck;

         ( stp->uexp ) = INI_UDEXP2;
      }
      else if (( opp->n[7] ) == ONE ) /* additional pipe cooling */
      {
         if ( stp->cyl[4] )
            tjck = tinn;

         if ( stp->cyl[3] )
            tout = tinn;
         else
            tout = tjck;

         if ( stp->cyl[2] ) /* solid dielectric */
         {
            tdel = .30*tinn - .30*tenv;
/*............................................................................*/
# if INI_INITRS != 0
/* reduce dielectric losses in a first approximation */
/* [ pre-shaping ] - which will be corrected later */
            ( elp->tge ) *= 1.0e-01;
# endif /* INI_INITRS != 0 */
/*............................................................................*/
         };

         if ( stp->cyl[0] == ONE ) /* inner pipe cooling */
            ttub = .60*tinn + .40*tfld;
         else
            ttub = tinn;

         ( stp->uexp ) = INI_UDEXP1;
      }
      else /* if ( opp->n[7] == null ) [ no inner condct ( pipe ) cooling ] */
      {
         if ( stp->cyl[4] == ONE )  /* floating mantle temperature */
            tjck = .30*tinn + .70*tenv;
         else
            tjck = thp->tjck;

         if ( stp->cyl[3] == ONE )  /* mantle of finite wall thickness */
            tout = .01*tinn + .99*tjck;
         else
            tout = tjck;

         if ( stp->cyl[2] == ONE )  /* lossy dielectric */
         {
            tdel = .95*tinn + .05*tout;
/*............................................................................*/
# if INI_INITRS != 0
/* reduce dielectric losses in a first approximation */
/* [ pre-shaping ] - which will be corrected later */
            ( elp->tge ) *= 1.0e-02;
# endif /* INI_INITRS != 0 */
/*............................................................................*/
         };
         ( stp->uexp ) = INI_UDEXP0;
      };
/*............................................................................*/
/* copy these temperatures [ simultaneously counting the nodes ]: */

      ( cdp->ndp ) = -ONE;
      ( cdp->ndm ) = -ONE;

      nds_ = null;

      if ( opp->n[7] == ONE ) /* inner pipe cooling */
         ( thp->tfix[nds_] ) = tfld;

      if ( stp->cyl[0] == ONE )
      {
         ( thp->temp[nds_] ) = ttub;
         ( cdp->nde[1] ) = nds_;  /* <- cooling tube [ 'pipe'; ttub ] */
         nds_++;
      };

   /* if ( stp->cyl[1] == ONE ) - allways true */
         ( thp->temp[nds_] ) = tinn;

      cdp->nde[2] = nds_;         /* <- temperature sensed node [ tinn ] ... */

      if ( opp->n[4] == ONE )     /* forced convection between conductors */
         ( thp->tfix[nds_] ) = tdel; /* [ tdel = mean gas temperature ] */
      nds_++;
      
      if ( stp->cyl[2] == ONE ) /* lossy dielectric */
      {
         ( thp->temp[nds_] ) = tdel;
         ( cdp->nde[3] ) = nds_;      /* <- dielectric [ tdel ] */
         nds_++;
      }
      else /* if ( opp->n[4] == ONE ) - forced convection within waveguide */
         ( thp->tfix[nds_] ) = tdel;  /* [ tdel = mean gas temperature ] */

      if ( stp->cyl[3] == ONE ) /* mantle with finite wall thickness */
      {
         ( thp->temp[nds_] ) = tout;
         ( cdp->nde[4] ) = nds_;      /* <- mantle [ tout ] */
         nds_++;
      };

      if ( stp->cyl[4] == ONE ) /* floating mantle temperature */
      {
         ( thp->temp[nds_] ) = tjck;
         ( thp->tfix[nds_] ) = tenv;


         if ( stp->cyl[3] == null ) /* mantle, vanishing wall thickness */
            ( cdp->nde[4] ) = nds_; /* <- mantle [ tjck ] */

         ( cdp->nde[5] ) = nds_;
         nds_++;
      }
      else /* if ( stp->cyl[4] == null  ) - fixed mantle temperature */
         ( thp->tfix[nds_- ONE] ) = tjck;
/*...........................................................................*/
# if HLS_SDNODE == 1 /* use side nodes */
/* outer conductor [ mantle ] in thermal contact to one side: */

      if ( stp->cyl[5] == ONE ) /* side conditions, pipe */
      {
         cdp->ndp = cdp->nde[2];   /* <- inner conductor [ tinn ] */

         if (( opp->n[8] == TWO )\
	   ||( opp->n[8] == THREE ))
         {
            ( thp->tfix[nds_] ) = trfp; /* fixed [ = tsdp ] */
            ( thp->temp[nds_] ) = .5*( tinn + tsdp );
            ( cdp->nde[6] ) = nds_;
            nds_++;
         }
	 else if (( opp->n[9] == TWO )\
                ||( opp->n[9] == THREE ))
         {
            ( thp->temp[nds_] ) = ( thp->trfp );
            ( cdp->nde[7] ) = nds_;
            nds_++;
         };
      };

      if ( stp->cyl[6] == ONE ) /* side conditions, mantle */
      {
         cdp->ndm = cdp->nde[5];   /* <- mantle [ tjck ] */

         if (( opp->n[8] == ONE )\
	   ||( opp->n[8] == THREE ))
         {
            ( thp->tfix[nds_] ) = tsdm; /* fixed [ = tsdm ] */
            ( thp->temp[nds_] ) = .5*( tjck + tsdm );
            ( cdp->nde[8] ) = nds_;
            nds_++;
         }
	 else if (( opp->n[9] == ONE )\
                ||( opp->n[9] == THREE ))
         {
            ( thp->temp[nds_] ) = ( thp->trfm );
            ( cdp->nde[9] ) = nds_;
            nds_++;
         };
      };
# endif
/*...........................................................................*/
   }; /* end if ( opp->n[1] == ONE ): coaxial waveguide */
/*............................................................................*/
   nodes = nds_; /* the number of thermal nodes */
   ( cdp->nodes ) = nodes;
/*...........................................................................*/
# if INI_DEBUG == 1

   if (( state->ini_skp[1] != 's' )
     &&( state->hcd_skp[1] != 'S' ))
   {
      fprintf ( stdout, "\n Message from job %s, function %s :",
         lotos (( state->job ), null ), __func__ );
      fprintf ( stdout, "\n The number of nodes counted is %d .", nodes+1 );
      fprintf( stdout, "\n\n Please acknowledge:" );
      fprintf( stdout, "\n Enter any character"
         " [ enter 's' to skip this message ]: " );

      scanf( "%s", &state->ini_skp[1] );
   };
# endif 
/*............................................................................*/
/* degenerate cylinder structure: */

   if ( nodes <= null )
   { 
      if (( stp->uif ) != 't' )
      {
         fprintf( logfle,
            "\nError message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf( logfle,
            "\nDegenerate heat conductance matrix !" );
         fprintf( logfle,
            "\n[ Odd operating conditions "
            "or strange input parameters (?)" );
         fprintf( logfle,
            "\n- Please check parameters in log file " );
         fprintf( logfle,
            "par.log%s.]\n ", ( lotos(( stp->job ), null )));

         fclose( logfle );

         fprintf( errfle,
            "\nError message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf( errfle,
            "\nDegenerate heat conductance matrix !" );
         fprintf( errfle,
            "\n[ Odd operating conditions "
            "or strange input parameters (?)" );
         fprintf( errfle,
            "\n- Please check parameters in log file " );
         fprintf( errfle,
            "par.log%s.]\n ", ( lotos(( stp->job ), null )));

         fclose( errfle );
      };
/*...........................................................................*/
# if INI_DEBUG == 1

      if (( state->ini_skp[2] != 's' )
        &&( state->hcd_skp[2] != 'S' ))
      {
         fprintf ( stdout, "\n Error message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf ( stdout, "\n The number %d of formerly counted nodes differs"
            "\n from the number %d of re-defined nodes !", nodes+1, nds_+1 );

         fprintf( stdout, "\n\n Please acknowledge:" );
         fprintf( stdout, "\n Enter any character"
            " [ enter 's' to skip this message ]: " );

         scanf( "%s", &state->ini_skp[2] );
      };
# else
/*...........................................................................*/
      fprintf( stderr,
         "\n Error message from job %s, function %s :", \
         lotos (( state->job ), null ), __func__ ); 
      fprintf( stderr,
         "\n Degenerate heat conductance matrix !" );
      fprintf( stderr,
         "\n [ Odd operating conditions "
         "or strange input parameters (?)" );
      fprintf( stderr,
         "\n - Please check parameters in log file " );
      fprintf( stderr,
         "par.log%s.]\n\n", ( lotos(( stp->job ), null )));

      exit( EXIT_FAILURE );

# endif
/*...........................................................................*/
   }
   else
      rank = 2*nodes;
/*............................................................................*/
/* determine reference temperature [ ZERO < tref = maximum{fixed temps} ]: */

   tref = GIANT_VAL;
   nds_ = null; do
   {
/*
      PRINTF( lotos( nds_,null), "tfix=", ( thp->tfix[nds_] ));
*/
      if (( GRTOLRC < thp->tfix[nds_] )
        &&( thp->tfix[nds_] < tref ))
      {	
         tref = thp->tfix[nds_];
      };
      nds_++;
   } while ( nds_ < nodes );
/*............................................................................*/
/* transfer electric and initialized thermal parameters into structures: */

   ( elp->frn ) = frn;   /* number of frequency points */
   ( elp->frq0 ) = frq0; /* the first computed frequncy */
   ( elp->frq1 ) = frq1; /* the last computed frequency */

   ( elp->freq ) = frq0;
   ( elp->cwpr ) = cwpr;
/*............................................................................*/
/* initial temperatures: */

   ( thp->tdel ) = tdel;
   ( thp->tinn ) = tinn;
   ( thp->tjck ) = tjck;

   ( cdp->nodes ) = nodes;
   ( cdp->rank ) = rank;

   if (( opp->n[1] ) == ONE ) /* coaxial waveguide */
   {
      ( thp->ttub ) = ttub;
      ( thp->tout ) = tout;

      ( thp->sdedp ) = ZERO;
      ( thp->sdlsp ) = ZERO;
   };
/*............................................................................*/
# if INI_INITRS != 0
/* [ pre-shaping node temperature profile ] */
/*...........................................................................*/
   if (( stp->uif ) == 't' )
   {
      strcpy(( dsp->messge ), "[pre-shaping_temperature_profile]" );
      ( dsp->option ) = messge;
      dsplay( dsp );
      ( dsp->option ) = cursor;
   }
   else
   {
      fprintf( logfle, "\n%s", "pre-shaping temperature profile" );
      ( stp->fleps ) = ftell( logfle );
      ( dsp->fleps ) = ftell( logfle );
      ( dsp->option ) = percent;
   };
/*...........................................................................*/
   ( dsp->range ) = INI_INITRS;

   ii = null; jj = null; rtn = ONE; do
   {
/*............................................................................*/
/* approximate resistive and dielectric losses ( elp->lss*) for initial data: */

      ( stp->onerr ) = 's';       /* on error: don't exit [ 's'ilent return ] */
/*............................................................................*/
      elp = losses( stp );        /* first loss approximation                 */
/*..............................*/
      rtn = ( elp->rtn );

      if ( null < rtn )
         goto nexttrial;
/*............................................................................*/
      UPDHCRR( opp->n[1] );
/*............................................................................*/
/* approximate mutual heat conductances ( cdp->mhc[][] ) for initial data: */

      ( stp->onerr ) = 's'; /* on error: don't exit [ 's'ilent return ] */
/*............................................................................*/
      cdp = hcndct( stp );        /*                                          */
/*..............................*/
      if ( nodes != cdp->nodes ) 
      {
         if (( stp->uif ) != 't' )
         {
            fprintf( errfle,
               "\nError message from job %s, function %s :", \
               lotos (( state->job ), null ), __func__ ); 
            fprintf( errfle,
               "\nInconsistent node number !" );
            fprintf( errfle,
               "\n[ This is probably a bug in HELIOS source code." );
            fprintf( errfle,
               "\n- Please send a report to <contact@sfenx.de>" );
            fprintf( errfle,
               "\nannexing your parameter log file par.log%s.]\n ",
               ( lotos(( stp->job ), null )));

            fclose( errfle );
         };

         fprintf( stderr, 
            "\n Error message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf( stderr,
            "\n Inconsistent node number !" );
         fprintf( stderr,
            "\n [ This is probably bug in the source code." );
         fprintf( stderr,
            "\n - Please send a report to <contact@sfenx.de>" );
         fprintf( stderr,
            "\n annexing your parameter log file par.log%s.]\n\n",
            ( lotos(( stp->job ), null )));

         exit( EXIT_FAILURE );
      };

      rtn = ( cdp->rtn );

      if ( null < rtn )
         goto nexttrial;

      if ( nodes != cdp->nodes )
      {
         if (( stp->uif ) != 't' )
         {
            fprintf( errfle,
               "\nError message from job %s, function %s :", \
               lotos (( state->job ), null ), __func__ ); 
            fprintf( errfle,
               "\nInconsistent node number !" );
            fprintf( errfle,
               "\n[ This is probably bug in HELIOS source code." );
            fprintf( errfle,
               "\n - Please send a report to <contact@sfenx.de>" );
            fprintf( errfle,
               "\nannexing your parameter log file par.log%s.]\n ",
               ( lotos(( stp->job ), null )));

            fclose( errfle );
         };

         fprintf( stderr,
            "\n Error message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf( stderr,
            "\n Inconsistent node number !" );
         fprintf( stderr,
            "\n [ This is probably bug in HELIOS source code." );
         fprintf( stderr,
            "\n - Please send a report to <contact@sfenx.de>" );
         fprintf( stderr,
            "\n annexing your parameter log file par.log%s.]\n\n",
            ( lotos(( stp->job ), null )));

         exit( EXIT_FAILURE );
      };
/*............................................................................*/
/* approximate node temperatures ( thp->temp[] ) for initial data: */

      ( stp->onerr ) = 's'; /* on error: don't exit [ 's'ilent return ] */
/*............................................................................*/
      thp = kelvin( stp );        /* compute node temperatures */
/*..............................*/
      rtn = ( thp->rtn );

      if ( null < rtn )
         goto nexttrial;

     nexttrial: ii++;

      ( dsp->state ) = ii;
/*............................................................................*/
      dsplay( dsp );        /* running cursor function */
/*........................*/
      if ( null < rtn )
      {
         jj = null;

         if ( ii == ( INI_INITRS / 2 ))
         {
            quot = pow( 2., - ( double ) INI_INITSC );
            ( elp->cwpr ) = quot*cwpr;

            ( thp->tdel ) = tdel;
            ( thp->tinn ) = tinn;
            ( thp->tjck ) = tjck;
            ( thp->trfm ) = trfm;

            if (( opp->n[1] ) == ONE ) /* coaxial waveguide */
            {
               ( thp->tfld ) = tfld;
               ( thp->ttub ) = ttub;
               ( thp->tout ) = tout;
               ( thp->trfp ) = trfp;
            };
         }
         else
         {
            if ( ii < ( INI_INITRS / 2 ))
               quot = pow( 2., ( double ) INI_INITSC );
            else if (( INI_INITRS / 2 ) < ii )
               quot = pow( 2., - ( double ) INI_INITSC );

            ( elp->cwpr ) *= quot;
         };
      } /* end if ( rtn == ONE ) */
      else /* re-iterate loop [ at least number INI_REPEAT times ] */ 
      {
         if ( jj <= INI_REPEAT )
            UPDTEMP( opp->n[1] ); /* update node temperatures */
         jj++;
      };
   } while (( jj < INI_REPEAT )
          &&( ii < INI_INITRS ));

   if ( INI_INITRS <= ii )
   {
      if (( stp->uif ) != 't' )
      {
         fprintf( errfle,
            "\nError message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf( errfle,
            "\nDuring pre-shaping, " );
         fprintf( errfle,
            "unhappy with given initial data.\nCan't start HELIOS !" );
         fprintf( errfle,
            "\n[ Please check parameters in log file " );
         fprintf( errfle,
            "par.log%s.", ( lotos(( stp->job ), null )));
         if (( opp->n[2] ) == null ) /* fixed CW power */
            fprintf( errfle,
               "\nEventually modify initial temperature, s6," );
         else /* if (( opp->n[2] ) == ONE ): fixed inn cdc temp */
            fprintf( errfle,
               "\nEventually modify initial CW power input, s3," );
         fprintf( errfle,
            "\nand restart program.]\n" );

         fclose ( errfle );
      };

      fprintf( stderr,
         "\n Error message from job %s, function %s :", \
         lotos (( state->job ), null ), __func__ ); 
      fprintf( stderr,
         "\n During pre-shaping, " );
      fprintf( stderr,
         "unhappy with given initial data.\n Can't start HELIOS !" );
      fprintf( stderr,
         "\n [ Please check parameters in log file " );
      fprintf( stderr,
         "par.log%s.", ( lotos(( stp->job ), null )));

      if (( opp->n[2] ) == null ) /* fixed CW power */
         fprintf( stderr,
            "\n Eventually modify initial temperature, s6," );
      else /* if (( opp->n[2] ) == ONE ): fixed inn cdc temp */
         fprintf( stderr,
            "\n Eventually modify initial CW power input, s3," );
      fprintf( stderr,
         "\n and restart program.]\n\n" );

      exit( EXIT_FAILURE );
   }
   else
   {
      ( dsp->range ) = INI_INITRS;
      
      while( ii < INI_INITRS )
      {
         ( dsp->state ) = ++ii;
/*............................................................................*/
         dsplay( dsp );        /* terminate running cursor */
/*...........................*/
      };
   };
/*...........................................................................*/
/* scale & store initial temperature profile: */

   if ( opp->n[2] == ONE ) /* fixed inner conductor temperature */
      quot = ( tinn - tref ) / ( thp->tinn - tref );
   else
      quot = cwpr / ( elp->cwpr );

   ( elp->cwpr ) *= quot;

   nds_ = null;
   while ( nds_ < nodes )
   {
      if ( SMALL_VAL < ( thp->temp[nds_] ))
      {
         ( thp->temp[nds_] ) -= tref;
         ( thp->temp[nds_] ) *= quot;
         ( thp->temp[nds_] ) += tref;
      } 
      else
         ( thp->temp[nds_] ) = ZERO;

      ( thp->temp[nds_] ) = ( thp->temp[nds_] );
      nds_++;
   };
/*............................................................................*/
/* store initial node temperatures: */

   ( thp->tdel ) = tdel;
   ( thp->tinn ) = tinn;
   ( thp->tjck ) = tjck;

   if (( opp->n[1] ) == ONE ) /* coaxial waveguide */
   {
      ( thp->ttub ) = ttub;
      ( thp->tout ) = tout;
   };
   
   if (( stp->opmrk[1] ) == ONE ) /* side conditions */
   { ; 
/*
      if (( opp->n[8] == ONE )\
        ||( opp->n[8] == THREE ))
         ( thp->trfm ) = tsdm;
	 
      if (( opp->n[8] == TWO )\
        ||( opp->n[8] == THREE ))
         ( thp->trfp ) = tsdp;
	 
      if (( opp->n[9] == ONE )\
        ||( opp->n[9] == THREE ))
         ( thp->trfm ) = tjck;

      if (( opp->n[9] == TWO )\
        ||( opp->n[9] == THREE ))
         ( thp->trfp ) = tinn;
*/
   };
/*............................................................................*/
/* copy dielectric loss factor [ which was neglected during pre-shaping ] */

   ( elp->tge ) = tge;
/*............................................................................*/
/* node temperature profile pre-shaping terminated */
/*............................................................................*/
# endif /* INI_INITRS != 0 */
/*............................................................................*/
   ( stp->upd ) = upd;
   ( cdp->rank ) = rank;
   ( cdp->nodes ) = nodes;

   ( elp->cwpr ) = cwpr; /* the CW power */
   ( elp->frn ) = frn; /* the number of frequency points */
   ( elp->frq0 ) = frq0; /* lower frequency [Hz] */
   ( elp->frq1 ) = frq1; /* upper frequency [Hz] */
   ( elp->cwpr ) = cwpr; /* CW power [Watts] */
   ( elp->freq ) = freq; /* CW power [Watts] */
   ( elp->tge ) = tge; /* dielectric loss tangent */

   ( thp->tenv ) = tenv; /* environment temperature */
   ( thp->tfld ) = tfld; /* coolant fluid temperature */
   ( thp->ttub ) = ttub; /* cooling pipe temperature */
   ( thp->tdel ) = tdel; /* temperature, dielectric */
   ( thp->tinn ) = tinn; /* inner conductor temperature */
   ( thp->tout ) = tout; /* outer conductor temperature */
   ( thp->tjck ) = tjck; /* mantle temperature */
   
   ( thp->tsdp ) = tsdp; /* fixed side temperature, inner conductor */
   ( thp->tsdm ) = tsdm; /* fixed side temperature, mantle */
   ( thp->trfp ) = trfp; /* side temperature, inner conductor */
   ( thp->trfm ) = trfm; /* side temperature, mantle */
   
   ( thp->tref ) = tref; /* reference temperature */
   
   ( stp->fleps ) = ftell( logfle );
   ( dsp->fleps ) = ftell( logfle );

   fclose ( logfle );

   return stp;
}
/*============================================================================*/
