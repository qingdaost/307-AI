/*
 * feature.c 
 * pixel_based (low level) feature extraction functions 
 *
 *   author  : Mengjie Zhang
 *             mengjie@cs.rmit.edu.au
 *   date    : 13/07/98
 *
 *   related files:
 *             patt-feat-ascii.c
 *             patt-feat-bin.c
 *             nnsweep-feature.c
 *
 */

#include "feature.h"

double *neigh_mingap_feature(double **obj, int size)
{
  int i, j, m, xsize, ysize;
  double *x,     /* cross feature, (0, 90, 180, 270 degree) */
        *y,     /* corner or diagonal feature (45, 135, 225, 315 degree) */
        *z,     /* edge feature */
        cent,  /* central feature */
        *vector; /* a whole feature vector */
 
  double cent_up_left,  /* the up-left central pixel value */
        cent_up_right, /* the up-right central pixel value */
        cent_down_right, /* the down-right central pixel value */
        cent_down_left;  /* the down-left central pixel value */
 
  xsize = ysize = size;
  x = (double *)calloc(size/2, sizeof(double)); /* x[1],x[2], ..., x[size/2-1] */
  if ((double *)x == NULL)
  {
      printf("not enough memory to allocate x[]\n");
      exit(1);
  }
  y = (double *)calloc(size/2, sizeof(double));  /* y[1],y[2],..., y[size/2-1] */
  if ((double *)y == NULL)
  {
      printf("not enough memory to allocate y[]\n");
      exit(1);
  }
 
  z = (double *)calloc(size/2 - 1, sizeof(double)); /* z[1], ..., z[size/2-2] */
  if ((double *)z == NULL)
  {
      printf("not enough memory to allocate z[]\n");
      exit(1);
  }
 
   /* calculating cent, which represents the minimum difference or
      the maximum similarity between every two of the four central pixels */
 
  cent = abs(obj[xsize/2 - 1][ysize/2 - 1]    /* up-left central pixel */
           - obj[xsize/2][ysize/2 - 1]);      /* up-right central pixel */
  if (cent > abs(obj[xsize/2][ysize/2 - 1]
           - obj[xsize/2][ysize/2]))          /* down-right central pixel */
     cent = abs(obj[xsize/2][ysize/2 - 1] - obj[xsize/2][ysize/2]);
  if (cent > abs(obj[xsize/2][ysize/2]
           - obj[xsize/2 -1][ysize/2]))       /* down-left central pixel */
     cent = abs(obj[xsize/2][ysize/2] - obj[xsize/2 -1][ysize/2]);
  if (cent > abs(obj[xsize/2 -1][ysize/2] - obj[xsize/2 - 1][ysize/2 - 1]))
     cent = abs(obj[xsize/2 -1][ysize/2] - obj[xsize/2 - 1][ysize/2 - 1]);
 
 
  m = size / 2 - 1; /* number of element in x[] vector */
 
  cent_up_left = obj[xsize/2 - 1][ysize/2 - 1];
  cent_up_right = obj[xsize/2][ysize/2 - 1];
  cent_down_right = obj[xsize/2][ysize/2];
  cent_down_left = obj[xsize/2 -1][ysize/2];
 
  for (i = 1; i <= m; i++)
  {
      /* calculating x[] vector */
     x[i] = abs(obj[xsize/2 - 1 - i][ysize/2 - 1]  /* left side x pixel */
              - cent_up_left);
     if (x[i] > abs(obj[xsize/2 - 1 - i][ysize/2]  /* left side x pixel */
              - cent_down_left))
        x[i] = abs(obj[xsize/2 - 1 - i][ysize/2] - cent_down_left);
 
     if (x[i] > abs(obj[xsize/2 + i][ysize/2 - 1]  /*right side x pixel */
              - cent_up_right))
        x[i] = abs(obj[xsize/2 + i][ysize/2 - 1] - cent_up_right);
     if (x[i] > abs(obj[xsize/2 + i][ysize/2]      /* right side x pixel */
              - cent_down_right))
        x[i] = abs(obj[xsize/2 + i][ysize/2] - cent_down_right);
 
     if (x[i] > abs(obj[xsize/2 -1][ysize/2 - 1 - i] /* up side x pixel */
              - cent_up_left))
        x[i] = abs(obj[xsize/2 -1][ysize/2 - 1 - i] - cent_up_left);
     if (x[i] > abs(obj[xsize/2][ysize/2 - 1 - i]    /* up side x pixel */
              - cent_up_right))
        x[i] = abs(obj[xsize/2][ysize/2 - 1 - i] - cent_up_right);
 
     if (x[i] > abs(obj[xsize/2 -1][ysize/2 + i]     /* down side x pixel */
              - cent_down_left))
        x[i] = abs(obj[xsize/2 -1][ysize/2 + i] - cent_down_left);
     if (x[i] > abs(obj[xsize/2][ysize/2 + i]      /* down side x pixel */
              - cent_down_right))
        x[i] = abs(obj[xsize/2][ysize/2 + i] - cent_down_right);
 
     /* calculating y[] vector  */
     y[i] = abs(obj[xsize/2 + i][ysize/2 - 1 - i]
              - cent_up_right);
 
     if (y[i] < abs(obj[xsize/2 - 1 - i][ysize/2 - 1 - i] /* top-left corner */
              - cent_up_left))
        y[i] = abs(obj[xsize/2 - 1 - i][ysize/2 - 1 - i]- cent_up_left);
 
     if (y[i] < abs(obj[xsize/2 - 1 - i][ysize/2 + i]  /* down-left corner */
              - cent_down_left))
        y[i] = abs(obj[xsize/2 - 1 - i][ysize/2 + i] - cent_down_left);
 
     if (y[i] < abs(obj[xsize/2 + i][ysize/2 + i]    /* down-right corner */
              - cent_down_right))
        y[i] = abs(obj[xsize/2 + i][ysize/2 + i] - cent_down_right);
 
     /* calaulating z[] vector */
 
    if (i >= 2)
    {
      for (j = 1; j <= i - 1; j++)
      {
         /* down-right (two sides) */
         z[i-1] = abs(obj[xsize/2 + j][ysize/2 + i] - cent_down_right);
         if (z[i-1] > abs(obj[xsize/2 + i][ysize/2 + j] - cent_down_right))
           z[i-1] = abs(obj[xsize/2 + i][ysize/2 + j] - cent_down_right);
 
         /* up-right (two sides) */
         if (z[i-1] > abs(obj[xsize/2 + j][ysize/2 - 1 - i] - cent_up_right))
            z[i-1] = abs(obj[xsize/2 + j][ysize/2 - 1 - i] - cent_up_right);
         if (z[i-1] > abs(obj[xsize/2 + i][ysize/2 - 1 - j] - cent_up_right))
            z[i-1] = abs(obj[xsize/2 + i][ysize/2 - 1 - j] - cent_up_right);
 
         /* up-left (two sides) */
         if (z[i-1] > abs(obj[xsize/2 - 1 - j][ysize/2 - 1 - i] - cent_up_left))
            z[i-1] = abs(obj[xsize/2 - 1 - j][ysize/2 - 1 - i] - cent_up_left);
         if (z[i-1] > abs(obj[xsize/2 - 1 - i][ysize/2 - 1 - j] - cent_up_left))
            z[i-1] = abs(obj[xsize/2 - 1 - i][ysize/2 - 1 - j] - cent_up_left);
 
         /* down-left (two sides) */
         if (z[i-1] > abs(obj[xsize/2 - 1 - j][ysize/2 + i] - cent_down_left))
            z[i-1] = abs(obj[xsize/2 - 1 - j][ysize/2 + i] - cent_down_left);
         if (z[i-1] > abs(obj[xsize/2 - 1 - i][ysize/2 + j] - cent_down_left))
            z[i-1] = abs(obj[xsize/2 - 1 - i][ysize/2 + j] - cent_down_left);
      } /* for j */
    } /* if i > 2 */
  } /* for i */
/*
  printf("forming feature vector...\n");
*/ 
  /* allocate feature vector */
  vector = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vector == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vector[]\n");
     exit(1);
  }
 
  vector[0] = cent;
 
  for (i = 1; i <= size/2 - 1; i++)
     vector[i] = x[i];
/*
  printf("running here!...\n");
*/
  for (i = 1; i <= size/2 - 1; i++)
  {
     j = size/2 - 1 + i;
     vector[j] = y[i];
  }
/*
  printf("running here!...\n");
*/ 
  for (i = 1; i <= size/2 - 2; i++)
  {
     j = 2 * (size/2 - 1) + i;
     vector[j] = z[i];
  }
/*
   printf("the feature vector is: \n");
 
  for (i = 0; i < 3 * (size/2 - 1); i++)
  {
     printf("%5.2f ", vector[i]);
  }
  printf("\n\nfinish calculating feature vector[]\n");
*/
  return vector; 
}
 

double *neigh_meangap_feature(double **obj, int size)
{
  int i, j, m, xsize, ysize;
  double *x,     /* cross feature, (0, 90, 180, 270 degree) */
        *y,     /* corner or diagonal feature (45, 135, 225, 315 degree) */
        *z,     /* edge feature */
        cent,  /* central feature */
        *vector; /* a whole feature vector */
 
  double cent_up_left,  /* the up-left central pixel value */
        cent_up_right, /* the up-right central pixel value */
        cent_down_right, /* the down-right central pixel value */
        cent_down_left;  /* the down-left central pixel value */
 
  xsize = ysize = size;
  x = (double *)calloc(size/2, sizeof(double)); /* x[1],x[2], ..., x[size/2-1] */
  if ((double *)x == NULL)
  {
      printf("not enough memory to allocate x[]\n");
      exit(1);
  }
  y = (double *)calloc(size/2, sizeof(double));  /* y[1],y[2],..., y[size/2-1] */
  if ((double *)y == NULL)
  {
      printf("not enough memory to allocate y[]\n");
      exit(1);
  }
 
  z = (double *)calloc(size/2 - 1, sizeof(double)); /* z[1], ..., z[size/2-2] */
  if ((double *)z == NULL)
  {
      printf("not enough memory to allocate z[]\n");
      exit(1);
  }
 
   /* calculating cent, which represents the minimum difference or
      the maximum similarity between every two of the four central pixels */
 
  cent = abs(obj[xsize/2 - 1][ysize/2 - 1]    /* up-left central pixel */
           - obj[xsize/2][ysize/2 - 1])       /* up-right central pixel */
       + abs(obj[xsize/2][ysize/2 - 1]
           - obj[xsize/2][ysize/2])           /* down-right central pixel */
       + abs(obj[xsize/2][ysize/2]
           - obj[xsize/2 -1][ysize/2])        /* down-left central pixel */
       + abs(obj[xsize/2 -1][ysize/2] 
           - obj[xsize/2 - 1][ysize/2 - 1]);
  cent /= 4.0; 
 
  m = size / 2 - 1; /* number of element in x[] vector */
 
  cent_up_left = obj[xsize/2 - 1][ysize/2 - 1];
  cent_up_right = obj[xsize/2][ysize/2 - 1];
  cent_down_right = obj[xsize/2][ysize/2];
  cent_down_left = obj[xsize/2 -1][ysize/2];
 
  for (i = 1; i <= m; i++)
  {
      /* calculating x[] vector */
     x[i] = abs(obj[xsize/2 - 1 - i][ysize/2 - 1]  /* left side x pixel */
              - cent_up_left);
          + abs(obj[xsize/2 - 1 - i][ysize/2]  /* left side x pixel */
              - cent_down_left)
          + abs(obj[xsize/2 + i][ysize/2 - 1]  /*right side x pixel */
              - cent_up_right)
          + abs(obj[xsize/2 + i][ysize/2]      /* right side x pixel */
              - cent_down_right)
          + abs(obj[xsize/2 -1][ysize/2 - 1 - i] /* up side x pixel */
              - cent_up_left)
          + abs(obj[xsize/2][ysize/2 - 1 - i]    /* up side x pixel */
              - cent_up_right)
          + abs(obj[xsize/2 -1][ysize/2 + i]     /* down side x pixel */
              - cent_down_left)
          + abs(obj[xsize/2][ysize/2 + i]      /* down side x pixel */
              - cent_down_right);
     x[i] /= 8.0;
 
     /* calculating y[] vector  */
     y[i] = abs(obj[xsize/2 + i][ysize/2 - 1 - i]        /* top-right corner */
              - cent_up_right)                      
          + abs(obj[xsize/2 - 1 - i][ysize/2 - 1 - i] /* top-left corner */
              - cent_up_left)
          + abs(obj[xsize/2 - 1 - i][ysize/2 + i]  /* down-left corner */
              - cent_down_left)
          + abs(obj[xsize/2 + i][ysize/2 + i]    /* down-right corner */
              - cent_down_right);
     y[i] /= 4.0;
 
     /* calaulating z[] vector */
 
    if (i >= 2)
    {
      z[i-1] = 0;
      for (j = 1; j <= i - 1; j++)
      {
         /* down-right (two sides) */
         z[i-1] += abs(obj[xsize/2 + j][ysize/2 + i] - cent_down_right);
         z[i-1] += abs(obj[xsize/2 + i][ysize/2 + j] - cent_down_right);
 
         /* up-right (two sides) */
         z[i-1] += abs(obj[xsize/2 + j][ysize/2 - 1 - i] - cent_up_right);
         z[i-1] += abs(obj[xsize/2 + i][ysize/2 - 1 - j] - cent_up_right);
 
         /* up-left (two sides) */
         z[i-1] += abs(obj[xsize/2 - 1 - j][ysize/2 - 1 - i] - cent_up_left);
         z[i-1] += abs(obj[xsize/2 - 1 - i][ysize/2 - 1 - j] - cent_up_left);
 
         /* down-left (two sides) */
         z[i-1] += abs(obj[xsize/2 - 1 - j][ysize/2 + i] - cent_down_left);
         z[i-1] += abs(obj[xsize/2 - 1 - i][ysize/2 + j] - cent_down_left);
      } /* for j */
      z[i-1] /= (double)(8 * (i - 1));
    } /* if i > 2 */
  } /* for i */
/*
  printf("forming feature vector...\n");
*/ 
  /* allocate feature vector */
  vector = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vector == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vector[]\n");
     exit(1);
  }
 
  vector[0] = cent;
 
  for (i = 1; i <= size/2 - 1; i++)
     vector[i] = x[i];
/*
  printf("running here!...\n");
*/
  for (i = 1; i <= size/2 - 1; i++)
  {
     j = size/2 - 1 + i;
     vector[j] = y[i];
  }
/*
  printf("running here!...\n");
*/ 
  for (i = 1; i <= size/2 - 2; i++)
  {
     j = 2 * (size/2 - 1) + i;
     vector[j] = z[i];
  }
/*
   printf("the feature vector is: \n");
 
  for (i = 0; i < 3 * (size/2 - 1); i++)
  {
     printf("%5.2f ", vector[i]);
  }
  printf("\n\nfinish calculating feature vector[]\n");
*/
  return vector; 
}

double *neigh_mean_feature(double **obj, int size)
{
  int i, j, m, xsize, ysize;
  double *x,     /* cross feature, (0, 90, 180, 270 degree) */
        *y,     /* corner or diagonal feature (45, 135, 225, 315 degree) */
        *z,     /* edge feature */
        cent,  /* central feature */
        *vector; /* a whole feature vector */
 
  xsize = ysize = size;
  x = (double *)calloc(size/2, sizeof(double)); /* x[1],x[2], ..., x[size/2-1] */
  if ((double *)x == NULL)
  {
      printf("not enough memory to allocate x[]\n");
      exit(1);
  }
  y = (double *)calloc(size/2, sizeof(double));  /* y[1],y[2],..., y[size/2-1] */
  if ((double *)y == NULL)
  {
      printf("not enough memory to allocate y[]\n");
      exit(1);
  }
 
  z = (double *)calloc(size/2 - 1, sizeof(double)); /* z[1], ..., z[size/2-2] */
  if ((double *)z == NULL)
  {
      printf("not enough memory to allocate z[]\n");
      exit(1);
  }
 
   /* calculating cent, which represents the mean value of the 
      four central pixels */
 
  cent = obj[xsize/2 - 1][ysize/2 - 1]    /* up-left central pixel */
       + obj[xsize/2][ysize/2 - 1]        /* up-right central pixel */
       + obj[xsize/2][ysize/2]            /* down-right central pixel */
       + obj[xsize/2 -1][ysize/2];        /* down-left central pixel */
  cent /= 4.0; 
 
  m = size / 2 - 1; /* number of element in x[] vector */
 
  for (i = 1; i <= m; i++)
  {
      /* calculating x[] vector */
     x[i] = obj[xsize/2 - 1 - i][ysize/2 - 1]  /* left side x pixel */
          + obj[xsize/2 - 1 - i][ysize/2]  /* left side x pixel */
          + obj[xsize/2 + i][ysize/2 - 1]  /*right side x pixel */
          + obj[xsize/2 + i][ysize/2]      /* right side x pixel */
          + obj[xsize/2 -1][ysize/2 - 1 - i] /* up side x pixel */
          + obj[xsize/2][ysize/2 - 1 - i]    /* up side x pixel */
          + obj[xsize/2 -1][ysize/2 + i]     /* down side x pixel */
          + obj[xsize/2][ysize/2 + i];     /* down side x pixel */
     x[i] /= 8.0; 
 
     /* calculating y[] vector  */
     y[i] = obj[xsize/2 + i][ysize/2 - 1 - i]  /* top-right corner */
          + obj[xsize/2 - 1 - i][ysize/2 - 1 - i] /* top-left corner */
          + obj[xsize/2 - 1 - i][ysize/2 + i]  /* down-left corner */
          + obj[xsize/2 + i][ysize/2 + i];   /* down-right corner */
     y[i] /= 4.0;
 
     /* calaulating z[] vector */
 
    if (i >= 2)
    {
      for (j = 1; j <= i - 1; j++)
      {
         z[i-1] = obj[xsize/2 + j][ysize/2 + i]   /* down-right (two sides) */ 
                + obj[xsize/2 + i][ysize/2 + j] 
                + obj[xsize/2 + j][ysize/2 - 1 - i]  /* up-right (two sides) */ 
                + obj[xsize/2 + i][ysize/2 - 1 - j] 
                + obj[xsize/2 - 1 - j][ysize/2 - 1 - i]  /* up-left (two sides) */
                + obj[xsize/2 - 1 - i][ysize/2 - 1 - j] 
                + obj[xsize/2 - 1 - j][ysize/2 + i]  /* down-left (two sides) */
                + obj[xsize/2 - 1 - i][ysize/2 + j];
      } /* for j */
      z[i-1] /= (double)(8 * (i - 1));
    } /* if i >= 2 */
  } /* for i */
/*
  printf("forming feature vector...\n");
*/ 
  /* allocate feature vector */
  vector = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vector == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vector[]\n");
     exit(1);
  }
 
  vector[0] = cent;
 
  for (i = 1; i <= size/2 - 1; i++)
     vector[i] = x[i];
/*
  printf("running here!...\n");
*/
  for (i = 1; i <= size/2 - 1; i++)
  {
     j = size/2 - 1 + i;
     vector[j] = y[i];
  }
/*
  printf("running here!...\n");
*/ 
  for (i = 1; i <= size/2 - 2; i++)
  {
     j = 2 * (size/2 - 1) + i;
     vector[j] = z[i];
  }
/*
   printf("the feature vector is: \n");
 
  for (i = 0; i < 3 * (size/2 - 1); i++)
  {
     printf("%5.2f ", vector[i]);
  }
  printf("\n\nfinish calculating feature vector[]\n");
*/
  return vector; 
}

double *neigh_mingap_meangap_feature(double **obj, int size)
{
  double *vec1, *vec2, *vector;
  int i, j;


  vec1 = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vec1 == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vec1[]\n");
     exit(1);
  }

  vec2 = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vec2 == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vec2[]\n");
     exit(1);
  }
 
  vector = (double *)calloc(6 * (size/2 -1), sizeof(double));
  if ((double *)vector == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vector[]\n");
     exit(1);
  }
 

  vec1 = neigh_mingap_feature(obj,  size);

  vec2 = neigh_meangap_feature(obj, size);

  for (i = 0; i < 3 * (size/2 -1); i++)
     vector[i] = vec1[i];

  for (i = 0; i < 3 * (size/2 -1); i++)
  {
     j = 3 * (size/2 -1) + i;
     vector[j] = vec2[i];
  }

  return vector;
}

double *neigh_mean_meangap_feature(double **obj, int size)
{
  double *vec1, *vec2, *vector;
  int i, j;
 
 
  vec1 = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vec1 == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vec1[]\n");
     exit(1);
  }
 
  vec2 = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vec2 == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vec2[]\n");
     exit(1);
  }
 
  vector = (double *)calloc(6 * (size/2 -1), sizeof(double));
  if ((double *)vector == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vector[]\n");
     exit(1);
  }
 
 
  vec1 = neigh_mean_feature(obj,  size);
 
  vec2 = neigh_meangap_feature(obj, size);
 
  for (i = 0; i < 3 * (size/2 -1); i++)
     vector[i] = vec1[i];
 
  for (i = 0; i < 3 * (size/2 -1); i++)
  {
     j = 3 * (size/2 -1) + i;
     vector[j] = vec2[i];
  }
 
  return vector;
}

double *neigh_mean_mingap_feature(double **obj, int size)
{
  double *vec1, *vec2, *vector;
  int i, j;
 
 
  vec1 = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vec1 == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vec1[]\n");
     exit(1);
  }
 
  vec2 = (double *)calloc(3 * (size/2 -1), sizeof(double));
  if ((double *)vec2 == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vec2[]\n");
     exit(1);
  }
 
  vector = (double *)calloc(6 * (size/2 -1), sizeof(double));
  if ((double *)vector == NULL)
  {
     fprintf(stderr, "not enough memory to allocate vector[]\n");
     exit(1);
  }
 
 
  vec1 = neigh_mean_feature(obj,  size);
 
  vec2 = neigh_mingap_feature(obj, size);
 
  for (i = 0; i < 3 * (size/2 -1); i++)
     vector[i] = vec1[i];
 
  for (i = 0; i < 3 * (size/2 -1); i++)
  {
     j = 3 * (size/2 -1) + i;
     vector[j] = vec2[i];
  }
 
  return vector;
}


double *row_column_aver_feature(double **obj, int size)
{
  double *row,   /* mean intensive of a row */
         *col,   /* mean intensive of a column */
         *vector; /* the feature vector */
  int i, j;
  int xsize, ysize;

  xsize = ysize = size;

  /* memory allocation for dynamic arrays */
  row = (double *)calloc(size, sizeof(double));
  if ((double *)row == NULL)
  {
     printf("not enough memory to allocate row[]\n");
     exit(1);
  }

  col = (double *)calloc(size, sizeof(double));
  if ((double *)col == NULL)
  {
     printf("not enough memory to allocate col[]\n");
     exit(1);
  }

  vector = (double *)calloc(2*size, sizeof(double));
  if ((double *)vector == NULL)
  {
     printf("not enough memory to allocate vector[]\n");
     exit(1);
  }
/*
  printf("memory allocation finished!\n");
*/
  for (i = 0; i < ysize; i++)
  {
     row[i] = 0.0;
     for (j = 0; j < xsize; j++)
     {
        row[i] += obj[i][j];
     }
     row[i] /= (double)size;
  }
/*
  printf("row[] finished!\n");
*/
  for (j = 0; j < xsize; j++)
  {
     col[j] = 0.0;
     for (i = 0; i < ysize; i++)
     {
        col[j] += obj[i][j];
     }
     col[j] /= (double)size;
  }
/*
  printf("finishing calculating row[] and col[]!\n");
*/
  /* forming the feature vector */
  for (i = 0; i < size; i++)
  {
     vector[i] = row[i];
  }

  for (i = 0; i < size; i++)
  {
     j = size + i;
     vector[j] = col[i];
  }
/*
  printf("finishing the combinition of the vector[]!\n");
*/
  return vector;
}

   
