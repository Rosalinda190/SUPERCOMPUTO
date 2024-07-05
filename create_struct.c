/*
 -Materia: Supercómputo
 -Semestre: 2023-2024/II
 -Nombre del alumno: ROSALINDA MUÑOZ GÁMEZ
 -Clave del alumno: 303997
 -Carrera: Ingeniería en Sistemas Inteligentes
 -Nombre de tarea o programa: "MPI: MPI_Type_Create_struct"
 -Periodo de evaluación (parcial 1, 2, 3, 4 o 5): Parcial 3
 -Descripción: Realizar el código usando MPI struct para implementar cálculos en una estructura de alumnos.
 -Avance logrado (0 a 100%): 100%
 -Comentarios adicionales: Ninguno.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

typedef struct{
    int clave;
    int clave_materia;
    char nombre[255];
    float calificaciones[4];
    float promedio;
}ALUMNO;

void generaDatos(ALUMNO *alumnos, int N);
void ImprimirArreglo(ALUMNO *alumnos, int N);
void calculaPromedios(ALUMNO *alumnos, int nDatos, int *A_reprobados, int *A_aprobados);

int main(int argc, char *argv[]) {
    int idproc, numproc;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);
    
    MPI_Datatype MPI_ALUMNO;
    MPI_Datatype tipos[5] = {MPI_INT, MPI_INT, MPI_CHAR, MPI_FLOAT, MPI_FLOAT};
    int longitudes[5] = {1, 1, 255, 4, 1};
    MPI_Aint desplazamientos[5];

    desplazamientos[0] = offsetof(ALUMNO,clave);
    desplazamientos[1] = offsetof(ALUMNO,clave_materia);
    desplazamientos[2] = offsetof(ALUMNO,nombre);
    desplazamientos[3] = offsetof(ALUMNO,calificaciones);
    desplazamientos[4] = offsetof(ALUMNO,promedio);
    
    
    MPI_Type_create_struct(5, longitudes, desplazamientos, tipos, &MPI_ALUMNO);
    MPI_Type_commit(&MPI_ALUMNO);
    
    int N = 100;
    int nDatos = N / numproc;
    int nDatosU = nDatos + N % (numproc);
    int i,j, t_aprobados = 0, t_reprobados = 0;
  
    int *Arraycount = (int *)malloc((numproc) * sizeof(int));
    int *displs = (int *)malloc((numproc) * sizeof(int));
    
    ALUMNO *Alumnos;
    ALUMNO *a_local;
    
   for(i = 0; i < numproc-1; i++)
    {
        if(i==0)
        {
            displs[0]=0;
        }
        
        Arraycount[i] = nDatos;
        displs[i] = displs[i - 1] + Arraycount[i - 1];
     }
    
    Arraycount[numproc - 1] = nDatosU;
    displs[numproc - 1] = displs[numproc - 2] + Arraycount[numproc - 2];
    

    if(idproc == 0)
    {
        Alumnos = (ALUMNO *)malloc(N * sizeof(ALUMNO));
        generaDatos(Alumnos,N);
    }
    
  
    a_local = (ALUMNO*)malloc(Arraycount[idproc]*sizeof(ALUMNO));
   
    MPI_Scatterv(Alumnos, Arraycount, displs, MPI_ALUMNO, a_local, Arraycount[idproc], MPI_ALUMNO, 0, MPI_COMM_WORLD);
  
    int local_reprobados = 0;
    int local_aprobados = 0;
        
    calculaPromedios(a_local, Arraycount[idproc], &local_reprobados, &local_aprobados);
    
    MPI_Gatherv(a_local, Arraycount[idproc], MPI_ALUMNO, Alumnos, Arraycount, displs, MPI_ALUMNO, 0, MPI_COMM_WORLD);
    
    MPI_Reduce(&local_reprobados, &t_reprobados, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_aprobados, &t_aprobados, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (idproc == 0) {
          printf("\nArreglo de alumnos\n");
          ImprimirArreglo(Alumnos, N);
          printf("\n");
          printf("Alumnos reprobados: %d\n", t_reprobados);
          printf("Alumnos aprobados: %d\n", t_aprobados);
          free(Alumnos);
      }

       
    MPI_Type_free(&MPI_ALUMNO);
    free(displs);
    free(Arraycount);
    free(a_local);
    MPI_Finalize();
    return 0;
}


void generaDatos(ALUMNO *alumnos, int N) {
    int i,j;
    const char *nombres[] = {"Juan", "Ana", "Luis", "Maria", "Carlos", "Laura", "Miguel", "Marta", "Pedro", "Lucia"};
    const char *apellidos[] = {"Garcia", "Martinez", "Rodriguez", "Lopez", "Gonzalez", "Perez", "Sanchez", "Ramirez", "Cruz", "Flores"};
    
    int numNombres = sizeof(nombres) / sizeof(nombres[0]);
    int numApellidos = sizeof(apellidos) / sizeof(apellidos[0]);

    srand(time(NULL));

    for ( i = 0; i < N; i++) {
        alumnos[i].clave = rand() % 1000000;;
        alumnos[i].clave_materia = rand() % 100000;
        
        const char *nombre = nombres[rand() % numNombres];
        const char *apellido = apellidos[rand() % numApellidos];
        snprintf(alumnos[i].nombre, 255, "%s %s", nombre, apellido);
        
        for ( j = 0; j < 4; j++) {
            alumnos[i].calificaciones[j] = (float)(rand() % 101) / 10.0;
        }
    }
}

void calculaPromedios(ALUMNO *alumnos, int nDatos, int *reprobados, int *aprobados) {
    int i,j;
    
    *reprobados = 0;
    *aprobados = 0;

    for ( i = 0; i < nDatos; i++) {
        float sumaCalificaciones = 0.0;
        for ( j = 0; j < 4; j++) {
            sumaCalificaciones += alumnos[i].calificaciones[j];
        }
        alumnos[i].promedio = sumaCalificaciones/4.0;
        if (alumnos[i].promedio < 6.0) {
            (*reprobados)++;
        } else {
            (*aprobados)++;
        }
    }
}

void ImprimirArreglo(ALUMNO *alumnos, int N) {
    int i;
    for ( i = 0; i < N; i++) {
        printf("\nClave materia: %d\nClave: %d\nNombre: %s\nPromedio: %.2f\n", alumnos[i].clave_materia, alumnos[i].clave, alumnos[i].nombre, alumnos[i].promedio);
    }
}

