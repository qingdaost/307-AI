/*  nnsweep-feature.c
 * 
 *  Back propagation template sweeping code. This program produces 
 *  two sets of files after sweeping a large picture. The first set
 *  contains the output plane image for each class. The second one
 *  consists of a group of location files (including locations and 
 *  their activation) of every class/output unit.
 *
 *  Different from the pixel-based procedure, this program uses
 *  simple features as the inputs to the network.
 *
 *  author   : Mengjie Zhang
 *             mengjie@cs.rmit.edu.au
 *  version  : 2.0
 *  date     : 16/07/98
 *
 */

#include "backprop.h"
#include "feature.h"

/*
 *  Macros and definitions
 */

#define TRUE    1
#define FALSE   0
#define OK      1


/*
 *  Data structures and typedefs
 */

typedef unsigned char UCHAR;

typedef struct _PGMIMAGE     /* Structure for containing a PGM format image */
{
    int greylevel;
    int xsize;
    int ysize;
    UCHAR *image;
} PGMIMAGE;

typedef struct _LOC	     /* structure for storing object locations */
{
    double cf;
    int xloc;
    int yloc;
} LOC;

/*
 *  Function prototypes
 */

PGMIMAGE    *CreatePGM(int xsize, int ysize, int greylevel);
void        DestroyPGM(PGMIMAGE *pgm);
PGMIMAGE    *LoadPGM(char *filename);
int         SavePGM(char *filename, PGMIMAGE *pgm);
int         SaveLoc(char *fname, int xsize, int ysize, LOC *loc);
void 	    sweep(PGMIMAGE *src, char *objectname_fn, int feat_type);

/*
 * Globals variables
 */


int main(int argc, char *argv[])
{
    PGMIMAGE *src;
    char objects_names_fn[45];
    int feat_type;

    fprintf(stderr, "Backprop Neural Network Template Sweeper\n -- Pixel-based-feature(Version 1.0)\n");
    fprintf(stderr, "(C) 1998 Mengjie Zhang, RMIT \n mengjie@cs.rmit.edu.au\n");

    if (argc != 6)
    {
         fprintf(stderr, "\nInsufficient arguments (or too many!)\n");
         fprintf(stderr, "Usage: %s [feature_type_no] [network] [weights] [objects_names] [image]\n", argv[0]);
         fprintf(stderr, "Feature type number:\n");
         fprintf(stderr, "  1. minimum gap featurs    \n");
         fprintf(stderr, "  2. mean gap features      \n");
         fprintf(stderr, "  3. mean neighboring features\n");
         fprintf(stderr, "  4. both 1 and 2           \n");
         fprintf(stderr, "  5. both 2 and 3           \n");
         fprintf(stderr, "  6. both 3 and 1           \n");
         fprintf(stderr, "  7. mean-row-column        \n");
         fprintf(stderr, "Different feature type may lead to different net inputs! \n");
         fprintf(stderr, "A report will be written to plane image files and location files\n");
         return;
    }

    if (sscanf(argv[1], "%d", &feat_type) != 1)
    {
       fprintf(stderr, "Usage: %s [feature_type] [network] [weights] [objects_names] [image]\n", argv[0]);
       exit(1);
    }
    if (feat_type < 1 || feat_type > 7)
    {
       fprintf(stderr, "Feature type number should be between 1-7!\n");
       fprintf(stderr, "  1. minimum gap featurs    \n");
       fprintf(stderr, "  2. mean gap features      \n");
       fprintf(stderr, "  3. mean neighboring features\n");
       fprintf(stderr, "  4. both 1 and 2           \n");
       fprintf(stderr, "  5. both 2 and 3           \n");
       fprintf(stderr, "  6. both 3 and 1           \n");
       fprintf(stderr, "  7. mean-row-column        \n");
       exit(1);
    }

    if (!load_network(argv [2]))
    {
         fprintf(stderr, "Error on reading %s\n", argv [2]);
         return;
    }
    reset_network();

    if (!load_weights(argv [3]))
    {
        fprintf(stderr, "Error on reading %s\n", argv [3]);
        return;
    }
    
    strcpy(objects_names_fn, argv[4]);
    
    if ((src = LoadPGM(argv[5])) == NULL) 
    {
        fprintf(stderr, "Error on reading %s\n", argv [5]);
        return;
    }
    fprintf(stderr, "\nNetwork topology details:\n");
    fprintf(stderr, "  Input units = %d\n", units_in_layer[0]);
    fprintf(stderr, " Hidden units = %d\n", units_in_layer[1]);
    fprintf(stderr, " Output units = %d\n", units_in_layer[2]);
    fprintf(stderr, " Total computational units = %d\n\n", nunits);

    sweep(src, objects_names_fn, feat_type);
    fprintf(stderr, "ending\n");

    return 0;
}

void sweep(PGMIMAGE *src, char *objectname_fn, int feat_type)
{
    int x, y, xsize, ysize, u, i, j;
    byte *in;           /* input patterns */
    char **objects;     /* location and plane image file names */
    LOC **locations;    /* content of location files */
    PGMIMAGE **plane;   /* content of plane images */
    int size_of_object; /* cutout size, training square size */
    FILE *objectname_fp, *obj_loc_fp;
    char file_na[45], file_na1[45];
    double **temp, /* containing the sweeping object (square) */
           *feature; /* feature vector */


    /* calculating the size of objects */
    if (feat_type == 1 || feat_type == 2 || feat_type == 3)
       size_of_object = (int)((units_in_layer[0] / 3 + 1) * 2);
    else if (feat_type == 4 || feat_type == 5 || feat_type == 6)
       size_of_object = (int)((units_in_layer[0] / 6 + 1) * 2);
    else if (feat_type == 7)
       size_of_object = (int)(units_in_layer[0] / 2);

    fprintf(stderr, " Object size = %d pixels\n", size_of_object);

    /* allocating memory for dynamic arrays  */

    in = (byte *)calloc(units_in_layer[0], sizeof(byte));
    if ((byte *)in == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate in[]\n");
       exit(1);
    }

    feature = (double *)calloc(units_in_layer[0], sizeof(double));
    if ((double *)feature == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate feature[]\n");
       exit(1);
    }

    temp = (double **)calloc(size_of_object, sizeof(double *));
    if ((double **)temp == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate temp[][]\n");
       exit(1);
    }
    for (i = 0; i < size_of_object; i++)
    {
       temp[i] = (double *)calloc(size_of_object, sizeof(double));
       if ((double *)temp[i] == NULL)
       {
         fprintf(stderr, "Not enough memoey to allocate temp[][]\n");
         exit(1);
       }
    }

    objects = (char **)calloc(units_in_layer[nlayers - 1], sizeof(char *));
    if ((char **)objects == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate objects[]\n");
       exit(1);
    }
    for (i = 0; i < units_in_layer[nlayers - 1]; i++)
    {
       objects[i] = (char *)malloc(30 * sizeof(char));
       if ((char *)objects[i] == NULL)
       {
       fprintf(stderr, "Not enough memoey to allocate objects[]\n");
       exit(1);
       }
    }

    plane = (PGMIMAGE **)calloc(units_in_layer[nlayers - 1], sizeof(PGMIMAGE *));

    if ((PGMIMAGE **)plane == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate plane[]\n");
       exit(1);
    }


    locations = (LOC **)calloc(units_in_layer[nlayers - 1], sizeof(LOC *));
    if ((LOC **)locations == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate location[]\n");
       exit(1);
    }
 
    /* getting the objects' names from a file "*.nam"   */

   if ((objectname_fp = fopen(objectname_fn, "r")) == NULL)
   {
      printf("cannot open file %s\n", objectname_fn);
      exit(1);
   }

   for (i = 0; i < units_in_layer[nlayers - 1]; i++)
      fscanf(objectname_fp, "%s", objects[i]);

   fclose(objectname_fp);

    /* set up data */

    xsize = src->xsize - size_of_object;
    ysize = src->ysize - size_of_object;

    for (i = 0; i < units_in_layer[nlayers - 1]; i++) 
    {
	plane[i] = CreatePGM(xsize, ysize, 255);
    }

    for (i = 0; i < units_in_layer[nlayers - 1]; ++i) 
    {
       locations[i] = (LOC *)calloc(xsize * ysize, sizeof(LOC));
       if ((LOC *)locations[i] == NULL)
       {
          fprintf(stderr, "Not enough memory to allocate location[][]\n");
          exit(1);
       }
    }

    /* sweep the image */

    for (y = 0; y < ysize; ++y)			
    {
	for (x = 0; x < xsize; ++x)
	{
	    int i, j, k = 0;

	    /* load the features of the image portion into in[] array */

	    for (j = 0; j < size_of_object; ++j)
		for (i = 0; i < size_of_object; ++i)
		    temp[j][i] = (double) src->image[(y+j) * src->xsize + (x+i)];

            if (feat_type == 1)
               feature = neigh_mingap_feature(temp, size_of_object);
            else if (feat_type == 2)
	       feature = neigh_meangap_feature(temp, size_of_object);
            else if (feat_type == 3)
               feature = neigh_mean_feature(temp, size_of_object);
            else if (feat_type == 4)
               feature = neigh_mingap_meangap_feature(temp, size_of_object);
            else if (feat_type == 5)
               feature = neigh_mean_meangap_feature(temp, size_of_object);
            else if (feat_type == 6)
               feature = neigh_mean_mingap_feature(temp, size_of_object);
            else if (feat_type == 7)
               feature = row_column_aver_feature(temp, size_of_object);
 
            for (k = 0; k < units_in_layer[0]; k++)
               in[k] = (byte) feature[k];

            /* calculate network output */

	    for (u = units_in_layer[0]; u < nunits; ++u)
	    {
		netinput[u] = bias[u];
		k = first_weight[u];
		for (i = first_to[u]; i < last_to[u]; ++i)
		{
		    netinput[u] += ((i < units_in_layer[0]) ? (in[i]) / 255.0 :
			activate[i]) * weight[k];
                    ++k;
                }
                activate[u] = logistic(netinput[u]);
            }
	    /* store the results */

            u = nunits - units_in_layer [nlayers - 1];
            for (i = 0; i < units_in_layer [nlayers - 1]; ++i)
            {
		plane[i]->image[xsize * y + x] = (byte)(activate[i+u] * 255.0);
		locations[i][xsize * y + x].xloc = x + size_of_object / 2;
		locations[i][xsize * y + x].yloc = y + size_of_object / 2;
		locations[i][xsize * y + x].cf   = activate[i+u];
            }
	}
    }
    fprintf(stderr, "Completed plane sweeping...\n");
    for (i = 0; i < units_in_layer [nlayers - 1]; i++)
    {
      strcpy(file_na, objects[i]);
      strcat(file_na, "_plane.pgm");
      SavePGM(file_na, plane[i]);
    }

    for (i = 0; i < units_in_layer [nlayers - 1]; ++i)
    {
      strcpy(file_na1, objects[i]);
      strcat(file_na1, "_loc.txt");
      SaveLoc(file_na1, xsize, ysize, locations[i]);
    }

    /* free dynamic arrays */
   
    free(plane);
    free(objects);
    for (i = 0; i < units_in_layer [nlayers - 1]; i++)
      free(objects[i]);
    free(locations);
    for (i = 0; i < units_in_layer [nlayers - 1]; i++)
      free(locations[i]);
    free(in);
    free(feature);
    free(temp);
    for (i = 0; i < size_of_object; i++)
      free(temp[i]);
    DestroyPGM(src);
    destroy_patterns();
    destroy_network();
}

/*
 *  CreatePGM() -- Creates a PGM image structure in memory
 */

PGMIMAGE *CreatePGM(int xsize, int ysize, int greylevel)
{
    PGMIMAGE *pgm;

    if ((pgm = (PGMIMAGE *) malloc(sizeof(PGMIMAGE))) == NULL) return NULL;
 
    pgm->greylevel = greylevel;
    pgm->xsize = xsize;
    pgm->ysize = ysize;

    if ((pgm->image = (UCHAR *) malloc(sizeof(UCHAR) * xsize * ysize)) == NULL)
    {
        free(pgm);
        return NULL;
    }

    memset(pgm->image, 0, xsize * ysize);

    return pgm;
}

/*
 *  DestroyPGM() -- Destroys a PGM format image in memory
 */

void DestroyPGM(PGMIMAGE *pgm)
{
    free(pgm->image);
    free(pgm);
}

/*
 *  LoadPGM() -- Loads a PGM format image file into memory
 */

PGMIMAGE *LoadPGM(char *filename)
{
    FILE *fp;
    PGMIMAGE *pgm;
    int i;

    if ((fp = fopen(filename, "rb")) == NULL) return NULL;

    if ((pgm = (PGMIMAGE *) malloc(sizeof(PGMIMAGE))) == NULL)
    {
        fclose(fp);
        return NULL;
    }

    fscanf(fp, "P5 %d %d %d\n", &pgm->xsize, &pgm->ysize, &pgm->greylevel);

    if ((pgm->image = (UCHAR *) malloc(sizeof(UCHAR) * pgm->xsize * pgm->ysize)) == NULL)
    {
        free(pgm);
        fclose(fp);
        return NULL;
    }

    for (i = 0; i < pgm->xsize * pgm->ysize; i++)
    {
        pgm->image[i] = (UCHAR) fgetc(fp);
    }

    return pgm;
}

/*
 *  SavePGM() -- Save a PGM format image to a file
 */

int SavePGM(char *filename, PGMIMAGE *pgm)
{
    FILE *fp;
    int i;

    if ((fp = fopen(filename, "wb")) == NULL) return (int) NULL;
    
    fprintf(fp, "P5\n %d %d %d\n", pgm->xsize, pgm->ysize, pgm->greylevel);

    for (i = 0; i < pgm->xsize * pgm->ysize; i++)
    {
        fputc(pgm->image[i], fp);
    }
 
    fclose(fp);

    return OK;
}

/*
 *  SaveLoc() -- Save a text format center positions  to a file
 */

int SaveLoc(char *fname, int xsize, int ysize, LOC *loc)
{
   FILE *fp;
   int i;

   fp = fopen(fname, "w");
   if (fp == NULL)
      return (int)NULL;

   fprintf(fp, "%d\n", xsize * ysize);

   for (i = 0; i < xsize * ysize; i++)
   {
      fprintf(fp, "%10d %10d %12.8f\n", loc[i].xloc, loc[i].yloc, loc[i].cf);
   }

   fclose(fp);

   return OK;
}

