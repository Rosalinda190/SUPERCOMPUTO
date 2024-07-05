/*
 -Materia: Supercómputo
 -Semestre: 2023-2024/II
 -Nombre del alumno: ROSALINDA MUÑOZ GÁMEZ
 -Clave del alumno: 303997
 -Carrera: Ingeniería en Sistemas Inteligentes
 -Nombre de tarea o programa: "MPI: Suma de dos arreglos"
 -Periodo de evaluación (parcial 1, 2, 3, 4 o 5): Parcial 2
 -Descripción: Realizar la suma de dos arreglos y formar un tercer arreglo con los resultados.
 -Avance logrado (0 a 100%): 100%
 -Comentarios adicionales: Ninguno.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

void llenarArreglo(int a[], int N);
void imprimirArreglo(int a[], int N);
int* crearArreglo(int N);

int main(int argc, char *argv[])
{
    int idProc, numProc, iProc;
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    
    int N = 10, nDatos, nDatosU;
    srand(time(NULL));

    nDatos = N / (numProc - 1);
    nDatosU = nDatos + N % (numProc - 1);
    
    printf("NDATOS: %d",nDatos);
    printf("NDATOSU: %d",nDatosU);

    if (idProc == 0)
    {
        int *a = crearArreglo(N);
        int *b = crearArreglo(N);
        int *cf = crearArreglo(N);

        llenarArreglo(a, N);
        llenarArreglo(b, N);

        for (iProc = 1; iProc <= numProc-2; iProc++)
        {
            MPI_Send(a + (iProc - 1) * nDatos, nDatos, MPI_INT, iProc, 0, MPI_COMM_WORLD);
            MPI_Send(b + (iProc - 1) * nDatos, nDatos, MPI_INT, iProc, 0, MPI_COMM_WORLD);
        }

        MPI_Send(a + (numProc - 2) * nDatos, nDatosU, MPI_INT, numProc - 1, 0, MPI_COMM_WORLD);
        MPI_Send(b + (numProc - 2) * nDatos, nDatosU, MPI_INT, numProc - 1, 0, MPI_COMM_WORLD);


        for (iProc = 1; iProc < numProc-1; iProc++)
       {
            MPI_Recv(cf + (iProc - 1) * nDatos, nDatos, MPI_INT, iProc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       }
        
         MPI_Recv(cf + (iProc - 1) * nDatos, nDatosU, MPI_INT, numProc-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

        printf("\nArreglo A original\n");
        imprimirArreglo(a, N);
        printf("\nArreglo B original\n");
        imprimirArreglo(b, N);
        printf("\nArreglo C final \n");
        imprimirArreglo(cf, N);

        free(a);
        free(b);
        free(cf);
    }
    else
    {
       if(idProc == numProc - 1)
        {
            nDatos = nDatosU;
        }
        
        int *a = crearArreglo(nDatos);
        int *b = crearArreglo(nDatos);
        int *c = crearArreglo(nDatos);

        MPI_Recv(a, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("\nProceso %d recibio los siguientes datos:\n", idProc);
        printf("Arreglo A: ");
        imprimirArreglo(a, nDatos);
        printf("Arreglo B: ");
        imprimirArreglo(b, nDatos);
        
        int i;
        for (i = 0; i < nDatos; i++)
        {
            c[i] = b[i] + a[i];
        }
        
        MPI_Send(c, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
        free(a);
        free(b);
        free(c);
    }

    MPI_Finalize();
    return 0;
}


int* crearArreglo(int N)
{
    int *a = (int*)malloc(N * sizeof(int));
    return a; 
}

void llenarArreglo(int a[], int N)
{
    int i; 
    for (i = 0; i < N; i++)
    {
        a[i] = rand() % 10 + 1;
    }
}

void imprimirArreglo(int a[], int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}
