/*
 * estructurasMaRTA.h
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#ifndef ESTRUCTURASMARTA_H_
#define ESTRUCTURASMARTA_H_

#include<commons/collections/list.h>

////////// Planificación JOB ///////////
typedef struct {
	char *ipPuerto; //quizas conviene en un futuro que
	int numeroDeBloqueEnNodo;
} t_bloqueEnNodo;

typedef struct {
	int numeroDeBloqueArchivo;
	int mapOReduce;
	int estado;
	t_list *copiasDelBloque; //serían de t_bloqueEnNodo
} t_planificacion; //esta la genera con el algoritmo de planificar las tareas

typedef struct {
	char *nombreArchivo;
	t_list *planificacion; //serían de t_planificacion
} t_planificacionPorCadaArchivo;

typedef struct {
	int id_Job;	//Va a ser otra cosa. Solo es la idea
	char *nombreArchivoResultado;
	int combinerONO;
	int estado;
	t_list *planificacionesArchivo; //serían de t_planificacionPorCadaArchivo
} t_jobsAPlanificar;

////////// Info pedidad a FS /////////////
typedef struct{
	int numeroDeBloque;
	t_list *bloquesEnNodo; //serían t_bloqueEnNodo
} t_bloqueDeArchivo;


typedef struct {
	char *nombreArchivo;
	t_list *bloquesDeArch; //serían t_bloqueDeArchivo
} t_infoDeFileSystem;

///////// Info de carga de Nodos en el Sistema /////////
typedef struct {
	char *ipPuerto;
	int carga;
	int estado;
} t_infoNodo;

#endif /* ESTRUCTURASMARTA_H_ */
