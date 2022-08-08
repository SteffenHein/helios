/* [ file: losses.c ] */
# define DO_LOSSES "losses(*)"
/*******************************************************************************
*                                                                              *
*   Function losses(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guide Structures ]             *
*                                                                              *
*   This function computes the dielectric and skin effect losses in a coaxial  *
*   line, given the frequency, [ reflectionless ] transmitted CW power, and    *
*   the outer and inner conductor temperatures.                                *
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
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
/* Edit and customize this general configuration header: */
# include "../CONFIG.H"
/*----------------------------------------------------------------------------*/
# define PRINTF(SS1,SS2,XX) \
{ \
   printf( "\n %s   % .12e", (SS2), ( double )(XX)); \
   printf( "\n please acknowledge (%s) [ enter any character ]:", (SS1)); \
   scanf( "%s", ptr ); \
}
/*----------------------------------------------------------------------------*/
# include "./DISPLAY.M"
/*----------------------------------------------------------------------------*/
# ifndef EPS_VAC
   # define EPS_VAC ( 8.8541878170e-12 ) /* vac. permittivity [A*sec/(V*m)]   */
# endif
/*----------------------------------------------------------------------------*/
# ifndef MY_VAC_
   # define MY_VAC_ ( 1.2566370614e-06 ) /* "    permeability [V*sec/(A*m)]   */
# endif
/*----------------------------------------------------------------------------*/
# ifndef LSS_UPDWGT /* [ may be defined in CONFIG.H ] */
   # define LSS_UPDWGT ( 7.70e-01 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
/* Gavitation acceleration [m/s^2] */
# ifndef GRAVIT_ACC
   # define GRAVIT_ACC ( 9.81000e+00 )
# endif
/*----------------------------------------------------------------------------*/
/* structures typedefs, etc.: */

# include "./types.h"
/*----------------------------------------------------------------------------*/
/* the function body [ ELECTRIC *losses( HELIOSSTATE *state ) ]*/

# include "./losses.h"
/*============================================================================*/
# undef CELSIUS_TO_KELVIN 
/*********************** end of function losses(*) ***************************/
