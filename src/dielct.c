# define DO_DIELCT "dielct(*)"
/*******************************************************************************
*                                                                              *
*   Function dielec(*); HELIOS release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns physical parameters for dielectric material commonly *
*   used in electric waveguides                                                *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
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
# ifndef DEL_DEFLT
   # define DEL_DEFLT 1
# endif
/*----------------------------------------------------------------------------*/
# include "./types.h"
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_DIELCT
/*----------------------------------------------------------------------------*/
/* the dielectric materials structure type */
# define HLSTP_DIELCT 1
typedef struct
{
   signed char
      rtn;

   char
      del[30], /* name of dielectric: "Teflon", "Polyurethane", ..., e.g. */
      shd[30]; /* sign: "PTFE", "PU", ..., e.g.  */
   
   double  /* parameters at 0 DEG Celsius: */
      md,  /* mass density [ Kg/m^3 ] */
      epr, /* relative permittivity */
      tge, /* electric loss factor, tangent delta_E */
      myr, /* relative permeability */
      tgm, /* magnetic loss factor; tangent delta_M */
      hc,  /* heat conductivity [ W/(K*m) ] */
      dhc, /* temperature coefficient heat conductivity [ 1/K ] */
      cv,  /* specific heat capacity [ J/(Kg*K) ] */
      ny,  /* cinematic viscosity [ m^2/s - only defined for gases & fluids ]*/
      dny, /* temperature coefficient of the latter [ 1/K ] */
      pr,  /* Prandtl number [ dimensionless - only gases & fluids ] */
      dpr; /* temperature coefficient of the latter [ 1/K ] */

} DIELCT;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_DIELCT */
/*----------------------------------------------------------------------------*/
/*
static DIELCT del = \
{ 0, 
  { 'T', 'e', 'f', 'l', 'o', 'n' },
  { 'P', 'T', 'F', 'E' },
  0., 0., 0., 0., 0., 0., 0., 0. \
};
*/
/*----------------------------------------------------------------------------*/
# ifndef null
   # define null 0
# endif
# ifndef ZERO
   # define ZERO ( 0.00000 ) 
# endif
/*============================================================================*/
/* include the function body [ DIELCT *dielct( DIELCT *dpt ) ] */
# include "dielct.h"
/*============================================================================*/
# undef DEL_DEFLT
/*********************** end of function dielct(*) ****************************/
