/* [ file: STOREMAT.M ] */
/* Update: December 19, 2021 */
/*----------------------------------------------------------------------------*/
# define MAT_FORMATS( ) \
 \
   static const char \
     *format1 = "\n%-5s <--- %-59s", \
     *format2 = "\n%-22s%c%22s", \
     *format3 = "\n%-20s%c%20s", \
     *line1 = "\n---------------------------------------------", \
     *line2 = "\n-----------------------------------------", \
     *line3 = "================================="; 
 
# define STOREMAT( FN, MD ) \
{ \
   fprintf((FN), "%s\n", ( state->name )); \
   fprintf((FN), "%s\n", ( state->text )); \
 \
   if (( MD ) == 't' ) \
   { \
      fprintf((FN), format1, \
         mat.shi, mat.mti ); \
 \
      fprintf((FN), format1, \
         mat.sho, mat.mto ); \
 \
      fprintf((FN), format1, \
         mat.shd, mat.del ); \
 \
      fprintf((FN), format1, \
         mat.shj, mat.jck ); \
 \
      fprintf((FN), format1, \
         mat.shc, mat.cpe ); \
 \
      fprintf((FN), format1, \
         mat.shf, mat.fld ); \
      fprintf((FN), "\n" ); \
   } \
   else \
   { \
      fprintf((FN), "\n%-s: %5s", \
         mat.mti, mat.shi ); \
 \
      fprintf((FN), "\n%-s: %5s", \
         mat.mto, mat.sho ); \
 \
      fprintf((FN), "\n%-s: %5s", \
         mat.del, mat.shd ); \
 \
      fprintf((FN), "\n%-s: %5s", \
         mat.jck, mat.shj ); \
 \
      fprintf((FN), "\n%-s: %5s", \
         mat.cpe, mat.shc ); \
 \
      fprintf((FN), "\n%-s: %5s", \
         mat.fld, mat.shf ); \
      fprintf((FN), "\n" ); \
   }; \
 \
   if (( MD ) == 't' ) \
   { \
      fprintf((FN), "\n%s%s", \
         "LEGENDE [ material shorthands ] ====", line3 ); \
 \
      fprintf((FN), "\n%s\n", \
         mat.avm ); \
 \
      fprintf((FN), format2, \
         mat.msx[null], 124, mat.mtx[null] ); \
 \
      fprintf((FN), line1 ); \
 \
      for ( ii=ONE; ii<=mat.m; ii++ ) \
      { \
         fprintf((FN), format2, \
            mat.msx[ii], 124, mat.mtx[ii] ); \
      }; \
 \
      fprintf((FN), "\n\n%s\n", \
         mat.avd ); \
 \
      fprintf((FN), format2, \
         mat.dsx[null], 124, mat.dtx[null] ); \
 \
      fprintf((FN), line1 ); \
 \
      for ( ii=ONE; ii<=mat.d; ii++ ) \
      { \
         fprintf((FN), format2, \
            mat.dsx[ii], 124, mat.dtx[ii] ); \
      }; \
 \
      fprintf((FN), "\n\n%s\n", \
         mat.avs ); \
 \
      fprintf((FN), format2, \
         mat.ssx[null], 124, mat.stx[null] ); \
 \
      fprintf((FN), line1 ); \
 \
      for ( ii=ONE; ii<=mat.s; ii++ ) \
      { \
         fprintf((FN), format2, \
            mat.ssx[ii], 124, mat.stx[ii] ); \
      }; \
 \
      fprintf((FN), "\n\n%s\n", \
         mat.avf ); \
 \
      fprintf((FN), format2, \
         mat.fsx[null], 124, mat.ftx[null] ); \
 \
      fprintf((FN), line2 ); \
 \
      for ( ii=ONE; ii<=mat.f; ii++ ) \
      { \
         fprintf((FN), format2, \
            mat.fsx[ii], 124, mat.ftx[ii] ); \
      }; \
      fprintf( (FN), "\n" ); \
   } \
   else if ((MD) == 'm' ) \
   { \
      fprintf((FN), "\n%s%s", \
         "LEGENDE [ material shorthands ] ", line3 ); \
 \
      fprintf((FN), "\n%s\n", \
         mat.avm ); \
 \
      fprintf((FN), format3, \
         mat.mtx[null], 124, mat.msx[null] ); \
 \
      fprintf((FN), line2 ); \
 \
      for ( ii=ONE; ii<=mat.m; ii++ ) \
      { \
         fprintf((FN), format3, \
            mat.mtx[ii], 124, mat.msx[ii] ); \
      }; \
 \
      fprintf((FN), "\n\n%s\n", \
         mat.avd ); \
 \
      fprintf((FN), format3, \
         mat.dtx[null], 124, mat.dsx[null] ); \
 \
      fprintf((FN), line2 ); \
 \
      for ( ii=ONE; ii<=mat.d; ii++ ) \
      { \
         fprintf((FN), format3, \
            mat.dtx[ii], 124, mat.dsx[ii] ); \
      }; \
 \
      fprintf((FN), "\n\n%s\n", \
         mat.avs ); \
 \
      fprintf((FN), format3, \
         mat.stx[null], 124, mat.ssx[null] ); \
 \
      fprintf((FN), line2 ); \
 \
      for ( ii=ONE; ii<=mat.s; ii++ ) \
      { \
         fprintf((FN), format3, \
            mat.stx[ii], 124, mat.ssx[ii] ); \
      }; \
 \
      fprintf((FN), "\n\n%s\n", \
         mat.avf ); \
 \
      fprintf((FN), format3, \
         mat.ftx[null], 124, mat.fsx[null] ); \
 \
      fprintf((FN), line2 ); \
 \
      for ( ii=ONE; ii<=mat.f; ii++ ) \
      { \
         fprintf((FN), format3, \
            mat.ftx[ii], 124, mat.fsx[ii] ); \
      }; \
 \
      fprintf((FN), "\n" ); \
   }; \
} /* end of macro STOREMAT( ) */
