/* Routines to display the population information using gnuplot */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <unistd.h>

# include "nsga2.h"
# include "rand.h"

/* Function to display the current population for the subsequent generation */
void onthefly_display (NSGA2Type *nsga2Params, population *pop, FILE *gp, int ii)
{
    int i;
    int flag;
    FILE *fpt;
    fpt = fopen("plot.out","w");
    flag = 0;
    for (i=0; i<nsga2Params->popsize; i++)
    {
        if (pop->ind[i].constr_violation==0)
        {
            if (nsga2Params->choice!=3)
                fprintf(fpt,"%e\t%e\n",pop->ind[i].obj[nsga2Params->obj1-1],pop->ind[i].obj[nsga2Params->obj2-1]);
            else
                fprintf(fpt,"%e\t%e\t%e\n",pop->ind[i].obj[nsga2Params->obj1-1],pop->ind[i].obj[nsga2Params->obj2-1],pop->ind[i].obj[nsga2Params->obj3-1]);
            fflush(fpt);
            flag = 1;
        }
    }
    if (flag==0)
    {
        printf("\n No feasible soln in this pop, hence no display");
    }
    else
    {
        if (nsga2Params->choice!=3)
            fprintf(gp,"set title 'Generation #%d'\n unset key\n plot 'plot.out' w points pointtype 6 pointsize 1\n",ii);
        else
            fprintf(gp,"set title 'Generation #%d'\n set view %d,%d\n unset key\n splot 'plot.out' w points pointtype 6 pointsize 1\n",ii,nsga2Params->angle1,nsga2Params->angle2);
        fflush(gp);
    }
    fclose(fpt);
    return;
}
