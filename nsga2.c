/* NSGA-II routine (implementation of the 'main' function) */

# include "nsga2.h"
# include "rand.h"

FILE *fpt1;
FILE *fpt2;
FILE *fpt3;
FILE *fpt4;
FILE *fpt5;
FILE *gp;
population *parent_pop;
population *child_pop;
population *mixed_pop;


NSGA2Type ReadParameters(int argc, char **argv){
    int i;
    NSGA2Type nsga2Params;
    
    if (argc<2)
    {
        printf("\n Usage ./nsga2r random_seed \n");
        exit(1);
    }
    nsga2Params.seed = (double)atof(argv[1]);
    if (nsga2Params.seed<=0.0 || nsga2Params.seed>=1.0)
    {
        printf("\n Entered seed value is wrong, seed value must be in (0,1) \n");
        exit(1);
    }
    printf("\n Enter the problem relevant and algorithm relevant parameters ... ");
    printf("\n Enter the population size (a multiple of 4) : ");
    scanf("%d",&nsga2Params.popsize);
    if (nsga2Params.popsize<4 || (nsga2Params.popsize%4)!= 0)
    {
        printf("\n population size read is : %d",nsga2Params.popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of generations : ");
    scanf("%d",&nsga2Params.ngen);
    if (nsga2Params.ngen<1)
    {
        printf("\n number of generations read is : %d",nsga2Params.ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of objectives : ");
    scanf("%d",&nsga2Params.nobj);
    if (nsga2Params.nobj<1)
    {
        printf("\n number of objectives entered is : %d",nsga2Params.nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of constraints : ");
    scanf("%d",&nsga2Params.ncon);
    if (nsga2Params.ncon<0)
    {
        printf("\n number of constraints entered is : %d",nsga2Params.ncon);
        printf("\n Wrong number of constraints enetered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of real variables : ");
    scanf("%d",&nsga2Params.nreal);
    if (nsga2Params.nreal<0)
    {
        printf("\n number of real variables entered is : %d",nsga2Params.nreal);
        printf("\n Wrong number of variables entered, hence exiting \n");
        exit (1);
    }
    if (nsga2Params.nreal != 0)
    {
        nsga2Params.min_realvar = (double *)malloc(nsga2Params.nreal*sizeof(double));
        nsga2Params.max_realvar = (double *)malloc(nsga2Params.nreal*sizeof(double));
        for (i=0; i<nsga2Params.nreal; i++)
        {
            printf ("\n Enter the lower limit of real variable %d : ",i+1);
            scanf ("%lf",&nsga2Params.min_realvar[i]);
            printf ("\n Enter the upper limit of real variable %d : ",i+1);
            scanf ("%lf",&nsga2Params.max_realvar[i]);
            if (nsga2Params.max_realvar[i] <= nsga2Params.min_realvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of real variable, hence exiting \n");
                exit(1);
            }
        }
        printf ("\n Enter the probability of crossover of real variable (0.6-1.0) : ");
        scanf ("%lf",&nsga2Params.pcross_real);
        if (nsga2Params.pcross_real<0.0 || nsga2Params.pcross_real>1.0)
        {
            printf("\n Probability of crossover entered is : %e",nsga2Params.pcross_real);
            printf("\n Entered value of probability of crossover of real variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the probablity of mutation of real variables (1/nsga2Params.nreal) : ");
        scanf ("%lf",&nsga2Params.pmut_real);
        if (nsga2Params.pmut_real<0.0 || nsga2Params.pmut_real>1.0)
        {
            printf("\n Probability of mutation entered is : %e",nsga2Params.pmut_real);
            printf("\n Entered value of probability of mutation of real variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the value of distribution index for crossover (5-20): ");
        scanf ("%lf",&nsga2Params.eta_c);
        if (nsga2Params.eta_c<=0)
        {
            printf("\n The value entered is : %e",nsga2Params.eta_c);
            printf("\n Wrong value of distribution index for crossover entered, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the value of distribution index for mutation (5-50): ");
        scanf ("%lf",&nsga2Params.eta_m);
        if (nsga2Params.eta_m<=0)
        {
            printf("\n The value entered is : %e",nsga2Params.eta_m);
            printf("\n Wrong value of distribution index for mutation entered, hence exiting \n");
            exit (1);
        }
    }
    printf("\n Enter the number of binary variables : ");
    scanf("%d",&nsga2Params.nbin);
    if (nsga2Params.nbin<0)
    {
        printf ("\n number of binary variables entered is : %d",nsga2Params.nbin);
        printf ("\n Wrong number of binary variables entered, hence exiting \n");
        exit(1);
    }
    if (nsga2Params.nbin != 0)
    {
        nsga2Params.nbits = (int *)malloc(nsga2Params.nbin*sizeof(int));
        nsga2Params.min_binvar = (double *)malloc(nsga2Params.nbin*sizeof(double));
        nsga2Params.max_binvar = (double *)malloc(nsga2Params.nbin*sizeof(double));
        for (i=0; i<nsga2Params.nbin; i++)
        {
            printf ("\n Enter the number of bits for binary variable %d : ",i+1);
            scanf ("%d",&nsga2Params.nbits[i]);
            if (nsga2Params.nbits[i] < 1)
            {
                printf("\n Wrong number of bits for binary variable entered, hence exiting");
                exit(1);
            }
            printf ("\n Enter the lower limit of binary variable %d : ",i+1);
            scanf ("%lf",&nsga2Params.min_binvar[i]);
            printf ("\n Enter the upper limit of binary variable %d : ",i+1);
            scanf ("%lf",&nsga2Params.max_binvar[i]);
            if (nsga2Params.max_binvar[i] <= nsga2Params.min_binvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of binary variable entered, hence exiting \n");
                exit(1);
            }
        }
        printf ("\n Enter the probability of crossover of binary variable (0.6-1.0): ");
        scanf ("%lf",&nsga2Params.pcross_bin);
        if (nsga2Params.pcross_bin<0.0 || nsga2Params.pcross_bin>1.0)
        {
            printf("\n Probability of crossover entered is : %e",nsga2Params.pcross_bin);
            printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the probability of mutation of binary variables (1/nsga2Params.nbits): ");
        scanf ("%lf",&nsga2Params.pmut_bin);
        if (nsga2Params.pmut_bin<0.0 || nsga2Params.pmut_bin>1.0)
        {
            printf("\n Probability of mutation entered is : %e",nsga2Params.pmut_bin);
            printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
    }
    if (nsga2Params.nreal==0 && nsga2Params.nbin==0)
    {
        printf("\n Number of real as well as binary variables, both are zero, hence exiting \n");
        exit(1);
    }
    nsga2Params.choice=0;
    printf("\n Do you want to use gnuplot to display the results realtime (0 for NO) (1 for yes) : ");
    scanf("%d",&nsga2Params.choice);
    if (nsga2Params.choice!=0 && nsga2Params.choice!=1)
    {
        printf("\n Entered the wrong nsga2Params.choice, hence exiting, nsga2Params.choice entered was %d\n",nsga2Params.choice);
        exit(1);
    }
    if (nsga2Params.choice==1)
    {
        gp = popen(GNUPLOT_COMMAND,"w");
        if (gp==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }
        if (nsga2Params.nobj==2)
        {
            printf("\n Enter the objective for X axis display : ");
            scanf("%d",&nsga2Params.obj1);
            if (nsga2Params.obj1<1 || nsga2Params.obj1>nsga2Params.nobj)
            {
                printf("\n Wrong value of X objective entered, value entered was %d\n",nsga2Params.obj1);
                exit(1);
            }
            printf("\n Enter the objective for Y axis display : ");
            scanf("%d",&nsga2Params.obj2);
            if (nsga2Params.obj2<1 || nsga2Params.obj2>nsga2Params.nobj)
            {
                printf("\n Wrong value of Y objective entered, value entered was %d\n",nsga2Params.obj2);
                exit(1);
            }
            nsga2Params.obj3 = -1;
        }
        else
        {
            printf("\n #obj > 2, 2D display or a 3D display ?, enter 2 for 2D and 3 for 3D :");
            scanf("%d",&nsga2Params.choice);
            if (nsga2Params.choice!=2 && nsga2Params.choice!=3)
            {
                printf("\n Entered the wrong nsga2Params.choice, hence exiting, nsga2Params.choice entered was %d\n",nsga2Params.choice);
                exit(1);
            }
            if (nsga2Params.choice==2)
            {
                printf("\n Enter the objective for X axis display : ");
                scanf("%d",&nsga2Params.obj1);
                if (nsga2Params.obj1<1 || nsga2Params.obj1>nsga2Params.nobj)
                {
                    printf("\n Wrong value of X objective entered, value entered was %d\n",nsga2Params.obj1);
                    exit(1);
                }
                printf("\n Enter the objective for Y axis display : ");
                scanf("%d",&nsga2Params.obj2);
                if (nsga2Params.obj2<1 || nsga2Params.obj2>nsga2Params.nobj)
                {
                    printf("\n Wrong value of Y objective entered, value entered was %d\n",nsga2Params.obj2);
                    exit(1);
                }
                nsga2Params.obj3 = -1;
            }
            else
            {
                printf("\n Enter the objective for X axis display : ");
                scanf("%d",&nsga2Params.obj1);
                if (nsga2Params.obj1<1 || nsga2Params.obj1>nsga2Params.nobj)
                {
                    printf("\n Wrong value of X objective entered, value entered was %d\n",nsga2Params.obj1);
                    exit(1);
                }
                printf("\n Enter the objective for Y axis display : ");
                scanf("%d",&nsga2Params.obj2);
                if (nsga2Params.obj2<1 || nsga2Params.obj2>nsga2Params.nobj)
                {
                    printf("\n Wrong value of Y objective entered, value entered was %d\n",nsga2Params.obj2);
                    exit(1);
                }
                printf("\n Enter the objective for Z axis display : ");
                scanf("%d",&nsga2Params.obj3);
                if (nsga2Params.obj3<1 || nsga2Params.obj3>nsga2Params.nobj)
                {
                    printf("\n Wrong value of Z objective entered, value entered was %d\n",nsga2Params.obj3);
                    exit(1);
                }
                printf("\n You have chosen 3D display, hence location of eye required \n");
                printf("\n Enter the first angle (an integer in the range 0-180) (if not known, enter 60) :");
                scanf("%d",&nsga2Params.angle1);
                if (nsga2Params.angle1<0 || nsga2Params.angle1>180)
                {
                    printf("\n Wrong value for first angle entered, hence exiting \n");
                    exit(1);
                }
                printf("\n Enter the second angle (an integer in the range 0-360) (if not known, enter 30) :");
                scanf("%d",&nsga2Params.angle2);
                if (nsga2Params.angle2<0 || nsga2Params.angle2>360)
                {
                    printf("\n Wrong value for second angle entered, hence exiting \n");
                    exit(1);
                }
            }
        }
    }
    return nsga2Params;
}



void InitNSGA2(NSGA2Type *nsga2Params)
{
    int i;
    
    printf("\n == InitNSGA2 ==");
    
    // Initialize the files...
    fpt1 = fopen("initial_pop.out","w");
    fpt2 = fopen("final_pop.out","w");
    fpt3 = fopen("best_pop.out","w");
    fpt4 = fopen("all_pop.out","w");
    fpt5 = fopen("params.out","w");
    fprintf(fpt1,"# This file contains the data of initial population\n");
    fprintf(fpt2,"# This file contains the data of final population\n");
    fprintf(fpt3,"# This file contains the data of final feasible population (if found)\n");
    fprintf(fpt4,"# This file contains the data of all generations\n");
    fprintf(fpt5,"# This file contains information about inputs as read by the program\n");
    
    fprintf(fpt5,"\n Population size = %d",nsga2Params->popsize);
    fprintf(fpt5,"\n Number of generations = %d",nsga2Params->ngen);
    fprintf(fpt5,"\n Number of objective functions = %d",nsga2Params->nobj);
    fprintf(fpt5,"\n Number of constraints = %d",nsga2Params->ncon);
    fprintf(fpt5,"\n Number of real variables = %d",nsga2Params->nreal);
    if (nsga2Params->nreal!=0)
    {
        for (i=0; i<nsga2Params->nreal; i++)
        {
            fprintf(fpt5,"\n Lower limit of real variable %d = %e",i+1,nsga2Params->min_realvar[i]);
            fprintf(fpt5,"\n Upper limit of real variable %d = %e",i+1,nsga2Params->max_realvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of real variable = %e",nsga2Params->pcross_real);
        fprintf(fpt5,"\n Probability of mutation of real variable = %e",nsga2Params->pmut_real);
        fprintf(fpt5,"\n Distribution index for crossover = %e",nsga2Params->eta_c);
        fprintf(fpt5,"\n Distribution index for mutation = %e",nsga2Params->eta_m);
    }
    fprintf(fpt5,"\n Number of binary variables = %d",nsga2Params->nbin);
    if (nsga2Params->nbin!=0)
    {
        for (i=0; i<nsga2Params->nbin; i++)
        {
            fprintf(fpt5,"\n Number of bits for binary variable %d = %d",i+1,nsga2Params->nbits[i]);
            fprintf(fpt5,"\n Lower limit of binary variable %d = %e",i+1,nsga2Params->min_binvar[i]);
            fprintf(fpt5,"\n Upper limit of binary variable %d = %e",i+1,nsga2Params->max_binvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of binary variable = %e",nsga2Params->pcross_bin);
        fprintf(fpt5,"\n Probability of mutation of binary variable = %e",nsga2Params->pmut_bin);
    }
    fprintf(fpt5,"\n Seed for random number generator = %e",nsga2Params->seed);
    nsga2Params->bitlength = 0;
    if (nsga2Params->nbin!=0)
    {
        for (i=0; i<nsga2Params->nbin; i++)
        {
            nsga2Params->bitlength += nsga2Params->nbits[i];
        }
    }
    fprintf(fpt1,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nsga2Params->nobj,nsga2Params->ncon,nsga2Params->nreal,nsga2Params->bitlength);
    fprintf(fpt2,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nsga2Params->nobj,nsga2Params->ncon,nsga2Params->nreal,nsga2Params->bitlength);
    fprintf(fpt3,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nsga2Params->nobj,nsga2Params->ncon,nsga2Params->nreal,nsga2Params->bitlength);
    fprintf(fpt4,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nsga2Params->nobj,nsga2Params->ncon,nsga2Params->nreal,nsga2Params->bitlength);
    nsga2Params->nbinmut = 0;
    nsga2Params->nrealmut = 0;
    nsga2Params->nbincross = 0;
    nsga2Params->nrealcross = 0;
    
    // Initializing the populations
    parent_pop = (population *)malloc(sizeof(population));
    child_pop = (population *)malloc(sizeof(population));
    mixed_pop = (population *)malloc(sizeof(population));
    allocate_memory_pop (nsga2Params, parent_pop, nsga2Params->popsize);
    allocate_memory_pop (nsga2Params, child_pop, nsga2Params->popsize);
    allocate_memory_pop (nsga2Params, mixed_pop, 2*nsga2Params->popsize);
    
    // Preparing first Population
    randomize(nsga2Params->seed);
    initialize_pop (nsga2Params,  parent_pop);
    printf("\n Initialization done, now performing first generation");
    decode_pop(nsga2Params, parent_pop);
    evaluate_pop (nsga2Params, parent_pop);
    assign_rank_and_crowding_distance (nsga2Params, parent_pop);
    report_pop (nsga2Params, parent_pop, fpt1);
    fprintf(fpt4,"# gen = 1\n");
    report_pop(nsga2Params, parent_pop,fpt4);
    printf("\n -- Generation -- 1");
    
    fflush(stdout);
    if (nsga2Params->choice!=0)    onthefly_display (nsga2Params, parent_pop,gp,1);
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
}




int NSGA2(NSGA2Type *nsga2Params)
{
    int i;
    
    for (i=2; i<=nsga2Params->ngen; i++)
    {
        selection (nsga2Params,  parent_pop, child_pop);
        mutation_pop (nsga2Params,  child_pop);
        decode_pop(nsga2Params,  child_pop);
        evaluate_pop(nsga2Params,  child_pop);
        merge (nsga2Params,  parent_pop, child_pop, mixed_pop);
        fill_nondominated_sort (nsga2Params,  mixed_pop, parent_pop);
        /* Comment following four lines if information for all
         generations is not desired, it will speed up the execution */
        fprintf(fpt4,"# gen = %d\n",i);
        report_pop(nsga2Params, parent_pop,fpt4);
        fflush(fpt4);
        if (nsga2Params->choice!=0)    onthefly_display (nsga2Params, parent_pop,gp,i);
        printf("\n -- Generation %d --", i);
    }
    printf("\n Generations finished, now reporting solutions");
    
    report_pop(nsga2Params,  parent_pop,fpt2);
    report_feasible(nsga2Params,  parent_pop,fpt3);
    
    if (nsga2Params->nreal!=0)
    {
        fprintf(fpt5,"\n Number of crossover of real variable = %d",nsga2Params->nrealcross);
        fprintf(fpt5,"\n Number of mutation of real variable = %d",nsga2Params->nrealmut);
    }
    if (nsga2Params->nbin!=0)
    {
        fprintf(fpt5,"\n Number of crossover of binary variable = %d",nsga2Params->nbincross);
        fprintf(fpt5,"\n Number of mutation of binary variable = %d",nsga2Params->nbinmut);
    }
    
    // Closing the files and freeing up memories...
    fflush(stdout);
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    fclose(fpt1);
    fclose(fpt2);
    fclose(fpt3);
    fclose(fpt4);
    fclose(fpt5);
    if (nsga2Params->choice!=0)
    {
        pclose(gp);
    }
    if (nsga2Params->nreal!=0)
    {
        free (nsga2Params->min_realvar);
        free (nsga2Params->max_realvar);
    }
    if (nsga2Params->nbin!=0)
    {
        free (nsga2Params->min_binvar);
        free (nsga2Params->max_binvar);
        free (nsga2Params->nbits);
    }
    deallocate_memory_pop (nsga2Params,  parent_pop, nsga2Params->popsize);
    deallocate_memory_pop (nsga2Params,  child_pop, nsga2Params->popsize);
    deallocate_memory_pop (nsga2Params,  mixed_pop, 2*nsga2Params->popsize);
    free (parent_pop);
    free (child_pop);
    free (mixed_pop);
    printf("\n Routine successfully exited \n");
    return (0);
}


void print_nsga2Params(NSGA2Type *nsga2Params){
    int i;
    
    printf("NSGA2 Parameters:\n");
    printf("\n seed number is : %lf", nsga2Params->seed);
    printf("\n population size read is : %d",nsga2Params->popsize);
    printf("\n number of generations read is : %d",nsga2Params->ngen);
    printf("\n number of objectives entered is : %d",nsga2Params->nobj);
    printf("\n number of constraints entered is : %d",nsga2Params->ncon);
    printf("\n number of real variables entered is : %d",nsga2Params->nreal);
    printf("\n variables bounds: ");
    for (i=0; i<nsga2Params->nreal; i++){
        printf("[%lf", nsga2Params->min_realvar[i]);
        printf(" %lf], ", nsga2Params->max_realvar[i]);
    }
    printf("\n Probability of crossover entered is : %e",nsga2Params->pcross_real);
    printf("\n Probability of mutation entered is : %e",nsga2Params->pmut_real);
    printf("\n The eta_c entered is : %e",nsga2Params->eta_c);
    printf("\n The eta_m entered is : %e",nsga2Params->eta_m);
    if (nsga2Params->nbin != 0){
        printf ("\n number of binary variables entered is : %d",nsga2Params->nbin);
        printf("\n Probability of crossover entered is : %e",nsga2Params->pcross_bin);
        printf("\n Probability of mutation entered is : %e",nsga2Params->pmut_bin);
    }
}




