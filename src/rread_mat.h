/* [ file: rread_mat.h ] */
/*******************************************************************************
*                                                                              *
*   function rread_mat.h(*); HELIOS, release v1.0r1                            *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Material parameter file [ opr.log<N> ] reading function                    *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 28, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

short rread_matter( char *filename, char mode )
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

   static short 
      ii = null;

   static char
      ptr[STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null};

   static const char
     *scformat = "%80s";
/*
   static char 
    **endp = NULL;
*/
/*----------------------------------------------------------------------------*/
   ( state->map ) = &mat;

   strcpy( fleptr, filename );

   while( *fleptr != '0' )
   {
      materls = fopen( fleptr, "r+" );

      while ( materls == null )
      {
         if (( state->uif ) == 't' )
	 {
            printf( "\n Material parameters file %s not found "
               "in present directory.", fleptr  );
            printf( "\n Please re-enter filename [ Escape: "
               "enter null ] >----> " );
            scanf( "%s", fleptr );

            if ( *fleptr == '0' )
               return null;
         }
	 else
	 {
            fprintf( stderr, "\nmaterial parameters file %s not found "
	       "in working directory\n", fleptr );
            exit( EXIT_FAILURE );
         };

         materls = fopen( fleptr, "r+" );
      };

      ii = null;
      while ( ii < IPT_MAXLBL )
      {
         fscanf( materls, scformat, ptr );

         if ( null == strncmp( ptr, "MATERIALS", 9 ))
         {
            fscanf( materls, scformat, ptr ); /* spt->name */
            fscanf( materls, scformat, ptr ); /* spt->text */

            if ( mode == 't' ) /* read from temporary file */
	    {
               fscanf( materls, scformat, mat.shi );
               fscanf( materls, scformat, ptr ); /* string "<---", e.g. */
               fscanf( materls, scformat, ptr );

               fscanf( materls, scformat, mat.sho );
               fscanf( materls, scformat, ptr ); /* string "<---", e.g. */
               fscanf( materls, scformat, ptr );

               fscanf( materls, scformat, mat.shd );
               fscanf( materls, scformat, ptr ); /* string "<---", e.g. */
               fscanf( materls, scformat, ptr );

               fscanf( materls, scformat, mat.shj );
               fscanf( materls, scformat, ptr ); /* string "<---", e.g. */
               fscanf( materls, scformat, ptr );

               fscanf( materls, scformat, mat.shc );
               fscanf( materls, scformat, ptr ); /* string "<---", e.g. */
               fscanf( materls, scformat, ptr );

               fscanf( materls, scformat, mat.shf );
               fscanf( materls, scformat, ptr ); /* string "<---", e.g. */
            }
	    else /* mode != 't'emporary file */
	    {
               fscanf( materls, scformat, ptr );
               fscanf( materls, scformat, mat.shi );

               fscanf( materls, scformat, ptr );
               fscanf( materls, scformat, mat.sho );

               fscanf( materls, scformat, ptr );
               fscanf( materls, scformat, mat.shd );

               fscanf( materls, scformat, ptr );
               fscanf( materls, scformat, mat.shj );

               fscanf( materls, scformat, ptr );
               fscanf( materls, scformat, mat.shc );

               fscanf( materls, scformat, ptr );
               fscanf( materls, scformat, mat.shf );
            }; /* end if mode != 't' */

            fclose( materls );
            return null;
            break;
         }
         else
         {
            ii++;
            if ( ii == IPT_MAXLBL )
            {
               if (( state->uif ) == 't' )
	       {
                  printf( "\n material parameters not found "
                     "in file %s:\n", fleptr );
                  printf( "\n please re-enter filename [ Escape: "
                     "enter null ] >----> " );
                  scanf( "%s", fleptr );

                  if ( *fleptr == '0' )
                     return null;
               }
	       else
	       {
                  fprintf( stderr, "\nmaterial parameters not found "
	             "in file %s !\n", fleptr );
                  exit( EXIT_FAILURE );
               };
            }
         }; /* end if( ptr != "MATERIALS" ) */
      }; /* end while( ii < IPT_MAXLBL ) */
   }; /* end while( *flelbl == '0' ) */

   fclose( materls );
   return null;
}
/*============================================================================*/
/********************* end of function rread_matter(*) ************************/
