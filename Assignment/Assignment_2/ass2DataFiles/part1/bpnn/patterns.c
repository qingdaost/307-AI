/*
 *  patterns.c -- N layer back propagation neural network pattern maintenance
 *
 *    author: Mengjie Zhang
 *     email: mengjie@mcs.vuw.ac.nz
 *
 *   version: 4.0
 *    date: 05/09/00
 *
 *  related files:   backprop.c
 *             backprop.h
 *             network.c
 *             globals.c
 *             weights.c
 *             memory.c
 */

#include "backprop.h"
#include <stdlib.h>

/*
 *  int load_patterns(char *pattern_file)
 *
 *  loads a file of input/output pattern pairs, and stores the patterns in
 *  inputs [] and targets []. the pattern associations loaded are used for
 *  network training and testing purposes. returns FALSE if an error occures,
 *  TRUE otherwise.
 *  this function must only be called after a network specification file
 *  has been read in successfully, via the load_network() function.
 *
 */

int load_patterns(char *pattern_file)
{
    FILE *fp;
    int i, j;

    if ((fp = fopen(pattern_file, "r")) == NULL)
    {
        return FALSE;
    }

    fscanf(fp, "%d", &ntrain);
    fscanf(fp, "%d", &ntest);

    inputs = (double **) malloc((ntrain + ntest) * sizeof(double *));
    targets = (double **) malloc((ntrain + ntest) * sizeof(double *));

    if ((double *) targets == NULL || (double *) inputs == NULL)
    {
        fclose(fp);
        return FALSE;
    }

    for (j = 0; j < ntrain + ntest; ++j)
    {
        inputs [j] = (double *) malloc (units_in_layer [0] * sizeof(double));
        targets [j] = (double *) malloc (units_in_layer [nlayers - 1] * sizeof(double));

        if ((double *) inputs [j] == NULL || (double *) targets [j] == NULL)
        {
            fclose(fp);
            return FALSE;
        }
   }

    for (i = 0; i < ntrain + ntest; i++) 
    {
      for (j = 0; j < units_in_layer [0]; j++)
      {
	if (fscanf(fp, "%lf", &inputs[i][j]) != 1)
        {
            fclose(fp);
            printf("read from pattern file failed\n");
            return FALSE;
        }
      } /* for j */
      for (j = 0; j < units_in_layer [nlayers-1]; j++)
      {
        if (fscanf(fp, "%lf", &targets[i][j]) != 1)
 	{
            fclose(fp);
            printf("read from pattern file failed\n");
            return FALSE;
        }
      } /* for j */
    } /* for i */

    fclose(fp);
    return TRUE;
}


/*
 *  void destroy_patterns(void)
 *
 *  frees the memory allocated for pattern storage. returns nothing.
 *
 */

void destroy_patterns(void)
{
    int j;

    for (j = 0; j < ntrain + ntest; ++j)
    {
        free(inputs [j]);
        free(targets [j]);
    }
    free(targets);
    free(inputs);
}

