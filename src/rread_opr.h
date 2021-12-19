/* [ file: rread_opr.h ] */
/*******************************************************************************
*                                                                              *
*   function rread_opr.h(*); HELIOS, release v1.0r1                            *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Oparation parameter file [ opr.log<N> ] reading function                   *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

short rread_operts( char *filename, char mode )
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

   static short 
      ii = null,
      jj = null;

   static char
      ptr[STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null},
    **endp = NULL;

   static const char
     *scformat = "%80s";
/*----------------------------------------------------------------------------*/
   ( state->opp ) = &opr;

   strcpy( fleptr, filename );

   while( *fleptr != '0' )
   {
      operats = fopen( fleptr, "r+" );

      while ( operats == null )
      {
         if (( state->uif ) == 't' )
         {
            printf( "\n operations file %s not found "
               "in present directory:\n", fleptr );
            printf( "\n please re-enter filename [ Escape: "
               "enter null ] >----> " );

            scanf( "%s", fleptr );

            if ( *fleptr == '0' )
               return null;
         }
	 else
         {
            fprintf( stderr, "\noperations file %s not found "
	       "in working directory\n", fleptr );
            exit( EXIT_FAILURE );
         };

         operats = fopen( fleptr, "r+" );
      };

      ii = null;
      while ( ii < IPT_MAXLBL )
      {
         fscanf( operats, scformat, ptr );

         if ( null == strncmp( ptr, "OPERATIONS", 10 ))
         { 
            fscanf( operats, scformat, ptr ); /* string spt->name */
            fscanf( operats, scformat, ptr ); /* string spt->text */
            fscanf( operats, scformat, ptr ); /* string opr.ntx[null] */

            if ( mode == 't' ) /* read from temporary file */
	    {
               for ( jj=ONE; jj<=opr.n[null]; jj++)
               {
                  fscanf( operats, scformat, ptr ); /* parameter [string] */
                  opr.n[jj] = ( short ) strtol( ptr, endp, DEC ); /* option */
                  fscanf( operats, scformat, ptr ); /* string "<---", e.g. */
                  fscanf( operats, scformat, ptr ); /* parameter name: opr.ntx[jj]*/
               };
            }
	    else /* parameter log file, e.g. */
	    {
               for ( jj=ONE; jj<=opr.n[null]; jj++)
               {
                  fscanf( operats, scformat, ptr ); /* parameter name */
                  fscanf( operats, scformat, ptr ); /* parameter */
                  opr.n[jj] = ( short ) strtol( ptr, endp, DEC ); /* option */
               };
            };

            fclose( operats );
            return null;
	    break;
         }
         else
         {
            ii++;

            if( ii == IPT_MAXLBL )
            {
               if (( state->uif ) == 't' )
	       {
                  printf( "\n operation parameters not found "
                     "in file '%s':\n", fleptr );
                  printf( "\n please re-enter filename [ Escape: "
                     "enter null ] >----> " );

                  scanf( "%s", fleptr );

                  if ( *fleptr == '0' )
                     return null;
               }
	       else
	       {
                  fprintf( stderr, "\noperation parameters not found "
	             "in file '%s'\n", fleptr );
                  exit( EXIT_FAILURE );
               };
            };
         }; /* end if( ptr != "OPERATIONS" ) */
      }; /* end while( ii < IPT_MAXLBL ) */
   }; /* end while( *fleptr != '0' */

   return null;
}
/*============================================================================*/
/********************* end of function rread_operts(*) ************************/
