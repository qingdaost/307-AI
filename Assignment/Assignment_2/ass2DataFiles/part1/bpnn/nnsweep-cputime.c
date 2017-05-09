/* nnsweep-cputime.c
 *  
 * This program is to test the sweeping time and the computational time
 * of every network (at each point) for object detection.
 * 
 *  author:  Mengjie Zhang
 *           mengjie@cs.rmit.edu.au
 *  date  :  18/11/1998
 *
 *  related file:  u_lib.c, ... ...
 */

#include "backprop.h"

/*
 *  Macros and definitions
 */

#define ABS(x)  (((x) < 0) ? -(x) : (x))

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
    double max;
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
void 	    sweep(PGMIMAGE *src, char *objectname_fn);

/*
 * Globals variables
 */


int main(int argc, char *argv[])
{
    PGMIMAGE *src;
    char objects_names_fn[25];

    fprintf(stderr, "Tester for network computation 1.0\n");
    fprintf(stderr, "(C) 1998 Mengjie Zhang, RMIT \n mengjie@cs.rmit.edu.au\n");

    if (argc != 5)
    {
         fprintf(stderr, "\nInsufficient arguments (or too many!)\n");
         fprintf(stderr, "Usage: %s [network] [weights] [objects_names] [image]\n", argv[0]);
         fprintf(stderr, "A report will be written to stdout\n");
         return;
    }

    if (!load_network(argv [1]))
    {
         fprintf(stderr, "Error on reading %s\n", argv [1]);
         return;
    }
    reset_network();

    if (!load_weights(argv [2]))
    {
        fprintf(stderr, "Error on reading %s\n", argv [2]);
        return;
    }
    
    strcpy(objects_names_fn, argv[3]);
    
    if ((src = LoadPGM(argv[4])) == NULL) 
    {
        fprintf(stderr, "Error on reading %s\n", argv [3]);
        return;
    }
    fprintf(stderr, "\nNetwork topology details:\n");
    fprintf(stderr, "  Input units = %d\n", units_in_layer[0]);
    fprintf(stderr, " Hidden units = %d\n", units_in_layer[1]);
    fprintf(stderr, " Output units = %d\n", units_in_layer[2]);
    fprintf(stderr, " Total computational units = %d\n\n", nunits);

    sweep(src, objects_names_fn);
    fprintf(stderr, "ending\n");
    return 0;
}

void sweep(PGMIMAGE *src, char *objectname_fn)
{
    int x, y, xsize, ysize, u, i, count;
    byte *in;
    LOC *locations;
    char **objects;
    PGMIMAGE **plane;
    int size_of_object;
    FILE *objectname_fp;
    char file_na[40];

    /* calculating the size of objects */
    
    size_of_object = (int)sqrt(units_in_layer[0]);
    fprintf(stderr, " Object size = %d pixels\n", size_of_object);

    /* allocating memory for dynamic arrays  */

    locations = (LOC *)calloc(units_in_layer[nlayers - 1], sizeof(LOC));
    if ((LOC *)locations == NULL)
    {
       fprintf(stderr, "Not enough memoey to allocate location[]\n");
       exit(1);
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
    for (i = 0; i < units_in_layer[nlayers - 1]; ++i) {
	locations[i].max = 0.0; 
	plane[i] = CreatePGM(xsize, ysize, 255);
    }

       in = (byte *)calloc(units_in_layer[0], sizeof(byte));
       if ((byte *)in == NULL)
       {
          fprintf(stderr, "Not enough memoey to allocate in[]\n");
          exit(1);
       }


    /* sweep the image */
/*
    printf("\n\nStaring to count the cpu time for loading image portion!!!\n\n");

    start_time();
*/
    for (y = 0; y < ysize; ++y)			
    {
	for (x = 0; x < xsize; ++x)
	{
	    int i, j, k = 0;

	    /* load image portion into in[] array */
	      for (j = 0; j < size_of_object; ++j)
		for (i = 0; i < size_of_object; ++i)
		    in[k++] = (byte) src->image[(y+j) * src->xsize + (x+i)];
/*
    prn_time();
    printf("\nfinish loading image portion into in[]!!!\n");
*/

            /* calculate network output */


    printf("\n\nStaring to count the cpu time for network calculation!!!\n\n");

    start_time();
         for (count = 0; count < 20000; count++)
         {
	    for (u = units_in_layer[0]; u < nunits; ++u)
	    {
		netinput[u] = bias[u];
		k = first_weight[u];
		for (i = first_to[u]; i < last_to[u]; ++i)
		{
		    netinput[u] += ((i < units_in_layer[0]) ? (in[i]) / 255.0 : activate[i]) * weight[k];
                    ++k;
                }
                activate[u] = logistic(netinput[u]);
            }
         }  /* for count */
    prn_time();
    printf("\n\nFinish counting the cpu time for network calculation!!!\n\n"); 
    printf("\n\nthe total number of the network for calculation: 20000\n\n");
    exit(1);
        }
    }

    printf("\n\nthe total number of the network for calculation: %d * %d = %d\n\n", xsize, ysize, xsize * ysize);

    fprintf(stderr, "Completed plane sweep...\n");


    /* free dynamic arrays */
   
    free(plane);
    free(objects);
    for (i = 0; i < units_in_layer [nlayers - 1]; ++i)
      free(objects[i]);
    free(locations);
    free(in);
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

