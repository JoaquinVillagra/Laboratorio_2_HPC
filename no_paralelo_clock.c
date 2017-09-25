#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pmmintrin.h>
#include <sys/times.h>
#include <time.h>
#define C 1.0
#define DT 0.1
#define DD 2.0 

clock_t timestart, timeend;

int main(int argc, char * const argv[])
{
	timestart = clock(); // registramos el tiempo hasta el momento
	int K1 = pow(C, 2)/2*pow(DT/DD, 2);
	int K2 = pow(C, 2)*pow(DT/DD, 2);
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
	//Definiendo matrices necesarias
	float H_t2[dim][dim], H_t1[dim][dim], H[dim][dim];
	//Asignando ceros como valor por defecto de las matrices generadas
	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			H[i][j] = 0;
			H_t1[i][j] = 0;
			H_t2[i][j] = 0;
		}	
	}
	//Asignando configuración inicial de onda
	for (i = 1; i < dim; i++)
	{
		for (j = 1; j < dim; j++)
		{
			if(i>0.4*dim && i<0.6*dim && j>0.4*dim && j<0.6*dim)
           		H_t1[i][j] = 20.0;
		}	
	}
	//Operando iteraciones de trabajo
	for (index = 0; index < steps; index++)
	{
		for (i = 1; i < dim; i++)
		{	
			for (j = 1; j < dim; j++)
			{
				if (index == 1)
				{
					H[i][j] = 2*H_t1[i][j] + K1 * (H_t1[i+1][j] + H_t1[i-1][j] + H_t1[i][j-1] + H_t1[i][j+1] - 4*H_t1[i][j]);
				}
				else
				{
					if (index==2)
					{
						memcpy(H_t2,H, dim*dim*sizeof(float));
                    	H_t1[i][j] = 2*H_t2[i][j] + K1 * (H_t2[i+1][j] + H_t2[i-1][j] + H_t2[i][j-1] + H_t2[i][j+1] - 4*H_t2[i][j]);
					}
					else
					{
						H[i][j] = 2*H_t1[i][j] - H_t2[i][j] + K2 * (H_t1[i+1][j] + H_t1[i-1][j] + H_t1[i][j-1] + H_t1[i][j+1] - 4*H_t1[i][j]);
					}	
				}
			}
		}
		memcpy(H_t2,H_t1, dim*dim*sizeof(float));
		memcpy(H_t1,H, dim*dim*sizeof(float));
    	//Comprobando iteración de salida
    	if(iteration_exit==index)
    	{
    		FILE *salida = fopen(file_exit,"wb");
			fwrite(H,dim*dim, sizeof(float),salida);
			fclose(salida);
    	}	
	}
	timeend = clock(); // registramos el tiempo hasta el final
	printf("Total = %f\n", (double) (timeend-timestart)/(double)CLOCKS_PER_SEC);
	return 0;
}
