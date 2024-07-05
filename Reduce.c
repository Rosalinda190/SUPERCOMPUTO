/*
 -Materia: Supercómputo
 -Semestre: 2023-2024/II
 -Nombre del alumno: ROSALINDA MUÑOZ GÁMEZ
 -Clave del alumno: 303997
 -Carrera: Ingeniería en Sistemas Inteligentes
 -Nombre de tarea o programa: "MPI: MPI_Reduce"
 -Periodo de evaluación (parcial 1, 2, 3, 4 o 5): Parcial 2
 -Descripción: Realizar dos operaciones con arreglos usando la función MPI_Reduce.
 -Avance logrado (0 a 100%): 100%
 -Comentarios adicionales: Ninguno.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

void llenarArreglo(int A[], int N);
void imprimirArreglo(int A[], int N);

int main(int argc, char *argv[])
{
    int idProc, numproc, iProc;
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);
    
    srand(time(NULL) + idProc);
    int N = 10;

    int *A = (int*)malloc(N * sizeof(int));
    int *B_max= NULL;
    int *B_sum= NULL;
    
    llenarArreglo(A,N);
    imprimirArreglo(A,N);
    
    if(idProc==0)
    {
        B_max = (int*)malloc(N * sizeof(int));
        B_sum = (int*)malloc(N * sizeof(int));
        
    }
    
    MPI_Reduce(A,B_max,N,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
  
    MPI_Reduce(A,B_sum,N,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    
    if(idProc==0)
    {
        printf("\nMaximos: \n");
        imprimirArreglo(B_max,N);
        printf("\nSumas: \n");
        imprimirArreglo(B_sum,N);
    }
      
    
     MPI_Finalize();
     free(A);
     free(B_max);
     free(B_sum);

     return 0;
}


void llenarArreglo(int A[], int N)
{
    int i;
    
    for (i = 0; i < N; i++)
    {
        A[i] = (int)(rand()%100);
    }
}

void imprimirArreglo(int A[], int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
}
