/* Memory allocation and deallocation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "nsga2.h"
# include "rand.h"

/* Function to allocate memory to a population */
void allocate_memory_pop (NSGA2Type *nsga2Params,  population *pop, int size)
{
    int i;
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (nsga2Params, &(pop->ind[i]));
    }
    return;
}

/* Function to allocate memory to an individual */
void allocate_memory_ind (NSGA2Type *nsga2Params,  individual *ind)
{
    int j;
    if (nsga2Params->nreal != 0)
    {
        ind->xreal = (double *)malloc(nsga2Params->nreal*sizeof(double));
    }
    if (nsga2Params->nbin != 0)
    {
        ind->xbin = (double *)malloc(nsga2Params->nbin*sizeof(double));
        ind->gene = (int **)malloc(nsga2Params->nbin*sizeof(int *));
        for (j=0; j<nsga2Params->nbin; j++)
        {
            ind->gene[j] = (int *)malloc(nsga2Params->nbits[j]*sizeof(int));
        }
    }
    ind->obj = (double *)malloc(nsga2Params->nobj*sizeof(double));
    if (nsga2Params->ncon != 0)
    {
        ind->constr = (double *)malloc(nsga2Params->ncon*sizeof(double));
    }
    return;
}

/* Function to deallocate memory to a population */
void deallocate_memory_pop (NSGA2Type *nsga2Params, population *pop, int size)
{
    int i;
    for (i=0; i<size; i++)
    {
        deallocate_memory_ind (nsga2Params, &(pop->ind[i]));
    }
    free (pop->ind);
    return;
}

/* Function to deallocate memory to an individual */
void deallocate_memory_ind (NSGA2Type *nsga2Params, individual *ind)
{
    int j;
    if (nsga2Params->nreal != 0)
    {
        free(ind->xreal);
    }
    if (nsga2Params->nbin != 0)
    {
        for (j=0; j<nsga2Params->nbin; j++)
        {
            free(ind->gene[j]);
        }
        free(ind->xbin);
        free(ind->gene);
    }
    free(ind->obj);
    if (nsga2Params->ncon != 0)
    {
        free(ind->constr);
    }
    return;
}
