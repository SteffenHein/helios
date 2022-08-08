/* [ file: metltp.h ] */
/* Update: December 17, 2021 */
/*----------------------------------------------------------------------------*/
/* the metals [parameter] structure type */
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_METALS
/*----------------------------------------------------------------------------*/
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
