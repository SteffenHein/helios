/* [ file: cpylne.c ] */
/*******************************************************************************
*                                                                              *
*   Function cpylin(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function copies a text string together with a comment into a text     *
*   line of fixed length nn, the comment appearing in brackets at the line     *
*   end.                                                                       *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# include <stdio.h>
# include <string.h>
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
# ifndef LINLEN
   # define LINLEN 50
# endif
/*============================================================================*/

void cpylne( char txlne[], const char *ltext, const char *bracket, short ll )
{
   static short
      ii = null,
      jj = null,
      nn = null;
/*
   size_t strlen ( const char *ptr );  
   char *strcpy( char *tgt, const char *src );
   char *strcat( char *tgt, const char *src );
*/
/*----------------------------------------------------------------------------*/
   if ( ll <= null )
      nn = LINLEN;
   else
      nn = ll;
/*............................................................................*/

   ii = null;
   while( ii <= nn )
      txlne[(ii++)] = null;

   ii = ( short ) strlen( ltext );
   strcpy( txlne, ltext );

   jj = ( short ) strlen( bracket );

   if ( null < jj )
      jj += TWO;

   while( ii < ( nn-jj ))
      txlne[(ii++)] = 95; /* ASCII: '_' */

   if ( null < jj  )
   {
      txlne[(ii++)] = 91; /* ASCII: '[' */
      strcat( txlne, bracket );
      txlne[(nn-ONE)] = 93; /* ASCII ']' */
   };
} 
/*============================================================================*/
# undef LINLEN
/************************* end of function cpylne(*) **************************/
