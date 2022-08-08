# define DO_FLUIDS "fluids(*)"
/*******************************************************************************
*                                                                              *
*   Function fluid(*); HELIOS release v1.0r1                                   *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns physical parameters for gases and cooling fluids     *
*   used in electric waveguides                                                *
*   [ Parameters at ZERO degree Celsius.]                                      *
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
# include "./types.h"
/*----------------------------------------------------------------------------*/
# ifndef null
   # define null 0
# endif
# ifndef ZERO
   # define ZERO ( 0.00000 ) 
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_FLUIDS
/*----------------------------------------------------------------------------*/
/* the gases and coolant fluids structure type: */
# define HLSTP_FLUIDS 1
typedef struct
{
   signed char
      rtn;

   char
      fld[30], /* name of dielectric: "air", "nitrogen", e.g. */
      shf[30]; /* shorthand [sign]: "AIR", "N2", "SF6",..., e.g.  */
   
   double  /* parameters at 0 DEG Celsius: */
      md,  /* mass density [ Kg/m^3 ] */
      epr, /* relative permittivity [ dimensionless ] */
      myr, /* relative permeability [ dimensionless ] */
      hc,  /* heat conductivity [ W/(K*m) ] */
      dhc, /* temperature coefficient of the latter [ 1/K ] */
      cv,  /* cp, heat capacity [ J/(Kg*K) ] */
      ny,  /* cinematic viscosity [ m^2 / s ] */
      dny, /* temperature coefficient of the latter [ 1/K ] */
      pr,  /* Prandtl number [ dimensionless ] */
      dpr; /* temperature coefficient of the latter [ 1/K ] */

} FLUIDS;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_FLUIDS */
/*----------------------------------------------------------------------------*/
/*
static FLUIDS fld = \
{ 0, 
  { 'a', 'i', 'r' },
  { 'A', 'I', 'R' },
  0., 0., 0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include the function body [ FLUIDS *fluids( FLUIDS *flp ) ] */
# include "fluids.h"
/*============================================================================*/
/*********************** end of function fluids(*) ****************************/
