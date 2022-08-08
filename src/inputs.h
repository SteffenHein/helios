# define DO_INPUT "input(*)"
/*******************************************************************************
*                                                                              *
*   Parameter input function; HELIOS release v1.0r1                            *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   In  option 'o'peration this function enters the operation parameters.      *
*   parameters on calling function deflt_operts( ), or from file opr.input     *
*   [ formatted as part 'OPERATIONS' of files opr.log_N or par.log_N ].        *
*   These parameters may be revised in function rvise_operts(*), on head of    *
*   helios driver function heliosdrv.c.                                        *
*                                                                              *
*   In option `m'aterials the present function enters the material parame-     *
*   ters on calling function deflt_matter( ), or from a file mat.input         *
*   [ formatted as part 'MATERIALS' of file mat.log_N ].                       *
*   These parameters may be revised in function rvise_matter( ).               *
*                                                                              *
*   In option `p'arameters the present function enters the continuous          *
*   model parameters on calling function deflt_params( ), or from a file       *
*   `par.input' [ formatted as part 'PARAMETERS' of file par.log_N ].          *
*   Again, these parameters may be revised in function rvise_params( ).        *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: February 08, 2022 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/
/* display tool: "more -e", e.g. */
# ifndef HLS_PAGER
   # define HLS_PAGER "more" 
# endif
/* editor command: "vi", "vim", "vile", "ed", "joe", "emacs", e.g. */
# ifndef HLS_EDITOR
   # define HLS_EDITOR "vile"
# endif
/* printer: "lp", e.g. */
# ifndef HLS_PRINTER
   # define HLS_PRINTER "lp"
# endif
/*----------------------------------------------------------------------------*/
/* maximum number of [connected] strings to be read from file */
# ifndef IPT_MAXLBL
   # define IPT_MAXLBL 500
# endif
/* [don't] display default operations: [0] 1 */
# ifndef IPT_OPRDEF
   # define IPT_OPRDEF 0
# endif
/* [don't] display default metals: [0] 1 */
# ifndef IPT_MATDEF
   # define IPT_MATDEF 0
# endif
/* [don't] display default parameters: [0] 1 */
# ifndef IPT_PARDEF
   # define IPT_PARDEF 0
# endif
/*----------------------------------------------------------------------------*/
/* name of operations log file: */
# ifndef IPT_OPRLOG
   # define IPT_OPRLOG "opr.log"
# endif
/* name of metals log file: */
# ifndef IPT_MATLOG
   # define IPT_MATLOG "mat.log"
# endif
/* name of parameter log file: */
# ifndef IPT_PARLOG
   # define IPT_PARLOG "par.log"
# endif
/*----------------------------------------------------------------------------*/
# define IPT_SCRFLS 1 /* 0: use insecure temporary file names [ tmpnam(*) ] */
                      /* 1: use secure temporary file names [ mkstemp(*) ] */
# if defined ( _Ansi )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1
# elif defined ( _Unix )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1
# elif defined ( _BSD )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1
# elif defined ( _Linux )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1
# elif defined ( _GNU_Linux )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1
# elif defined ( _SuSE )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1
# elif defined ( _Wine )
   # undef IPT_SCRFLS
   # define IPT_SCRFLS 1 
# endif
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* 'my_terminal' configuration: */

   # include <termcap.h>     /* terminal type header */
   static char *term;        /* terminal type string */ 

# ifndef CLSCREEN
   # define CLSCREEN { \
     fprintf( stdout, "%s", tgetstr( "cl", null )); \
   }
# endif
# ifndef PRBLDCLR
   # define PRBLDCLR(a) {\
     fprintf( stdout, \
        "%s%s", tgetstr( "md", null ), (a)); /* bold clear output */ \
   }
# endif
# ifndef PRINVERS
   # define PRINVERS(a) {\
     fprintf( stdout, \
        "%s%s", tgetstr( "mr", null ), (a)); /* inverse */ \
   }
# endif
# ifndef PRNORMAL
   # define PRNORMAL(a) {\
     fprintf( stdout, \
        "%s%s", tgetstr( "me", null ), (a)); /* back to normal output */ \
   }
# endif
# else /* if USE_NCURSES != 1 */
# ifndef CLSCREEN
   # define CLSCREEN { \
     fprintf( stdout, "\f" ); \
   }
# endif
# ifndef PRBLDCLR
   # define PRBLDCLR(a) {\
     fprintf( stdout, "%s", (a));\
   }
# endif
# ifndef PRINVERS
   # define PRINVERS(a) {\
     fprintf( stdout, "%s", (a));\
   }
# endif
# ifndef PRNORMAL
   # define PRNORMAL(a) {\
     fprintf( stdout, "%s", (a));\
   }
# endif
# endif /* USE_NCURSES != 1 */
/*----------------------------------------------------------------------------*/
# if IPT_SCRFLS == 1
   # include <unistd.h>
# endif
/*----------------------------------------------------------------------------*/
# include "../tools/cpypar.h"
/*----------------------------------------------------------------------------*/
# ifndef OPR_STRNGS
   # include "OPRSTRNGS.M"
# endif
/*----------------------------------------------------------------------------*/
# include "STOREOPR.M"
# include "STOREMAT.M"
# include "STOREPAR.M"
/*----------------------------------------------------------------------------*/
# include "../default/deflt_opr.h"
# include "../default/deflt_mat.h"
# include "../default/deflt_par.h"
/*----------------------------------------------------------------------------*/
# include "init_opr.h"
# include "init_mat.h"
# include "init_par.h"
# include "rvise_opr.h"
# include "rvise_mat.h"
# include "rvise_par.h"
# include "rread_opr.h"
# include "rread_mat.h"
# include "rread_par.h"
# include "store_opr.h"
# include "store_mat.h"
# include "store_par.h"
/*============================================================================*/

short input ( char *option )
{
/* allusions: */
/*
   extern char
      tmpfle[STS_SIZE] = {null}; 
*/
/* declarations: */

   static HELIOSSTATE
     *state = &helios;

   static TXCNSL 
     *csp = &cns;

   static short
      ii = null,
      ind = null,
      metals = CMETALS,
      dielectrics = DIELCTS,
      solids = SLDMTRS,
      fluids = COOLFLS,
      parameters = PARMTRS;

   static char 
      ptr[2*STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null},
      timeptr[STS_SIZE] = {null};

   const char
     *oprlog = IPT_OPRLOG,
     *matlog = IPT_MATLOG,
     *parlog = IPT_PARLOG,
     *timefrm = " created: %.24s  ";

   time_t
      nseconds = null,
     *timer = null;

# ifndef HLS_PAGER
   static WVGDPAR *wve = NULL;
# endif

/* prototypes: */

   time_t
      time( time_t *timer );

   char
      *ctime( const time_t *timer );

# if IPT_SCRFLS == 1
   int mkstemp( char *s );
# else
   char *tmpnam( char *s );
# endif

   double 
      cos( double x ), 
      sin( double x ),
     sqrt( double x );

   void
      init_operts( void ),
      init_matter( void ),
      init_params( void );

   void
      deflt_operts( void ),
      deflt_matter( void ),
      deflt_params( void );

   short 
      rvise_operts( void ),
      rvise_matter( void ),
      rvise_params( void );

   short 
      rread_operts( char *filename, char mode ),
      rread_matter( char *filename, char mode ),
      rread_params( char *filename, char mode );

   short 
      store_operts( char *filename, char mode ),
      store_matter( char *filename, char mode ),
      store_params( char *filename, char mode );

   TXCNSL 
      *txcnsl( TXCNSL *csp );

   char
      *lotos( long mm, char cc );
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* get the terminal info: */

   term = ( char *) getenv( "TERM" ); /* get the terminal */

   ind = tgetent( null, term );

   if( ONE != ind )
   {
      fprintf( stderr, "Error on getting the termcap info\n" ); 
      exit( EXIT_FAILURE );
   };
# endif
/*............................................................................*/
/* set buffer length = null: */

   ind = setvbuf( stdin, null, _IONBF, null );
   ind = setvbuf( stdout, null, _IONBF, null ); 
/*...........................................................................*/
/* memory allocations; initializations [explicit]: */

   ii = null; do
   {
      ptr[ii] = null;
      fleptr[ii] = null;
      timeptr[ii] = null;
      ++ii;
   } while( ii < STS_SIZE );
   do
   {
      ptr[ii] = null;
      ++ii;
   } while( ii < ( 2*STS_SIZE ));
/*...........................................................................*/
/* option "operations": */

   if ( *option == 'o' )
   {
/*............................................................................*/
      ind = rvise_operts( );      /* revise/reconfigure default operations    */
/*..............................*/

      if ( ind == null )
      {
         if ( OPERATS < opr.n[null] )
         {
            printf( "\n\n Message from function %s :", DO_INPUT );
            printf( "\n\n Too many operation parameters !!!" );
            printf( "\n [ Number %d exceeds maximum %d = macro OPERATS",
               opr.n[null], OPERATS );
            printf( "\n   in file 'model.h'. ]" );
            printf( "\n   - Change macro in compliance with memory "
               "resources.\n" );

            exit( EXIT_FAILURE );
         };
      };
/*............................................................................*/
/* store the actually charged operations: */
# if defined ( IPT_OPRTMP )
      strcpy( tmpfle, IPT_OPRTMP );
# else
/*............................................................................*/
/* assign a temporary file name: */
# if IPT_SCRFLS == 1
      mkstemp( tmpfle ); /* [ secure ] */
# else
      tmpnam( tmpfle ); /* [ insecure ] */
# endif                                                        
/*............................................................................*/
# endif
/*............................................................................*/
      store_operts( tmpfle, 't' );          /* store divs on temporary file   */
/*.......................................,*/
# if IPT_OPRDEF == 1
      strcpy( ptr, HLS_PAGER );
      strcat( ptr, " " );
      strcat( ptr, tmpfle );

/*....................................*/
      system( ptr );                  /* display operation parameters         */
      rread_operts( tmpfle, 't' );   /*                                       */
      rvise_operts( );              /*                                        */
      store_operts( tmpfle, 't' ); /*   restore revised operats on tmp file   */
/*...............................*/
# endif
/*............................................................................*/
      csp = txcnsl( null ); /* clear text console */
      csp->clscr = 1; /* if N != 0: clear screen; scroll N lines */
      csp->dfopt = 3;

     opr_menu:

      csp->items = 7;
      csp->dflnf = csp->dfopt; /* set line feed before option menu line */

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      strcpy( csp->title, "Program HELIOS: " );
      strncat( csp->title, timeptr, 24 );

      strcpy( csp->envmt, "INPUT" );
      strcpy( csp->cmmnt, "Operation modes" );
      strcpy( csp->tasks, "OPERATIONS" );

      strcpy( csp->mline[1], "* Display presently defined operation modes" );
      strcpy( csp->mline[2], "* Enter new operation modes from file" );
      strcpy( csp->mline[3], "* Edit [ and evtly. modify ] operation modes" );
      strcpy( csp->mline[4], "* Reload default operation modes" );
      strcpy( csp->mline[5], "* Print operation modes" );
      strcpy( csp->mline[6], "* Start computation" );
      strcpy( csp->mline[7], "* Continue" );

      strcpy( csp->escpe, "End of program / escape" );

      if ( state->job == null )
         strcpy( csp->cnfrm,\
            "Nothing done! Do you really want to quit ?" );
      else
         strcpy( csp->cnfrm, 
            "Do you really want to quit ?" );

/*............................................................................*/
      csp = txcnsl( csp );   /* menu building function */
/*.........................*/

      switch( csp->option )
      {
        default: /* continue program [ go to material parameter input ] */
        break;

/*............................................................................*/
        case 1:  /* display the current configuration */

/*............................................................................*/
# if defined ( HLS_PAGER )
         strcpy( ptr, HLS_PAGER );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*.....................................*/
         system( ptr );                /* display operation parameters */
/*...................................*/
# else
         printf( "\n\n %s", state->name );
         printf( "\n %s", state->text );

         printf( "\n\n %-d  %s\n", opr.n[null], opr.ntx[null] );

         for ( ii=ONE; ii<=opr.n[null]; ii++ )
            printf( "\n %-s:   %d", opr.ntx[ii], opr.n[ii] );
# endif
/*............................................................................*/
         csp->clscr = 0; /* clear screen; scroll N lines */
         csp->dfopt = 3;
         goto opr_menu;
        break;

/*............................................................................*/
        case 2: /* enter another configuration from file */

         fprintf( stdout, "\n Please enter filename [ Continue/"
            "Escape: null ] >----> " );
         scanf( "%s", ptr );

         if( *ptr == '0' )
         { 
            remove( tmpfle );

            PRBLDCLR( "" );
            fprintf( stdout, "\r%*s", 79, "INPUT" );
            PRNORMAL( "" );
            fprintf( stdout, "\n ======================================="
                                "=======================================" );
            return -ONE;
         };

/*......................................*/
         rread_operts( ptr, 'f' );      /* enter the new configuration file   */
         rvise_operts( );              /*  revise/reconfigure ...             */
         store_operts( tmpfle, 't' ); /*   restore on tmp file                */
/*..................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 7;
         goto opr_menu;
        break;

/*............................................................................*/
        case 3:  /* edit the current "operations" file */

/*............................................................................*/
# if defined ( HLS_EDITOR )
         strcpy( ptr, HLS_EDITOR );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*.......................................*/
         system( ptr );                  /* edit operation parameters file    */
         rread_operts( tmpfle, 't' );   /*  re-read temporary operations file */
         rvise_operts( );              /*   revise/reconfigure ...            */
         store_operts( tmpfle, 't' ); /*    restore on tmp file               */
/*..................................*/
# else
         fprintf( stdout, 
	    "\n No editor defined in file \"CONFIG.H\" !" );
# endif
/*............................................................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 7;
         goto opr_menu;
        break;

/*............................................................................*/
        case 4:  /* reload the default operation parameters */

/*......................................*/
         deflt_operts( );               /* enter default divions              */
         rvise_operts( );              /*  revise/reconfigure ...             */
         store_operts( tmpfle, 't' ); /*   store on tmp file                  */
/*..................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 7;
         goto opr_menu;
        break;

/*............................................................................*/
        case 5:
		
/*............................................................................*/
# if defined ( HLS_PRINTER )
         strcpy( ptr, HLS_PRINTER );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*....................................*/
         system( ptr );               /* print operation parameters */
/*..................................*/
# else
         printf( "\n No printer is defined for this option !" );
# endif
/*............................................................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 7;
         goto opr_menu;
        break;

/*............................................................................*/
        case 6: /* start computation */
         return ONE;
	break;

/*............................................................................*/
        case 0:  /* end of program / escape */
	case 'y':
	case 'Y':

         remove( tmpfle );

         PRBLDCLR( "" );
         PRNORMAL( "" );

         return -ONE;
        break;
      }; /* end switch(*) */
/*............................................................................*/
      remove( tmpfle );

      strcpy( fleptr, oprlog );
      strcat( fleptr, lotos( state->job, null ));

      printf( "\n opened: Operation logfile %s ", fleptr );
      
/*............................................................................*/
      store_operts( fleptr, 'o' );  /* save final configuration on log file   */
/*................................*/

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      printf( "\r Operation logfile %s ", fleptr );
      printf( timefrm, timeptr );
      printf( CLEAR_LINE );

      return null;

   }; /* end if *option == 'o'[perations] */
/*............................................................................*/
/* option "materials": */

   if ( *option == 'm' )
   {
      metals = mat.m;
      dielectrics = mat.d;
      solids = mat.s;
      fluids = mat.f;

/*............................................................................*/
      ind = rvise_matter( );      /*                                          */
/*..............................*/

      if ( ind == null )
      {
         if ( CMETALS < metals )
         {
            printf( "\n\n Message from function %s :", DO_INPUT );
            printf( "\n\n Too many metals defined !!!" );
            printf( "\n [ Number %d exceeds maximum %d = macro CMETALS",
               metals, CMETALS );
            printf( "\n   in file 'CONFIG.H'. ]" );
            printf( "\n   - Change macro in compliance with memory "
               "resources.\n" );

            exit( EXIT_FAILURE );
         };

         if ( DIELCTS < dielectrics )
         {
            printf( "\n\n Message from function %s :", DO_INPUT );
            printf( "\n\n Too many dielectrics defined !!!" );
            printf( "\n [ Number %d exceeds maximum %d = macro DIELCTS",
               dielectrics, DIELCTS );
            printf( "\n   in file 'CONFIG.H'. ]" );
            printf( "\n   - Change macro in compliance with memory "
               "resources.\n" );

            exit( EXIT_FAILURE );
         };

         if ( SLDMTRS < solids )
         {
            printf( "\n\n Message from function %s :", DO_INPUT );
            printf( "\n\n Too many solid materials defined !!!" );
            printf( "\n [ Number %d exceeds maximum %d = macro SLDMTRS",
               solids, SLDMTRS );
            printf( "\n   in file 'CONFIG.H'. ]" );
            printf( "\n   - Change macro in compliance with memory "
               "resources.\n" );

            exit( EXIT_FAILURE );
         };

         if ( COOLFLS < fluids )
         {
            printf( "\n\n Message from function %s :", DO_INPUT );
            printf( "\n\n Too many cooling fluids defined !!!" );
            printf( "\n [ Number %d exceeds maximum %d = macro COOLFLS",
               fluids, COOLFLS );
            printf( "\n   in file 'CONFIG.H'. ]" );
            printf( "\n   - Change macro in compliance with memory "
               "resources.\n" );

            exit( EXIT_FAILURE );
         };
      };
/*............................................................................*/
/* store the actually charged metals: */

# if defined ( IPT_MATTMP )
      strcpy( tmpfle, IPT_MATTMP );
# else
/*............................................................................*/
/* assign a temporary file name: */

# if IPT_SCRFLS == 1
      mkstemp( tmpfle ); /* [ secure ] */
# else
      tmpnam( tmpfle ); /* [ insecure ] */
# endif                                                        
/*............................................................................*/
# endif

/*............................................................................*/
      store_matter( tmpfle, 't' );          /* store prmtrs on temporary file */
/*.......................................,*/

# if IPT_PARDEF == 1
      strcpy( ptr, HLS_PAGER );
      strcat( ptr, " " );
      strcat( ptr, tmpfle );

/*....................................*/
      system( ptr );                  /* display material parameters          */
      rread_matter( tmpfle, 't' );   /*  re-read materials from tmp file      */
      rvise_matter( );              /*   revise/reconfigure ...               */
      store_matter( tmpfle, 't' ); /*    restore on temporary file            */
/*...............................*/
# endif
/*............................................................................*/
      csp = txcnsl( null ); 
      csp->clscr = 1; /* if N != 0: clear screen; scroll N lines */
      csp->dfopt = 3;

     mat_menu:

      csp->items = 8;
      csp->dflnf = csp->dfopt; /* set line feed before option menu line */

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      strcpy( csp->title, "Program HELIOS: " );
      strncat( csp->title, timeptr, 24 );

      strcpy( csp->envmt, "INPUT" );
      strcpy( csp->cmmnt, "Conductor metals and dielectric" );
      strcpy( csp->tasks, "MATERIALS" );
      
      strcpy( csp->mline[1], "* Display the presently charged materials" );
      strcpy( csp->mline[2], "* Enter new materials from file" );
      strcpy( csp->mline[3], "* Edit [ and evtly. modify ] the materials" );
      strcpy( csp->mline[4], "* Reload the default materials" );
      strcpy( csp->mline[5], "* Print the materials" );
      strcpy( csp->mline[6], "* Start computation" );
      strcpy( csp->mline[7], "* Return to previous menu" );
      strcpy( csp->mline[8], "* Continue" );

      strcpy( csp->escpe, "End of program / escape" );

      if ( state->job == null )
         strcpy( csp->cnfrm,\
            "Nothing done! Do you really want to quit ?" );
      else
         strcpy( csp->cnfrm, 
            "Do you really want to quit ?" );

/*............................................................................*/
      csp = txcnsl( csp );   /* menu building function */
/*.........................*/

      switch( csp->option )
      {
        default: /* continue program [ go to parameter input ] */
         break;

/*............................................................................*/
        case 1:  /* display the currently charged materials */

/*............................................................................*/
# if defined ( HLS_PAGER )
         strcpy( ptr, HLS_PAGER );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*.....................................*/
         system( ptr );                /* display material parameters */
/*...................................*/
# else
         metals = mat.m;
         dielectrics = mat.d;
         solids = mat.s;
         fluids = mat.f;

         printf( "\n\n %s", ( state->name ));
         printf( "\n %s", ( state->text ));

         printf( "\n\n %03d %s %s\n",
	    ( short ) mat.m, mat.mtx[null], mat.msx[null] );

         printf( "\n\n %03d %s %s\n",
	    ( short ) mat.d, mat.dtx[null], mat.dsx[null] );

         for ( ii=ONE; ii<=metals; ii++ )
            printf( "\n %s %s", mat.mtx[ii], mat.msx[ii] );

         for ( ii=ONE; ii<=dielectrics; ii++ )
            printf( "\n %s %s", mat.dtx[ii], mat.dsx[ii] );

         for ( ii=ONE; ii<=solids; ii++ )
            printf( "\n %s %s", mat.stx[ii], mat.ssx[ii] );

         for ( ii=ONE; ii<=fluids; ii++ )
            printf( "\n %s %s", mat.ftx[ii], mat.fsx[ii] );
# endif
/*............................................................................*/
         csp->clscr = 0; /* clear screen; scroll N lines */
         csp->dfopt = 8;
         goto mat_menu;
        break;

/*............................................................................*/
        case 2: /* enter another configuration from file */

         printf( "\n Please enter filename [ Continue/"
            "Escape: null ] >----> " );
         scanf( "%s", ptr );

         if( *ptr == '0' )
         { 
            remove( tmpfle );

            PRBLDCLR( "" );
            fprintf( stdout, "\r%*s", 79, "INPUT" );
            PRNORMAL( "" );
            fprintf( stdout,
	       "\n ======================================="
                  "=======================================" );
            return -ONE; 
         };
/*............................................................................*/
         rread_matter( ptr, 'f' );       /* read materials from file          */
         rvise_matter( );               /*  revise/reconfigure ...            */
         store_matter( tmpfle, 't' );  /*   restore on tmp file               */
/*...................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 8;
         goto mat_menu;
        break;

/*............................................................................*/
        case 3:  /* edit the current configuration */

# if defined ( HLS_EDITOR )
         strcpy( ptr, HLS_EDITOR );
         strcat( ptr, " " );
         strncat( ptr, tmpfle, STS_SIZE );

/*.......................................*/
         system( ptr );                  /* edit materials file               */
         rread_matter( tmpfle, 't' );   /*  re-read materials from tmp file   */
         rvise_matter( );              /*   revise/reconfigure ...            */
         store_matter( tmpfle, 't' ); /*    restore on tmp file               */
/*..................................*/
# else
         fprintf( stdout, 
	    "\n No editor defined in file \"CONFIG.H\" !" );
# endif
/*............................................................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 8;
         goto mat_menu;
        break;

/*............................................................................*/
        case 4:  /* reload the default configuration */

/*......................................*/
         deflt_matter( );               /* enter default materials            */
         rvise_matter( );              /*  revise/reconfigure materials       */
         store_matter( tmpfle, 't' ); /*   restore on tmp file                */
/*..................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 8;
         goto mat_menu;
        break;

/*............................................................................*/
        case 5:

/*............................................................................*/
# if defined ( HLS_PRINTER )
         strcpy( ptr, HLS_PRINTER );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*.....................................*/
         system( ptr );               /* print file */
/*...................................*/
# else
         fprintf( stdout,
	    "\n No printer defined in file \"CONFIG.H\"" );
# endif
/*............................................................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 8;
         goto mat_menu;
        break;

/*............................................................................*/
        case 6: /* start computation */
         return ONE;
	break;

/*............................................................................*/
        case 7: /* return to previous menu */
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 3;
         goto opr_menu;
	break;

/*............................................................................*/
        case 0:  /* end of program / escape */
	case 'y':
	case 'Y':

         remove( tmpfle );

         PRBLDCLR( "" );
         PRNORMAL( "" );
	 
         return -ONE;
         break;
      }; /* end switch(*) */
/*............................................................................*/
      remove( tmpfle );

      strcpy( fleptr, matlog  );
      strcat( fleptr, lotos( state->job, null ));

      printf( "\n opened: Materials logfile %s ", fleptr );

/*............................................................................*/
      store_matter( fleptr, 'm' );   /* save final materials on log file      */
/*.................................*/

      printf( CLEAR_LINE );

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      printf( "\r Materials logfile %s ", fleptr );
      printf( timefrm, timeptr );

      return null;

   }; /* end of *option == 'm'aterials */ 
/*............................................................................*/
/* option "parameters": */

   if ( *option == 'p' )
   {
      parameters = ( short ) par.s[null];

/*............................................................................*/
      ind = rvise_params( );      /*                                          */
/*..............................*/
      
      if ( ind == null )
      {
         if ( PARMTRS < parameters )
         {
            printf( "\n\n Message from function %s :", DO_INPUT );
            printf( "\n\n Too many parameters !!!" );
            printf( "\n [ Number %d exceeds maximum %d = macro PARMTRS",
               parameters, PARMTRS );
            printf( "\n   in file 'model.h'. ]" );
            printf( "\n   - Change macro in compliance with memory "
               "resources.\n" );

            exit( EXIT_FAILURE );
         };
      };
/*............................................................................*/
/* store the actually charged parameters: */
# if defined ( IPT_PARTMP )
      strcpy( tmpfle, IPT_PARTMP );
# else
/*............................................................................*/
/* assign a temporary file name: */
# if IPT_SCRFLS == 1
      mkstemp( tmpfle ); /* [ secure ] */
# else
      tmpnam( tmpfle ); /* [ insecure ] */
# endif                                                        
/*............................................................................*/
# endif
/*............................................................................*/

/*............................................................................*/
      store_params( tmpfle, 't' );          /* store prmtrs on temporary file */
/*.......................................,*/

# if IPT_PARDEF == 1
      strcpy( ptr, HLS_PAGER );
      strcat( ptr, " " );
      strcat( ptr, tmpfle );

/*....................................*/
      system( ptr );                  /* edit parameter file                  */
      rread_params( tmpfle, 't' );   /*  re-read parameters from tmp file     */
      rvise_params( );              /*   revise/reconfigure ...               */
      store_params( tmpfle, 't' ); /*    restore on temporary file            */
/*...............................*/
# endif
/*............................................................................*/
      csp = txcnsl( null ); 

      csp->clscr = 1; /* if N != 0: clear screen; scroll N lines */
      csp->dfopt = 3;

     par_menu:

      csp->items = 8;
      csp->dflnf = csp->dfopt; /* set line feed before option menu line */

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      strcpy( csp->title, "Program HELIOS: " );
      strncat( csp->title, timeptr, 24 );

      strcpy( csp->envmt, "INPUT" );
      strcpy( csp->cmmnt, "Geometric and physical" );
      strcpy( csp->tasks, "PARAMETERS" );

      strcpy( csp->mline[1], "* Display the presently charged parameters" );
      strcpy( csp->mline[2], "* Enter new parameters from file" );
      strcpy( csp->mline[3], "* Edit [ and evtly. modify ] the parameters" );
      strcpy( csp->mline[4], "* Reload the default parameters" );
      strcpy( csp->mline[5], "* Print the parameters" );
      strcpy( csp->mline[6], "* Start computation" );
      strcpy( csp->mline[7], "* Return to previous menu" );
      strcpy( csp->mline[8], "* Continue" );

      strcpy( csp->escpe, "End of program / escape" );

      if ( state->job == null )
         strcpy( csp->cnfrm,\
            "Nothing done! Do you really want to quit ?" );
      else
         strcpy( csp->cnfrm, 
            "Do you really want to quit ?" );

/*............................................................................*/
      csp = txcnsl( csp );   /* menu building function */
/*.........................*/

      switch( csp->option )
      {
/*............................................................................*/
        case 1:  /* display the configuration */

/*............................................................................*/
# if defined ( HLS_PAGER )
         strcpy( ptr, HLS_PAGER );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*.....................................*/
         system( ptr );                /* edit divisions file */
/*...................................*/

# else
         parameters = ( short ) par.s[null];

         printf( "\n\n %s", ( state->name ));
         printf( "\n %s", ( state->text ));

         printf( "\n\n %-3d %s\n", ( short ) par.s[null], par.stx[null] );

         for ( ii=ONE; ii<=parameters; ii++ )
            printf( "\n %-s:  % .12e ", par.stx[ii], par.s[ii] );
# endif
/*............................................................................*/
         csp->clscr = 0; /* clear screen; scroll N lines */
         csp->dfopt = 3;
         goto par_menu;
         break;

/*............................................................................*/
        case 2: /* load parameters from file */

         printf( "\n Please enter filename [ Continue/"
            "Escape: null ] >----> " );
         scanf( "%s", ptr );

         if( *ptr == '0' )
         { 
            remove( tmpfle );

            PRBLDCLR( "" );
            fprintf( stdout, "\r%*s", 79, "INPUT" );
            PRNORMAL( "" );
            fprintf( stdout,
	       "\n ======================================="
                  "=======================================" );
            return -ONE; 
         };
/*............................................................................*/
         rread_params( ptr, 'f' );       /* read new parameters from file     */
         rvise_params( );               /*  revise/reconfigure ...            */
         store_params( tmpfle, 't' );  /*   restore parameters on tmp file    */
/*...................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 6;
         goto par_menu;
         break;

/*............................................................................*/
        case 3:  /* edit and/or modify the actual configuration */

/*............................................................................*/
# if defined ( HLS_EDITOR )
         strcpy( ptr, HLS_EDITOR );
         strcat( ptr, " " );
         strncat( ptr, tmpfle, STS_SIZE );

/*.......................................*/
         system( ptr );                  /* edit parameters file               */
         rread_params( tmpfle, 't' );   /*  re-read parameters from tmp file  */
         rvise_params( );              /*   revise/reconfigure ...            */
         store_params( tmpfle, 't' ); /*    restore on temporary file         */
/*..................................*/
# else
         printf( "\n No editor is defined for this option !" );
# endif
/*............................................................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 6;
         goto par_menu;
         break;

/*............................................................................*/
        case 4:  /* reload the default configuration */

/*......................................*/
         deflt_params( );               /* enter default parameters           */
         rvise_params( );              /*  revise/reconfigure parameters      */
         store_params( tmpfle, 't' ); /*   store on temporary file            */
/*..................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 6;
         goto par_menu;
        break;

/*............................................................................*/
        case 5:

/*............................................................................*/
# if defined ( HLS_PRINTER )
         strcpy( ptr, HLS_PRINTER );
         strcat( ptr, " " );
         strcat( ptr, tmpfle );

/*....................................*/
         system( ptr );               /* print file */
/*..................................*/
# else
         fprintf( stdout,
	    "\n No printer defined in file \"CONFIG.H\" !" );
# endif
/*............................................................................*/
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 6;
         goto par_menu;
        break;

/*............................................................................*/
        case 6:
         return ONE;
	break;

/*............................................................................*/
        case 7:
         csp->clscr = 1; /* clear screen; scroll N lines */
         csp->dfopt = 3;
         goto mat_menu;
	break;

/*............................................................................*/
        default: /* end or continue program */
        case 0: 
        case 'y': 
        case 'Y': 

         remove( tmpfle ); 

         PRBLDCLR( "" );
         PRNORMAL( "" );

         return -ONE;
        break;
      }; /* end switch(*) */
/*............................................................................*/
      remove( tmpfle );

      strcpy( fleptr, parlog  );
      strcat( fleptr, lotos( state->job, null ));

      printf( "\n opened: Parameters logfile %s ", fleptr );
/*............................................................................*/
      store_params( fleptr, 'p' );  /* save final parameters on log file      */
/*................................*/

      printf( CLEAR_LINE );

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      printf( "\r Parameters logfile %s ", fleptr );
      printf( timefrm, timeptr );

      return null;

   };/* end of *option == 'p'arameters */ 

   return null;
} 
# undef IPT_OPRDEF
# undef IPT_PARDEF
/***************** end of parameter input function input(*) *******************/
