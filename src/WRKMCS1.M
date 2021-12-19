/* [ file: WRKMCS1.M ] HELIOS-v1.0r1 */
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
   norm = ZERO; \
 \
   if ( OPT == null ) /* rectangular waveguide */ \
   { \
      if ( NN == 1 ) /* [ l_1 norm ] */ \
      { \
         nds_ = ( cdp->nde[1] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tdel ); \
\
         nds_ = ( cdp->nde[2] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tinn ); \
\
         nds_ = ( cdp->nde[3] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tjck ); \
/*..........................................................................*/ \
/* mantle in thermal contact to one side: */ \
/*..........................................................................*/ \
         nds_ = ( cdp->nde[4] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] \
                  - .5*( thp->tsdm + thp->tjck )); \
 \
         nds_ = ( cdp->nde[5] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->trfm ); \
/*..........................................................................*/ \
         norm /= CELSIUS_TO_KELVIN; \
/*..........................................................................*/ \
      } /* end [ l-1 norm ] */ \
      else /* l_2 norm */ \
      { \
         nds_ = ( cdp->nde[1] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tdel ), 2.); \
\
         nds_ = ( cdp->nde[2] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tinn ), 2.); \
\
         nds_ = ( cdp->nde[3] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tjck ), 2.); \
/*..........................................................................*/ \
/* mantle in thermal contact to one side: */ \
/*..........................................................................*/ \
         nds_ = ( cdp->nde[4] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] \
            - .5*( thp->tsdm + thp->tjck )), 2.); \
 \
         nds_ = ( cdp->nde[6] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->trfm ), 2.); \
/*..........................................................................*/ \
         norm = sqrt( norm )/CELSIUS_TO_KELVIN; \
/*..........................................................................*/ \
      }; /* end [ l_2 norm */ \
   } /* end [ rectangular waveguide ] */ \
   else /* coaxial waveguide */ \
   { \
      if ( NN == 1 ) /* l_1 norm */ \
      { \
         nds_ = ( cdp->nde[1] ); \
         if ( null <= nds_ ) \
               norm += fabs( thp->temp[nds_] - thp->ttub ); \
 \
         nds_ = ( cdp->nde[2] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tinn ); \
 \
         nds_ = ( cdp->nde[3] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tdel ); \
 \
         nds_ = ( cdp->nde[4] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tout ); \
 \
         nds_ = ( cdp->nde[5] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->tjck ); \
/*..........................................................................*/ \
/* pipe in thermal contact to one side: */ \
/*..........................................................................*/ \
         nds_ = ( cdp->nde[6] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] \
            - .5*( thp->tsdp + thp->tinn )); \
 \
         nds_ = ( cdp->nde[7] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->trfp ); \
/*..........................................................................*/ \
/* mantle in thermal contact to one side: */ \
/*..........................................................................*/ \
         nds_ = ( cdp->nde[8] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] \
            - .5*( thp->tsdm + thp->tjck )); \
 \
         nds_ = ( cdp->nde[9] ); \
         if ( null <= nds_ ) \
            norm += fabs( thp->temp[nds_] - thp->trfm ); \
/*..........................................................................*/ \
         norm /= CELSIUS_TO_KELVIN; \
/*..........................................................................*/ \
      } /* end [ l_1 norm ] */ \
      else /* l_2 norm */ \
      { \
         nds_ = ( cdp->nde[1] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->ttub ), 2.); \
 \
         nds_ = ( cdp->nde[2] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tinn ), 2.); \
 \
         nds_ = ( cdp->nde[3] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tdel ), 2.); \
 \
         nds_ = ( cdp->nde[4] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tout ), 2.); \
 \
         nds_ = ( cdp->nde[5] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->tjck ), 2.); \
/*..........................................................................*/ \
/* pipe in thermal contact to one side: */ \
/*..........................................................................*/ \
         nds_ = ( cdp->nde[6] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] \
            - .5*( thp->tsdp + thp->tinn )), 2.); \
 \
         nds_ = ( cdp->nde[7] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->trfp ), 2.); \
/*..........................................................................*/ \
/* mantle in thermal contact to one side: */ \
/*..........................................................................*/ \
         nds_ = ( cdp->nde[8] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] \
            - .5*( thp->tsdm + thp->tjck )), 2.); \
 \
         nds_ = ( cdp->nde[9] ); \
         if ( null <= nds_ ) \
            norm += pow(( thp->temp[nds_] - thp->trfm ), 2.); \
/*..........................................................................*/ \
         norm = sqrt( norm )/CELSIUS_TO_KELVIN; \
/*..........................................................................*/ \
      }; /* end [ l_2 norm ] */ \
   }; /* end [ coaxial waveguide ] */ \
   ( thp->tnorm ) = norm; \
} /* end of macro TMPNORM */
/*----------------------------------------------------------------------------*/
# define UPDTEMP( OPT ) \
{ \
   upd = WRK_UPDTMP; \
 \
   if ( OPT == null ) /* rectangular wg */ \
   { \
      nds_ = ( cdp->nde[1] ); \
      if ( null <= nds_ ) \
         ( thp->tdel ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->tdel ); \
 \
      nds_ = ( cdp->nde[2] ); \
      if ( null <= nds_ ) \
         ( thp->tinn ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->tinn ); \
 \
      nds_ = ( cdp->nde[3] ); \
      if ( null <= nds_ ) \
      { \
         if ( stp->cyl[2] == null ) /* mantle wall thickness vanishing */ \
             ( thp->tinn ) = ( thp->tjck ); \
         else if ( stp->opp->n[5] == ONE ) /* floating mantle temperature */ \
            ( thp->tjck ) = upd*( thp->temp[nds_] ) \
            + ( 1. - upd )*( thp->tjck ); \
      }; \
/*..........................................................................*/ \
/* conductor [ mantle ] in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_ = ( cdp->nde[4] ); \
      if ( null <= nds_ ) \
         { ; }; /* nothing to do */ \
 \
      nds_ = ( cdp->nde[5] ); \
      if ( null <= nds_ ) \
         ( thp->trfm ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->trfm ); \
   } /* end [ rectangular waveguide ] */ \
/*..........................................................................*/ \
   else /* coaxial wg */ \
   { \
      nds_ = ( cdp->nde[1] ); \
      if ( null <= nds_ ) \
         ( thp->ttub ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->ttub ); \
 \
      nds_ = ( cdp->nde[2] ); \
      if ( null <= nds_ ) \
         ( thp->tinn ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->tinn ); \
 \
      nds_ = ( cdp->nde[3] ); \
      if ( null <= nds_ ) \
         ( thp->tdel ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->tdel ); \
 \
      nds_ = ( cdp->nde[4] ); \
      if ( null <= nds_ ) \
         ( thp->tout ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->tout ); \
 \
      nds_ = ( cdp->nde[5] ); \
      if ( null <= nds_ ) \
      { \
         if ( stp->cyl[3] == null ) /* mantle wall thickness vanishing */ \
             ( thp->tout ) = ( thp->tjck ); \
         else if ( stp->opp->n[5] == ONE ) /* floating mantle temperature */ \
            ( thp->tjck ) = upd*( thp->temp[nds_] ) \
            + ( 1. - upd )*( thp->tjck ); \
      }; \
/*..........................................................................*/ \
/* inner conductor [ pipe ] in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_ = ( cdp->nde[6] ); \
      if ( null <= nds_ ) \
         { ; }; /* nothing to do */ \
 \
      nds_ = ( cdp->nde[7] ); \
      if ( null <= nds_ ) \
         ( thp->trfp ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->trfp ); \
/*..........................................................................*/ \
/* mantle in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_ = ( cdp->nde[8] ); \
      if ( null <= nds_ ) \
         { ; }; /* nothing to do */ \
 \
      nds_ = ( cdp->nde[9] ); \
      if ( null <= nds_ ) \
         ( thp->trfm ) = upd*( thp->temp[nds_] ) \
         + ( 1. - upd )*( thp->trfm ); \
   }; \
} /* end of macro UPDTEMP */
/*----------------------------------------------------------------------------*/
# define UPDHCRR( OPT ) \
{ \
   upd = WRK_UPDHCR; \
 \
   if ( OPT == null ) /* rectangular wg */ \
   { \
      nds_ = ( cdp->nde[1] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssd ) \
         + ( 1. - upd )*( hcp->hcrr[nds_])); \
 \
      nds_ = ( cdp->nde[2] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssi ) \
          + ( 1. - upd )*( hcp->hcrr[nds_])); \
/*..........................................................................*/ \
/* conductor [ mantle ] in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_ = ( cdp->nde[4] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = ZERO; \
 \
      nds_ = ( cdp->nde[5] ); \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( - thp->sdedm ) \
          + ( 1. - upd )*( hcp->hcrr[nds_] )); \
/*..........................................................................*/ \
   } /* end [ rectangular waveguide ] */ \
   else /* coaxial waveguide */ \
   { \
      nds_ = ( cdp->nde[2] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssi ) \
          + ( 1. - upd )*( hcp->hcrr[nds_] )); \
 \
      nds_ = ( cdp->nde[3] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssd ) \
         + ( 1. - upd )*( hcp->hcrr[nds_] )); \
 \
      nds_ = ( cdp->nde[4] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lsso ) \
          + ( 1. - upd )*( hcp->hcrr[nds_] )); \
/*..........................................................................*/ \
/* inner conductor [ pipe ] in thermal side contact: */ \
/*..........................................................................*/ \
      nds_ = ( cdp->nde[6] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = ZERO; \
 \
      nds_ = ( cdp->nde[7] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( - thp->sdedp ) \
          + ( 1. - upd )*( hcp->hcrr[nds_] )); \
/*..........................................................................*/ \
/* outer conductor [ mantle ] in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_ = ( cdp->nde[8] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = ZERO; \
 \
      nds_ = ( cdp->nde[9] ); \
      if ( null <= nds_ ) \
         ( hcp->hcrr[nds_] ) = rscl*( upd*( - thp->sdedm ) \
          + ( 1. - upd )*( hcp->hcrr[nds_] )); \
/*..........................................................................*/ \
   }; /* end [ coaxial waveguide ] */ \
/*..........................................................................*/ \
} /* end of macro UPDHCRR */
/*----------------------------------------------------------------------------*/
