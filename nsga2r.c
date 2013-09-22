# include "rand.h"
# include "nsga2.h"

int main(int argc, char **argv)
{
	/* code */
	NSGA2Type nsga2Params;
	nsga2Params = ReadParameters(argc, argv);
		InitNSGA2(&nsga2Params);
		NSGA2(&nsga2Params);

	return 0;
}