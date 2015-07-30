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

/////////////Nuevas estructuras de Marta que surgen del algorit. de distribuir y manda mensajes////////////
//////////// Estructuras de MaRTA Para envio de mesajes y demás /////////////////////////
typedef struct {
	int idJob;
	int combiner;
	char* pathDeResultado;
	char** pathsDeArchivos;
	int cantArchivos;
} t_InfoJob;

typedef struct {
	int idArchivo;
	int cantidadDeBloques;
} t_InfoArchivo;

typedef struct {
	uint32_t id_map;
	uint32_t id_nodo;
	uint32_t ip_nodo;
	uint32_t puerto_nodo;
	uint32_t block;
	char* temp_file_name;
	uint32_t block_size;
} t_DestinoMap;

typedef struct {
	uint32_t id_reduce;
	int id_nodo;
	char* temp_file_name;
} t_OrigenReduce;
//xxx el origenReduce y destinoReduce son lo mismo, salvo que se los usa
//distinto en cuanto al file. En un caso es el que extrae, en el otro el que genera.
//por ahora lo dejo así, porque OrigenReduce no se si ponerle ip-puerto o no

typedef struct {
	uint32_t id_reduce;
	int id_nodo;
	int ip_nodo;
	int puerto_nodo;
	char* temp_file_name;
} t_DestinoReduce;


typedef struct {
	t_InfoArchivo* file;
	uint32_t block;
	t_DestinoMap* map_dest;
} t_MapPendiente;

typedef struct {
	uint32_t numeroDeReducePendiente;
	t_DestinoReduce* map_dest;
} t_ReducePendiente;

typedef struct {
	uint32_t prot;
	uint32_t id_map;
	uint32_t id_nodo;
} t_ResultadoMap;

typedef struct {
	uint32_t prot;
	uint32_t id_reduce;
} t_ResultadoReduce;

typedef struct {
	uint32_t idMapTemporal;
	char* path;
	uint32_t id_nodo;
	uint32_t idArchivoOrigen;
	uint32_t bloqueOrigen;
} t_MapTemporal; //Revisar bien esta

///Esto va en envioDeMensajes me parece///

typedef struct {
	uint32_t id_nodo;
	uint32_t ip_nodo;
	uint32_t puerto_nodo;
	uint32_t block;
	uint32_t size;
} t_CopiaDeBloque;

typedef struct {
	uint32_t id_nodo;
	uint32_t cantidadOperacionesEnCurso;
} t_CargaNodo;

#endif /* ESTRUCTURASMARTA_H_ */
