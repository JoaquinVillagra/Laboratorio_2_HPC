#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pmmintrin.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define K2 0.0025
#define K1 0.00125

void imprimir(float** matriz, int N){
	int i,j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%.01f ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("------------------------\n");
}

float** crear_Matriz(int N){
	int i,j;
	float** matriz = (float**) malloc(sizeof(float*)*N);
	for (i = 0; i < N; i++)
	{
		matriz[i] = (float*) malloc(sizeof(float)*N);
		for (j = 0; j < N; j++)
		{
			matriz[i][j] = 0;
		}
	}
	//imprimir(matriz, N);
	return matriz;
}

int main(int argc, char * const argv[])
{
	int i, j, c, dim, steps, hebras, iteration_exit, index;
	char* file_exit = NULL;
	opterr = 0;
	while ((c = getopt (argc, argv, "N:T:H:f:t:")) != -1)
	switch (c)
	{
		case 'N':
			dim = atoi(optarg);
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
	double timestart = omp_get_wtime();
	//Definiendo matrices necesarias
	float **H = crear_Matriz(dim);
	float **H_t1 = crear_Matriz(dim);
	float **H_t2 = crear_Matriz(dim);
	float **H_aux;
	#pragma omp parallel num_threads(hebras)
	{
		#pragma omp for schedule(dynamic, 4)
		for (i = 1; i < dim; i++)
		{
			for (j = 1; j < dim; j++)
			{
				if(i>0.4*dim && i<0.6*dim && j>0.4*dim && j<0.6*dim)
	           		H_t1[i][j] = 20.0;
			}	
		}
	}
	//Operando iteraciones de trabajo
	for (index = 1; index < steps; index++)
	{
		#pragma omp parallel num_threads(hebras)
		{
			#pragma omp for schedule(dynamic, 4)
			for (i = 1; i < dim-1; i++)
			{
				for (j = 1; j < dim-1; j++)
				{
					if (index == 1)
					{	
						H[i][j] = H_t1[i][j] + K1 * (H_t1[i+1][j] + H_t1[i-1][j] + H_t1[i][j-1] + H_t1[i][j+1] - 4*H_t1[i][j]);
					}
					else
					{	
						H[i][j] = 2*H_t1[i][j] - H_t2[i][j] + K2 * (H_t1[i+1][j] + H_t1[i-1][j] + H_t1[i][j-1] + H_t1[i][j+1] - 4*H_t1[i][j]);	
					}
				}
			}
		}
		H_aux = H_t2;
		H_t2 = H_t1;
		H_t1 = H;
		H = H_aux;
		//memcpy(H_t2,H_t1, dim*dim*sizeof(float))
    	//Comprobando iteración de salida
    	if(iteration_exit==index)
    	{
    		FILE *salida = fopen(file_exit,"w+b");
			fwrite(H, sizeof(float), dim*dim, salida);
			fclose(salida);
    	}	
	}
	double timefinish = omp_get_wtime();
	printf("%f\n",timefinish-timestart);
	return 0;
}
