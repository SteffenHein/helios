/* [ file: rdunit.h ] */
# define DO_RDUNIT "rdunit(*)"
/*******************************************************************************
*                                                                              *
*   Function rdunit(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
/*===========================================================================*/

char *rdunit( char *filename, char *unit )
{
   static FILE
     *fleptr;

   static char
     ptr0[80] = {0},
     ptr1[80] = {0};

   static short
      ii = 0,
      jj = 0;
/*............................................................................*/
   fleptr = fopen( filename, "r" );

   if ( NULL == fleptr )
   {
      fprintf( stderr, "\n can't open file %s\n", filename );
      exit( EXIT_FAILURE );
   }
   else
   {
      jj = strlen( unit );

      ii = 0; do
      {
         strcpy( ptr1, ptr0 );
         fscanf( fleptr, "%s", ptr0 );
      } while ((( ++ii ) < 20 )&&( strspn( unit, ptr0 ) < jj ));

      if ( ii >= 20 )
      {
         fprintf( stderr, "\n can't stat unit\n" );
         exit( EXIT_FAILURE );
      }
      else if ( 4 == strspn( "unit", ptr1 ))
      {
         return unit;
      }
      else
      {
         strcpy ( ptr0, ptr1 );
         strcat ( ptr0, " " );
         strcat ( ptr0, unit );
         return ptr0;
      };

      fclose( fleptr );
   };
   return NULL;
}
/*============================================================================*/
/************************ end of function rdunit(*) ***************************/
