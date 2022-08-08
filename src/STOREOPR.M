/* [ file: STOREOPR.M ] */
/* Update: December 19, 2021 */
/*----------------------------------------------------------------------------*/
# define STOREOPR( FN, MD ) \
{ \
   fprintf((FN), "%s\n", ( state->name )); \
   fprintf((FN), "%s\n", ( state->text )); \
   fprintf((FN), "\n%s\n", opr.ntx[null] ); \
 \
   if (( MD ) == 't' ) \
   { \
      for ( ii=ONE; ii<=opr.n[null]; ii++ ) \
      { \
         fprintf(( FN ), "\n%-3d <---  %s", opr.n[ii], opr.ntx[ii] ); \
      }; \
   } \
   else \
   { \
      for ( ii=ONE; ii<=opr.n[null]; ii++ ) \
      { \
         fprintf(( FN ), "\n%s: %5d", opr.ntx[ii], opr.n[ii] ); \
      }; \
   }; \
 \
   fprintf((FN), "\n" ); \
} 
