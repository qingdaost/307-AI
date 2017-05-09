/*
 *  backprop.h -- header file for N layer back propagation neural network
 *
 *    author: Steven Hayes
 *     email: steveh@rmit.edu.au
 *
 *   version: 2.0
 *	date: 16/04/93
 *
 *  related files:   backprop.c
 *		     network.c
 *		     globals.c
 *		     weights.c
 *		     patterns.c
 */

/* standard include files required  */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>      
#include <fcntl.h>
#include <time.h>
#include <math.h>
/*
typedef unsigned char byte;
*/
#define	TRUE 1
#define	FALSE 0

#define	DEBUG

/*
 *	system specific prototypes
 */

double drand48();

/*
 *  external global variables which store network values
 */

extern double *weight;	     /*	the weight vector  */
extern double *bias;	     /*	a vector of bias values	*/
extern double *netinput;     /*	vector used during calculations	*/
extern double *activate;     /*	input or activation values */

extern double   **inputs;	     /*	vector of network input	vectors	*/
extern double   **targets;     /*	vector of network output vectors  */

extern double *delta;	     /* change of weights vector */
extern double *error;	     /*	error of each output unit vector */

extern double *dweight;	     /*	store momentum values */
extern double *dbias;	     /* store momentum values */

extern double *weight_error_derivative;	     /*	weight error derivative	vector */
extern double *bias_error_derivative;	     /*	bias error derivative vector */

/*
 *  global variables which determine network structure
 */

extern int *first_to;		/* stores first	connecting unit	*/
extern int *last_to;		/*/ stores last	connecting unit	*/
extern int *first_weight;	/* stores the index of the first weight	*/
extern int *units_in_layer;	/* number of units in each layer */

/*
 *  global variables which store sizes of vectors
 */

extern int nlayers;		/* number of layers in the network */
extern int nunits;		/* number of units in the network */
extern int nweights;		/* number of weights in	the network */
extern int ntrain;		/* number of training pattern pairs */
extern int ntest;		/* number of test pattern pairs	 */

/*
 *  global variables which store network learning parameters
 */

extern double momentum;		 /* momentum value */
extern double epsilon;		 /* contains the learning rate */
extern double range;		 /* contains range for weight initialization */
extern double ecrit;		 /* minimum acceptable error cutoff */
extern double corrpercent;       /* maximum correct percentage of training */

/*
 *  global variables which store options
 */

extern int fanin;		/* true	if learning rate is scaled by fan in */
extern int verbose;		/* frequency of	progress reports */
extern int vverbose;		/* show	network	outputs	in report if true */

/*
 *  file name variables
 */

extern char network_file [255];	/* stores the name of the network description */
extern char weight_file	 [255];	/* stores the name of the weights file */
extern char pattern_file [255];	/* stores the name of the pattern file */

/*
 *  function prototypes
 */

int create_network();		    /* in network.c */
void destroy_network();
int load_network();
int save_network();
void reset_network();

int load_weights();   /* in weights.c */
int save_weights();
void change_weights();
void scale_weights();

int load_patterns();  /* in patterns.c	*/
void destroy_patterns();

void compute_output();	     /*	in backprop.c */
double compute_error();
void compute_error_derivative();
double logistic();
