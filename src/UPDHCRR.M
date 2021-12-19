/* [ file: UPDHCRR.M ] */
/*----------------------------------------------------------------------------*/
/* Update: December 19, 2021 */
/*----------------------------------------------------------------------------*/
# define UPDHCRR( OPT ) \
{ \
   upd = WRK_UPDHCR; \
   nds_ = null; \
   if ( OPT == null ) /* rectangular wg */ \
   { \
      while ( nds_ < nodes ) \
      { \
         if ( nds_ == cdp->nde[1] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssd ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
         } \
         else if ( nds_ == cdp->nde[2] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssi ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
            hh = nds_; \
            break; \
         }; \
         nds_++; \
      }; \
/*..........................................................................*/ \
/* conductor [ mantle ] in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_= hh; \
      while ( nds_ < nodes ) \
      { \
         if ( nds_ == cdp->nde[4] ) \
         { \
         /* ( hcp->hcrr[nds_] ) = ZERO; */\
            break; \
         } \
         else if ( nds_ == cdp->nde[5] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( - thp->sdedm ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
            break; \
         }; \
         nds_++; \
      }; \
/*..........................................................................*/ \
   } /* end [ rectangular waveguide ] */ \
   else /* coaxial waveguide */ \
   { \
      while ( nds_ < nodes ) \
      { \
         if ( nds_ == cdp->nde[2] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssi ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
         } \
         else if ( nds_ == cdp->nde[3] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lssd ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
         } \
         else if (( nds_ == cdp->nde[4] ) \
                &&( stp->cyl[3] == ONE )) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lsso ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
 \
            hh = nds_; \
            break; \
         } \
         else if (( nds_ == cdp->nde[5] ) \
                &&( stp->cyl[3] == null )) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( elp->lsso ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
 \
            hh = nds_; \
            break; \
         }; \
         nds_++; \
      }; \
/*..........................................................................*/ \
/* inner conductor [ pipe ] in thermal side contact: */ \
/*..........................................................................*/ \
      nds_= hh; \
      while ( nds_ < nodes ) \
      { \
         if ( nds_ == cdp->nde[6] ) \
         { \
         /* ( hcp->hcrr[nds_] ) = ZERO; */\
            break; \
         } \
         else if ( nds_ == cdp->nde[7] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( - thp->sdedp ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
            break; \
         }; \
         nds_++; \
      }; \
/*..........................................................................*/ \
/* outer conductor [ mantle ] in thermal contact to one side: */ \
/*..........................................................................*/ \
      nds_= hh; \
      while ( nds_ < nodes ) \
      { \
         if ( nds_ == cdp->nde[8] ) \
         { \
         /* ( hcp->hcrr[nds_] ) = ZERO; */\
            break; \
         } \
         else if ( nds_ == cdp->nde[9] ) \
         { \
            ( hcp->hcrr[nds_] ) = rscl*( upd*( - thp->sdedm ) \
             + ( 1. - upd )*( hcp->hcrr[nds_])); \
            break; \
         }; \
         nds_++; \
      }; \
/*..........................................................................*/ \
   }; /* end [ coaxial waveguide ] */ \
   while( nds_ < nodes ) { \
      ( hcp->hcrr[nds_] ) = ZERO; \
      nds_++; }; \
/*..........................................................................*/ \
} /* end of macro UPDHCRR */
/*----------------------------------------------------------------------------*/
