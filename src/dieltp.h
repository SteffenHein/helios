/* [ file: dieltp.h ] */
/* Update: December 21, 2021 */
/*----------------------------------------------------------------------------*/
/* the dielectric materials structure type */
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_DIELCT
/*----------------------------------------------------------------------------*/
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
