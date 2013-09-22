/* Mutation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "nsga2.h"
# include "rand.h"

/* Function to perform mutation in a population */
void mutation_pop (NSGA2Type *nsga2Params,  population *pop)
{
    int i;
    for (i=0; i<nsga2Params->popsize; i++)
    {
        mutation_ind(nsga2Params, &(pop->ind[i]));
    }
    return;
}

/* Function to perform mutation of an individual */
void mutation_ind (NSGA2Type *nsga2Params,  individual *ind)
{
    if (nsga2Params->nreal!=0)
    {
        real_mutate_ind(nsga2Params, ind);
    }
    if (nsga2Params->nbin!=0)
    {
        bin_mutate_ind(nsga2Params, ind);
    }
    return;
}

/* Routine for binary mutation of an individual */
void bin_mutate_ind (NSGA2Type *nsga2Params,  individual *ind)
{
    int j, k;
    double prob;
    for (j=0; j<nsga2Params->nbin; j++)
    {
        for (k=0; k<nsga2Params->nbits[j]; k++)
        {
            prob = randomperc();
            if (prob <=nsga2Params->pmut_bin)
            {
                if (ind->gene[j][k] == 0)
                {
                    ind->gene[j][k] = 1;
                }
                else
                {
                    ind->gene[j][k] = 0;
                }
                nsga2Params->nbinmut+=1;
            }
        }
    }
    return;
}

/* Routine for real polynomial mutation of an individual */
void real_mutate_ind (NSGA2Type *nsga2Params,  individual *ind)
{
    int j;
    double rnd, delta1, delta2, mut_pow, deltaq;
    double y, yl, yu, val, xy;
    for (j=0; j<nsga2Params->nreal; j++)
    {
        if (randomperc() <= nsga2Params->pmut_real)
        {
            y = ind->xreal[j];
            yl = nsga2Params->min_realvar[j];
            yu = nsga2Params->max_realvar[j];
            delta1 = (y-yl)/(yu-yl);
            delta2 = (yu-y)/(yu-yl);
            rnd = randomperc();
            mut_pow = 1.0/(nsga2Params->eta_m+1.0);
            if (rnd <= 0.5)
            {
                xy = 1.0-delta1;
                val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(nsga2Params->eta_m+1.0)));
                deltaq =  pow(val,mut_pow) - 1.0;
            }
            else
            {
                xy = 1.0-delta2;
                val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(nsga2Params->eta_m+1.0)));
                deltaq = 1.0 - (pow(val,mut_pow));
            }
            y = y + deltaq*(yu-yl);
            if (y<yl)
                y = yl;
            if (y>yu)
                y = yu;
            ind->xreal[j] = y;
            nsga2Params->nrealmut+=1;
        }
    }
    return;
}
