
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pmmintrin.h>

int main(int argc, char * const argv[])
{
	int c, size, steps, hebras, file_exit, iteration_exit;
	opterr = 0;
	while ((c = getopt (argc, argv, "i:o:N:d:")) != -1)
		switch (c)
		{
			case 'N':
				size = atoi(optarg);
				break;
			case 'T':
				steps = atoi(optarg);
				break;
			case 'H':
				hebras = atoi(optarg);
				break;
			case 'f':
				file_exit = optarg;
				break;
			case 't':
				iteration_exit = atoi(optarg);
				break;
			case '?':
				if (optopt == 'N' ||optopt == 'T' ||optopt == 'H' ||optopt == 'f' ||optopt == 't')
					fprintf (stderr, "La opcion -%c  debe ir acompañada de un argumento.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "No se reconoce opcion `-%c'.\n", optopt);
				else
					fprintf (stderr, "No se reconoce opcion `\\x%x'.\n",optopt);
				return 1;
			default:
				abort ();
		}
	return 0;
}
