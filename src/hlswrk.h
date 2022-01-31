/* [ file: hlswrk.h ] */
/*******************************************************************************
*                                                                              *
*   Body of function hlswrk(*); HELIOS, release v1.0r1                         *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function iterates the temperature function kelvin(*), the loss        *
*   function losses(*), and the heat conductances function hcndct(*).          *
*   It returns the final results and plot data by function values(*).          *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: January 31, 2022 ]                        <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef HLS_SDNODE
   # define HLS_SDNODE 0  /* [0] / 1 [don't] use side nodes */
# endif
/*............................................................................*/
# if HLS_SDNODE == 1
   # include "./WRKMCS1.M"
# else
   # include "./WRKMCS0.M"
# endif
/*----------------------------------------------------------------------------*/
# define HLS_USSDLS 0
# define WRK_DEBUG 0
/*============================================================================*/

HELIOSSTATE *\
hlswrk( HELIOSSTATE *state )
{
/* allusions: */

/* extern HELIOSSTATE helios; *//* [hlswrk] menu state transfer structure */

/* declarations: */

   static HELIOSSTATE
     *stp;

   static OPERATIONS
     *opp;

   static MATERIALS
     *map;

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

   static RESULTS
     *rsp;

   static DSPLAY
     *dsp = NULL;

   static FILE
     *logfle = NULL,
     *errfle = NULL;
/*
   static char
      ptr[STS_SIZE] = {null},
    **endp = null;
*/
   static char
      sgn = null;

   static short
      ii = null,
      jj = null,
      kk = null,
      mm = null,
      frc = null,
      frn = null,
      nds_ = null,
      nodes = null,
      rank = null,
      mxitr = null,
      fstmm = null,
      sense = null; /* temperature sensed node [ inner conductor, e.g. ] */

   static size_t
      bufsize =  null;

   static double
      scl = ZERO,
      upd = ZERO, /* update coefficient [ used in macro UMDTEMP( ) ] */
      hcm = ZERO, /* heat conductivity, mantle */
      hpp = ZERO, /* heat conductivity, cooling pipe */
      frq0 = ZERO, /* starting frequeny */
      frq1 = ZERO, /* stopping frequency */
      freq = ZERO, /* actual frequency */
      tinn = ZERO, /* inner conductor temperature */
      tref = ZERO, /* reference temperature */
      hres = ZERO, /* any heat resistance */
      dtmp = ZERO, /* any temperature difference */
      quot = ZERO,
      norm = ZERO,
      rscl = 1.;   /* hcrr rescale factor */
      
   static const char /* dsplay(*) options */
      start = 's',   /* print 's'tart message */
      messge = 'm',  /* write 'm'essage under running cursor */
      cursor = 'c',  /* running 'c'ursor with percent display */
      percent = 'p'; /* only 'p'ercent display */

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
# if (( WRK_OPRLOG == 1 ) \
    ||( WRK_TMPLOG == 1 ) \
    ||( WRK_CHCLOG == 1 ))

   time_t
      time( time_t *timer );

   void
      store_val( HELIOSSTATE *state, FILE *stream, char opt );

# endif /* create log files WRK_... */
/*----------------------------------------------------------------------------*/
/* copy state [ structure pointer ]: */

   stp = state;
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
/* initialize structure pointers: */

   opp = ( stp->opp );
   map = ( stp->map );
   dmp = ( stp->dmp );
   elp = ( stp->elp );
   thp = ( stp->thp );
   hcp = ( stp->hcp );
   cdp = ( stp->cdp );
   rsp = ( stp->rsp );
   
   ( rsp->opp ) = opp;
   ( rsp->map ) = map;
   ( rsp->dmp ) = dmp;
   ( rsp->elp ) = elp;
   ( rsp->thp ) = thp;
   ( rsp->hcp ) = hcp;
   ( rsp->cdp ) = cdp;
/*............................................................................*/
/* recall side temperatures if such are fixed: */

   if (( opp->n[8] == ONE )\
     ||( opp->n[8] == THREE ))
   {
      ( thp->trfm ) = ( thp->tsdm );
   };

   if (( opp->n[8] == TWO )\
     ||( opp->n[8] == THREE ))
   {
      ( thp->trfp ) = ( thp->tsdp );
   };
/*............................................................................*/
/* reset result variables: */

   kk = null; do
   {
      ( rsp->freq[kk] ) = ZERO;
      ( rsp->cwpr[kk] ) = ZERO;
      
      ( rsp->lsso[kk] ) = ZERO;
      ( rsp->lssi[kk] ) = ZERO;
      ( rsp->lssd[kk] ) = ZERO;
      ( rsp->lsdo[kk] ) = ZERO;
      ( rsp->lsdi[kk] ) = ZERO;
      ( rsp->lsst[kk] ) = ZERO;

      ( rsp->cao[kk] ) = ZERO;
      ( rsp->rao[kk] ) = ZERO;
      ( rsp->rfo[kk] ) = ZERO;
      ( rsp->rro[kk] ) = ZERO;
      ( rsp->cai[kk] ) = ZERO;
      ( rsp->rai[kk] ) = ZERO;
      ( rsp->rfi[kk] ) = ZERO;
      ( rsp->rri[kk] ) = ZERO;

      ( rsp->tjck[kk] ) = ZERO;
      ( rsp->tinn[kk] ) = ZERO;
      ( rsp->tdel[kk] ) = ZERO;
      ( rsp->tout[kk] ) = ZERO;
      ( rsp->ttub[kk] ) = ZERO;
      ( rsp->trfm[kk] ) = ZERO;
      ( rsp->trfp[kk] ) = ZERO;
      ( rsp->tmxm[kk] ) = ZERO;
      ( rsp->tmxp[kk] ) = ZERO;
      
      ( rsp->grdi[kk] ) = ZERO;
      ( rsp->grdo[kk] ) = ZERO;
      
      ( rsp->sdifp[kk] ) = ZERO;
      ( rsp->sdifm[kk] ) = ZERO;
      ( rsp->sdofp[kk] ) = ZERO;
      ( rsp->sdofm[kk] ) = ZERO;
      ( rsp->sdedp[kk] ) = ZERO;
      ( rsp->sdedm[kk] ) = ZERO;
      ( rsp->sdhcp[kk] ) = ZERO;
      ( rsp->sdhcm[kk] ) = ZERO;
   } while(( ++kk ) < FREQPTS );
/*............................................................................*/
/* heat currents rescale factor: */
# ifdef HLS_RESCLE
   rscl = HLS_RESCLE/( dmp->lwgd );
# endif
/*............................................................................*/
/* update weight factor */

   upd = ( stp->upd );
/*............................................................................*/
/* copy [ initial ] frequencies, CW power, temperatures etc. */

   frn = ( elp->frn ); /* the number of frequency points */

   frq0 = ( elp->frq0 ); /* lower frequency [Hz] */
   frq1 = ( elp->frq1 ); /* upper frequency [Hz] */
   freq = ( elp->freq ); /* CW power [Watts] */

   tinn = ( thp->tinn ); /* inner conductor temperature */

   tref = ( thp->tref ); /* pertinent reference temperature */
   
   rank = ( cdp->rank );
   nodes = ( cdp->nodes );
   sense = ( cdp->nde[2] ); /* temperature sensed node [ tinn fixed ] */
/*............................................................................*/
/* store initial node temperatures: */
/* terminal stage: */

   ( stp->opmrk[0] ) = ONE;

   nds_ = null; do
   {
      ( thp->temp_init[nds_] ) = ( thp->temp[nds_] );
      nds_++;
   } while ( nds_ < nodes );
   while ( nds_ < rank )
   {
      ( thp->temp_init[nds_] ) = ( thp->temp[nds_] );
      nds_++;
   };
/*............................................................................*/
   ( stp->onerr ) = null; /* on error: exit with message */
   ( elp->onerr ) = null;
   ( hcp->onerr ) = null;
   ( thp->onerr ) = null;
/*............................................................................*/
   if ( stp->opmrk[1] == null ) /* no side conditions */
   {
      mxitr = 10;
      fstmm = null;
   }
   else /* if ( stp->opmrk[1] == ONE ) - side conditions */
   {
      mxitr = WRK_MXITSD;
      fstmm = mxitr - 3;
   };
/*...........................................................................*/

/* display start message */
   if ( stp->uif == 't' )
   {
      dsp->option = start;
      dsplay( dsp );
      dsp->option = messge;
      strcpy( dsp->messge, "[frequency_sweep]" );
      dsplay( dsp );
      dsp->option = cursor;
   }
   else
   {
      fprintf( logfle, "\n%s", "frequency sweep" );
      ( dsp->fleps ) = ftell( logfle );
      ( dsp->option ) = percent;
   };
/*............................................................................*/
/* NOW START THE SWEEP */
/*............................................................................*/
   dsp->range = frn;

   if ( elp->frq0 <= elp->frq1 )
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

         if ( opp->n[2] ) /* fixed inner conductor temperature */
            ( elp->cwpr ) = ( elp->cwpr )*sqrt( freq / ( elp->freq ));

         freq = ( elp->freq );
      };
/*...........................................................................*/
/* first loop [ CW power sweep with given inner conductor temperature tinn ]*/

      ii = null; do
      {
         if (( thp->temp[sense] ) <= tref )
            ( thp->temp[sense] ) = tref + WRK_DTEXLP;
/*...........................................................................*/
/* CW power scaling: */

         if ( opp->n[2] ) /* fixed [ inner, if coax ] conductor temperature */
         {
            quot = ( tinn - tref )/(( thp->temp[sense] ) - tref );
            quot = pow( quot, ( stp->uexp ));
            ( elp->cwpr ) *= quot;

	    nds_ = null; do
            {
               if ( SMALL_VAL < ( thp->temp[nds_] ))
               {
                  ( thp->temp[nds_] ) -= tref;
                  ( thp->temp[nds_] ) *= quot;
                  ( thp->temp[nds_] ) += tref;
               } 
               else
                  ( thp->temp[nds_] ) = ZERO;
            } while(( ++nds_ ) < nodes );
         };
/*............................................................................*/
/* second loop */

	 if ( kk == null )
            mm = null;
         else
            mm = fstmm;

         do /* [ while ( mm++ < mxitr ) ] */
         {
/*...........................................................................*/
/* smoothly increase scl from 0. to 1.  */ 
/* [ slowly in the neighbourhood of 1.] */   

            if (( stp->opmrk[1] ) == ONE )
            {

               scl = (( double ) mm )/(( double) mxitr );
               scl = pow( fabs( 2*scl - scl*scl ), .5 );
	       
/* other possible choices: */
/*
               scl = sin( PI*scl/2. );
               scl = fabs( 2*scl - scl*scl );
*/	       
	       ( cdp->scl ) = scl;
            };
/*...........................................................................*/
/* third loop: power sweep [ fixed CW power; apprx temperature steady stp ] */
/* transfer parameters to function hcndct(*) */

            jj = null; do
            {
/*............................................................................*/
/* cycle: UPDTEMP(*) -> losses(*) -> UPDHCRR(*) -> hcncdt(*) -> kelvin(*) ->..*/
/* update node temperatures: */

               upd = ( stp->upd );
               UPDTEMP( opp->n[1] );
/*............................................................................*/
/* resistive and dielectric losses: */

               elp = losses( stp );
/*............................................................................*/
/* update heat currents */

               upd = ( stp->upd );
               UPDHCRR( opp->n[1] );          
/*............................................................................*/
/* parameters transferred to function hcndct(*) */
# if HLS_USSDLS == 1

               if ( stp->opmrk[1] == ONE )
               {	       
                  if ( opp->n[1] == null ) /* rectangular waveguide */
		  {
                     if ( stp->cyl[3] == ONE )
		        ( thp->sdlsm ) = ( elp->cwpr )*( elp->lssfm[frc] );
                  }
                  else /* if ( opp->n[1] == ONE ) - coaxial line */
                  { 
                    if ( stp->cyl[5] == ONE )
		       ( thp->sdlsp ) = ( elp->cwpr )*( elp->lssfp[frc] );

                    if ( stp->cyl[6] == ONE )
		       ( thp->sdlsm ) = ( elp->cwpr )*( elp->lssfm[frc] );

                  }; /* end if (( opp->n[1] ) == ONE ) - coaxial line */
               }; /* end if (( stp->opmrk[1] ) == ONE ) */
# endif
/*............................................................................*/
/* update mutual heat conductances: matrix mhc[][]: */
/*...........................................................................*/
# if WRK_DEBUG == 1
/* check function hcndct(*) */

      if (( state->wrk_skp[1] != 's' )
        &&( state->wrk_skp[1] != 'S' ))
      {
          fprintf ( stdout, "\n Message from job %s, function %s :",
             lotos (( state->job ), null ), __func__ );
          fprintf ( stdout, "\n The mantle temperature is "
             "%.7e C.", ( thp->tjck ) - CELSIUS_TO_KELVIN );
          fprintf ( stdout, "\n The side temperature is "
             "%.7e C.", ( thp->trfm ) - CELSIUS_TO_KELVIN );
          fprintf( stdout, "\n\n Please acknowledge:" );
          fprintf( stdout, "\n Enter any character"
             " [ enter 's' to skip this message ]: " );

          scanf( "%s", &state->wrk_skp[1] );
       };
# endif 
/*............................................................................*/
               cdp = hcndct( stp );             /*                            */
/*............................................*/
# if WRK_DEBUG == 1

      if (( state->wrk_skp[2] != 's' )
        &&( state->wrk_skp[2] != 'S' ))
      {
          fprintf ( stdout, "\n Message from job %s, function %s :",
             lotos (( state->job ), null ), __func__ );
          fprintf ( stdout, "\n The mantle temperature is "
             "%.7e C.", ( thp->tjck ) - CELSIUS_TO_KELVIN );
          fprintf ( stdout, "\n The side temperature is "
             "%.7e C.", ( thp->trfm ) - CELSIUS_TO_KELVIN );
          fprintf( stdout, "\n\n Please acknowledge:" );
          fprintf( stdout, "\n Enter any character"
             " [ enter 's' to skip this message ]: " );

          scanf( "%s", &state->wrk_skp[2] );
       };
# endif 
/*............................................................................*/
/* update temperatures: */

               thp = kelvin( stp );      
/*............................................................................*/
/* check for convergence: */

               TMPNORM( opp->n[1], WRK_USENRM ); /* temperature deviation norm*/

           /*  printf( "\n t-norm = % .15e [ please acknowledge ]", norm );
               scanf( "\n%s", ptr ); */

	       jj++;
            } while (( WRK_DTINLP < norm )
	           &&( jj < WRK_MAXITR ));

            if ( jj == WRK_MAXITR )
            {
               if (( stp->uif ) != 't' )
               {
                  fprintf( errfle,
                     "\nError message from job %s, function %s :", \
                     lotos (( stp->job ), null ), __func__ );
                  fprintf( errfle,
                     "\non step ( ii, kk, mm ) = ( %d, %d, %d ):", \
                     ii, kk, mm ); 
                  fprintf( errfle,
                     "\nToo many iterations !" );
                  fprintf( errfle,
                     "\n[ Unhappy with given initial data," );
                  fprintf( errfle,
                     "\n  check and modify parameter input," );
                  fprintf( errfle,
                     " cf. file par.log%s. ]\n", ( lotos(( stp->job ), null )));

                  fclose( errfle );
               };

               fprintf( stderr,
                  "\n Error message from job %s, function %s :", \
                  lotos (( stp->job ), null ), __func__ );
               fprintf( stderr,
                  "\n on step ( ii, kk, mm ) = ( %d, %d, %d ):", \
                  ii, kk, mm ); 
               fprintf( stderr,
                  "\n Too many iterations !" );
               fprintf( stderr,
                  "\n [ Unhappy with given initial data," );
               fprintf( stderr,
                  "\n   check and modify parameter input," );
               fprintf( stderr,
                  " cf. file par.log%s. ]\n\n", ( lotos(( stp->job ), null )));

               exit( EXIT_FAILURE );
            }
         } while ( mm++ < mxitr );
/*............................................................................*/
# if WRK_USENRM == 1
         norm = fabs( tinn - ( thp->temp[sense] ));
         norm /= CELSIUS_TO_KELVIN;
# elif WRK_USENRM == 2
         norm = pow(( tinn - ( thp->temp[sense] )), TWO );
         norm = sqrt( norm ) / CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
# if SDNODE == 1

         if (( stp->opmrk[1] ) == ONE )
         {
            if ( opp->n[1] == 0 ) /* rectangular waveguide */
            {
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "mantle", ( - thp->sdedm ), ZERO, ONE );
/*............................................................................*/
/* endothermie check [ exlude tjck <= trfm ]: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "mantle", thp->sdifm, thp->sdofm, ONE );
            }
            else if ( opp->n[1] == ONE ) /* coaxal waveguide */
            {
               HMGCHK( "pipe", ( - thp->sdedp ), ZERO, ONE );
               HMGCHK( "mantle", ( - thp->sdedm ), ZERO, ONE );
               ENDOTH( "pipe", thp->sdifp, thp->sdofp, ONE );
               ENDOTH( "mantle", thp->sdifm, thp->sdofm, ONE );
            };
         };              
# endif
/*............................................................................*/
         ii++;
      } while (( opp->n[2] )
             &&( WRK_DTEXLP < norm )
             &&( ii < WRK_MAXITR ));

      if ( ii == WRK_MAXITR )
      {
         if (( stp->uif ) != 't' )
         {
            fprintf( errfle,
               "\n\nError message from job %s, function %s :", \
               lotos (( stp->job ), null ), __func__ );
            fprintf( errfle,
               "\non step ( ii, kk, mm ) = ( %d, %d, %d ):", \
               ii, kk, mm ); 
            fprintf( errfle,
               "\nToo many iterations !" );
            fprintf( errfle,
               "\n[ Unhappy with given initial data," );
            fprintf( errfle,
               "\n  check and modify parameter input," );
            fprintf( errfle,
               " cf. file par.log%s. ]\n", ( lotos(( stp->job ), null )));

            fclose( errfle );
         };

         fprintf( stderr,
            "\n\n Error message from job %s, function %s :", \
            lotos (( stp->job ), null ), __func__ );
         fprintf( stderr,
            "\n on step ( ii, kk, mm ) = ( %d, %d, %d ):", \
            ii, kk, mm ); 
         fprintf( stderr,
            "\n Too many iterations!" );
         fprintf( stderr,
            "\n [ Unhappy with given initial data," );
         fprintf( stderr,
            "\n   check and modify parameter input," );
         fprintf( stderr,
            " cf. file par.log%s. ]\n", ( lotos(( stp->job ), null )));

         exit( EXIT_FAILURE );
      }
/*............................................................................*/
      ( rsp->freq[frc] ) = ( elp->freq );
      ( rsp->cwpr[frc] ) = ( elp->cwpr );

      ( rsp->lssi[frc] ) = ( elp->lssi );
      ( rsp->lssd[frc] ) = ( elp->lssd );
      ( rsp->lsso[frc] ) = ( elp->lsso );
      ( rsp->lsst[frc] ) = ( elp->lsst );

      ( rsp->tdel[frc] ) = ( thp->tdel );
      ( rsp->tjck[frc] ) = ( thp->tjck );
/*............................................................................*/
/* RECTANGULAR WAVEGUIDE: */
/*............................................................................*/
      if (( opp->n[1] ) == null )
      {
         if (( stp->cyl[1] ) == ONE )
            ( rsp->tinn[frc] ) = ( thp->tinn );
         else if (( thp->mbe ) == ONE )
         {
            hcm = ( thp->hcm );
            hcm += (( thp->dhm )* \
		    (( thp->tjck ) - CELSIUS_TO_KELVIN ));

            if ( ZERO < hcm )
            {
               hcm *= (( dmp->winn )/(( dmp->hout ) - ( dmp->hinn )) + \
                       ( dmp->hinn )/(( dmp->wout ) - ( dmp->winn )));
	       hcm *=  4.;
	       dtmp = ( elp->lsst )/hcm;

               ( rsp->tinn[frc] ) = ( thp->tjck ) + dtmp;
            }
            else
               ( rsp->tinn[frc] ) = ( rsp->tdel[frc] );
         }; /* end if (( thp->mbe ) == ONE ) */
/*............................................................................*/
         if ( stp->cyl[3] == ONE )
         {
            ( rsp->trfm[frc] ) = ( thp->trfm ); 
            ( rsp->tmxm[frc] ) = ( thp->tmxm );

            ( rsp->sdifm[frc] ) = ( thp->sdifm ); 
            ( rsp->sdofm[frc] ) = ( thp->sdofm ); 
            ( rsp->sdedm[frc] ) = ( thp->sdedm );
	    ( rsp->sdhcm[frc] ) = ( thp->sdhcm );
/*............................................................................*/
/* endothermie check [ warning message ]: */

	    if (( opp->n[8] == ONE )
	      ||( opp->n[8] == 	THREE ))
	    {
               ENDOTH( "mantle", thp->sdifm, thp->sdofm, 1 );
            }
	    else if (( opp->n[9] == ONE )
	      ||( opp->n[9] == 	THREE ))
            {
               ENDOTH( "mantle", thp->sdifm, thp->sdofm, 1 );
            };
         };
      }
/*............................................................................*/
/* COAXIAL WAVEGUIDE: */
/*............................................................................*/
      else if (( opp->n[1] ) == ONE ) /* coaxial line */
      {
         ( rsp->tinn[frc] ) = ( thp->tinn );

         if (( stp->cyl[3] ) == ONE )
            ( rsp->tout[frc] ) = ( thp->tout );
         else if (( thp->mbe ) == ONE )
         {
            if ((( stp->opp->n[6] ) == null )
              &&(( stp->opp->n[7] ) == null ))
            {
               hcm = ( thp->hcm );
               hcm += (( thp->dhm )* \
		      (( thp->tjck ) - CELSIUS_TO_KELVIN ));

               if ( ZERO < hcm )
               {
		  dtmp = ( elp->lsst );
                  dtmp *= log(( dmp->djck )/( dmp->dout ));
                  dtmp /= ( 2.*PI*hcm );

                  ( rsp->tout[frc] ) = ( thp->tjck ) + dtmp;
               }
               else
                  ( rsp->tout[frc] ) = ( thp->tjck );
            }
            else /* heat current may exceed tloss */ 
               ( rsp->tout[frc] ) = ( thp->tjck );
         }; /* end if (( thp->mbe ) == ONE ) */

         if (( opp->n[7] ) == ONE ) /* inner conductor cooling */
         {
            if ( null < ( stp->cyl[null] ))
               ( rsp->ttub[frc] ) = ( thp->ttub );
            else if (( thp->ppe ) == ONE )
            {
               if (( stp->opp->n[6] ) == null )
               {
                  hpp = ( thp->hcp );
                  hpp += (( thp->dhp )* \
                         (( thp->tinn ) - CELSIUS_TO_KELVIN ));

                  if ( ZERO < hpp )
                  {
                     dtmp = ( elp->lsst )*log(( dmp->dinn )/( dmp->dtub ));
                     dtmp /= ( 2.*PI*hpp );
		  
                     if (( thp->tjck ) < ( thp->tinn ))
                        ( rsp->ttub[frc] ) = ( thp->tinn ) - dtmp;
		     else /* heat current may exceed total loss */
                        ( rsp->ttub[frc] ) = ( thp->tinn );
                  }
                  else
                     ( rsp->ttub[frc] ) = ( thp->tinn );
               }
               else /* heat current may exceed total loss */
                  ( rsp->ttub[frc] ) = ( thp->tinn );
            }; /* end if (( thp->ppe ) == ONE ) */

/* characteristic cooling length [ on that */
/* cooling efficiency declines to 1/e ] */

            ( dmp->lfef ) = PI*( thp->vlf )*( thp->cvf )*( thp->mdf );
            ( dmp->lfef ) *= ( QUART*( dmp->dtub )*( dmp->dtub ));

            hres = ( cdp->mhr[null][null] );

            if (( thp->ppe ) == null )
               hres += ( cdp->mhr[null][ONE] );

	    ( dmp->lfef ) *= hres;
	    ( rsp->lfef[frc] ) = ( dmp->lfef );

/* temperature rise on cooled section: */

            dtmp = ZERO;
            if ( ZERO < ( dmp->lfef ))
            {
               dtmp = (( thp->tinn ) - ( thp->tfld ));
/*............................................................................*/
# if WRK_TMPRSE == 1
               dtmp *= (( dmp->ltub )/( dmp->lfef ));
# elif WRK_TMPRSE == 2
               dtmp *= ( 1. - exp( - ( dmp->ltub )/( dmp->lfef )));
# endif
/*............................................................................*/
            };
            ( thp->tflr ) = dtmp;
            ( rsp->tflr[frc] ) = dtmp;
         }; /* end if ( opp->n[7] == ONE ) etc. */
	 
         if ( stp->cyl[5] == ONE )
         {
            ( rsp->trfp[frc] ) = ( thp->trfp ); 
            ( rsp->tmxp[frc] ) = ( thp->tmxp );

            ( rsp->sdifp[frc] ) = ( thp->sdifp );
            ( rsp->sdofp[frc] ) = ( thp->sdofp );
            ( rsp->sdedp[frc] ) = ( thp->sdedp );
            ( rsp->sdhcp[frc] ) = ( thp->sdhcp );
/*............................................................................*/
/* endothermie check [ warning message ]: */

	    if (( opp->n[8] == TWO )
	      ||( opp->n[8] == 	THREE ))
            {
               ENDOTH( "pipe", thp->sdifp, thp->sdofp, 1 );
            }
	    else if (( opp->n[9] == TWO )
	      ||( opp->n[9] == 	THREE ))
            {
               ENDOTH( "pipe", thp->sdifp, thp->sdofp, 1 );
            };
         };
/*............................................................................*/
         if ( stp->cyl[6] == ONE )
         {
            ( rsp->trfm[frc] ) = ( thp->trfm ); 
            ( rsp->tmxm[frc] ) = ( thp->tmxm );

            ( rsp->sdifm[frc] ) = ( thp->sdifm );
            ( rsp->sdofm[frc] ) = ( thp->sdofm );
            ( rsp->sdedm[frc] ) = ( thp->sdedm );
	    ( rsp->sdhcm[frc] ) = ( thp->sdhcm );
/*............................................................................*/
/* endothermie check [ warning message ]: */

	    if (( opp->n[8] == ONE )
	      ||( opp->n[8] == 	THREE ))
            {
               ENDOTH( "mantle", thp->sdifm, thp->sdofm, 1 );
            }
	    else if (( opp->n[9] == ONE )
	      ||( opp->n[9] == 	THREE ))
            {
               ENDOTH( "mantle", thp->sdifm, thp->sdofm, 1 );
            };
/*............................................................................*/
         };
      }; /* end if ( opp->n[1] == ONE ) - coaxial line */

      frc += sgn;
      ( dsp->state ) = ( ++kk );
/*............................................................................*/
      dsplay( dsp );     /* running cursor function */
/*.....................*/
   } while( kk < frn );
/*............................................................................*/

   fclose( logfle );
/*............................................................................*/
/* generate graphics files and store results: */

   ( rsp->fstjob ) = ( stp->fstjob );
   ( rsp->job ) = ( stp->job );
/*............................................................................*/
    rsp = values( stp );      /*                                              */
/*..........................*/

/*............................................................................*/
# if (( WRK_OPRLOG == 1 ) \
    ||( WRK_TMPLOG == 1 ) \
    ||( WRK_CHCLOG == 1 ))

   logfle = fopen(( state->logfle ), "a+" );
# endif
   
# if WRK_OPRLOG == 1
   store_val( stp, logfle, 'o' );
# endif

# if WRK_TMPLOG == 1
   store_val( stp, logfle, 't' );
# endif

# if WRK_CHCLOG == 1
   store_val( stp, logfle, 'c' );
# endif

# if (( WRK_OPRLOG == 1 ) \
    ||( WRK_TMPLOG == 1 ) \
    ||( WRK_CHCLOG == 1 ))

   fclose( logfle );
# endif
/*............................................................................*/
# if WRK_CWPPLT == 1
   ;
# endif /* WRK_CWPPLT == 1 */
/*............................................................................*/
# if WRK_TMPPLT == 1
   ;
# endif /* WRK_TMPPLT == 1 */
/*............................................................................*/
# if WRK_LSSPLT == 1
   ;
# endif /* WRK_LSSPLT == 1 */
/*............................................................................*/
   if (( stp->uif ) == 't' )
   {
      strcpy(( dsp->messge ), "\n Job no " );
      strcat(( dsp->messge ), ( lotos(( stp->job ), null )));
      strcat(( dsp->messge ), " terminated" );

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ) + 11 );
      strncat(( dsp->messge ), " at ", 4 );
      strncat(( dsp->messge ), timeptr, 8 );
      strcat(( dsp->messge ), "\r" );
/*............................................................................*/
/* termination message - display on screen:                                   */

      ( dsp->option ) = start;
      dsplay( dsp );
   };
/*............................................................................*/
   return stp;
}  
/*============================================================================*/
