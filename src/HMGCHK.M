/* [ file: HMGCHK.M ] */
/*******************************************************************************
*                                                                              *
*   Macro HMGCHK [ homogeneity check ]                                         *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 19, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/
# define HMGCHK( STUB, CHCKD, BOUND, OPT ) \
{ \
   if ( stp->hls_skp[2] != 's' ) \
   { \
      if ( null < OPT ) \
      { \
         if ( BOUND < CHCKD ) \
         { \
            errfle = fopen(( stp->errfle ), "a+" ); \
 \
            if ( OPT == ONE ) \
            { \
               fprintf( errfle, \
                  "\nWarning message from job %s, function %s :", \
                  lotos (( stp->job ), null ), __func__ ); \
            } \
            else if ( OPT == TWO ) \
            { \
               fprintf( errfle, \
                  "\nError message from job %s, function %s :", \
                  lotos (( stp->job ), null ), __func__ ); \
            }; \
 \
            fprintf( errfle, \
               "\nHighly inhomogeneous temperature distribution" \
               " on "STUB"," ); \
            fprintf( errfle, \
               "\nHELIOS requires approximately homogeneous conditions !" ); \
 \
            fprintf( errfle, \
               "\n\nPossible reasons:" ); \
            fprintf( errfle, \
               "\nSide temperature set too low ?" ); \
            fprintf( errfle, \
               "\nSide heat outflow set too high ?" ); \
            fprintf( errfle, \
               "\nHeat inflow on opposite side set too high ?" ); \
            fprintf( errfle, \
               "\nHeat resistance over "STUB" length too low ?" ); \
            fprintf( errfle, \
               "\n[ viz. - for instance: "); \
            fprintf( errfle, \
               "\n  Waveguide too short ?" ); \
            fprintf( errfle, \
               "\n  Heat resistance over "STUB" length too low ?" ); \
            fprintf( errfle, \
               "\n  "STUB" wall thickness too high ? ... ]" ); \
            fprintf( errfle, \
               "\n\nRevise your parameters " \
                  "and operating conditions.\n" ); \
 \
            if ((( stp->uif ) != 'g' ) \
              &&(( stp->uif ) != 'b' ) \
              &&(( stp->uif ) != 'f' )) \
            { \
               if ( OPT == ONE ) \
               { \
                  fprintf( stdout, \
                     "\n Warning message from job %s, function %s :", \
                     lotos (( stp->job ), null ), __func__ ); \
               } \
               else if ( OPT == TWO ) \
               { \
                  fprintf( stdout, \
                     "\n Error message from job %s, function %s :", \
                  lotos (( stp->job ), null ), __func__ ); \
               }; \
 \
               fprintf( stdout, \
                  "\n Highly inhomogeneous temperature distribution" \
                  " on "STUB"," ); \
               fprintf( stdout, \
                  "\n HELIOS requires approximately homogeneous" \
                  " conditions !" ); \
               fprintf( stdout, \
                  "\n\n Possible reasons:" ); \
               fprintf( stdout, \
                  "\n Side temperature set too low ?" ); \
               fprintf( stdout, \
                  "\n Side heat outflow set too high ?" ); \
               fprintf( stdout, \
                  "\n Heat inflow on opposite side set too high ?" ); \
               fprintf( stdout, \
                  "\n Heat resistance over "STUB" length too low ?" ); \
               fprintf( stdout, \
                  "\n [ viz. - for instance: "); \
               fprintf( stdout, \
                  "\n   Waveguide too short ?" ); \
               fprintf( stdout, \
                  "\n   Heat resistance over "STUB" length too low ?" ); \
               fprintf( stdout, \
                  "\n   "STUB" wall thickness too high ? ... ]" ); \
               fprintf( stdout, \
                  "\n\n Revise your parameters " \
                  "and operating conditions.\n\n" ); \
            }; /* end [ if ( stp->uif != 'b', 'f', 'g' ) ] */ \
 \
            if ( OPT == TWO ) \
               exit( EXIT_FAILURE ); \
 \
            stp->hls_skp[2] = 's'; \
         }; \
      }; /* end [ if ( null < OPT ) \ */ \
   }; /* end [ if ( hls_skp[2] != 's' ) - don't 's'kip this message ] */ \
}
/*............................................................................*/
