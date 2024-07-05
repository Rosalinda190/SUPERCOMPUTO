/*
 -Materia: Supercómputo
 -Semestre: 2023-2024/II
 -Nombre del alumno: ROSALINDA MUÑOZ GÁMEZ
 -Clave del alumno: 303997
 -Carrera: Ingeniería en Sistemas Inteligentes
 -Nombre de tarea o programa: "MPI: MPI_Type_Contiguous "
 -Periodo de evaluación (parcial 1, 2, 3, 4 o 5): Parcial 3
 -Descripción: Código de suma de dos arreglos por medio de estructuras y nuevos tipos de datos.
 -Avance logrado (0 a 100%): 100%
 -Comentarios adicionales: Ninguno.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

typedef struct {
    float x;
    float y;
    float z;
} COORDENADA;

void llenarArreglo(COORDENADA *array, int size);
void ImprimirArreglo(COORDENADA *array, int size);

int main(int argc, char *argv[]) {
    int idproc, numproc;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);
    
    MPI_Datatype MPI_COOR;
    MPI_Type_contiguous(3, MPI_FLOAT, &MPI_COOR);
    MPI_Type_commit(&MPI_COOR);
    
    int N = 5;
    int nDatos = N / (numproc-1);
    int nDatosU = nDatos + N % (numproc-1);
    
    srand(time(NULL));
    
    COORDENADA *a;
    COORDENADA *b;
    COORDENADA *c;
    
    //send/recvcounts:
    int *Arraycount = (int *)malloc((numproc) * sizeof(int));
    int *displs = (int *)malloc((numproc) * sizeof(int));
    
    Arraycount[0] = 0;
    displs[0] = 0;
    int i;
    for( i = 1; i <= numproc-1; i++)
    {
        Arraycount[i] = nDatos;
        displs[i] = displs[i - 1] + Arraycount[i - 1];
    }
    
    Arraycount[numproc - 1] = nDatosU;
    displs[numproc - 1] = displs[numproc - 2] + Arraycount[numproc - 2];

    if(idproc == 0)
    {
        a = (COORDENADA *)malloc(N * sizeof(COORDENADA));
        b = (COORDENADA *)malloc(N * sizeof(COORDENADA));
        c = (COORDENADA *)malloc(N * sizeof(COORDENADA));
        
        llenarArreglo(a, N);
        llenarArreglo(b, N);
        printf("Array A:\n");
        ImprimirArreglo(a, N);
        printf("\nArray B:\n");
        ImprimirArreglo(b, N);
    }
    else
    {
        a = (COORDENADA *)malloc(Arraycount[idproc] * sizeof(COORDENADA));
        b = (COORDENADA *)malloc(Arraycount[idproc]* sizeof(COORDENADA));
        c = (COORDENADA *)malloc(Arraycount[idproc] * sizeof(COORDENADA));
    }
    
    MPI_Scatterv(a, Arraycount, displs, MPI_COOR, a, Arraycount[idproc], MPI_COOR, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, Arraycount, displs, MPI_COOR, b, Arraycount[idproc], MPI_COOR, 0, MPI_COMM_WORLD);

    for( i = 0; i < Arraycount[idproc]; i++)
    {
        c[i].x = a[i].x + b[i].x;
        c[i].y = a[i].y + b[i].y;
        c[i].z = a[i].z + b[i].z;
    }
    
    MPI_Gatherv(c, Arraycount[idproc], MPI_COOR, c, Arraycount, displs, MPI_COOR, 0, MPI_COMM_WORLD);

    if(idproc == 0)
    {
       printf("\nArray C:\n");
       ImprimirArreglo(c, N);
    }
   
    MPI_Finalize();
    
    free(a);
    free(b);
    free(c);
    free(displs);
    free(Arraycount);

    return 0;
}

void llenarArreglo(COORDENADA *array, int N) {
    int i;
    for (i = 0; i < N; i++) {
        array[i].x = (float)(rand() % 1000) / 10.0;
        array[i].y = (float)(rand() % 1000) / 10.0;
        array[i].z = (float)(rand() % 1000) / 10.0;
    }
}

void ImprimirArreglo(COORDENADA *array, int N) {
    
    int i;
    for ( i = 0; i < N; i++) {
        printf("(%.1f, %.1f, %.1f)\n", array[i].x, array[i].y, array[i].z);
    }
}

