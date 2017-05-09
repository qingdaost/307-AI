/*
 *  patterns.c -- N layer back propagation neural network pattern maintenance
 *
 *    author: Steven Hayes
 *     email: steveh@rmit.edu.au
 *
 *   version: 3.0
 *    date: 06/06/95
 *
 *  related files:   backprop.c
 *             backprop.h
 *             network.c
 *             globals.c
 *             weights.c
 *             memory.c
 */

#include "backprop.h"


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
    int fd;
    int j;

    if ((fd = open(pattern_file, O_RDONLY)) == 0)
    {
        return FALSE;
    }

    read(fd, &ntrain, 4);
    read(fd, &ntest, 4);
#ifdef DEBUG
    fprintf(stderr, "%d training patterns, %d testing patterns\n", ntrain, ntest);
    fprintf(stderr, "%d inputs and %d outputs per patern\n", units_in_layer [0], units_in_layer [nlayers - 1]);
    fprintf(stderr, "Trying to allocate %u bytes for pattern storage... ",
        (unsigned)((ntrain + ntest) * units_in_layer [0] * units_in_layer [nlayers - 1]));
    fflush(stderr);
#endif

    inputs = (byte **) malloc((ntrain + ntest) * sizeof(byte *));
    targets = (byte **) malloc((ntrain + ntest) * sizeof(byte *));

    if ((char *) targets == NULL || (char *) inputs == NULL)
    {
#ifdef DEBUG
        fprintf(stderr, "Failed\n");
#endif
        close(fd);
        return FALSE;
    }

    for (j = 0; j < ntrain + ntest; ++j)
    {
        inputs [j] = (byte *) malloc (units_in_layer [0]);
        targets [j] = (byte *) malloc (units_in_layer [nlayers - 1]);

        if ((char *) inputs [j] == NULL || (char *) targets [j] == NULL)
        {
#ifdef DEBUG
            fprintf(stderr, "Failed\n");
#endif
            close(fd);
            return FALSE;
        }
        if (read(fd, inputs[j], units_in_layer [0]) != units_in_layer [0])
        {
            close(fd);
            printf("read from pattern file failed\n");
            return FALSE;
        }
        if (read(fd, targets[j], units_in_layer [nlayers-1]) != units_in_layer [nlayers-1])
        {
            close(fd);
            printf("read from pattern file failed\n");
            return FALSE;
        }
    }
#ifdef DEBUG
    fprintf(stderr, "Successfull\n");
#endif

    close(fd);
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

