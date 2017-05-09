/*
 *  weights.c -- N layer back propagation neural network weight	maintenance
 *
 *    author: Steven Hayes
 *     email: steveh@rmit.edu.au
 *
 *   version: 2.0
 *	date: 16/04/93
 *
 *  related files:   backprop.c
 *		     backprop.h
 *		     network.c
 *		     globals.c
 *		     patterns.c
 *		     memory.c
 */

#include "backprop.h"


/*
 *  int	load_weights(char * weight_file)
 *
 *  loads a weight file, including bias	values,	into the network. returns
 *  FALSE if there was an error, TRUE otherwise;
 *  this function should not be	called until after load_network() has been
 *  called.
 *
 */

int load_weights(weight_file)
char * weight_file;
{
    FILE *fp;
    int	i;

    if ((fp = fopen(weight_file, "r")) == NULL)
    {
	return FALSE;
    }

	for (i = 0; i <	nweights; ++i)		/* load	weight values into memory */
    {
		if (fscanf(fp, "%lf", &weight [i]) == EOF)
		{
			fclose(fp);
			return FALSE;
		}
	}

	for (i = 0; i <	nunits;	i++)		/* now read the	biases	*/
	{
		if (fscanf(fp, "%lf", &bias [i]) == EOF)
		{
			fclose(fp);
			return FALSE;
		}
	}
	fclose(fp);
	return TRUE;
}


/*
 *  int	save_weights(char *weight_file)
 *
 *  saves a weight file, including bias	values,	to a file. returns
 *  FALSE if there was an error, TRUE otherwise.
 *  this function should not be	called until after load_network() is called.
 *
 */

int save_weights(weight_file)
char *weight_file;
{
	FILE *fp;
	int i;

	if ((fp	= fopen(weight_file, "w")) == NULL)
	{
		return FALSE;
	}

	for (i = 0; i <	nweights; ++i)	       /* save weight values to	file */
	{
		if (fprintf(fp,	"%lf\n", weight	[i]) < 0)
		{
			fclose(fp);
			return FALSE;
		}
	}

	for (i = 0; i <	nunits;	i++)	       /* now save the biases */
	{
		if (fprintf(fp,	"%lf\n", bias [i]) < 0)
		{
			fclose(fp);
			return FALSE;
		}
	}
	fclose(fp);
	return TRUE;
}


/*
 *  void change_weights(void)
 *
 *  adjusts the	weight and bias	values to decrease the overall network error.
 *  once the weights and bias values have been adjusted, the vectors used
 *  in calculating the adjustment amounts are result to	zero, in preparation
 *  for	the next learning cycle.
 *  do not call	this function unless load_network() was	called previously.
 *
 */

void change_weights()
{
    int	i, k, u;
    double lr;

    for	(u = units_in_layer [0]; u < nunits; ++u)
    {
	k = first_weight [u];
	if (fanin)
	{
	    lr = epsilon / (last_to [u]	- first_to [u]);
	} else {
	    lr = epsilon;
	}
	for (i = first_to [u]; i < last_to [u];	++i)
	{
	    weight [k] += (lr *	weight_error_derivative	[k]);
	    weight_error_derivative [k]	= 0.0;
	    ++k;
	}
	bias [u] += (epsilon * bias_error_derivative [u]);
	bias_error_derivative [u] = 0.0;
    }
}

void scale_weights()
{
    int	i, k, u;
    double lr;

    for	(u = units_in_layer [0]; u < nunits; ++u)
    {
	    k = first_weight [u];
	    for (i = first_to [u]; i < last_to [u];	++i)
	    {
	        weight [k] /= (last_to [u] - first_to [u]);
	        ++k;
    	}
    	bias [u] /= (last_to [u] - first_to [u]);
    }
}

