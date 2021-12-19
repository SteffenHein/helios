/* [ file: hlsdrv.h ] */
/*******************************************************************************
*                                                                              *
*   Body of HELIOS driver function heliosdrv(*); release v1.0r1                *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function controls the parameter input and some corrections,           *
*   the initialization function initlze(*) and the final computational         *
*   routines of hlswrk(*).                                                     *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/
short heliosdrv( int argn, char **args )
{
/* allusions: */

/* extern HELIOSSTATE helios; *//* [heliosdrv] menu state transfer structure */

/* declarations: */

   static FILE
     *logfle = NULL;

# if USE_NCURSES == 1
   static FILE
     *errfle;
# endif

   static HELIOSSTATE
     *state = &helios;

   static TXCNSL
     *csp; /* cf. txcnsl(*) [ text console ] */

   static char 
      item,
      fleptr[STS_SIZE] = {null};

   static char 
      ptr[STS_SIZE] = {null},           
    **endp = null;

   static const char 
     *logptr = "hls.log",
     *errptr = "hls.err",
     *dline = "========================================"\
              "========================================";

   static short 
      ii = null,
      job = null;

   static long 
      kk = null;
/*............................................................................*/
/* time_t types: */

   time_t nseconds = null;
   time_t   *timer = null;
   time_t time( time_t *timer );

   static char
      timeptr[STS_SIZE] = {null};

/* prototyping: */

# ifndef _CCBUG
   char
      *strcpy( char *ptr1, const char *ptr2 ),  
      *strcat( char *ptr1, const char *ptr2 ),
      *strncat( char *ptr1, const char *ptr2, size_t n );
# endif

   char
      *lotos( long mm, char cc );

   char
      *dotos( double x, char precision, char *format );

   TXCNSL 
     *txcnsl( TXCNSL *csp );

   HELIOSSTATE
     *initlze( HELIOSSTATE *state );

   HELIOSSTATE
     *hlswrk( HELIOSSTATE *state );

   HELIOSSTATE
     *heliostest( HELIOSSTATE *state );

   short
      input ( char *option );

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
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* get the terminal info: */

   term = ( char *) getenv( "TERM" ); /* get the terminal */

   kk = tgetent( null, term );

   if( ONE != kk )
   {
      errfle = fopen( errptr, "a+" );

      fprintf( errfle, \
         "\nError message from job %s, function %s :", \
         lotos (( state->job ), null ), __func__ ); \
      fprintf( errfle, "\nCan't get the termcap info\n" );

      fclose ( errfle );

      fprintf( stderr, \
         "\n Error message from job %s, function %s :", \
         lotos (( state->job ), null ), __func__ ); \
      fprintf( stderr, "\n Can't get the termcap info\n" );

      exit( EXIT_FAILURE );
   };
# endif
/*............................................................................*/
/* set buffer length = null: */

   kk = setvbuf( stdin, null, _IONBF, null );
   kk = setvbuf( stdout, null, _IONBF, null ); 
/*............................................................................*/
/* initialize: */

   kk = null; do
   {
      tmpfle[kk] = null;
      fleptr[kk] = null;
   } while(( ++kk ) < STS_SIZE );
/*...........................................................................*/
   ( state->opp ) = &opr;
   ( state->map ) = &mat;
   ( state->prp ) = &par;
   ( state->mtp ) = &met;
   ( state->dlp ) = &del;
   ( state->slp ) = &sld;
   ( state->flp ) = &fld;
   ( state->dmp ) = &dms;
   ( state->elp ) = &elt;
   ( state->thp ) = &trm;
   ( state->hcp ) = &hcr;
   ( state->cdp ) = &cdc;
   ( state->rsp ) = &res;

   ii = null; do
   {
      ( state->logfle[ii] ) = null;
      ( state->errfle[ii] ) = null;
   } while(( ++ii ) < STS_SIZE );

   ( state->fleps ) = null;
   
   strcpy(( state->name ), "HELIOS" );
   strcpy(( state->text ), "waveguide_temperatures_vs_admissible_power" );
   strcpy(( state->logfle ), logptr );
   strcpy(( state->errfle ), errptr );
/*............................................................................*/
/* read command line: */

   job = null;
   if ( argn )
   {
      do
      {
         strcpy( ptr, *( args++ ));

         if ( null == strncmp( ptr, "--file", 6 )) /* paramater file */
            ( state->uif ) = 'f';                  /* input mode */

         if ( null == strncmp( ptr, "--init-job", 9 ))
         {
            if ( --argn )
            {
               strcpy ( ptr, *( args++ ));
               job = strtol( ptr, endp, DEC );
            };
         };

         if ( null == strncmp( ptr, "--init-fle", 9 ))
         {                                         /* parameter file */
            ( state->uif ) = 'f';                  /* input mode */
            if ( --argn )
            {
               strcpy ( fleptr, *( args++ ));
            };
         };
      } while ( --argn );
   };
/*............................................................................*/
   ( state->fstjob ) = job; /* the 1st job index */
   ( state->job ) = job;    /* the running job index */

   strcpy( tmpfle, "/tmp/temp.XXXXX" );
/*............................................................................*/
   init_operts( );
   init_matter( );
   init_params( );
/*............................................................................*/
/* text mode: */

   if ((( state->uif ) != 'g' )
     &&(( state->uif ) != 'b' )
     &&(( state->uif ) != 'f' ))
   {
      ( state->uif ) = 't'; /* user interface: 't'ext console */

      csp = txcnsl( null ); /* initialize the text console */

      ( csp->option ) = 3; do
      {
         strcpy(( csp->cmmnt ), "Welcome to HELIOS !" );
         strcpy(( csp->cnfrm ), "Nothing done! Do you really want to quit ?" );

         ( csp->dfopt ) = 2; /* the initial default menu option */
         ( csp->clscr ) = -ONE;

         ( csp->items ) = 3;
         ( csp->dflnf ) = 0; /* 1: set line feed before default option line */

         strcpy(( csp->envmt ), "HELIOS" );
         strcpy(( csp->tasks ), "Select ...:" );

         strcpy(( csp->mline[1] ), "* parameter file input " );
         strcpy(( csp->mline[2] ), "* text console input " );
         strcpy(( csp->mline[3] ), "* support" );

         strcpy(( csp->escpe ), "End of program / escape" );
/*............................................................................*/
         csp = txcnsl( csp );   /* build the [ start ] menu                   */
/*............................*/
         if (( csp->option ) == 0 )
            return null;
         else if (( csp->option ) == 1 )
            ( state->uif ) = 'f'; /* 'f'ile input mode */
         else if (( csp->option ) == 3 )
         {
            PRBLDCLR( "" );
	    fprintf( stdout, "\n Call: +49+8061.936362 or" );
            fprintf( stdout, "\n email: contact@sfenx.de" );
	    fprintf( stdout, "\n [ Don't hesitate to ask your questions.]" );
            PRNORMAL( "\n");
            ( csp->dfopt ) = 2; /* the initial default menu option */
         }
         else /* csp->option == 2, e.g. */
            ( state->uif ) = 't';
      } while(( csp->option ) == 3 );
   };

   if (( state->uif ) == 't' ) /* text console input */
   {
      strcpy(( csp->cmmnt ), "Welcome back to HELIOS !" );
      strcpy(( csp->cnfrm ), "Nothing done! Do you really want to quit ?" );

      ( csp->dfopt ) = 4; /* the initial default menu option */
      ( csp->clscr ) = 1;

     text_menu:

      ( csp->items ) = 6;
      ( csp->dflnf ) = 6; /* set special line feed before */
                          /* that option line */

      nseconds = time( timer );

      strcpy( timeptr, ctime( &nseconds ));
      strcpy(( csp->title ), "Program HELIOS: " );
      strncat(( csp->title ), timeptr, 24 );

      strcpy(( csp->envmt ), "HELIOS" );
      strcpy(( csp->tasks ), "Define ...:" );

      strcpy(( csp->mline[1] ), "* computation modes" );
      strcpy(( csp->mline[2] ), "* conductors, dielectric materials" );
      strcpy(( csp->mline[3] ), "* geometric and physical parameters" );
      strcpy(( csp->mline[4] ), "* the complete configuration" );
      strcpy(( csp->mline[5] ), "* start computation" );
      strcpy(( csp->mline[6] ), "* support" );

      strcpy(( csp->escpe ), "End of program / escape" );
/*............................................................................*/
      csp = txcnsl( csp );      /* build the [ start ] menu                   */
/*............................*/
      item = ( csp->option );
      ( state->act ) = null; /* the actual prog stage [ null = par input ] */

      switch( item )
      {
        default:
         break;

        case 0:
         goto terminal;
         break;

        case 1:
/*............................................................................*/
         input( "operations" );     /*                                        */
/*................................*/
         ( csp->dfopt ) = 2; /* the next default menu option */
         ( csp->clscr ) = 1; /* N != 0: clear screen; scroll N lines */
         break;

        case 2:
/*............................................................................*/
         input( "materials" );     /*                                         */
/*...............................*/
         ( csp->dfopt ) = 3; /* the next default menu option */
         ( csp->clscr ) = 1; /* N != 0: clear screen; scroll N lines */
         break;

        case 3:
/*............................................................................*/
         input( "parameters" );     /*                                        */
/*................................*/
         ( csp->dfopt ) = 5; /* the next default menu option */
         ( csp->clscr ) = 1; /* N != 0: clear screen; scroll N lines */
         break;

        case 4:
/*............................................................................*/
         input( "operations" );
         input( "materials" );
         input( "parameters" );
/*...........................................................................*/
         ( csp->dfopt ) = 5; /* the next default menu option */
         ( csp->clscr ) = 1; /* N != 0: clear screen; scroll N lines */
         break;

        case 5:
         break;

        case 6:
         PRBLDCLR( "" );
	 fprintf( stdout, "\n Call: +49+8061.936362 or" );
         fprintf( stdout, "\n email: contact@sfenx.de" );
         fprintf( stdout, "\n [ Don't hesitate to ask your questions.]" );
         PRNORMAL( "\n");
         ( csp->dfopt ) = 4; /* the initial default menu option */
         break;
      };

      if (( null < item )
        &&( item != FIVE ))
      {
         strcpy(( csp->cmmnt ), "Welcome back to HELIOS !" );
         goto text_menu;
      }
      else if ( item == FIVE )
      {
         rvise_params( );
/*............................................................................*/
/* clear screen: */
         printf( CLEAR_LINE );
  
         printf( "\n ==================================="
            "===========================================" );
         nseconds = time( timer );
         strcpy( timeptr, ctime( &nseconds ));
         printf( "\n ");
         PRBLDCLR( "HELIOS started:");
         printf( "\n %s", timeptr );
         PRNORMAL( "" );
      }; /* end if item == 5 */
   } /* end if ( state->uif ) != 'f','g','b' */
   else /* if (( state->uif ) != 't' ) *//* parameter 'f'ile input */
   {                           /* [graphical mode, e.g.] */
/*............................................................................*/
/* enter computation modes: */

      if ( null == fleptr[null] )
      {
         strcpy( fleptr, IPT_PARINIT );
         strcat( fleptr, lotos(( state->job ), null ));
      };

      rvise_operts( ); /* revise/reconfigure ...*/
      rread_operts( fleptr, 'f' );
      rvise_operts( ); /* revise/reconfigure ...*/

      strcpy( tmpfle, "opr.log" );
      strcat( tmpfle, lotos(( state->job ), null ));

      store_operts( tmpfle, 'o' ); /* restore revised file as log file */
/*............................................................................*/
/* enter material parameters: */

      rvise_matter( ); /* revise/reconfigure ...*/
      rread_matter( fleptr, 'f' );
      rvise_matter( ); /* revise/reconfigure ...*/

      strcpy( tmpfle, "mat.log" );
      strcat( tmpfle, lotos(( state->job ), null ));

      store_matter( tmpfle, 'm' ); /* restore revised file as log file */
/*............................................................................*/
/* enter physical parameters: */

      rvise_params( );             /* revise/reconfigure */
      rread_params( fleptr, 'f' ); /* enter physical parameters */
   };
/*............................................................................*/
/* open process log file: */

   logfle = fopen( logptr , "a+" );
   setvbuf( logfle, null, _IONBF, null );

   nseconds = time( timer );
   strcpy( timeptr, ( ctime( &nseconds ) + 11 ));

   fprintf( logfle, "\nJob no " );
   fprintf( logfle, "%s ", ( lotos(( state->job ), null )));
   fprintf( logfle, "launched at " );
   fprintf( logfle, "%-.8s", timeptr );

   ( state->fleps ) = ftell( logfle );

   fclose( logfle );
/*............................................................................*/
/* create final parameter log: */

   ( state->act ) = ONE; /* the actual program stage [ ONE = computation ] */

   rvise_params( ); /* the final parameter revision */
   strcpy( fleptr, IPT_PARLOG );
   strcat( fleptr, lotos(( state->job ), null ));
   store_params( fleptr, 'p' ); /* restore revised parameter file as log file */
   
/* initialize: */
   state->ctl = null; /* <- deactivates homogeneity check initially */
/*............................................................................*/
   state = initlze( state );     /*                                           */
/*.............................*/
/*
   fprintf( stdout, "\n%s", "initialization ready" );
   exit (null) ;
*/
/* ... and start the iterations: */

/*............................................................................*/
   state = hlswrk( state );     /*                                            */
/*............................*/
/* ... it's terminated [ continue ? ] */

   if (( state->uif ) == 't' ) /* text input */
   {
      ( state->job )++; /* next job label */

      strcpy(( csp->cmmnt ), "Welcome back to HELIOS !" );
      ( csp->dfopt ) = 0; /* the next default menu option */
      ( csp->clscr ) = 0; /* N != 0: clear screen; scroll N lines */

      goto text_menu;
   };
   
   logfle = fopen( logptr, "a+" );

   nseconds = time( timer );
   strcpy( timeptr, ( ctime( &nseconds ) + 4 ));

   fprintf( logfle, "\nJob no %d terminated ", ( state->job ));
   fprintf( logfle, "%-.20s", timeptr );
   fprintf( logfle, "\n%s", dline );

   ( state->fleps ) = ftell( logfle );
   fclose( logfle );

  terminal:;
/*
   heliostest( state ); 
*/
   return null;
}  
/*============================================================================*/
