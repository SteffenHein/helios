/* [ file: soldtp.h ] */
/* Update: December 17, 2021 */
/*----------------------------------------------------------------------------*/
/* the solid materials structure type */
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_SOLIDS
/*----------------------------------------------------------------------------*/
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
# endif
/*----------------------------------------------------------------------------*/
