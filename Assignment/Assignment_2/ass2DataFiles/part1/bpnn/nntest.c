/*
 *  Back propagation feed forward analyser
 *
 */

#include "backprop.h"

/* local prototypes */

void display();

int main(argc, argv)
int argc;
char *argv [];
{
    fprintf(stderr, "Backprop Neural Network Analyser\n Version 4.0\n (C) 1998 Mengjie Zhang, RMIT \n mengjie@cs.rmit.edu.au\n");

    epsilon = 0.3;
    momentum = 0.0;
    range = 0.3;
    ecrit = 0.03;

    fanin = FALSE;
    verbose = 1;
    vverbose = FALSE;

    if (argc != 4)
    {
         fprintf(stderr, "\nInsufficient arguments (or too many!)\n");
         fprintf(stderr, "Usage: %s [network] [patterns] [weights]\n", argv[0]);
         fprintf(stderr, "A report will be written to stdout\n");
         return;
    }

    if (!load_network(argv [1]))
    {
         fprintf(stderr, "Error on reading %s\n", argv [1]);
         return;
    }
    reset_network();
    if (!load_patterns(argv [2]))
    {
        fprintf(stderr, "Error on reading %s\n", argv [2]);
        return;
    }

    if (!load_weights(argv [3]))
    {
        fprintf(stderr, "Error on reading %s\n", argv [3]);
        return;
    }

    display();
    fprintf(stderr, "ending\n");
}

void display()
{
    int i, j;
    double error;
    double total, mse;
    int errcount = 0;

    if (ntrain > 0)
    {
        total = 0.0;
        fprintf(stdout, "Performance of network on training data:\n\n");
        fprintf(stdout, "Pat #\tError\tActivations\n");
        for (i = 0; i < ntrain; ++i)         /* show results */
        {
            int oc = 0;
            int rc = 0;
            int k = 0;
            double ac = 0.0;
 
            compute_output(i);
            error = compute_error(i);

/*  change this - it is stupid */
/*
            if (error > 1.0)	
                ++errcount;
*/
            total += error;
            fprintf(stdout, "%d\t%1.3lf\t", i, error/units_in_layer[nlayers-1]);
            for (j = nunits - units_in_layer [nlayers - 1]; j < nunits; ++j)
            {
                fprintf(stdout, "%1.2lf ", activate [j]);
                /* new error calculation method here */
                if (activate [j] > ac) 
                {
                    ac = activate [j];
                    oc = k;
                }
                ++k;
                /* new error calculation method ends here */
            }

            for (j = 0; j < units_in_layer [nlayers - 1]; ++j)
            {
                /* new error calculation method here */
                if ((1.0 - targets[i][j]) <=  0.1)
                    rc = j;
                /* new error calculation method ends here */
            }
            /* new error calculation method here */
            if (oc != rc) {
               ++errcount;
               printf("ERROR");
            }
            /* new error calculation method ends here */
            fprintf(stdout, "\n");
        }
        mse = total / (ntrain * units_in_layer[nlayers - 1]);

        fprintf(stdout, "\nMean squared error for training data: %2.3f", mse);
        fprintf(stdout, "\nNumber of incorrect classifications: %d/%d\n\n", errcount, ntrain);
    }

    errcount = 0;
    if (ntest > 0)
    {
        total = 0.0;
        fprintf(stdout, "Performance of network on test data:\n");
        fprintf(stdout, "Pat #\tError\tActivations\n");
        for (i = ntrain; i < ntrain + ntest; ++i)
        {
	    int oc = 0;
	    int rc = 0;
	    int k = 0;
	    double ac = 0.0;

            compute_output(i);
            error = compute_error(i);
	    /* this is a spastic way of computing the error
            if (error > 1.0) 
                ++errcount;
            */
            total += error;
            fprintf(stdout, "%d\t%1.3lf\t", i, error / units_in_layer[nlayers - 1]);
	    oc = 0;
	    rc = 0;
	    ac = 0.0;
            for (j = nunits - units_in_layer [nlayers - 1]; j < nunits; ++j)
            {
                fprintf(stdout, "%1.2lf\t", activate [j]);
		/* new error calculation method here */
		if (activate [j] > ac) 
		{
		    ac = activate [j];
		    oc = k;
                }
		++k;
                /* new error calculation method ends here */
            }
            fprintf(stdout, "\nDesired:\t");
            for (j = 0; j < units_in_layer [nlayers - 1]; ++j)
            {
                fprintf(stdout, "%1.2lf\t", ((double) targets[i][j]));
		/* new error calculation method here */
		if ((1.0 - targets[i][j]) <= 0.1) 
		    rc = j;
                /* new error calculation method ends here */
            }
	    /* new error calculation method here */
	    if (oc != rc) {
	       ++errcount;
	       printf("ERROR");
            }
            /* new error calculation method ends here */
            printf("\n\n");
        }
        mse = total / (ntest * units_in_layer[nlayers - 1]);

        fprintf(stdout, "\nMean squared error for test data: %2.3lf", mse);
        fprintf(stdout, "\nNumber of incorrect classifications: %d/%d\n\n", errcount, ntest);
    }
}
