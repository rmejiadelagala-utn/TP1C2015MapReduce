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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <socketes/envioDeMensajes.h>

//estructura Archivo
/*
typedef struct {
	int id;
	int numeroDeBloqueEnNodo;

} t_bloqueEnNodo;
*/
typedef struct {
	t_list *copiasDeBloque;
} t_bloqueArch;

typedef struct { //XXX hace falta esta estructura  t_bloquesEnArch;
	char nombre[255];
	int nroBloqueArch;
} t_bloquesEnArch;

typedef struct {
	char *nombre;
	int padre;
	float tamanio;
	t_list *bloquesDeArch;
	int estado;
} t_archivo;

//estructura nodo

typedef struct {
	float tamanio;
	int cantidadBloquesOcupados;
	t_queue *bloquesLiberados;
	int activo; //para saber si el bloque se cayo o esta activo
	int socket;
	int id;
} t_nodo;

//estructura informacion de archivo a enviar a MaRTA

typedef t_list t_listBloquesDeArchivo;

 t_listBloquesDeArchivo* listaBloquesArch; //lista de bloques de archivo con sus copias

 typedef t_list t_listCopiasEnNodo;

 t_listCopiasEnNodo* sublistaCopiasEnNodo;

 /*                  _________________________________      ___________________________________
listaBloquesArch -->|bloque_arch | sublista    | SGTE |     |bloque_arch | sublista    | SGTE |
                    |            |copiasEnNodos|   *--|---->|            |copiasEnNodos|  *---|--->
                    |____________|_____________|______|     |____________|_____________|______|

                               _______________________       _______________________
    sublistaCopiasEnNodos --> | Nodo-Bloque    | SGTE |     | Nodo-Bloque    | SGTE |
                              |(t_bloqueEnNodo)|   *--|---->|(t_bloqueEnNodo)|  *---|---->
                              |________________|______|     |________________|______|
*/


//estructura tabla de directorios
typedef struct {
	int index;
	char *nombre;
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
t_archivo *nuevoArchivo(char *nombre, int padre, float tamanio,
		t_list *bloquesDeArch, int estado);

/**
 * @NAME: nuevoBloqueArchivo
 * @DESC: Inicializa un nuevo bloqueArchivo
 */
t_bloqueArch *nuevoBloqueArchivo(t_list *copiasDeBloques);

/**
 * @NAME: nuevoBloqueEnNodo
 * @DESC: Inicializa un nuevo bloqueNodo
 */
t_bloqueEnNodo *nuevoBloqueEnNodo(int id, int numeroDeBloqueEnNodo);

//funciones para la estructura nodo
/**
 * @NAME: nuevoNodo
 * @DESC: Inicializa un nuevo nodo
 */
t_nodo *nuevoNodo(int id, int tamanio);

/**
 * @NAME: activarNodo
 * @DESC: activa un nodo
 */
void activarNodo(t_nodo *unNodo);

/**
 * @NAME: desactivarNodo
 * @DESC: desactiva un nodo
 */
void desactivarNodo(t_nodo *unNodo);

/**
 * @NAME: desactivarNodo
 * @DESC: dado un string y un socket, devuelve un nodo
 */
t_nodo* string_to_nodo(char* infoDeNodo,int socket);

//funciones para la estructura tabla de directorios
/**
 * @NAME: nuevoDirectorio
 * @DESC: crea una nueva entrada para el directorio
 */
t_directorio *nuevoDirectorio(int index, char *nombre, int padre);

//funcion para dividir el archivo en bloques
t_list* divideArchivoEnBloques(char* pathArch);

/**
 * @NAME: esHijo
 * @DESC: Devuelve 1 si directorioHijo tiene como padre a directorioPadre, sino 0.
 */
int esHijo(t_directorio *directorioHijo, t_directorio *directorioPadre);

//Funciones para liberar estructuras

/**
 * @NAME: liberarBloqueEnNodo
 * @DESC: Libera la estructura bloqueEnNodo
 */
void liberarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo);

/**
 * @NAME: liberarBloqueArch
 * @DESC: Libera la estructura bloqueArch
 */
void liberarBloqueArch(t_bloqueArch *bloqueArch);

/**
 * @NAME: liberarArchivo
 * @DESC: Libera la estructura archivo
 */
void liberarArchivo(t_archivo *unArchivo);

/**
 * @NAME: liberarNodo
 * @DESC: Libera la estructura nodo
 */
void liberarNodo(t_nodo *unNodo);

/**
 * @NAME: liberarDirectorio
 * @DESC: Libera la estructura directorio
 */
void liberarDirectorio(t_directorio *unDirectorio);

//Funciones para mostrar estructuras

/**
 * @NAME: mostrarRegistro
 * @DESC: muestra la estructura registro
 */
void mostrarRegistro(t_registro_id_ipPuerto *registro);
/**
 * @NAME: mostrarBloqueEnNodo
 * @DESC: muestra la estructura bloqueEnNodo
 */
void mostrarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo);

/**
 * @NAME: mostrarBloqueArch
 * @DESC: muestra la estructura bloqueArch
 */
void mostrarBloqueArch(t_bloqueArch *bloqueArch);

/**
 * @NAME: mostrarArchivo
 * @DESC: muestra la estructura archivo
 */
void mostrarArchivo(t_archivo *unArchivo);

/**
 * @NAME: mostrarNombreArchivo
 * @DESC: muestra el nombre de un archivo
 */

void mostrarNombreArchivo(t_archivo *unArchivo);

/**
 * @NAME: mostrarNodo
 * @DESC: muestra la estructura nodo
 */
void mostrarNodo(t_nodo *unNodo);

/**
 * @NAME: mostrarDirectorio
 * @DESC: muestra la estructura directorio
 */
void mostrarDirectorio(t_directorio *unDirectorio);

/**
 * @NAME: mostrarNombreDirectorio
 * @DESC: muestra el nombre de un directorio
 */
void mostrarNombreDirectorio(t_directorio *unDirectorio);

/**
 * @NAME: mostrarLista
 * @DESC: muestra una lista con el shower de dicha estructura
 */
void mostrarLista(t_list *unaLista, void (*shower)(void*));

t_list *listaNodo;
t_list *listaDirectorio;
t_directorio *directorioUser;
t_archivo *listaArchivo;

#endif /* ESTRUCTURASFILESYSTEM_H_ */
