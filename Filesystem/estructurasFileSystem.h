#ifndef ESTRUCTURASFILESYSTEM_H_
#define ESTRUCTURASFILESYSTEM_H_
/*
 * estructurasFileSystem.h
 *
 *  Created on: 31/5/2015
 *      Author: JuanRios
 */
//estructura FileSytem
#include<commons/collections/list.h>
#include<commons/collections/queue.h>

//estructura Archivo

typedef struct {
	char ip[255]; //quizas conviene en un futuro que
	int puerto; //ip y puerto sean un solo char
	int numeroDeBloqueEnNodo;

} t_bloqueEnNodo;

typedef struct {
	t_list copiasDeBloque;
} t_bloqueArch;

typedef struct {
	char nombre[255];
	int nroBloqueArch;
} t_bloquesEnArch;

typedef struct {
	char nombre[255];
	int padre;
	float tamanio;
	t_list bloquesDeArch;
	int estado;
} t_archivo;

//estructura nodo

typedef struct {
	char ip[255]; //quizas conviene en un futuro que
	int puerto; //ip y puerto sean un solo char
	int tamanio;
	int cantidadBloquesOcupados;
	t_queue *bloqueLiberados;
	int activo; //para saber si el bloque se cayo o esta activo
} t_nodo;

//estructura tabla de directorios
typedef struct {
	int index;
	char nombre[255];
	int padre;
} t_directorio;
//estructura del fileSystem
/*typedef struct//No vamos a usar esta estructura, directamente vamos a usar las 3 listas en el FS
 {
 t_list *archivo;
 t_list *nodo
 t_list *directorio;
 }t_fileSystem ;
 */
//Funciones de estructura
//funciones para estructura Archivo
/**
 * @NAME: nuevoArchivo
 * @DESC: Inicializa un nuevo archivo
 */
t_archivo nuevoArchivo(char nombre[255], int padre, float tamanio,
		t_bloqueArch *bloqueArch, int estado);

/**
 * @NAME: nuevoBloqueEnNodo
 * @DESC: Inicializa un nuevo bloqueNodo
 */
t_bloqueEnNodo nuevoBloqueEnNodo(char ip[255], int puerto,
		int numeroDeBloqueEnNodo);

//funciones para la estructura nodo
/**
 * @NAME: nuevoNodo
 * @DESC: Inicializa un nuevo nodo
 */
t_nodo nuevoNodo(char ip[255], int puerto, int tamanio);

/**
 * @NAME: activarNodo
 * @DESC: activa un nodo
 */
void activarNodo(t_nodo unNodo);

/**
 * @NAME: desactivarNodo
 * @DESC: desactiva un nodo
 */
void desactivarNodo(t_nodo unNodo);

//funciones para la estructura tabla de directorios
/**
 * @NAME: nuevoDirectorio
 * @DESC: crea una nueva entrada para el directorio
 */
t_directorio nuevoDirectorio(int index, char nombre[255], int padre);

//funcion para dividir el archivo en bloques
t_list* divideArchivoEnBloques(char* pathArch);

#endif /* ESTRUCTURASFILESYSTEM_H_ */
