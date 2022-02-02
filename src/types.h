/* [ file: types.h ] */
/* [ Update: January 31, 2022 ] */
/*----------------------------------------------------------------------------*/
/* typedef header of program helios.c */
/*----------------------------------------------------------------------------*/
# define HLSTP_DSPLAY
/* The running pointer function dsplay(*) structure type */
/*----------------------------------------------------------------------------*/
typedef struct
{
   char 
      rtn;

   char 
      option,
      messge[LGS_SIZE];

   short
      rcsps; /* relative curser position */
   
   long
      fleps, /* file position pointer [ fleps = ftell(*) etc. ] */
      state,
      range;

   FILE 
     *display;

} DSPLAY;
/*----------------------------------------------------------------------------*/
# define HLSTP_GAUSS_JRD 1
/* The structure type definition of Gauss-Jordan algorithm functions          */
/* gssjrd(*) [line pivoting] and gssjpv(*) [full pivoting ]                   */
/*----------------------------------------------------------------------------*/
# ifndef GSS_DISP
   # define GSS_DISP  0     /* 1: display if coeff. mtx.gss.m[][] is singular */
# endif
# ifndef GSS_IMPLCT
   # define GSS_IMPLCT 1    /* 1: implicit pivoting  [ normalizing equations ]*/
# endif
# ifndef GSS_STRUCT
   # define GSS_STRUCT 1    /* 1: define struct gaussc gss [may be done elsw.]*/
# endif
# ifndef GSS_LONG_DBL
   # define GSS_LONG_DBL 0  /* 1: long double prexcision */
# endif
# ifndef GSS_MAXRNK
   # define GSS_MAXRNK (2*NODES) /* maximum rank of matrix M */
# endif
# ifndef GSS_SINGLR 
   # define GSS_SINGLR ( 1.e-301 ) /* bound for singularity check */
# endif
/*----------------------------------------------------------------------------*/
typedef struct
{
   signed char
      rtn;

   char 
      opt;

   short
      rank, /* rank of matrix M = ( mr[][]+j*mi[][] ) */
      neqs; /* number of linear equations to be solved [ simultaneously ] */
            /* - only used in function gssjpv(*) */

   # if GSS_LONG_DBL  == 1
      long double
   # else
      double
   # endif

      dtr,                         /* determinant  ( real  part )          */
      dti,                         /* "            ( imag. part )          */
      mr[GSS_MAXRNK][2*GSS_MAXRNK],/* r = real part of matrix              */
      mi[GSS_MAXRNK][2*GSS_MAXRNK],/* i = imag."    "  "                   */
      zr[GSS_MAXRNK][GSS_MAXRNK],
      zi[GSS_MAXRNK][GSS_MAXRNK];

} GAUSS_JRD;
/*----------------------------------------------------------------------------*/
/* the computation mode [ operations ] structure type: */
# define HLSTP_OPERATIONS 1
typedef struct
{
   signed char
      rtn;

   char
      opt[SHS_SIZE];

   char
      ntx[OPERATS+ONE][STS_SIZE];

   short
      n[OPERATS+ONE];

} OPERATIONS;
/*----------------------------------------------------------------------------*/
/* the materials structure type: */
# define HLSTP_MATERIALS 1
typedef struct
{
   signed char
      rtn;

   char
      avm[STS_SIZE], /* string "available_metals", e.g. */
      avd[STS_SIZE], /* string "available_dielectrics", e.g. */
      avs[STS_SIZE], /* string "available_solid_[mantle,_pipe]_materials" */
      avf[STS_SIZE], /* string "available_cooling_fluids", e.g. */
      mto[STS_SIZE], /* string "Outer_conductor_metal", e.g. */
      sho[STS_SIZE], /* string "Al", e.g. [short_hand for selected metal] */ 
      mti[STS_SIZE], /* string "Inner_conductor_metal", e.g. */
      shi[STS_SIZE], /* string "Cu", e.g. [short_hand for selected metal] */ 
      del[STS_SIZE], /* string "Dielectric", e.g. */
      shd[STS_SIZE], /* string "PE", e.g. [short_hand for selected dielect] */
      jck[STS_SIZE], /* string "Jacket_/_tube_mantle", e.g. */
      shj[STS_SIZE], /* string "PE", e.g. [short_hand for mantle material] */
      cpe[STS_SIZE], /* string "Inner_cooling_pipe", e.g. */
      shc[STS_SIZE], /* string "PE", e.g. [short_hand for pipe material] */
      fld[STS_SIZE], /* string "Cooling_fluid", e.g. */
      shf[STS_SIZE]; /* string "H2O", e.g. [short_hand for cooling fluid */

   char
     *mtx[CMETALS+ONE], /* metal [chemical name] strings */
     *msx[CMETALS+ONE], /* metal short hands */
     *dtx[DIELCTS+ONE], /* dielectric [chemical/technical name] strings */
     *dsx[DIELCTS+ONE], /* dielectric material short hands */
     *stx[SLDMTRS+ONE], /* solid material [chemical/technical name] strings */
     *ssx[SLDMTRS+ONE], /* solid material short hands */
     *ftx[COOLFLS+ONE], /* cooling fluids [chemical/technical name] strings */
     *fsx[COOLFLS+ONE]; /* cooling fluids short hands */

   short
      m, d, s, f;

} MATERIALS;
/*----------------------------------------------------------------------------*/
/* the parameters structure type: */
# define HLSTP_PARAMETERS 1
typedef struct
{
   signed char
      rtn;

   char
     *ntx[PARMTRS+ONE],
     *stx[PARMTRS+ONE];

   double
      s[PARMTRS+ONE];

   double
      tfix[NODES];     /* fixed environment or cooling fluid temperatures */

} PARAMETERS;
/*----------------------------------------------------------------------------*/
/* the metals [parameter] structure type */
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
/* the gases and cooling fluids structure type: */
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
/* the geometric parameters type: */
# define HLSTP_DIMENSIONS 1
typedef struct
{
   signed char
      rtn;

   double
      djck, /* outer [jacket;mantel] diameter of coaxial line [m] */
      dout, /* outer conductor diameter, coaxial line [m] */
      dinn, /* inner conductor diameter, coaxial line [m] */
      dtub, /* inner [ cooling tube ] diameter of coaxial line [m] */
      wout, /* outer waveguide width */
      winn, /* inner waveguide width [ conductive side 'a'] */
      hout, /* outer waveguide height */
      hinn, /* inner waveguide height [ conductive side 'b'] */
      lwgd, /* length of waveguide [m] */
      ldel, /* length of cooled section, dielectric [m] */
      ltub, /* length of cooled section, pipe [m] */
      lfef, /* effective length, inner conductor cooling */
      ldef, /* effective length, dielectric cooling */
      ffo,  /* form factor, outer conductor */
      ffi;  /* form factor, inner conductor */

} DIMENSIONS;
/*----------------------------------------------------------------------------*/
/* the electric parameters type: */
# define HLSTP_ELECTRIC 1
typedef struct
{
   signed char
      onerr,
      rtn;

   short
      frc, /* actual frequency point [ counter ] */
      frn; /* number of frequency points */

   double
      limp, /* char line impedance [Ohm] */
      freq, /* operation frequency [Hz] */
      frq0, /* 1st frequency [Hz] */
      frq1, /* 2nd frequency [Hz] */
      cwpr, /* transmitted CW power [W] */
      reso, /* resistance [per length] outer conductor [Ohm/m] */
      resi, /* resistance [per length] inner conductor [Ohm/m] */
      cono, /* conductance [per length] outer conductor [S*m] */
      coni, /* conductance [per length] inner conductor [S*m] */
      lsso, /* losses, outer conductor [W/m] */
      lssi, /* losses, inner conductor [W/m] */
      lssd, /* dielectric losses [W/m] */
      lsdo, /* dielectric loss current into outer conductor [W/m] */
      lsdi, /* dielectric loss current into inner conductor [W/m] */
      lsst; /* total losses [W/m] */

   double
      epd, /* relative permittivity, dielectric */
      myd, /* relative permeability, dielectric */
      myo, /* relative permeability, outer conductor */
      myi, /* relative permeability, inner conductor */
      tge, /* electric loss tangent, dielectic */
      tgm, /* magnetic loss tangent, dielectric */
      rso, /* resistivity, outer conductor [Ohm*m] */
      dro, /* temperature coeff of the latter [1/KELVIN] */
      cdo, /* conductivity, outer conductor [S/m] */
      rsi, /* resistivity, inner conductor [Ohm*m]*/
      dri, /* temperature coeff of the latter [1/KELVIN] */
      cdi; /* conductivity, inner conductor [S/m]*/
      
   double
      lssfp[FREQPTS], /* electric loss density factor, pipe [1/m] */
      lssfm[FREQPTS]; /* electric loss density factor, mantle [1/m] */

} ELECTRIC;
/*----------------------------------------------------------------------------*/
/* the thermal parameters structure type: */
# define HLSTP_THERMDYN 1
typedef struct
{
   signed char
      onerr,
      rtn;

   signed char
      mbe, /* switches from 0 to 1 if mantle temperature difference passes */
           /* below a critical value [<=DTMNGL], which turns the algorithm */
           /* instable */
      ppe; /* same role for pipe temperature difference */

   double
      mdo, /* mass density, outer conductor [Kh/m^3] */
      mdi, /* mass density, inner conductor [Kg/m^3] */
      mdd, /* mass density, dielectric [Kg/m^3] */
      mdj, /* mass density, jacket [Kg/m^3] */
      mdf, /* mass density, cooling fluid [Kg/m^3] */
      mde, /* mass density, gas environment */
      hco, /* heat conductivity, outer conductor [W/(m*K)] */
      dho, /* temperature coeff of the latter [1/K] */
      cvo, /* heat capacity, outer conductor [J/(kg*K)] */
      emo, /* relative heat radiation emissivity, outer conductor */
      hci, /* heat conductivity, inner conductor [K/(m*K)] */
      dhi, /* temperature coeff of the latter [1/K] */
      cvi, /* heat capacity, inner conductor [J/(kg*K)] */
      emi, /* relative heat radiation emissivity, inner conductor */
      hcd, /* heat conductivity, dielectric [W/(m*K)] */
      dhd, /* temperature coeff of the latter [1/K] */
      cvd, /* heat capacity, dielectric [J/(kg*K)] */
      nyd, /* cinematic viscosity, gas or liquid dielectric [ m^2/s ] */
      dnd, /* temperature coefficient of the latter [ 1/K ] */
      prd, /* pressure, dielectric [Bar] */
      vld, /* fluid velocity, dielectric [ m/s ] */
      red, /* Reynolds number, dielectric [dimensionless] */
      pnd, /* Prandtl number, dielectric [dimensionless] */
      dpd, /* temperature coefficient of the latter [ 1/K ] */
      nud, /* Nusselt number, dielectric [dimensionless] */
      hcj, /* heat conductivity, mantle ('jacket') [W/(m*K)] */
      dhj, /* temperature coeff of the latter [1/K] */
      cvj, /* heat capacity, mantle [J/(kg*K)] */
      hcm, /* intermediately stored heat conductivity, mantle [W/(m*K)] */
      dhm, /* intermediately stored temperature coeff of the latter [1/K] */
      hct, /* heat conductivity, pipe [W/(m*K)] */
      dht, /* temperature coeff of the latter [1/K] */
      cvt, /* heat capacity, pipe [J/(kg*K)] */
      hcp, /* intermediately stored heat conductivity, pipe [W/(m*K)] */
      dhp, /* intermediately stored temperature coeff of the latter [1/K] */
      eme, /* relative heat radiation emissivity, cable jacket */
      cve, /* heat capacity, environment [J/(kg*K)] */
      hce, /* heat conductivity, environment [W/(m*K)] */
      dhe, /* temperature coeff of the latter [1/K] */
      nye, /* cinematic viscosity, environment [ m^2/s ] */
      dne, /* temperature coefficient of the latter [ 1/K ] */
      pre, /* pressure, environment [Bar] */
      vle, /* air velocity, environment [ m/s ] */
      ree, /* Reynolds number, environmant [dimensionless] */
      pne, /* Prandtl number, environment [dimensionless] */
      dpe, /* temperature coefficient of the latter [ 1/K ] */
      nue, /* Nusselt number, environment [dimensionless] */
      hcf, /* heat conductivity, cooling fluid [W/(m*K)] */
      dhf, /* temperature coeff of the latter [1/K] */
      cvf, /* heat capacity, cooling fluid [J/(kg*K)] */
      nyf, /* cinematic viscosity, cooling fluid [ m^2/s ] */
      dnf, /* temperature coefficient of the latter [ 1/K ] */
      prf, /* pressure, coolant */
      vlf, /* fluid velocity, coolant [ m/s ] */
      ref, /* Reynolds number, cooling fluid [dimensionless] */
      pnf, /* Prandtl number, cooling fluid [dimensionless] */
      dpf, /* temperature coefficient of the latter [ 1/K ] */
      nuf; /* Nusselt number, cooling fluid [dimensionless] */

   double   /* [ all temperatures in KELVIN ] */
      grdo, /* temperature gradient in dielectric at outer conductor */
      grdi; /* temperature gradient in dielectric at inner conductor */

   double   /* [ all temperatures in KELVIN ] */
      tref, /* reference temperature */
      tfld, /* any inner conductor cooling fluid [mean] temperature */
      ttub, /* inner cooling tube temperature */
      tinn, /* temperature, inner conductor */
      tdel, /* [ effective maximum ] temperature, dielectric */
      tout, /* temperature, outer conductor */
      tjck, /* outer [jacket] temperature */
      tflr, /* temperature rise, cooling fluid */
      tdrs, /* temperature rise, dielectric */
      tenv, /* environment temperature */
      tnrm, /* normalized temperature maximum in waveguide [cf. trnorm(*)] */
      tsdm, /* fixed mantle temperature on one side */
      tsdp, /* fixed inner conductor temperature on one side */
      trfm, /* transient side temperature, mantle */
      trfp, /* transient side temperature, pipe */
      topm, /* opposite side temperature, mantle */
      topp, /* opposite side temperature, pipe */
      tmxm, /* maximum mantle temperature for fixed side conditions */
      tmxp, /* max. inn. cond. [pipe] temperature for fixed side conditions */
      tgrnd, /* the ground temperature defined in and returned from kelvin(*) */
      tgmin, /* the minimum base temperature returned from kelvin(*) */
      tgmax, /* the maximum base temperature returned from kelvin(*) */
      tmean, /* the mean base temperature returned from kelvin(*) */
      tnorm, /* temperature deviation norm */
      
      tfix[NODES], /* any fixed temperature [cooling fluids, gases, e.g.] */
      temp[(2*NODES)], /* transfer temperatures */
      temp_init[(2*NODES)]; /* initial transfer temperatures */

/* side heat current variables */
   double
      sdhcm, /* mantle heat conductance at side temperature tsdm [ W*m/K ] */
      sdhcp, /* pipe heat conductance at side temperature tsdp [W*m/K] */
      sdedm, /* equiv uniform heat current density [W/m] */
      sdedp, /* equiv uniform heat current density [W/m] */
      sdmcm, /* efficient heat conductance to side [temp], mantle [W/(m*K)]*/
      sdmcp, /* efficient heat conductance to side [temp], pipe [W/(m*K)] */
      sdlsm, /* outer conductor loss density at temperature tsdm [W/m] */
      sdlsp, /* inner conductor loss density at temperature tsdp [W/m] */
      sdifm, /* [imposed] heat inflow on opposite side, mantle [W] */
      sdofm, /* [imposed or computed] side heat outflow, mantle [W] */
      sdifp, /* [imposed] heat inflow on opposite side, mantle [W] */
      sdofp; /* [imposed or computed] side heat outflow, pipe [W] */

} THERMDYN;
/*----------------------------------------------------------------------------*/
/* the heat currents and temperatures structure type: */
# define HLSTP_HCURRS 1
typedef struct
{
   signed char
      onerr,
      rtn;

   double
      hcrr[NODES];

} HCURRS;
/*----------------------------------------------------------------------------*/
/* the heat conductance matrix structure type: */
# define HLSTP_HCNDCT 1
typedef struct  
{
   signed char
      onerr,
      rtn;

   short
      rank,
      mrank,
      nodes,
      ngrnd,
      sdnde,
      ndm, /* node connected to mantle side node */
      ndp, /* node connected to pipe side node */
      nde[NODES]; /* special nodes [ to be evaluated, e.g.] */

   double
      scl;

   double
      mhr[(2*NODES)][(2*NODES)],
      mhc[(2*NODES)][(2*NODES)],
      chc[(2*NODES)][(2*NODES)],
      rhc[(2*NODES)][(2*NODES)];

} HCNDCT;
/*--------------------------------------------------------------------------*/
/* graphics data transfer structure [ used in function graphp(*), e.g.]: */
# define HLSTP_GRAPHICS 1
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
/* the evaluation mode structure type: */
# define HLSTP_RESULTS 1
typedef struct
{
   signed char
      rtn;

   short
      job, /* running job index */
      fstjob;/* initial job index */

   double
      freq[FREQPTS], /* operation frequency [Hz] */
      cwpr[FREQPTS], /* transmitted CW power [W] */
      lsso[FREQPTS], /* losses, outer conductor [W/m] */
      lssi[FREQPTS], /* losses, inner conductor [W/m] */
      lssd[FREQPTS], /* dielectric losses [W/m] */
      lsdo[FREQPTS], /* dielectric loss current into outer cond [W/m] */
      lsdi[FREQPTS], /* dielectric loss current into inner cond [W/m] */
      lsst[FREQPTS]; /* total losses [W/m] */

   double
      cao[FREQPTS], /* electric conductivity, outer conductor [S/m] */
      rao[FREQPTS], /* resistivity, outer conductor [Ohm*m] */
      rfo[FREQPTS], /* surface resistance, outer conductor [Ohm] */
      rro[FREQPTS], /* resistance per length, outer conductor [Ohm/m] */
      cai[FREQPTS], /* electric conductivity, inner conductor [S/m]*/
      rai[FREQPTS], /* resistivity, inner conductor [Ohm*m]*/
      rfi[FREQPTS], /* surface resistance, inner conductor [Ohm] */
      rri[FREQPTS]; /* resistance per length, inner conductor [Ohm/m] */

   double /* actual parameters [ as computed for any temperature, e.g.] */
      hco, /* heat conductivity, outer conductor [W/(m*K)] */
      eme, /* relative heat radiation emissivity, cable jacket */
      emo, /* relative heat radiation emissivity, outer conductor */
      hci, /* heat conductivity, inner conductor [K/(m*K)] */
      emi, /* relative heat radiation emissivity, inner conductor */
      hcd, /* heat conductivity, dielectric [W/(m*K)] */
      hcj, /* heat conductivity, outer mantle [W/(m*K)] */
      hct, /* heat conductivity, pipe [W/(m*K)] */
      hce, /* heat conductivity, environment [W/(m*K)] */
      hcf, /* heat conductivity, cooling fluid [W/(m*K)] */
      nyd, /* cinematic viscosity, gas or liquid dielectric [ m^2/s ] */
      prd, /* pressure, dielectric [Bar] */
      red, /* Reynolds number, dielectric [dimensionless] */
      nud, /* Nusselt number, dielectric [dimensionless] */
      nye, /* cinematic viscosity, environment [ m^2/s ] */
      pre, /* pressure, environment [Bar] */
      ree, /* Reynolds number, environmant [dimensionless] */
      nue, /* Nusselt number, environment [dimensionless] */
      nyf, /* cinematic viscosity, cooling fluid [ m^2/s ] */
      ref, /* Reynolds number, cooling fluid [dimensionless] */
      nuf; /* Nusselt number, cooling fluid [dimensionless] */

   double    /* [ all temperatures in KELVIN ] */
      tfld[FREQPTS], /* any inner conductor cooling fluid [mean] temperature */
      ttub[FREQPTS], /* inner cooling tube temperature */
      tinn[FREQPTS], /* temperature, inner conductor */
      tdel[FREQPTS], /* temperature, dielectric */
      tout[FREQPTS], /* temperature, outer conductor */
      tjck[FREQPTS], /* outer [jacket] temperature */
      trfm[FREQPTS], /* side temperature, mantle */
      trfp[FREQPTS], /* side temperature, pipe */
      topm[FREQPTS], /* opposite side temperature, mantle */
      topp[FREQPTS], /* opposite side temperature, pipe */
      tmxm[FREQPTS], /* maximum mantle temperature */
      tmxp[FREQPTS], /* average pipe temperature */
      tflr[FREQPTS], /* temperature rise, cooling fluid */
      tdrs[FREQPTS], /* temperature rise, dielectric */
      lfef[FREQPTS], /* effective length, inner conductor cooling */
      ldef[FREQPTS], /* effective length, dielectric cooling */
      grdo[FREQPTS], /* temperature gradient in dielectric at outer conductor */
      grdi[FREQPTS], /* temperature gradient in dielectric at inner conductor */

      sdofp[FREQPTS], /* heat outflow, pipe side [W] */
      sdifp[FREQPTS], /* heat inflow, opposite side [W] */
      sdedp[FREQPTS], /* eq. heat flow density, pipe [W/m] */
      sdhcp[FREQPTS], /* pipe heat conductance at temperature tsdp [W*m/K] */
      sdofm[FREQPTS], /* heat outflow, mantle side [W] */
      sdifm[FREQPTS], /* heat inflow, opposite side [W] */
      sdedm[FREQPTS], /* eq. heat flow density mantle [W/m] */
      sdhcm[FREQPTS]; /* mantle heat conductance at temperature tsdm [W*m/K] */
      
   OPERATIONS
     *opp;

   MATERIALS
     *map;

   PARAMETERS
     *prp;

   DIMENSIONS
     *dmp;

   ELECTRIC 
     *elp;

   THERMDYN 
     *thp;

   HCURRS
     *hcp;

   HCNDCT
     *cdp;
     
   GRAPHICS
     *grp;

} RESULTS;
/*----------------------------------------------------------------------------*/
/* The structure type header of text console function txcnsl(*) */

# ifndef TP_TXCNSL
    # define TP_TXCNSL 1
# endif
/*----------------------------------------------------------------------------*/
# define CNS_ITEMS 10 /* maximum number of menu items                         */
# define CNS_LNLEN 79 /* number of characters in menu line                    */
# define CNS_LNINT  1 /* number of characters in menu line                    */
# define CNS_POSIT 67 /* position of menu items [labels] in line              */
/*----------------------------------------------------------------------------*/
typedef struct
{
   signed char 
      clscr,
      rtn; 

   int
      items, dfopt, dflnf,
      lnlen, posit, lnint,
      lglen, dblen, stlen,
      option;

   char
      title[LGS_SIZE],
      rqfrm[SHS_SIZE],
      tasks[CNS_LNLEN],
      flags[CNS_LNLEN],
      cmmnt[CNS_LNLEN],
      envmt[CNS_LNLEN],
      escpe[CNS_LNLEN],
      cnfrm[CNS_LNLEN],
      instr[CNS_LNLEN],
      dfstr[CNS_LNLEN],
      rqdbl[CNS_LNLEN],
      rqlng[CNS_LNLEN],
      rqstr[CNS_LNLEN],
      mflag[CNS_ITEMS],
      mflin[CNS_ITEMS],
      mline[CNS_ITEMS][CNS_LNLEN];

   long
      dflng, inlng;

   double
      dfdbl, indbl;
   
} TXCNSL;
/*----------------------------------------------------------------------------*/
/* driver function elsydrv(*) state structure */
/* [ reflects actually charged topology, parameter, boundary conditions, ..., */
/*   file names, file labels etc.]: */
# define HLSTP_HELIOSSTATE 1
typedef struct
{
   signed char
      cpmrk,
      onerr,
      rtn;

/* if 'skp[N] = S'or's': skip message <N> next time [ debugging purposes ] */
   char
      ctl, /* control parameter, set to null during initialization */
      hls_skp[VSS_SIZE],
      wrk_skp[VSS_SIZE],
      ini_skp[VSS_SIZE],
      hcd_skp[VSS_SIZE],
      klv_skp[VSS_SIZE];

   unsigned char
      uif, /* user interface [ 'g'raphical, or 't'ext console, e.g. ] */
      act; /* actual program stage */

   char
      name[STS_SIZE],
      text[STS_SIZE],
      logfle[STS_SIZE],
      errfle[STS_SIZE];

   char
      cyl[NODES],
      opmrk[OPERATS+ONE]; /* opmrk[0]: algorithm stage counter */
                         /* opmrk[N]: special operation mark for any option N */
   short
      job,   /* running job index */
      fstjob;/* initial job index */

   long
      fleps; /* file position pointer */

   double
      upd, /* update coefficient [ transferred to any function ] */
      uexp, /* initial update coefficient */
      scale;

   FILE 
     *display;

   OPERATIONS
     *opp;

   MATERIALS
     *map;

   PARAMETERS
     *prp;

   DIMENSIONS
     *dmp;

   METALS
     *mtp;

   DIELCT
     *dlp;

   SOLIDS
     *slp;

   FLUIDS
     *flp;

   ELECTRIC
     *elp;

   THERMDYN
     *thp;

   HCURRS
     *hcp;

   HCNDCT
     *cdp;

   GRAPHICS
     *grp;

   RESULTS
     *rsp;

   TXCNSL
     *csp;

} HELIOSSTATE;
/*************************** end of file types.h ******************************/
