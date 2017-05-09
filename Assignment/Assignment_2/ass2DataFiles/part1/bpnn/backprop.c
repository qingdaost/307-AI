/*
 *  backprop.c -- N layer back propagation neural network source code
 *
 *    author: Steven Hayes
 *     email: steveh@rmit.edu.au
 *
 *   version: 3.0
 *    date: 06/06/95
 *
 *  related files:   backprop.h
 *             network.c
 *             globals.c
 *             weights.c
 *             patterns.c
 *
 */

#include "backprop.h"


/*
 *  double logistic(double)
 *
 *  calculates the transfer value for a units output. the current function
 *  is a monotonicly decreasing sigmoid, symetric about (0,0).
 *  literature indicates that a symetric sigmoid about (0,0) with bounds
 *  between -1.0 an 1.0 leads to faster network convergence in many cases.
 *  for efficiency, the function should be coded as a look-up table.
 *
 */

double logistic(x)      /* the logistic function */
double x;
{
    return (1.0 / (1.0 + exp(-x)));
}


/*
 *  void compute_output(int pattern)
 *
 *  computes the output of the network from the input unit activations
 *  stored in the inputs [pattern] vector. the networks output is stored
 *  in the activate [] vector entries which correspond to the output units.
 *  do not call this function unless load_network() was called previously.
 *
 */

void compute_output(pattern)     /* computes the output of the network */
int pattern;                     /* given some input values */
{
    int i, k, u;
    double *in;

    in = inputs [pattern];

    for (u = units_in_layer [0]; u < nunits; ++u)    /* u is a unit */
    {
        netinput [u] = bias [u];
        k = first_weight [u];
        for (i = first_to [u]; i < last_to [u]; i++)
        {
            netinput [u] += ((i < units_in_layer [0]) ? (in [i]) : activate [i])
                * weight [k];
            ++k;
        }
        activate [u] = logistic(netinput [u]);
    }
}


/*
 *  double compute_error(int pattern)
 *
 *  computes the error in a networks weights and bias values after the
 *  compute_output() function has been called. first the error in the
 *  results of the output units is calculated, by comparing the generated
 *  values to the actual values required in the targets [pattern] vector. then
 *  the error values are back-propagated through the hidden units, where
 *  further error estimation occures.
 *  the function returns the squared error for the current pattern.
 *  do not call this function unless load_network() was called previously.
 *
 */

double compute_error(pattern)
int pattern;
{
    int i, k, u;
    double *out;
    double p;

    out = targets [pattern];

    for (u = 0; u < nunits; ++u)
    {
        error [u] = 0.0;
    }

    /* calculate error for output units  */

    i = 0;
    p = 0.0;
    for (u = nunits - units_in_layer [nlayers - 1]; u < nunits; ++u)
    {
        error [u] = (out [i]) - activate [u];
        p += error [u] * error [u];
        ++i;
    }

    /* calculate error for hidden units using error back-propagation */

    for (u = nunits - 1; u >= units_in_layer [0]; --u)
    {
        delta [u] = error [u] * (activate [u] * (1.0 - activate [u]) + 0.1);
        k = first_weight [u];
        for (i = first_to [u]; i < last_to [u]; ++i)
        {
            error [i] += delta [u] * weight [k];
            ++k;
        }
    }
    return p;    /* return the squared error for the pattern */
}


/*
 *  void compute_error_derivative(int pattern)
 *
 *  computes the error derivative for each weight and bias value with respect
 *  to a pattern network. weight error derivatives are stored in the
 *  weight_error_derivative [] vector and bias error derivatives are stored
 *  in the bias_error_derivative [] vector. these derivatives are later used in
 *  adjusting the networks bias and weight values.
 *  do not call this function unless load_network() was called previously.
 *
 */

void compute_error_derivative(pattern)
int pattern;
{
    int i, k, u;
    double *in;

    in = inputs [pattern];

    for (u = units_in_layer [0]; u < nunits; ++u)
    {
        k = first_weight [u];
        for (i = first_to [u]; i < last_to [u]; ++i)
        {
            weight_error_derivative [k] += delta [u] *
                ((i < units_in_layer [0]) ? (in[i]) : activate [i]);
            ++k;
        }
        bias_error_derivative [u] += delta [u];
    }
}

