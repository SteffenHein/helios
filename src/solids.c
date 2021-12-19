# define DO_SOLIDS "solids(*)"
/*******************************************************************************
*                                                                              *
*   Function solids(*); HELIOS release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns physical parameters for solid materials used in      *
*   electric waveguides.                                                       *
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
# ifndef SLD_DEFLT
   # define SLD_DEFLT 1
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLS_PARSET
   # define HLS_PARSET 1
# endif
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
# ifndef HLSTP_SOLIDS
/*----------------------------------------------------------------------------*/
/* the solid materials structure type */
# define HLSTP_SOLIDS 1
typedef struct
{
   signed char
      rtn;

   char
      sld[30], /* name of dielectric: "Teflon", "Polyurethane", ..., e.g. */
      shs[30]; /* sign: "PTFE", "PU", ..., e.g.  */
   
   double   /* parameters at 0 DEG Celsius: */
      md,   /* mass density [ Kg/m^3 ] */
      er,   /* electric resistivity [ Ohm*m ] */
      der,  /* temperature coefficient of the latter [ 1/K ] */
      epr,  /* relative permittivity */
      tge,  /* electric loss factor, tangent delta_E */
      myr,  /* relative permeability */
      tgm,  /* magnetic loss factor; tangent delta_M */
      hc,   /* heat conductivity [ W/(K*m) ] */
      dhc,  /* temperature coefficient heat conductivity [ percent/K ] */
      cv;   /* specific heat capacity [ J/(Kg*K) ] */

} SOLIDS;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_SOLIDS */
/*----------------------------------------------------------------------------*/
/*
static SOLIDS sld = \
{ 0, 
  { 'T', 'e', 'f', 'l', 'o', 'n' },
  { 'P', 'T', 'F', 'E' },
  0., 0., 0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include the function body [ SOLIDS *solids( SOLIDS *spt ) ] */
# include "solids.h"
/*============================================================================*/
# undef SLD_DEFLT
# undef HLS_PARSET
/*********************** end of function solids(*) ****************************/
