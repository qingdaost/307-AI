#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define abs(x)  (((x) < 0) ? -(x) : (x))

/* feature type 1 */
double *neigh_mingap_feature(double **, int);           
/* feature type 2 */
double *neigh_meangap_feature(double **, int);
/* feature type 3 */
double *neigh_mean_feature(double **, int);
/* feature type 4 (= 1 & 2) */
double *neigh_mingap_meangap_feature(double **, int);
/* feature type 5 (= 2 & 3) */
double *neigh_mean_meangap_feature(double **, int);
/* feature type 6 (= 3 & 1) */
double *neigh_mean_mingap_feature(double **, int);
/* feature type 7 */
double *row_column_aver_feature(double **, int);

