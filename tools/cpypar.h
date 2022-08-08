/* [ file: cpypar.h ] */
/*******************************************************************************
*                                                                              *
*   Function cpypar(*); HELIOS release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function copies a parameter string [ along with a bracketed text ]    *
*   into the parameter structure *.par of program HELIOS.                      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define LLENGTH 53
# include <string.h>
/*============================================================================*/

void cpypar( short index, const char *line, const char *bracket )
{
   static short
      ii = null,
      jj = null;

   static char
      ptr[STS_SIZE] = {null};

/* prototypes: */   
/*
   size_t strlen( const char *s );
   char *strcpy( char *dst, const char *src ):
   char *strcat( char *dst, const char *src ):
*/
/*----------------------------------------------------------------------------*/
/* clear ptr[]: */

   ii = null;
   while( ii < LLENGTH )
      ptr[(ii++)] = null;

   strcpy( ptr, line );

   ii = ( short ) strlen( line );
   jj = ( short ) strlen( bracket )+TWO;
   while( ii < ( LLENGTH-jj ))
      ptr[(ii++)] = 95; /* ASCII: '_' */

   ptr[(ii++)] = 91; /* ASCII: '[' */
   strcat( ptr, bracket );
   ptr[(LLENGTH-ONE)] = 93; /* ASCII ']' */
   strcpy( par.stx[index], ptr );
}
/*============================================================================*/
# undef LLENGTH
/************************** end of function cpypar ****************************/
