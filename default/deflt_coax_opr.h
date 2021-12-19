/* [ file: deflt_opr.h ] */
/* Update: December 17, 2021 */
/*----------------------------------------------------------------------------*/
# ifndef LINLEN
   # define LINLEN 58
# endif
/*----------------------------------------------------------------------------*/
# ifdef OPR_SSTRNG
   # include "../src/OPRSTRNGS.M"
# endif
/*============================================================================*/

void deflt_operts( void ) /* default HELIOS operation modes */
{
/* declarations: - */

   static short
      ll = LINLEN;

   static char
      ptr[STS_SIZE] = {null};

/* allusions: - */
/*
   extern CODISSTATE *spt;
*/
/* prototypes: */   

   char
      *lotos( long nn, char cc );

   void
      cpylne( char lne[], const char *txt, const char *brc, short linlen );
/*----------------------------------------------------------------------------*/
   OPRSTRNGS( ); /* copy the operation options strings */
/*............................................................................*/
/* number of parameters: */

   opr.n[0] = 10;     /* number M of parameters opr.n[1],...,opr.n[M] */

/*............................................................................*/
/* defaults: structure/reference line; time/frequency domain: */

   opr.n[1] = 1; /* 0/1: rectangular / coaxial waveguide */
   opr.n[2] = 0; /* 0/1: fixed CW power / maximum inner condctr temperature */
   opr.n[3] = 1; /* 0/1: solid / gas dielectric */
   opr.n[4] = 0; /* 0/1: free / forced convection [ between conductors ] */
   opr.n[5] = 1; /* 0/1: fixed wall temperature / convection */
   opr.n[6] = 0; /* 0/1: free / forced convection [ in environment ] */
   opr.n[7] = 0; /* 0/1: remove / set additional inner conductor cooling */
   opr.n[8] = 3; /* 0/1/2/3 fixed temperature on one side */
   opr.n[9] = 0; /* 0/1/2/3 fixed heat outflow on one side */
   opr.n[10] = 20; /* number of frequency points */

   return;
}
/*============================================================================*/
# undef LINLEN
/************************ end of function deflt_opr(*) ************************/
