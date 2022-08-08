/* [ file: ENDOTH.M ] */
/*******************************************************************************
*                                                                              *
*   Macro ENDOTH [ endothermie check ]                                         *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 19, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/
# define ENDOTH( STUB, CHCKD, BOUND, OPT ) \
{ \
   if ( stp->hls_skp[3] != 's' ) \
   { \
      if ( null < OPT ) \
      { \
         if ( BOUND < CHCKD ) \
         { \
            errfle = fopen(( state->errfle ), "a+" ); \
 \
            if ( OPT == ONE ) \
            { \
               fprintf( errfle, \
                  "\nWarning message from job %s, function %s :", \
                  lotos (( state->job ), null ), __func__ ); \
            } \
            else if ( OPT == TWO ) \
            { \
               fprintf( errfle, \
                  "\nError message from job %s, function %s :", \
                  lotos (( state->job ), null ), __func__ ); \
            }; \
 \
            fprintf( errfle, \
               "\nEndothermic operating conditions" \
               " on "STUB" at %.7e Hz :", ( stp->elp->freq )); \
            fprintf( errfle, \
               "\nNet heat inflow into "STUB" from waveguide sides !" ); \
 \
            if ( null == strcmp( STUB, "mantle" )) \
            { \
               fprintf( errfle, \
                  "\nHeat outflow = %.7e Watts < inflow = %.7e Watts.", \
                  stp->thp->sdofm, stp->thp->sdifm ); \
            } \
            else if ( null == strcmp( STUB, "pipe" )) \
            { \
               fprintf( errfle, \
                  "\nHeat outflow = %.7e Watts < inflow = %.7e Watts.", \
                  stp->thp->sdofp, stp->thp->sdifp ); \
            }; \
 \
            fprintf( errfle, \
               "\n\nRevise your parameters " \
               "and operating conditions.\n" ); \
 \
            fclose( errfle ); \
 \
            if ((( stp->uif ) != 'b' ) \
              &&(( stp->uif ) != 'f' ) \
              &&(( stp->uif ) != 'g' )) \
            { \
               if ( OPT == ONE ) \
               { \
                  fprintf( stdout, \
                     "\n\n Warning message from job %s, function %s :", \
                     lotos (( state->job ), null ), __func__ ); \
               } \
               else if ( OPT == TWO ) \
               { \
                  fprintf( stdout, \
                     "\n\n Error message from job %s, function %s :", \
                  lotos (( state->job ), null ), __func__ ); \
               }; \
 \
               fprintf( stdout, \
                  "\n\n Endothermic operating conditions on "STUB" at" \
                  " %.7e Hz :", ( stp->elp->freq )); \
               fprintf( stdout, \
                  "\n Net heat inflow into "STUB" from waveguide sides !" ); \
 \
               if ( null == strcmp( STUB, "mantle" )) \
               { \
                  fprintf( stdout, \
                  "\n Heat outflow = %.7e Watts < inflow = %.7e Watts.", \
                  stp->thp->sdofm, stp->thp->sdifm ); \
               } \
               else if ( null == strcmp( STUB, "pipe" )) \
               { \
                  fprintf( stdout, \
                     "\n Heat outflow = %.7e Watts < inflow = %.7e Watts.", \
                     stp->thp->sdofp, stp->thp->sdifp ); \
               }; \
 \
               fprintf( stdout, \
                  "\n\n Revise your parameters " \
                  "and operating conditions.\n\n" ); \
            }; /* end [ if ( uif != 'b', 'f', 'g') ] */ \
 \
            if ( OPT == TWO ) \
               exit( EXIT_FAILURE ); \
 \
            stp->hls_skp[3] = 's'; \
         }; \
      }; /* end [ if ( null < OPT ) \ */ \
   }; /* end [ if ( hls_skp[3] != 's' ) - don't 's'kip this message ] */ \
}
/*............................................................................*/
