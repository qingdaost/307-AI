/*
 * Note: this program is for tesing the the new pattern file.
 */

#include "backprop.h"

int main(argc, argv)
char **argv;
{
    int i, j;

    if (argc < 3)
    {
	printf("usage: %s network patterns \n", argv[0]);
	return;
    }

    if (!load_network(argv [1]))
    {
        printf("Error on reading %s\n", argv [1]);
        return;
    }
  
    if (!load_patterns(argv [2]))
    {
        printf("Error on reading %s\n", argv [2]);
        return;
    }

    printf("%d\n", ntrain);
    printf("%d\n", ntest);

    for (i = 0; i < ntrain + ntest; ++i)
    {
	for (j = 0; j < units_in_layer [0]; j++)
	   printf("%6.2f ", inputs[i][j]);

	for (j = 0; j < units_in_layer [nlayers - 1]; j++)
	   printf("%6.2f ", targets[i][j]);

	printf("\n");
   }
   
   return 0;
}
