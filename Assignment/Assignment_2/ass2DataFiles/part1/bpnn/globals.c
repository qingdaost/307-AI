/*
 *  globals.c -- global	variabkes for N	layer back propagation neural network
 *
 *    author: Steven Hayes
 *     email: steveh@rmit.edu.au
 *
 *   version: 3.0
 *	date: 06/06/95
 *
 *  related files:   backprop.h
 *		     backprop.c
 *		     network.c
 *		     weights.c
 *		     patterns.c
 */

/*	mengjie 05/09/00
typedef unsigned char byte;
*/

/* global variables which store	network	values */

double *weight;		 /* the	weight vector */
double *bias;		 /* a vector of	bias values */
double *netinput;	 /* vector used	during calculations */
double *activate;	 /* input or activation	values */

double **inputs;	 /* vector of network input vectors */
double **targets;	 /* vector of network output vectors  */

double *delta;		 /* change of weights vector */
double *error;		 /* error of each output unit vector */
double *dweight;	 /* use	to hold	previous momentum values */
double *dbias;		 /* use	to hold	previous momentum values  */

double *weight_error_derivative;    /* weight error derivative vector */
double *bias_error_derivative;	    /* bias error derivative vector */

double momentum;	 /* momentum value */
double epsilon;		 /* contains the learning rate */
double range;		 /* contains range for weight initialization */
double ecrit;		 /* minimum acceptable error cutoff */
double corrpercent;      /* maximum correct percentage of training */

/* global variables which determine network structure */

int *first_to;		/* stores first	connecting unit	*/
int *last_to;		/* stores last connecting unit */
int *first_weight;	/* stores the index of the first weight	*/
int *units_in_layer;	/* number of units in each layer */

/* global variables which store	sizes of vectors */

int nlayers;		/* number of layers in the network */
int nunits;		/* number of units in the network */
int nweights;		/* number of weights in	the network */
int ntrain;		/* number of training pattern pairs */
int ntest;		/* number of test pattern pairs	 */

/* global variables which store	options	*/

int fanin;		/* true	if learning rate is scaled by fan in */
int verbose;		/* frequency of	progress reports */
int vverbose;		/* show	network	outputs	in report if true */

/* file	name variables */

char network_file [255];    /* stores the name of the network description */
char weight_file  [255];    /* stores the name of the weights file */
char pattern_file [255];    /* stores the name of the pattern file */
