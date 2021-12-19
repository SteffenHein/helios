/* [ file: store_opr.h ] */
/*******************************************************************************
*                                                                              *
*   Function store_opr(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Operation modes storage function                                           *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

short store_operts( char *filename, char mode )
{
/* allusions: */
/*
   extern HELIOSSTATE helios;
*/
/* declarations: */

   static HELIOSSTATE
     *state = &helios;

   static FILE 
     *operats = NULL;

   static long 
      ll = null;

   static short
      ii = null;

   static char
     *timeptr = NULL,
      ptr[STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null};

   static const char
     *scformat = "%80s";

   time_t 
      nseconds = null,
     *timer = NULL;

   char
      *lotos( long mm, char cc );
/*----------------------------------------------------------------------------*/
   strcpy( fleptr, filename );

   operats = fopen( fleptr, "w+" );

   ll = null;
   ii = null;
   while ( ii < IPT_MAXLBL )
   {
      fscanf( operats, scformat, ptr );

      if ( null == strncmp( ptr, "OPERATIONS", FIVE ))
      {
         ll = ftell( operats ) + ONE;
            break;
      }
      else
      {
         ii++;
         if( ii == IPT_MAXLBL )
         {
            if ( mode == 't' ) /* temporary file */
            {
               fprintf( operats, "%s\n", "The actually defined" );
               fprintf( operats,
                  "%s%s\n", "--> OPERATIONS-", lotos(( state->job ), null ));
            }
            else
               fprintf( operats,
                  "%s%s\n", "OPERATIONS-", lotos(( state->job ), null ));

            ll = ftell( operats );
         };
      };
   };

   fseek( operats, ll, SEEK_SET );

   STOREOPR( operats, mode );

   nseconds = time( timer );
   timeptr = ctime( &nseconds );

   fprintf( operats, "\nHELIOS operation logfile %s created:", fleptr );
   fprintf( operats, "\n%.24s", timeptr );
   fprintf( operats, "\n%c", EOF );

   fclose( operats );

   return null;
}
/*============================================================================*/
/********************* end of function store_operts(*) ************************/
