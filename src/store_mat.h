/* [ file: store_mat.h ] */
/*******************************************************************************
*                                                                              *
*   Function store_mat(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Material parameter storage function                                        *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

short store_matter( char *filename, char mode )
{
/* allusions: */
/*
   extern HELIOSSTATE helios;
*/
/* declarations: */

   static HELIOSSTATE
     *state = &helios;

   static FILE 
     *materls = NULL;

   static long 
      ll = null;

   static short
      ii = null;

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

   materls = fopen( fleptr, "w+" );  /* save coordinates on log file */

   ii = null;
   while ( ii < IPT_MAXLBL )
   {
      fscanf( materls, scformat, ptr );

      if ( null == strncmp( ptr, "MATERIALS", FIVE ))
      {
         ll = ftell( materls ) + ONE;
         break;
      }
      else
      {
         ii++;
         if ( ii == IPT_MAXLBL )
         {
            if ( mode == 't' ) /* temporary file */
            { 
               fprintf( materls, "%s\n", "The actually charged" );
               fprintf( materls,
                  "%s%s\n", "--> MATERIALS-", lotos(( state->job ), null ));
            }
            else
               fprintf( materls,
                  "%s%s\n", "MATERIALS-", lotos(( state->job ), null ));

            ll = ftell( materls );
         };
      };
   };

   fseek( materls, ll, SEEK_SET );

   STOREMAT( materls, mode );

   if ( mode != 't' ) /* eventually append other configuration files */
   {
/* append operation parameters file: */

      fprintf( materls, "\n%s\n", dline );

      fprintf( materls, "\n%s%s\n",
         "OPERATIONS-", lotos (( state->job ), null ));

      STOREOPR( materls, 'm' );
   };

   nseconds = time( timer );
   timeptr = ctime( &nseconds );

   fprintf( materls, "\nHELIOS materials input file %s created:", fleptr );
   fprintf( materls, "\n%.24s", timeptr );
   fprintf( materls, "\n%c", EOF );

   fclose( materls );

   return null;
}
/*============================================================================*/
/********************* end of function store_matter(*) ************************/
