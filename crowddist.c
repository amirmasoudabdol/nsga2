/* Crowding distance computation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "nsga2.h"
# include "rand.h"

/* Routine to compute crowding distance based on ojbective function values when the population in in the form of a list */
void assign_crowding_distance_list (NSGA2Type *nsga2Params,  population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index].crowd_dist = INF;
        pop->ind[lst->child->index].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nsga2Params->nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nsga2Params->nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    assign_crowding_distance (nsga2Params, pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nsga2Params->nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

/* Routine to compute crowding distance based on objective function values when the population in in the form of an array */
void assign_crowding_distance_indices (NSGA2Type *nsga2Params, population *pop, int c1, int c2)
{
    int **obj_array;
    int *dist;
    int i, j;
    int front_size;
    front_size = c2-c1+1;
    if (front_size==1)
    {
        pop->ind[c1].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[c1].crowd_dist = INF;
        pop->ind[c2].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nsga2Params->nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nsga2Params->nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = c1++;
    }
    assign_crowding_distance(nsga2Params,  pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nsga2Params->nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

/* Routine to compute crowding distances */
void assign_crowding_distance (NSGA2Type *nsga2Params, population *pop, int *dist, int **obj_array, int front_size)
{
    int i, j;
    for (i=0; i<nsga2Params->nobj; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_obj (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]].crowd_dist = 0.0;
    }
    for (i=0; i<nsga2Params->nobj; i++)
    {
        pop->ind[obj_array[i][0]].crowd_dist = INF;
    }
    for (i=0; i<nsga2Params->nobj; i++)
    {
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]].crowd_dist != INF)
            {
                if (pop->ind[obj_array[i][front_size-1]].obj[i] == pop->ind[obj_array[i][0]].obj[i])
                {
                    pop->ind[obj_array[i][j]].crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]].crowd_dist += (pop->ind[obj_array[i][j+1]].obj[i] - pop->ind[obj_array[i][j-1]].obj[i])/(pop->ind[obj_array[i][front_size-1]].obj[i] - pop->ind[obj_array[i][0]].obj[i]);
                }
            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]].crowd_dist != INF)
        {
            pop->ind[dist[j]].crowd_dist = (pop->ind[dist[j]].crowd_dist)/nsga2Params->nobj;
        }
    }
    return;
}
