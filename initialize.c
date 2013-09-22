/* Data initializtion routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "nsga2.h"
# include "rand.h"

/* Function to initialize a population randomly */
void initialize_pop (NSGA2Type *nsga2Params, population *pop)
{
    int i;
    for (i=0; i<nsga2Params->popsize; i++)
    {
        initialize_ind (nsga2Params, &(pop->ind[i]));
    }
    return;
}

/* Function to initialize an individual randomly */
void initialize_ind (NSGA2Type *nsga2Params, individual *ind)
{
    int j, k;
    if (nsga2Params->nreal!=0)
    {
        for (j=0; j<nsga2Params->nreal; j++)
        {
            ind->xreal[j] = rndreal (nsga2Params->min_realvar[j], nsga2Params->max_realvar[j]);
        }
    }
    if (nsga2Params->nbin!=0)
    {
        for (j=0; j<nsga2Params->nbin; j++)
        {
            for (k=0; k<nsga2Params->nbits[j]; k++)
            {
                if (randomperc() <= 0.5)
                {
                    ind->gene[j][k] = 0;
                }
                else
                {
                    ind->gene[j][k] = 1;
                }
            }
        }
    }
    return;
}
