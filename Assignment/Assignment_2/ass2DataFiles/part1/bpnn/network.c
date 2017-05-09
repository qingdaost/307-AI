/*
 *  network.c -- back propagation neural network architecture maintenance code
 *
 *    author: Steven Hayes
 *     email: steveh@rmit.edu.au
 *
 *   version: 3.0
 *    date: 06/06/95
 *
 *  related files:   backprop.h
 *             backprop.c
 *             globals.c
 *             weights.c
 *             patterns.c
 *             memory.c
 */

#include "backprop.h"
#include <signal.h>
#include <stdlib.h>

/*
 *  int    create_network(void)
 *
 *  constructs the data structures necessary for efficiently storing a
 *  back-propagation network of any size, number of layers, etc.
 *  requires that the variables nlayers, nunits and nweights be set
 *  so reflect the desired network. returns FALSE if an error occures, TRUE
 *  otherwise.
 *  this function gets automatically called by load_network().
 *
 */

int create_network()
{
    int    i, j, k, l, u;

    /* do mallocs and other data structure creation */

    first_to = (int *) malloc(nunits * sizeof(int));
    last_to = (int *) malloc(nunits * sizeof(int));
    first_weight = (int *) malloc(nweights * sizeof(int));

    if ((char *) first_to == NULL || (char *) last_to == NULL ||
        (char *) first_weight == NULL)
    {
        return FALSE;
    }

#ifdef DEBUG
    fprintf(stderr, "Trying to allocate %u bytes during network creation... ",
         (unsigned)((nweights * 3 + nunits * 7) * sizeof(double)));
#endif

    weight = (double *) malloc(nweights * sizeof(double));
    dweight = (double *) malloc(nweights * sizeof(double));
    bias = (double *) malloc(nunits * sizeof(double));
    dbias = (double *) malloc(nunits * sizeof(double));
    netinput = (double *) malloc(nunits * sizeof(double));
    activate = (double *) malloc(nunits * sizeof(double));
    delta = (double *) malloc(nunits * sizeof(double));
    error = (double *) malloc(nunits * sizeof(double));
    weight_error_derivative = (double *) malloc(nweights * sizeof(double));
    bias_error_derivative = (double *) malloc(nunits * sizeof(double));

    if ((char *) weight == NULL || (char *) bias == NULL ||
        (char *) netinput == NULL || (char *) activate == NULL ||
        (char *) delta == NULL || (char *) error == NULL ||
        (char *) weight_error_derivative == NULL ||
        (char *) bias_error_derivative == NULL ||
        (char *) dweight == NULL || (char *) dbias == NULL)
    {
#ifdef DEBUG
        fprintf(stderr, "Failed\n");
#endif
        return FALSE;
    }

#ifdef DEBUG
    fprintf(stderr, "Successfull\n");
#endif

    for (i = 0; i < nweights; ++i)
    {
        dweight    [i] = 0.0;
    }

    for (i = 0; i < nunits; ++i)
    {
        dbias [i] = 0.0;
    }

    /* now create the index vector for accessing weights efficiently  */
    /* and create the vectors which index unit connections */

    u = units_in_layer [0];        /* u is the next unit to be dealt with */
    k = 0;                /* k is the index to the next free weight */
    l = 0;                /* first connecting unit from a previous layer to current unit */

    for (i = 1; i < nlayers; ++i)             /* i represents a layer */
    {
        for (j = 0; j < units_in_layer [i]; ++j)     /* j is a unit in i */
        {
            first_weight [u] = k;             /* set first weight of u */
            k += units_in_layer [i - 1];         /* update k to point to next free weight */

            first_to [u] = l;             /* connect units together */
            last_to [u] = l + units_in_layer [i - 1];

            ++u;                     /* next unit to set up */
        }
        l += units_in_layer [i - 1];
    }
    return TRUE;
}


/*
 *  void destroy_network(void)
 *
 *  frees up all the memory allocated to a network. returns nothing.
 *
 */

void destroy_network()
{
    free(first_to);
    free(last_to);
    free(first_weight);
    free(units_in_layer);

    free(weight);
    free(bias);
    free(netinput);
    free(activate);
    free(delta);
    free(error);
    free(dweight);
    free(dbias);
    free(weight_error_derivative);
    free(bias_error_derivative);
}


/*
 *  int    load_network(char *network_file)
 *
 *  loads a network description file, and creates the data structures necessary
 *  to store network values by calling create_network(). returns FALSE if an
 *  error occures, TRUE otherwise.
 *
 */

int load_network(network_file)     /* loads a network from a file */
char *network_file;
{
    FILE *fp;
    int i;

    if ((fp = fopen(network_file, "r")) == NULL)
    {
        return FALSE;
    }

    if (fscanf(fp, "lr %lf ", &epsilon) == EOF)
    {
        fclose(fp);
        return FALSE;
    }

    if (fscanf(fp, "m %lf ", &momentum) == EOF)
    {
        fclose(fp);
        return FALSE;
    }

    if (fscanf(fp, "ce %lf ", &ecrit) == EOF)
    {
        fclose(fp);
        return FALSE;
    }

    if (fscanf(fp, "r %lf ", &range) == EOF)
    {
        fclose(fp);
        return FALSE;
    }

    if (fscanf(fp, "percent %lf ", &corrpercent) == EOF)
    {
        fclose(fp);
        return FALSE;
    }

    fprintf(stderr, "LR = %lf, M = %lf, CE = %lf, R = %lf PERCENT = %lf\n",
            epsilon, momentum, ecrit, range, corrpercent);
    if (fscanf(fp, "%d", &nlayers) == EOF)
    {
        fclose(fp);
        return FALSE;
    }

    if ((units_in_layer = (int *) malloc(nlayers * sizeof(int))) == NULL)
    {
        return FALSE;
    }

    nunits = 0;
    for (i = 0; i < nlayers; ++i)        /* read number of units in each layer */
    {
        if (fscanf(fp, "%d", &units_in_layer [i]) == EOF)
        {
            fclose(fp);
            return FALSE;
        }
        nunits += units_in_layer [i];
    }

    fclose(fp);

    for (i = 1; i < nlayers; ++i)        /* calculate total number of weights */
    {
        nweights += units_in_layer [i] * units_in_layer [i - 1];
    }

    return create_network();        /* create the data structures  */
}

/*
 *  int    save_network(char *network_file)
 *
 *  saves a network description file, which contains the number of layers
 *  and the number of units in each layer. if an error occures, FALSE is
 *  returned, otherwise TRUE.
 *  this function should not be called until after load_network() is called.
 *
 */

int save_network(network_file)
char *network_file;
{
    FILE *fp;
    int    i;

    if ((fp = fopen(network_file, "w")) == NULL)
    {
        return FALSE;
    }

    if (fprintf(fp, "%d", nlayers) < 0)     /* save number of layers  */
    {
        fclose(fp);
        return FALSE;
    }

    for (i = 0; i < nlayers; ++i)         /* save layer numbers to file */
    {
        if (fprintf(fp, "%d", units_in_layer [i]) < 0)
        {
            fclose(fp);
            return FALSE;
        }
    }
    fclose(fp);

    return TRUE;
}


/*
 *  void reset_network(void)
 *
 *  resets a networks weight values, bias values, learning rate and momentum
 *  value. returns nothing.
 *  do not call this function unless load_network() was called previously.
 *
 */

void reset_network()
{
    int i;

    srand48(time(NULL));        /* pick a random seed */

    for (i = 0; i < nweights; ++i)
    {
        weight [i] = (2.0 * drand48() - 1.0) * range;
    }

    for (i = units_in_layer [0]; i < nunits; ++i)
    {
        bias [i] = (2.0 * drand48() - 1.0) * range;
    }
}
