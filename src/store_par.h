/* [ file: store_par.h ] */
/*******************************************************************************
*                                                                              *
*   Function store_par(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Physical operation parameters storage function                             *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

short store_params( char *filename, char mode )
{
/* allusions: */
/*
   extern HELIOSSTATE helios;
*/
/* declarations: */

   static HELIOSSTATE
     *state = &helios;

   static FILE 
     *paramtrs = NULL;

   static long 
      ll = null;

   static short
      ii = null,
      parameters = null;

   static char
     *timeptr = NULL,
      ptr[STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null};

   static const char /* dsplay(*) options */
     *scformat = "%80s",
     *dline = "========================================"\
              "========================================";

   MAT_FORMATS( )

   time_t 
      nseconds = null,
     *timer = NULL;

   char
      *lotos( long mm, char cc );
/*----------------------------------------------------------------------------*/
   strcpy( fleptr, filename );

   paramtrs = fopen( fleptr, "w+" );  /* save coordinates on log file */

   ii = null;
   while ( ii < IPT_MAXLBL )
   {
      fscanf( paramtrs, scformat, ptr );

      if ( null == strncmp( ptr, "PARAMETERS", FIVE ))
      {
         ll = ftell( paramtrs ) + ONE;
         break;
      }
      else
      {
         ii++;
         if ( ii == IPT_MAXLBL )
         {
            if ( mode == 't' ) /* temporary file */
            { 
               fprintf( paramtrs, "%s\n", "The actually charged" );
               fprintf( paramtrs,
                  "%s%s\n", "--> PARAMETERS-", lotos(( state->job ), null ));
            }
            else
               fprintf( paramtrs,
                  "%s%s\n", "PARAMETERS-", lotos(( state->job ), null ));

            ll = ftell( paramtrs );
         };
      };
   };

   fseek( paramtrs, ll, SEEK_SET );

   parameters = ( short ) par.s[null]; /* the number of parameters */
/*............................................................................*/
   STOREPAR( paramtrs, mode );
/*............................................................................*/
/* append material parameters: */

   if ( mode != 't' ) /* eventually append complete configuration files */
   {
/* append material parameters file: */

      fprintf( paramtrs,
         "\n%s\n", dline );

      fprintf( paramtrs,
         "\n%s%s\n", "MATERIALS-", lotos(( state->job ), null ));

/*............................................................................*/
      STOREMAT( paramtrs, 'p' );
/*............................................................................*/
/* append operation parameters file: */

      fprintf( paramtrs,
         "\n%s\n", dline );

      fprintf( paramtrs,
         "\n%s%s\n", "OPERATIONS-", lotos(( state->job ), null ));

/*............................................................................*/
      STOREOPR( paramtrs, 'p' );
/*............................................................................*/
   };

   nseconds = time( timer );
   timeptr = ctime( &nseconds );

   fprintf( paramtrs, "\nHELIOS input parameter file %s created:", fleptr );
   fprintf( paramtrs, "\n%.24s", timeptr );
   fprintf( paramtrs, "\n%c", EOF );

   fclose( paramtrs );

   return null;
}
/*============================================================================*/
/********************* end of function store_params(*) ************************/
