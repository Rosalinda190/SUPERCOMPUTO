/*
 -Materia: Supercómputo
 -Semestre: 2023-2024/II
 -Nombre del alumno: ROSALINDA MUÑOZ GÁMEZ
 -Clave del alumno: 303997
 -Carrera: Ingeniería en Sistemas Inteligentes
 -Nombre de tarea o programa: "MPI: Suma de los elementos de un arreglo"
 -Periodo de evaluación (parcial 1, 2, 3, 4 o 5): Parcial 2
 -Descripción: Realizar la suma total de un arreglo.
 -Avance logrado (0 a 100%): 100%
 -Comentarios adicionales: Ninguno.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"mpi.h"

void llenarArreglo(int a[], int N);
void imprimirArreglo(int a[],int N);
int* crearArreglo(int N);

int main(int argc, char *argv[])
{
  int idProc, numProc,iProc;
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    
    int N=100;
  
  //nDatos = N/(numProc-1);
  //nDatosU= nDatos + N % (numProc-1);
  
    
  if(idProc == 0)
    {
        int *a=crearArreglo(N);
        llenarArreglo(a,N);

      for(iProc=1; iProc<=numProc-2; iProc++)
     {
      MPI_Send(a + (iProc-1)* nDatos,nDatos,MPI_INT,iProc,0,MPI_COMM_WORLD);

     }

        MPI_Send(a + (numProc-2) * nDatos,
                 nDatosU,MPI_INT,numProc-1,0,MPI_COMM_WORLD);
       
    for(iProc=1; iProc <= numProc-1; iProc++)
      {
         int sumap=0;
         MPI_Recv(&sumap,1,MPI_INT,iProc,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         suma+=sumap;
      }
        
    printf("\nArreglo original\n");
    imprimirArreglo(a,N);
    printf("\nSuma total: %d\n",suma);
        free(a);
    }else
    
    {
      if(idProc == numProc-1)
    {
      nDatos= nDatosU;
    }
        int i,suma=0;
        int *a= crearArreglo(nDatos);
        
         MPI_Recv(a,nDatos,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
       
         printf("\nProceso %d recibio los siguientes datos:\n", idProc);
        
        for(i = 0; i < nDatos; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");

      for( i=0;i<nDatos;i++)
        {
          suma+=a[i];
        }
       
      MPI_Send(&suma,1,MPI_INT,0,0,MPI_COMM_WORLD);
        
      printf("\nProceso %d envio la siguiente suma parcial: %d\n", idProc, suma);
      free(a);
    }
     MPI_Finalize();
  return 0;
}

int* crearArreglo(int N)
{
    int *a=(int*)malloc(N*sizeof(int));
    return a;
}

void llenarArreglo(int a[],int N)
{
  int i;
  srand(time(NULL));
  for(i=0; i<N; i++)
    {
      a[i]= rand()%10+1;
    }
}

 void imprimirArreglo(int a[],int N)
{
     int i;
     for( i=0; i<N; i++)
     {
         printf("%d ", a[i]);
     }
 }
