/* [ file: grphtp.h ] */
/* Update: 08 November 2016 */
/*--------------------------------------------------------------------------*/
/* graphics data transfer structure [ used in function graphp(*), e.g.]: */
# define TYPE_GRAPHICS 1
typedef struct
{
   signed char
      rtn, /* return operation mark: 0: returm with error */
      dsp; /* display operation mark: 1 display some file saving messages */

   char
      name[STS_SIZE],
      text[STS_SIZE];

   char
      file[STS_SIZE],
      curve[GPH_CURVES][STS_SIZE],
      format[SHS_SIZE];

   char
      xunit[SHS_SIZE],
      yunit[SHS_SIZE];

   short
      ptsize,
      nc;

   long
      nn,
      np[GPH_CURVES];

   double
      xmin,
      xmax,
      ymin,
      ymax,
      vct[GPH_CURVES+ONE][GPH_POINTS];

} GRAPHICS;
/*----------------------------------------------------------------------------*/
