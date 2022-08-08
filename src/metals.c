# define DO_METALS "metals(*)"
/*******************************************************************************
*                                                                              *
*   Function metal(*); HELIOS release v1.0r1                                   *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Returns physical parameters for metals commonly used in electric           *
*   waveguides.                                                                *
*   [ Parameters at ZERO degree Celsius ]                                      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define _POSIX_SOURCE 1 /* some headers of the POSIX.1 standard will be used */
/*----------------------------------------------------------------------------*/
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
# include <string.h>
# include <ctype.h>
# include <time.h>            /* cf. time( ),ctime( ),asctime( ),localtime( ) */
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
/* Edit and customize this general configuration header: */
# include "../CONFIG.H" 
/*----------------------------------------------------------------------------*/
# ifndef MET_DEFLT
   # define MET_DEFLT 1
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLS_PARSET
   # define HLS_PARSET 1
# endif
/*----------------------------------------------------------------------------*/
# include "types.h"
/*----------------------------------------------------------------------------*/
# ifndef null
   # define null 0
# endif
# ifndef ZERO
   # define ZERO ( 0.00000 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_METALS
/*----------------------------------------------------------------------------*/
/* the metals [parameter] structure type */
# define HLSTP_METALS 1
typedef struct
{
   signed char 
      rtn;

   char
      met[30], /* metal name: "Aluminium", "Copper", ..., e.g. */
      shm[30]; /* sign: "Al", "Cu", ..., e.g.  */
   
   double  /* parameters at 0 DEG Celsius: */
      md,  /* mass density [ Kg/m^3 ] */
      myr, /* relative permeability [ dimensionless ( enters skin effect )] */
      er,  /* electric resistivity [ Ohm*m ] */
      der, /* temperature coefficient of the latter [ 1/K ] */
      hc,  /* heat conductivity [ W/(K*m) ] */
      dhc, /* temperature coefficient of the latter [ 1/K ] */
      cv;  /* heat capacity [ J/(Kg*K) ] */

} METALS;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_METALS */
/*----------------------------------------------------------------------------*/
/*
static METALS met = \
{ 0, 
  { 'S', 'i', 'l', 'v', 'e', 'r' },
  { 'A', 'g' },
  0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include function body [ METALS *metals( METALS *mpt ) ] */
# include "metals.h"
/*============================================================================*/
# undef MET_DEFLT
# undef HLS_PARSET
/************************ end of function metals(*) ***************************/
