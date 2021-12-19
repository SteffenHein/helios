/* [ file: WRKMCS0.M ] HELIOS-v1.0r1 */
/* Update: December 19, 2021 */
/*----------------------------------------------------------------------------*/
# define PRINTF(SS1,SS2,XX) \
{ \
   printf( "\n %s   % .12e", (SS2), ( double )(XX)); \
   printf( "\n please acknowledge (%s) [ enter any character ]:", (SS1)); \
   scanf( "%s", ptr ); \
}
/*----------------------------------------------------------------------------*/
# define TMPNORM( OPT, NN ) \
{ \
   nds_ = null; \
   norm = ZERO; \
 \
   if ( OPT == null ) /* rectangular waveguide */ \
   { \
      if ( NN == 1 ) \
      { \
         if ( stp->cyl[0] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tdel )); \
            nds_++; }; \
 \
         if ( stp->cyl[1] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tinn )); \
            nds_++; }; \
 \
         if ( stp->cyl[2] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tjck )); \
            nds_++; }; \
 \
         norm /= CELSIUS_TO_KELVIN; \
      } \
      else \
      { \
         if ( stp->cyl[0] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tdel )), 2.); \
            nds_++; }; \
 \
         if ( stp->cyl[1] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tinn )), 2.); \
            nds_++; }; \
 \
         if ( stp->cyl[2] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tjck )), 2.); \
            nds_++; }; \
 \
         norm = sqrt( norm ) / CELSIUS_TO_KELVIN; \
      }; \
   } \
   else /* coaxial waveguide */ \
   { \
      if ( NN == 1 ) \
      { \
         if ( stp->cyl[0] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->ttub )); \
            nds_++; }; \
 \
      /* if ( stp->cyl[1] ) - allways true */ { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tinn )); \
            nds_++; }; \
 \
         if ( stp->cyl[2] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tdel )); \
            nds_++; }; \
 \
         if ( stp->cyl[3] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tout )); \
            nds_++; }; \
 \
         if ( stp->cyl[4] ) { \
            norm += fabs(( thp->temp[nds_] ) - ( thp->tjck )); \
            nds_++; }; \
 \
         norm /= CELSIUS_TO_KELVIN; \
      } \
      else \
      { \
         if ( stp->cyl[0] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->ttub )), 2.); \
            nds_++; }; \
 \
      /* if ( stp->cyl[1] ) - allways true */ { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tinn )), 2.); \
            nds_++; }; \
 \
         if ( stp->cyl[2] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tdel )), 2.); \
            nds_++; }; \
 \
         if ( stp->cyl[3] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tout )), 2.); \
            nds_++; }; \
 \
         if ( stp->cyl[4] ) { \
            norm += pow((( thp->temp[nds_] ) - ( thp->tjck )), 2.); \
            nds_++; }; \
 \
         norm = sqrt( norm ) / CELSIUS_TO_KELVIN; \
      }; \
   } \
   ( thp->tnorm ) = norm; \
} /* end of macro TMPNORM */
/*----------------------------------------------------------------------------*/
# define UPDTEMP( OPT ) \
{ \
   nds_ = null; \
 \
   if ( OPT == null ) \
   { \
      if ( stp->cyl[0] ) { \
         ( thp->tdel ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tdel ); \
         nds_++; }; \
 \
      if ( stp->cyl[1] ) { \
         ( thp->tinn ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tinn ); \
         nds_++; }; \
 \
      if ( stp->cyl[2] ) { \
         ( thp->tjck ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tjck ); \
         nds_++; }; \
   } \
   else \
   { \
      if ( stp->cyl[0] ) { \
         ( thp->ttub ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->ttub ); \
         nds_++; }; \
 \
   /* if ( stp->cyl[1] ) - allways true */ { \
         ( thp->tinn ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tinn ); \
         nds_++; }; \
 \
      if ( stp->cyl[2] ) { \
         ( thp->tdel ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tdel ); \
         nds_++; }; \
 \
      if ( stp->cyl[3] ) { \
         ( thp->tout ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tout ); \
         nds_++; }; \
 \
      if ( stp->cyl[4] ) { \
         ( thp->tjck ) = upd*( thp->temp[nds_] ) + \
         ( 1. - upd )*( thp->tjck ); \
 \
         if (( stp->cyl[3] ) == null ) \
            ( thp->tout ) = ( thp->tjck ); \
 \
         nds_++; }; \
   }; \
} /* end of macro UPDTEMP */
/*----------------------------------------------------------------------------*/
# define UPDHCRR( OPT ) \
{ \
   nds_ = null; \
 \
   if ( OPT == null ) \
   { \
      if (( stp->cyl[0] ) == ONE ) { \
         ( hcp->hcrr[nds_] ) = rscl*( elp->lssd ); \
         nds_++; }; \
 \
         { ( hcp->hcrr[nds_] ) = rscl*( elp->lssi ); } \
 \
      if (( stp->cyl[3] ) == ONE ) { \
         ( hcp->hcrr[nds_] ) -= ( rscl*( thp->sdedm )); } \
 \
      nds_++; \
 \
      while( nds_ < nodes ) { \
         ( hcp->hcrr[nds_] ) = ZERO; \
         nds_++; }; \
   } \
   else \
   { \
      if (( stp->cyl[0] ) == ONE ) { \
         ( hcp->hcrr[nds_] ) = ZERO; \
         nds_++; }; \
 \
   /* ( stp->cyl[1] ) == ONE ) - allways true */ \
         { ( hcp->hcrr[nds_] ) = rscl*( elp->lssi ); \
 \
         if ( stp->cyl[5] == ONE ) { \
            ( hcp->hcrr[nds_] ) -= ( rscl*( thp->sdedp )); } \
 \
         nds_++; }; \
 \
      if (( stp->cyl[2] ) == ONE ) { \
         ( hcp->hcrr[nds_] ) = rscl*( elp->lssd ); \
         nds_++; }; \
 \
      if ( nds_ < nodes ) { \
         ( hcp->hcrr[nds_] ) = rscl*( elp->lsso ); \
 \
         if ( stp->cyl[6] == ONE ) { \
            ( hcp->hcrr[nds_] ) -= ( rscl*( thp->sdedm )); } \
 \
         nds_++; }; \
 \
      while( nds_ < nodes ) { \
         ( hcp->hcrr[nds_] ) = ZERO; \
         nds_++; }; \
   }; \
} /* end of macro UPDHCRR */
/*----------------------------------------------------------------------------*/
