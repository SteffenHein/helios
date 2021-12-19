# ifndef PLOT_FORMAT
   # define PLOT_FORMAT "gnuplot_2D" /*plot file format: "SPLINE" or "gnuplot"*/
# endif
# ifndef GPH_USCALE
   # define GPH_USCALE 0     /* 1: take uniform scales on xy axes             */
# endif
# ifndef GPH_CURVES
   # define GPH_CURVES 20   /* maximum number of graphics */
# endif
# ifndef GPH_POINTS
   # define GPH_POINTS 500  /* maximum number of abscissae ( x-values ) */
# endif
# ifndef GPH_DATFLE
   # define GPH_DATFLE "dat"
# endif
# ifndef GPH_MXRNGE
   # define GPH_MXRNGE (9.e+00) /* The max. range of abs.values in gnuplot is */
# endif                         /* interval [1.e-GPH_MXRNGE, 1.e+GPH_MXRNGE]. */
                                /* Beyond this range all values are rescaled. */
