/* [ file: fluidtp.h ] */
/* Update: December 21, 2021 */
/*----------------------------------------------------------------------------*/
/* the gases and coolant fluids structure type: */
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_FLUIDS
/*----------------------------------------------------------------------------*/
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
