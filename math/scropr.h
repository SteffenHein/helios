# if defined ( _GNU_Linux )
   # define CLS fprintf( stdout, "\033[2J" )
   # define NRM fprintf( stdout, "\033[0" )
   # define BCKW fprintf( stdout, "\033[44" )
# elif defined ( _Linux )
   # define CLS fprintf( stdout, "\033[2J" )
   # define NRM fprintf( stdout, "\033[0" )
   # define BCKW fprintf( stdout, "\033[44" )
# elif defined ( _SuSE )
   # define CLS fprintf( stdout, "\033[2J" )
   # define NRM fprintf( stdout, "\033[0" )
   # define BCKW fprintf( stdout, "\033[44" )
# elif defined ( _Unix )
   # define CLS fprintf( stdout, "\033[2J" )
   # define NRM fprintf( stdout, "\033[0" )
   # define BCKW fprintf( stdout, "\033[44" )
# elif defined ( _Ansi )
   # define CLS fprintf( stdout, "\033[2J" )
   # define NRM fprintf( stdout, "\033[0" )
   # define BCKW fprintf( stdout, "\033[44" )
# endif 
