# define PRINTF(SS1,SS2,XX) \
{ \
   printf( "\n %s   % .12e", (SS2), ( double )(XX)); \
   printf( "\n please acknowledge (%s) [ enter any character ]:", (SS1)); \
   scanf( "%s", ptr ); \
}
