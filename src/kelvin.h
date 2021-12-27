/* [ file: kelvin.h ] */
/*******************************************************************************
*                                                                              *
*   Function body kelvin(*); HELIOS release v1.0r1                             *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function computes and inverts a heat conductance matrix for given     *
*   heat resistances between parallel conductors                               *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 27, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# include "vtempr.h"
/*============================================================================*/

THERMDYN *\
kelvin( HELIOSSTATE *state )
{
/* allusions: */

/* declarations: */

   static FILE
     *errfle;

   static THERMDYN
     *rtp;

   static GAUSS_JRD
     *gsp;

# if (( KLV_TMPLOG == 1 ) \
    ||( KLV_HTCLOG == 1 ) \
    ||( KLV_MHCLOG == 1 ) \
    ||( KLV_MHRLOG == 1 ) \
    ||( KLV_CHCLOG == 1 ) \
    ||( KLV_RHCLOG == 1 ) \
    ||( KLV_PRTSPC == 1 ) \
    ||( KLV_TSTRTP == 1 ))

   static char
      ptr[STS_SIZE] = {null};
# endif

   static long
      kk = null,
     *kkp = &kk;

   static short
      ii = null,
      jj = null,
      mm = null,
      nn = null,
      dii = null,
      djj = null,
      ngrnd = null,
      drank = null;

   static short
      grd[NODES] = {-ONE};

   static short
      nde[NODES] = {-ONE}; /* node identifier: nde[ii-nodes] = */
	                   /* node pertinent to expanded index ii */
   static long
      ntrials = KLV_MAXITR;

   static double
      tolxx = DXXNEWR,
      tolff = DFFNEWR,
      dxrtbis = DXRTBIS,
      lwbound = MHC_LWBOUND,
      upbound = MHC_UPBOUND,
      epsilon = DXFDJAC;

# if (( KLV_REORDR == 1 ) \
    ||( KLV_REORDR == (-1)))
   static double
      grtolrc = GRTOLRC;
# endif

   static double
      tmean = ZERO, /* mean base temperature */
      tgmin = ZERO, /* minimum base temperature */
      tgmax = ZERO, /* maximum base tempetarture */
      sumhc = ZERO, /* sum ( fabs( hcrr[ii] )), ii=0,...,nodes-1 */
      summc = ZERO, /* sum ( fabs( mhc[ii][ii+nodes] )), ii=0,...,nodes-1 */
      sclog = ZERO,
      scale = ZERO,
      errxx = ZERO,
      errff = ZERO;

   static double
      ss = ZERO,
      tt = ZERO,
      xx = ZERO,
      xx1 = ZERO,
      xx2 = ZERO,
      xx3 = ZERO,
      xx4 = ZERO,
      yy = ZERO;

   static double
      xxi[NODES] = {ZERO},
      xi1[NODES] = {ZERO},
      xi2[NODES] = {ZERO};
/*----------------------------------------------------------------------------*/
   static double
     *xx_,
     *yy_,
   **jcb;

   static double
     *xp1 = &xx3,
     *xp2 = &xx4;
/*............................................................................*/
/* time_t types: */

   time_t
      nseconds = null,
     *timer = null;

   static char
     *timeptr;
/*............................................................................*/
/* prototyping: */

   time_t
      time( time_t *timer );

# if defined ( _BSD )
   char 
     *strcpy( char *ptr1, const char *ptr2 ),  
     *strcat( char *ptr1, const char *ptr2 ),
     *strncat( char *ptr1, const char *ptr2, size_t n );
# endif

   char
     *lotos( long mm, char cc );

   char
     *dotos( double x, char precision, char *format );

   double
     *dvector( long nl, long nh );

   double
    **dmatrix( long nrl, long nrh, long ncl, long nch );

   void
      free_dvector ( double *dv, long nl, long nh );

   void
      free_dmatrix ( double **dm, long nrl, long nrh, long ncl, long nch );

   GAUSS_JRD
     *gssjpv( GAUSS_JRD *gjp );

   short
      zbracv ( double ( *funct )( double ),
               double *xp1, double *xp2,
               signed char opt );
   long
      zbrakl ( double ( *funct )( double ),
               double xx1, double xx2, long kk,
               double *xb1, double *xb2, long *kkp );
   double
      rtbis( double ( *funct )( double ), double xx1, double xx2, double xacc );

   void
      fdjac( void ( *vfunct )( double *xx, double *ff, short dimension ),
             double *xx, double *ff, double **df, double epsilon,
             short dimension );
   double
      stempr( double xx );

   void
      vtempr( double *xx, double *ff, short rank );
/*----------------------------------------------------------------------------*/
/* memory allocations: */   
  
   timeptr = ( char *) calloc( STS_SIZE, ONE );
   
   xx_ = dvector( null, NODES );
   yy_ = dvector( null, NODES );

   jcb = dmatrix( null, NODES, null, NODES);
/*...........................................................................*/
/* initialize: */

   strcpy( errptr, ( state->errfle )); /* log file name */

   tolxx = DXXNEWR;
   tolff = DFFNEWR;
   dxrtbis = DXRTBIS;
   lwbound = MHC_LWBOUND;
   upbound = MHC_UPBOUND;

/* initialize structure gss, pointed to by GAUSS_JRD *gsp */
/*............................................................................*/
   gsp = gssjpv( NULL );    /* initialize structrure pointer gsp */
/*............................................................................*/
   tgrnd = ZERO;

   rtp = ( state->thp );
   nodes = ( state->cdp->nodes );

   sumhc = ZERO;
   ii = null; do
   {
      grd[ii] = -ONE;
      nde[ii] = -ONE;

      hcrr[ii] = ( state->hcp->hcrr[ii] );
      sumhc += hcrr[ii];

      jj = null; do
      {
         ss = ( state->cdp->mhc[ii][jj] );

         if ( ss == ZERO ) /* symmetrize eventually incomplete matrix */
            ss = ( state->cdp->mhc[jj][ii] );

        mhc[ii][jj]  = ss;
        mhc[jj][ii]  = ss;
      } while (( ++jj ) < nodes );
   } while (( ++ii ) < nodes );

   if ( SMALL_VAL < fabs( sumhc )) 
   {
      sumhc = fabs( sumhc );
      sclog = ( double ) ( int ) log10( fabs( sumhc ));
      scale = pow( 10., sclog );
   }
   else
   { 
      sumhc = 1.;
      scale = 1.;
   };

   ( state->scale ) = scale;

   lwbound *= scale;
   upbound *= scale;
/*............................................................................*/
/* shrink heat conductance matrix to its essential part: */

   tgmax = - GIANT_VAL;
   tgmin = ZERO;
   tmean = ZERO;
   summc = ZERO;
   mrank = nodes;
   ngrnd = null;
   mm = - ONE;
/*............................................................................*/
# if (( KLV_REORDR == 1 ) \
    ||( KLV_REORDR == (-1)))

   tgrnd = GIANT_VAL;
   ii = null; do
   {
      ss = mhc[ii][ii];

      if ( lwbound < fabs( ss ))
      {
         tt = ( state->thp->tfix[ii] );
         if (( grtolrc < tt )
           &&( tt < tgrnd )) /* search the minimum [ greater ZERO ] */
         {
            tgrnd = tt;
            mm = ii;
            xx = ss;
         };
      }
      else
         mhc[ii][ii] = ZERO;
   } while (( ++ii ) < nodes );

   tgmin = tgrnd;
   tmean += tgrnd;
   summc += fabs( xx );

   grd[( ngrnd++ )] = mm; /* count the ground node */
   tgmax = tgrnd; /* just so far ! */
   
# if KLV_PRTSPC == 1
   PRINTF( "kelvin1", "gound node mm=", mm );
   PRINTF( "kelvin1", "heat conduct mhc=", xx );
# endif

# else
   tgrnd = ZERO;
# endif
/*............................................................................*/
   ii = nodes; do
   {
      dii = ii-nodes;
      nde[dii] = -ONE;
/*............................................................................*/
# if (( KLV_REORDR == 1 ) \
    ||( KLV_REORDR == 2 ))
/* reorder with increasing temperatures */
      tgoal[dii] = GIANT_VAL;
# elif (( KLV_REORDR == (-1)) \
      ||( KLV_REORDR == (-2)))
/* reorder with decreasing temperatures */
      tgoal[dii] = - GIANT_VAL;
# else
      tgoal[dii] = ZERO;
# endif
/*............................................................................*/
      jj = null; do
      {
         if ( jj == mm )  /* if ( null <= mm ) this is the ground node */
            goto next_jj;

         kk = null;
         while ( kk < dii )
         {                             /* look into nde[]: */
            if ( nde[( kk++ )] == jj ) /* if jj is yet stored */
               goto next_jj;           /* then jump to jj+ONE */
         };
            
         ss = mhc[jj][jj];
         if ( lwbound < fabs( ss ))
         {
            tt = ( state->thp->tfix[jj] );
/*............................................................................*/
# if KLV_REORDR == 1
            if (( ii == nodes )
              &&( tgrnd <= tt )               /* allow some tolerance to */
              &&( tt <= ( tgrnd + grtolrc ))) /* ground temperature for */
            {                                 /* additional ground nodes */
               tmean += tt;
               summc += fabs( ss );  
               grd[( ngrnd++ )] = jj;         /* count this ground node */
            };

            if ((( tgrnd + grtolrc ) < tt )   /* select base nodes with */
               &&( tt < tgoal[dii] ))         /* increasing temperature */
# elif KLV_REORDR == 2
            if ( tt < tgoal[dii] )
# elif KLV_REORDR == (-2)
            if ( tgoal[dii] < tt )            /* select base nodes with */
# elif KLV_REORDR == (-1)                     /* decreasing temperature */
            if (( ii == nodes )
              &&( tgrnd <= tt )               /* allow some tolerance to */
              &&( tt <= ( tgrnd + grtolrc ))) /* ground temperature for */
            {                                 /* additional ground nodes */
               tmean += tt;
               summc += fabs( ss );
               grd[( ngrnd++ )] = jj;         /* count this ground node */
            };
	    
            if ((( tgrnd + grtolrc ) < tt )
               &&( tgoal[dii] < tt ))
# else /* e.g. KLV_REORDR == 0 */
            if ( nde[dii] < null )            /* accept any base node */
# endif                                       /* [ the 'next best' ] */ 
/*............................................................................*/
            {                              
               nde[dii] = jj;
               tgoal[dii] = tt;
               xx = ss;
            };
         }; /* end if ( lwbound < fabs( ss )) */
        next_jj: ;
      } while(( ++jj ) < nodes );

      if ( null <= nde[dii] ) /* a new base node has been found */
      {
         if (( 3.3*CELSIUS_TO_KELVIN ) < tgoal[dii] )
         {
            if ( state->onerr == 's' ) /* error: 's'ilent return */
            {
               ( rtp->rtn ) = ONE;
               return rtp;
            }
            else
            {
               errfle = fopen(( state->errfle ), "a+" );

               fprintf( errfle, \
                  "\nError message from job %s, function %s :", \
                  lotos (( state->job ), null ), __func__ ); 
               fprintf( errfle,
                  "\nExcessively high external temperature T[%d] "
                  " = %.5e Kelvin !", nde[dii], tgoal[dii] );
               fprintf( errfle,
                  "\n[ Unhappy with given initial data," );
               fprintf( errfle,
                  "\n  check and modify parameter input," );
               fprintf( errfle,
                  " cf. file par.log%s. ]\n", 
                  ( lotos( state->job, null )));

               fclose( errfle );

               fprintf( stderr, \
                  "\n Error message from job %s, function %s :", \
                  lotos ( state->job, null ), __func__ ); 
               fprintf( stderr,
                  "\n Excessively high external temperature T[%d] "
                  " = %.5e Kelvin !", nde[dii], tgoal[dii] );
               fprintf( stderr,
                  "\n [ Unhappy with given initial data," );
               fprintf( stderr,
                  "\n   check and modify parameter input," );
               fprintf( stderr,
                  " cf. file par.log%s. ]\n\n",
                  ( lotos( state->job, null )));

               exit( EXIT_FAILURE );
            };
         }
         else if ( tgoal[dii] < ( CELSIUS_TO_KELVIN / 33. ))
         {
            if ( state->onerr == 's' ) /* error: 's'ilent return */
            {
               ( rtp->rtn ) = ONE; 
               return rtp;
            }
            else
            {
               errfle = fopen(( state->errfle ), "a+" );

               fprintf( errfle, \
                  "\nError message from job %s, function %s :", \
                  lotos ( state->job, null ), __func__ );
               fprintf( errfle,
                  "\nExcessively low external temperature T[%d] "
                  " = %.5e Kelvin !", nde[dii], tgoal[dii] );
               fprintf( errfle,
                  "\n[ Unhappy with given initial data," );
               fprintf( errfle,
                  "\n  check and modify parameter input," );
               fprintf( errfle,
                  " cf. file par.log%s. ]\n",
                  ( lotos( state->job, null )));

               fclose( errfle );

               fprintf( stderr, \
                  "\n Error message from job %s, function %s :", \
                  lotos ( state->job, null ), __func__ );
               fprintf( stderr,
                  "\n Excessively low external temperature T[%d] "
                  " = %.5e Kelvin !", nde[dii], tgoal[dii] );
               fprintf( stderr,
                  "\n [ Unhappy with given initial data," );
               fprintf( stderr,
                  "\n   check and modify parameter input," );
               fprintf( stderr,
                  " cf. file par.log%s. ]\n\n",
                  ( lotos( state->job, null )));

               exit( EXIT_FAILURE );
            };
         }; /* end for external temperature excess */

         djj = nde[dii];
/*............................................................................*/
# if KLV_PRTSPC == 1
         PRINTF( "kelvin2", "additional base node ii=", ii );
         PRINTF( "kelvin2", "pertinent to original node jj=", djj );
# endif
/*............................................................................*/
         mhc[djj][djj] = ZERO;
         mhc[ii][ii] = 1.; /* to be properly initialized, below */
         mhc[ii][djj] = xx;
         mhc[djj][ii] = xx;
/*............................................................................*/
# if KLV_PRTSPC == 1
         PRINTF( "kelvin3", "base heat conductance mhc=", xx );
         PRINTF( "kelvin3", "tgoal=", tgoal[dii] );
# endif
/*............................................................................*/
         tmean += tgoal[dii];
         summc += fabs( xx );
         nn++;

         if ( tgmax < tgoal[dii] )
            tgmax = tgoal[dii];
         if ( tgoal[dii] < tgmin )
            tgmin = tgoal[dii];

         mrank++;
      }; /* end if ( if < nde[dii] ) */
   } while ( null <= nde[( ii++ ) - nodes] );
/*............................................................................*/
# if KLV_PRTSPC == 1
   PRINTF( "kelvin4", "mrank=", mrank );
# endif
/*............................................................................*/
/* check heat contacts to environment or to cooling devices [ drank = 0 ]: */

   drank = mrank - nodes;
   nn = ( ngrnd + drank );

   if ( null < nn )
   {
      tmean /= nn;
      summc /= nn;
   }
   else if ( state->onerr == 's' ) /* error: 's'ilent return */
   {
      ( rtp->rtn ) = ONE; 
      return rtp;
   }
   else
   {
      errfle = fopen(( state->errfle ), "a+" );

      fprintf( errfle, \
         "\nError message from job %s, function %s :", \
         lotos ( state->job, null ), __func__ );
      fprintf( errfle,
         "\n[ Quasi ] Isolated system !" );
      fprintf( errfle,
         "\nTransmitted CW power too low " );
      fprintf( errfle,
         "or lacking thermal contact to environment." );
      fprintf( errfle,
         "\n[ Unhappy with given initial data," );
      fprintf( errfle,
         "\n  check and modify parameter input," );
      fprintf( errfle,
         " cf. file par.log%s. ]\n",
         ( lotos( state->job, null )));

      fclose( errfle );

      fprintf( stderr, \
         "\n Error message from job %s, function %s :", \
         lotos ( state->job, null ), __func__ );
      fprintf( stderr,
         "\n [ Quasi ] Isolated system !" );
      fprintf( stderr,
         "\n Transmitted CW power too low " );
      fprintf( stderr,
         "or lacking thermal contact to environment." );
      fprintf( stderr,
         "\n [ Unhappy with given initial data," );
      fprintf( stderr,
         "\n   check and modify parameter input," );
      fprintf( stderr,
         " cf. file par.log%s. ]\n\n",
         ( lotos( state->job, null )));

      exit( EXIT_FAILURE );
   }; /* end if (( drank == null )&&...) */
/*...........................................................................*/
/* read cross heat conductances [ inverse heat resistances ] */

   ii = null; do
   {
      jj = null; do
      {
         chc[ii][jj] = ZERO;
         rhc[ii][jj] = ZERO;
      } while (( ++jj ) < mrank );
   } while (( ++ii ) < mrank );
/*...........................................................................*/
/* read the mutual heat conductances [ inverse heat resistances ]: */

   ii = null; do
   {
      jj = null;
      while ( jj < ii )
      {
         chc[ii][ii] += mhc[ii][jj];
         chc[ii][jj] =- mhc[ii][jj];
         jj++;
      };

/* jj == ii: */

      if ( nodes <= ii )
      {
/* initialize base conductivities */
/* by coarsely approximated values: */

         dii = ii-nodes;
         djj = nde[dii];
         tgoal[dii] = ( state->thp->tfix[djj] );

         ss = mhc[ii][djj];
         ss *= ( sumhc / summc );
         ss /= ( tgoal[dii] - tgrnd );

         mhc[ii][ii] = ss;
         xxi[dii] = ss; /* start value */
         xi1[dii] = 1.e-05*ss; /* guess for lower bracketing value */
         xi2[dii] = 1.e+05*ss; /* guess for upper bracketing value */
      };

/* still jj = ii: */

      chc[ii][ii] += mhc[ii][ii];

      while (( ++jj ) < mrank )
      {
         chc[ii][ii] += mhc[ii][jj];
         chc[ii][jj] =- mhc[ii][jj];
      };
   } while (( ++ii ) < mrank );
/*...........................................................................*/
# if KLV_MHCLOG == 1

   if (( state->klv_skp[1] != 's' )&&( state->klv_skp[1] != 'S' ))
   {
      fprintf( stdout, "\n The [ essential ] heat conductances "
         "[W/(m*K)]:\n" );

      ii = null; do
      {
         fprintf( stdout, "\n |" );
         jj = null; do
         {
            fprintf( stdout, " % .3e |", mhc[ii][jj] );
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

      fprintf( stdout, "\n\n Please acknowledge:" );
      fprintf( stdout, "\n Enter any character"
         " [ enter 's' to skip this message ]: " );

      scanf( "%s", &state->klv_skp[1] );
   };
# endif
/*...........................................................................*/
# if KLV_CHCLOG == 1

   if (( state->klv_skp[2] != 's' )&&( state->klv_skp[2] != 'S' ))
   {
      fprintf ( stdout, "\n The [ essential ] heat conductance matrix "
         "[W/(m*K)]:\n" );

      ii = null; do   
      {
         fprintf( stdout, "\n |" );
         jj = null; do
         {
            fprintf( stdout, " % .2e |", chc[ii][jj] );
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

      fprintf( stdout, "\n\n Please acknowledge:" );
      fprintf( stdout, "\n Enter any character"
         " [ enter 's' to skip this message ]: " );

      scanf( "%s", &state->klv_skp[2] );
   };
# endif
/*............................................................................*/
   if (( null == drank )
     &&( ZERO < tgrnd ))
   {
      vtempr( xx_, yy_, null );
      goto store_matrices;
   };
/*............................................................................*/
# if KLV_TSTRTB == 1
/* test rtbis(*) */

   if (( state->klv_skp[3] != 's' )
     &&( state->klv_skp[3] != 'S' ))
   {
      fprintf ( stdout, "\n rtbis(*) test: \n" );

      ii = nodes;
      actbn = ii;
      dii = ii-nodes;
      djj = nde[dii];

      tgoal[dii] = ( state->thp->tfix[djj] );

      state->klv_skp[3] = '0'; do
      {
         fprintf( stdout, "\n tgoal[%d] = %.12e KELVIN", dii, tgoal[dii] );

         state->klv_skp[3] = '0'; do
         {
            fprintf( stdout, "\n" ); 
            SCANF( "xx1", xx1 );
            SCANF( "xx2", xx2 );
   
            xx = xx1;
            ii = null; do
            {
               ss = (( double ) ii ) / 10.;
               yy = stempr( xx );
               xx = ( 1.- ss )*xx1 + ss*xx2;
               fprintf( stdout, "\n stempr(%.12e) = %.12e", xx, yy );
            } while (( ++ii ) <= 10 );

            fprintf( stdout, "\n\n Please enter 1 for acceptance [ 0 else, " );
            fprintf( stdout, "\n or enter 's' to skip further tests ]: " );

            scanf( "%s", &state->klv_skp[3] );

            if ( state->klv_skp[3] == 's' )
               goto test_end;

         } while ( state->klv_skp[3] != '1' );

         xx = rtbis( stempr, xx1, xx2, dxrtbis );
         yy = stempr( xx );
/*............................................................................*/
# if KLV_PRTSPC == 1
      PRINTF( " xx = rtbis(*)", "xx", xx );
      PRINTF( " yy = stempr(*)", "yy", yy );
# endif
/*............................................................................*/
      } while ( state->klv_skp[3] != '0' );
   }; /* end if klv_skp == 's' or ... */
  test_end: ;
# endif
/*............................................................................*/
/* try to improve initial base heat conductivities */
/* [ i.e. improve start values xxi[], xx_[] ]: */

   ii = nodes;
   while (( null < KLV_REFINE )
        &&( ii < mrank ))
   {
      actbn = ii;
      dii = ii-nodes;
      djj = nde[dii];

      tgoal[dii] = ( state->thp->tfix[djj] );

     *xp1 = xi1[dii];
     *xp2 = xi2[dii];
  
      jj = null; kk = null;
      while (( kk <= null )&&( jj < KLV_REFINE ))
      {
         kk = zbracv( stempr, xp1, xp2, 1 );

/* bad initial range ( *xp1 = *xp2 ) ? */
/* [ eventually upscale *xp2 ]: */

         if ( kk < null )
            *xp2 = 1.001*( *xp2 );
         jj++;
      };

      if ( null < kk ) /* zbracv(*) has found a bracketing pair: try */
                       /* to refine it - look into kk subintervalls */
                       /* and search the first change of sign: */
      {
         xb1[null] = *xp1;
         xb2[null] = *xp2;
/*
# if KLV_PRTSPC == 1
         PRINTF( " zbracv(*)", "xp1", xb1[null] );
         PRINTF( " zbracv(*)", "xp2", xb2[null] );
# endif
*/
         jj = null; kk = null; do
         {
            xx1 = xb1[(short)kk];
            xx2 = xb2[(short)kk];

            kk = KLV_ZBRDIV;
           *kkp = 1;

            kk = zbrakl( stempr, xx1, xx2, kk, xb1, xb2, kkp );

	    jj++;
         } while (( null < kk )&&( jj < KLV_ZBRITR ));

         if ( null < kk )
         {
            xx = rtbis( stempr, xx1, xx2, dxrtbis );
/*
# if KLV_PRTSPC == 1
            PRINTF( " xx = rtbis(xx)", "xx", xx );
# endif
*/
            if ( fabs( xx ) < GIANT_VAL )
            {
               yy = stempr( xx );
/*
# if KLV_PRTSPC == 1
               PRINTF( " yy = stempr(xx)", "yy", yy );
# endif
*/
               if ( fabs( yy ) < GIANT_VAL )
               {
                  xxi[dii] = xx;
                  xx_[dii] = xx;
                  mhc[ii][ii] = xx;
               }
               else
               {
                  xxi[dii] = xx1;
                  xx_[dii] = xx1;
               };
               mhc[ii][ii] = xxi[dii];
            };
         }; /* end if ( null < kk ) [ a bracketing subintervall found ] */
      }; /* end if ( null < kk  ) [ a bracketing pair found ] */
      ii++;
   }; /* while (( null < KLV_REFINE )&&( ii < mrank )); */
/*............................................................................*/
/* Newton-Raphson: */

   ii = nodes; do
   {
      dii = ii - nodes;
      djj = nde[dii];
/*
# if KLV_PRTSPC == 1
      PRINTF( " xx_", "xx", xx_[dii] );
      PRINTF( " xxi", "xx", xxi[dii] );
# endif
*/
      tgoal[dii] = ( state->thp->tfix[djj] );
   } while (( ++ii ) < mrank );

/* epsilon = DXFDJAC; */

   kk = null; do
   {
      fdjac( vtempr, xx_, yy_, jcb, epsilon, drank );
   /* epsilon = DXFDJAC; */

      errff = ZERO;
      ii = null; do
      {
         errff += fabs( yy_[ii] );
      } while (( ++ii ) < drank ); 

      if ( errff < tolff )
         FREERETURN;

      ii = null; do
      { 
         jj = null; do
         {
            ( gsp->mr[ii][jj] ) = jcb[ii][jj];
            ( gsp->mi[ii][jj] ) = ZERO;
         } while (( ++jj ) < drank );
         ( gsp->mr[ii][drank] ) = -yy_[ii];
         ( gsp->mi[ii][drank] ) = ZERO;
      } while (( ++ii ) < drank );

      ( gsp->rank ) = drank;
      ( gsp->neqs ) = ONE;
      ( gsp->opt ) = 'e'; /* option 'equation' */
/*............................................................................*/
      gsp = gssjpv( gsp );     /* Gauss Jordan elimination */
/*...........................*/
      errxx = ZERO;
      
      ii = null; do
      {
	 ss = ( gsp->zr[ii][0] );
         errxx += fabs( ss );
	 xx_[ii] += ss;
      } while (( ++ii ) < drank ); 

      if ( errxx < tolxx )
         FREERETURN;

      fdjac( vtempr, xx_, yy_, jcb, epsilon, drank );

   } while (( ++kk ) < ntrials );

   if ( ntrials <= kk )
   {
      if ( state->onerr == 's' ) /* error: 's'ilent return */
      {
         ( rtp->rtn ) = ONE; 
         return rtp;
      }
      else
      {
         errfle = fopen(( state->errfle ), "a+" );

         fprintf( errfle, \
            "\nError message from job %s, function %s :", \
            lotos ( state->job, null ), __func__ );
         fprintf( errfle,
            "\nNo convergence in Newton-Raphson !" );
         fprintf( errfle,
            "\n[ Unhappy with given initial data," );
         fprintf( errfle,
            "\n  check and modify parameter input," );
         fprintf( errfle,
            " cf. file par.log%s. ]\n",
            ( lotos( state->job, null )));

         fclose( errfle );

         fprintf( stderr, \
            "\n Error message from job %s, function %s :", \
            lotos ( state->job, null ), __func__ );
         fprintf( stderr,
            "\n No convergence in Newton-Raphson !" );
         fprintf( stderr,
            "\n [ Unhappy with given initial data," );
         fprintf( stderr,
            "\n   check and modify parameter input," );
         fprintf( stderr,
            " cf. file par.log%s. ]\n\n",
            ( lotos( state->job, null )));

         exit( EXIT_FAILURE );
      };
   }; /* end if ( ntrials <= kk ) */
/*............................................................................*/
/* store final matrices and temperatures: */

  store_matrices:

   ( state->cdp->mrank ) = mrank;
   ( state->cdp->ngrnd ) = ngrnd;

   ( rtp->tgrnd ) = tgrnd;
   ( rtp->tmean ) = tmean;
   ( rtp->tgmin ) = tgmin;
   ( rtp->tgmax ) = tgmax;

   ii = null; do
   {
      jj = null; do
      {
         ( state->cdp->mhc[ii][jj] ) = ZERO;
         ( state->cdp->mhr[jj][ii] ) = HUGE_VALF;
      } while (( ++jj ) < mrank );
   } while (( ++ii ) < mrank );

   ii = null; do
   {
      jj = null; do
      {
         mm = ii;
         nn = jj;

         if (( nodes <= ii )
           &&( jj == nde[ii-nodes] ))
         {
            mm = jj;
            ss = mhc[ii][jj];
            ( state->cdp->mhc[mm][nn] ) = ss;
         }
         else if (( nodes <= jj )
           &&( ii == nde[jj-nodes] ))
         {
            nn = ii;
            ss = mhc[ii][jj];
            ( state->cdp->mhc[mm][nn] ) = ss;
         }
         else if (( ii < nodes )
                &&( jj < nodes ))
	 {
            ss = mhc[ii][jj];
            ( state->cdp->mhc[mm][nn] ) = ss;
         }
         else if (( nodes <= ii )
                &&( jj == ii ))
	 {
            ss = mhc[ii][ii];
            ( state->cdp->mhc[mm][nn] ) = ss;
         }
         else
            ss = ZERO;

         xx = fabs( ss );

         if (( SMALL_VAL <= xx )
           &&( xx <= GIANT_VAL ))
            ( state->cdp->mhr[mm][nn] ) = 1. / ss;
         else if ( xx < SMALL_VAL )
            ( state->cdp->mhr[mm][nn] ) = HUGE_VALF;
         else if ( GIANT_VAL < xx )
            ( state->cdp->mhr[mm][nn] ) = ZERO;

         ( state->cdp->chc[ii][jj] ) = chc[ii][jj];
         ( state->cdp->rhc[ii][jj] ) = rhc[ii][jj];
      } while (( ++jj ) < mrank );
   } while (( ++ii ) < mrank );
/*...........................................................................*/
   ii = null; do
   {
      if (( 77.*CELSIUS_TO_KELVIN ) < temp[ii] )
      {
         if ( state->onerr == 's' ) /* error: 's'ilent return */
         {
            ( rtp->rtn ) = ONE;
            return rtp;
         }
         else
         {
            errfle = fopen(( state->errfle ), "a+" );

            fprintf( errfle, \
               "\nError message from job %s, function %s :", \
               lotos ( state->job, null ), __func__ );

            fprintf( stderr, \
               "\n Error message from job %s, function %s :", \
               lotos ( state->job, null ), __func__ );

            if ( state->opp->n[2] == null )
            {
               fprintf( errfle,
                  "\nExcessively high temperature: T[%d] = "
                  "%.5e K !", ii, temp[ii] );

               fprintf( stderr,
                  "\n Excessively high temperature: T[%d] = "
                  "%.5e K !", ii, temp[ii] );
            }
            else
            {
               fprintf( errfle,
                  "\nExcessively high CW power: P = "
                  "%.7e watts !", ( state->elp->cwpr ));

               fprintf( stderr,
                  "\n Excessively high CW power: P = "
                  "%.7e watts !", ( state->elp->cwpr ));
            };

            fprintf( errfle,
               "\n[ Unhappy with given initial data," );
            fprintf( errfle,
               "\n  check and modify parameter input," );
            fprintf( errfle,
               " cf. file par.log%s. ]\n", 
               ( lotos( state->job, null )));

            fclose( errfle );

            fprintf( stderr,
               "\n [ Unhappy with given initial data," );
            fprintf( stderr,
               "\n   check and modify parameter input," );
            fprintf( stderr,
               " cf. file par.log%s. ]\n\n", 
               ( lotos( state->job, null )));

            exit( EXIT_FAILURE );
         };
      } /* end for node temperature excess */
      else
         ( rtp->temp[ii] ) = temp[ii];
   } while (( ++ii ) < nodes );

   while ( ii < mrank )
   {
      dii = ii-nodes;
      ( rtp->temp[nodes+nde[dii]] ) = temp[ii];
      ii++;
   };

   ii = null;
   while ( ii < ngrnd )
   {
      ( rtp->temp[nodes+grd[ii]] ) = ( state->thp->tfix[grd[ii]] );
/*
      ( rtp->temp[nodes+grd[ii]] ) = tgrnd;
*/
      ii++;
   };
/*............................................................................*/
# if KLV_RHCLOG == 1

   if (( state->klv_skp[4] != 's' )&&( state->klv_skp[4] != 'S' ))
   {
      fprintf( stdout, "\n The final [essential] heat"
         " resistance matrix [(K*m)/W]:\n" );
      ii = null; do   
      {
         fprintf( stdout, "\n |" );
         jj = null; do
         {
            fprintf( stdout, " % .2e |", rhc[ii][jj] ));
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

      fprintf( stdout, "\n\n Please acknowledge:" );
      fprintf( stdout, "\n Enter any character"
         " [ enter 's' to skip this message ]: " );

      scanf( "%s", &state->klv_skp[4] );
   };
# endif
/*............................................................................*/
# if KLV_TMPLOG == 1

   if (( state->klv_skp[5] != 's' )&&( state->klv_skp[5] != 'S' ))
   {
      fprintf( stdout, "\n The final node temperature(s) [DEG Celsius]:\n" );
      fprintf( stdout, "\n |" );

      ii = null; do
      {
         fprintf( stdout, " Tn[%d]=% .3e |", ii, 
            ( temp[ii] - CELSIUS_TO_KELVIN ));
         ii++;
      } while( ii < nodes );

      fprintf( stdout, "\n\n The final base temperature(s) [DEG Celsius]:\n" );
      fprintf( stdout, "\n |" );

      while ( ii < mrank )
      {
         jj = ii - nodes;
         fprintf( stdout, " Tb[%d]=% .3e |", jj, 
            ( temp[ii] - CELSIUS_TO_KELVIN ));
         ii++;
      };

      fprintf( stdout, "\n\n Please acknowledge:" );
      fprintf( stdout, "\n Enter any character"
         " [ enter 's' to skip this message ]: " );

      scanf( "%s", &state->klv_skp[5] );
   };
# endif
/*............................................................................*/

   nseconds = time( timer );
   strcpy( timeptr, ctime( &nseconds ));
/*
   fprintf( stdout, "\n protojob terminated " );
*/ 
   ( rtp->rtn ) = null;
   return rtp;
}  
/*============================================================================*/
