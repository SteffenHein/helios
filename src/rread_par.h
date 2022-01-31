/* [ file: rread_par.h ] */
/*******************************************************************************
*                                                                              *
*   function rread_par.h(*); HELIOS, release v1.0r1                            *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Parameter file [par.log<N>] reading function                               *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

short rread_params( char *filename, char mode )
{
/* allusions: */
/*
   extern char
      tmpfle[STS_SIZE] = null;
   extern HELIOSSTATE helios;
*/
/* declarations: */

   static HELIOSSTATE 
     *state = &helios;

   static FILE
     *paramtrs = NULL;

   static short 
      ii = null,
      jj = null,
      parameters;

   static char
      ptr[STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null},
    **endp = NULL;

   static const char 
     *scformat = "%80s";

/*----------------------------------------------------------------------------*/
   ( state->prp ) = &par;

   strcpy( fleptr, filename );

   while( *fleptr != '0' )
   {
      paramtrs = fopen( fleptr, "r+" );

      while ( paramtrs == null )
      {
         if (( state->uif ) == 't' )
         {
            printf( "\n Parameter file %s not found "
               "in present directory.", fleptr );
            printf( "\n Please re-enter filename [ Escape: "
               "enter null ] >----> " );
            scanf( "%s", fleptr );

            if ( *fleptr == '0' )
               return null;
         }
         else
         {
            fprintf( stderr, "\nparameter file %s not found "
               "in working directory !\n", fleptr );
            exit( EXIT_FAILURE );
         };

         paramtrs = fopen( fleptr, "r+" );
      };

      ii = null;
      while ( ii < IPT_MAXLBL )
      {
         fscanf( paramtrs, scformat, ptr );

         if ( null == strncmp( ptr, "PARAMETERS", 10 ))
         {
            fscanf( paramtrs, scformat, ptr ); /* string: program name */
            fscanf( paramtrs, scformat, ptr ); /* string: any comment */

            parameters = ( short ) par.s[null];

            fscanf( paramtrs, scformat, ptr ); /* string "Parameters" */
            fscanf( paramtrs, scformat, ptr ); /* string "[INTERNATIONAL..." */

            if ( mode == 't' )
            {
               for ( jj=ONE; jj<=parameters; jj++ )
               {
                  fscanf( paramtrs, scformat, ptr );

		  if ( null == strncmp( ptr, "Inf", THREE ))
                     par.s[jj] = HUGE_VALF;
                  else if ( null == strncmp( ptr, "HUGE", FOUR ))
                     par.s[jj] = HUGE_VALF;
                  else
                     par.s[jj] = strtod( ptr, endp );

                  fscanf( paramtrs, scformat, ptr ); /* string "<---" */
                  fscanf( paramtrs, scformat, ptr ); /* text string */
               };
            }
            else /* if mode != 't'emporary file */
            {
               for ( jj=ONE; jj<=parameters; jj++ )
               {
                  fscanf( paramtrs, scformat, ptr ); /* text string */
                  fscanf( paramtrs, scformat, ptr ); /* double string */

                  if ( null == strncmp( ptr, "Inf", THREE ))
                     par.s[jj] = HUGE_VALF;
                  else if ( null == strncmp( ptr, "HUGE", FOUR ))
                     par.s[jj] = HUGE_VALF;
                  else
                     par.s[jj] = strtod( ptr , endp );
               };
            };

            fclose( paramtrs );
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
                  printf( "\n parameters not found "
                     "in file %s ,\n", fleptr );
                  printf( "\n please re-enter filename [ Escape: "
                     "enter null ] >----> " );
                  scanf( "%s", fleptr );

                  if ( *fleptr == '0' )
                     return null;
               }
               else
               {
                  fprintf( stderr, "\nparameters not found "
                     "in file %s !\n", fleptr );
                  exit( EXIT_FAILURE );
               };
            };
         }; /* end if( ptr != "PARAMETERS" ) */
      }; /* end while( ii < IPT_MAXLBL ) */
   }; /* end while( *flelbl == '0' ) */

   fclose( paramtrs );
   return null;
}
/*============================================================================*/
/********************* end of function rread_params(*) ************************/
