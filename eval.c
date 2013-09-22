/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "nsga2.h"
# include "rand.h"

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (NSGA2Type *nsga2Params, population *pop)
{
    int i;
    for (i=0; i<nsga2Params->popsize; i++)
    {
        evaluate_ind (nsga2Params, &(pop->ind[i]));
    }
    return;
}

/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind (NSGA2Type *nsga2Params, individual *ind)
{
    int j;
    test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);
    
    if (nsga2Params->ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<nsga2Params->ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                ind->constr_violation += ind->constr[j];
            }
        }
    }
    return;
}
