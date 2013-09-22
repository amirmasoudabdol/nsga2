/* Routines to decode the population */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "nsga2.h"
# include "rand.h"

/* Function to decode a population to find out the binary variable values based on its bit pattern */
void decode_pop (NSGA2Type *nsga2Params, population *pop)
{
    int i;
    if (nsga2Params->nbin!=0)
    {
        for (i=0; i<nsga2Params->popsize; i++)
        {
            decode_ind (nsga2Params, &(pop->ind[i]));
        }
    }
    return;
}

/* Function to decode an individual to find out the binary variable values based on its bit pattern */
void decode_ind (NSGA2Type *nsga2Params, individual *ind)
{
    int j, k;
    int sum;
    if (nsga2Params->nbin!=0)
    {
        for (j=0; j<nsga2Params->nbin; j++)
        {
            sum=0;
            for (k=0; k<nsga2Params->nbits[j]; k++)
            {
                if (ind->gene[j][k]==1)
                {
                    sum += pow(2,nsga2Params->nbits[j]-1-k);
                }
            }
            ind->xbin[j] = nsga2Params->min_binvar[j] + (double)sum*(nsga2Params->max_binvar[j] - nsga2Params->min_binvar[j])/(double)(pow(2,nsga2Params->nbits[j])-1);
        }
    }
    return;
}
