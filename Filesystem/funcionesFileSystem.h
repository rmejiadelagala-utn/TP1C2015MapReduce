/*
 * funcionesFileSystem.h
 *
 *  Created on: 31/5/2015
 *      Author: JuanRios
 */

#ifndef FUNCIONESFILESYSTEM_H_
#define FUNCIONESFILESYSTEM_H_
#define BLOCK_SIZE 150 //2* 1024 * 1024;//Probar con menos
#include"estructurasFileSystem.h"
#include "fsystem.h"


int nodoEstaActivo (t_registro_id_ipPuerto* unRegistro);

/*
 * @NAME: distribuirBloquesEnNodos
 * @DESC: recibe una lista de bloques de archivo, por la cual crea 3 copias y cada una se la asigna a un nodo específico. Ademas internamente ordena la lista para lograr una mejor distribucion
 */
void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos);

void enviarCantBloquesDeArch(char* unArchivo,int socket);

/*
 * @NAME: setBloque
 * @DESC: envia el dataBloque al nodo para que lo almacene, se le pasa el tamanio del bloque y en donde comienza (por si el bloque es solo una parte de un archivo mapeado a memoria. Luego, guarda el archivo en la lista de copias del bloque de archivo.
 */
int setBloque(t_nodo* nodo, char* dataBloque, int tamanio, int comienzoDeBloque,t_list *copias);

/*
 * @NAME: formatear
 * @DESC: recibe la lista de archivos, nodos y directorios, y las vacía
 */
void formatear(t_list **listaNodos, t_list **listaArchivos,
		t_list **listaDirectorios);

/*
 * @NAME: renombrarArchivoPorNombre
 * @DESC: recibe una lista de archivos, un nombre de un archivo y un nuevo nombre para el mismo, y lo modifica en la lista de archivos
 */
void renombrarArchivoPorNombre(char *nombreBuscado, char *nuevoNombre,
		t_list *listaArchivos);

/*
 * @NAME: moverArchivoPorNombreYPadre
 * @DESC: recibe una lista de archivos, un nombre de un archivo y un nuevo padre para el mismo, y lo modifica en la lista de archivos
 */
void moverArchivoPorNombreYPadre(char *nombreBuscado, t_list *listaArchivos,
		t_list *listaDirectorios, int padre);

/*
 * @NAME: crearDirectorioDadoPadreYNom
 * @DESC: recibe una lista de directorios, un nombre de un directorio y un padre para el mismo, y lo crea agregando a lista de directorios
 */
void crearDirectorioDadoPadreYNom(char *nombre, int padre,
		t_list *listaDirectorio);

/*
 * @NAME: eliminarDirectorioDadoElIndice
 * @DESC: recibe una lista de directorios, un indice de un directorio y lo elimina
 */
void eliminarDirectorioDadoElIndice(int indice, t_list *listaDirectorio);

/*
 * @NAME: actualizarRegistro
 * @DESC: actualiza el registro y le avisa a marta.
 */
void actualizarRegistro(t_registro_id_ipPuerto* unRegistro,struct in_addr ip,uint16_t puerto);
/*
 * @NAME: esNodoNuevo
 * @DESC: recibe una lista de nodos y un nodo, y se fija si ese nodo esta en la lista comparando puerto e ip
 */

bool esNodoNuevo(t_nodo *nodoABuscar, t_list *listaNodos);

/*
 * @NAME: activarNodoReconectado
 * @DESC: busca en la lista de nodos el nodo reconectado y le cambia el flag activandolo
 */
void activarNodoReconectado(t_nodo *nodoABuscar, t_list *listaNodos);

/*
 * @NAME: eliminarNodoYRerencias
 * @DESC: elimina nodo de la lista nodos y las referencias de las copias en los archivos si es que pertenecen a ese nodo
 */
void eliminarNodoYReferencias(t_nodo *nodoAEliminar, t_list *listaNodos,
		t_list *archivos);

/*
 * @NAME: eliminarReferencias
 * @DESC: //recorro una lista de archivos y por cada archivo recorro cada lista de bloques de archivo, por cada bloque de archivo veo si tengo que eliminar alguna copia de su lista de copias
 */
void eliminarReferencias(t_nodo *nodoAEliminar, t_list *archivos);

/*
 * @NAME: eliminarNodoDeLista
 * @DESC: elimina nodo de la lista nodos
 */
void eliminarNodoDeLista(t_nodo *nodoAEliminar, t_list *listaNodos);

//funciones de busquedas
/**
 * @NAME: verificarRegistro
 * @DESC: devuelve true si el registro guardado en la tabla de registros tiene la misma ip y puerto que el provisto
 */
bool verificarRegistro(t_registro_id_ipPuerto* unRegistro, struct in_addr ip,int puerto);
/**
 * @NAME: buscarRegistroPorId
 * @DESC: busca un registro por id en una lista de registro
 */
t_registro_id_ipPuerto* buscarRegistroPorId(int id);
/**
 * @NAME: buscarNodoPorId
 * @DESC: busca un nodo por id en una lista de nodos
 */
t_nodo *buscarNodoPorId(int id, t_list *listaNodos);

/**
 * @NAME: buscarDirPorNombre
 * @DESC: busca un directorio por nombre en una lista de directorios
 */
t_directorio *buscarDirPorNombre(char *nombre, t_list *listaDirectorios);

/**
 * @NAME: buscarArchPorNombre
 * @DESC: busca un archivo por nombre en una lista de archivos
 */
t_archivo *buscarArchPorNombre(char *nombre,t_list* listaArchivos);

/**
 * @NAME: encontrarDirectorioHijo
 * @DESC: Devuelve el primer directorio de listaDirectorios que tenga como padre a directorioPadre
 */
t_directorio *encontrarDirectorioHijo(t_list *listaDirectorios,t_directorio *directorioPadre);

/**
 * @NAME: eliminarArchivoYreferencias
 * @DESC: elimina un archivo sacandolo de la lista y ademas en el nodo correspondiente elimina la copia que estaba y guarda en la cola de liberados el espacio liberado
 */
void eliminarArchivoYreferencias(t_archivo *unArchivo, t_list *listaArchivos,
		t_list *listaNodos);

/**
 * @NAME: eliminarArchivoDeLista
 * @DESC: elimina un archivo sacandolo de la lista
 */
void eliminarArchivoDeLista(t_archivo *unArchivo, t_list *listaArchivos);

/**
 * @NAME: renombrarDirectorioConNombre
 * @DESC: renombra un directorio con el nombre enviado por parametro
 */
void renombrarDirectorioConNombre(char *nombre,t_directorio *unDirectorio);

/**
 * @NAME: moverDirectorioConPadre
 * @DESC: le cambia el padre a un directorio, lo que provoca el movimiento del mismo
 */
void moverDirectorioConPadre(int padre,t_directorio *unDirectorio);
int obtenerArchivo(t_archivo *archivo);

t_bloqueEnNodo* encontrarBloqueDisponible(t_list* copiasDelBloque);

/**
 * @NAME: eliminarDirectorioYContenido
 * @DESC: elimina un Directorio eliminando todo su contenido
 */
void eliminarDirectorioYContenido(t_directorio *directorioAEliminar);

/**
 * @NAME: buscarDirPorIndex
 * @DESC: busca un directorio por indice
 */
t_directorio *buscarDirPorIndex(int index);

/**
 * @NAME: buscarDirPorPadre
 * @DESC: busca un directorio que tenga el padre enviado por parametro
 */
t_directorio *buscarDirPorPadre(int padre);

/**
 * @NAME: buscarArchPorPadre
 * @DESC: busca un archivo por el padre
 */
t_archivo *buscarArchPorPadre(int padre);

/**
 * @NAME: disminuirNodo
 * @DESC: libera el espacio del nodo que era ocupado por la copia de bloque
 */
void disminuirNodo(t_bloqueEnNodo *copia);

/**
 * @NAME: dameUnSubdir
 * @DESC: devuelve un subdirectorio del directorio enviado por parametro o NULL si no existe
 */
t_directorio *dameUnSubdir(t_directorio *unDirectorio);

/**
 * @NAME: dameUnSubArch
 * @DESC:devuelve un archivo perteneciente del directorio enviado por parametro o NULL si no existe
 */
t_archivo *dameUnSubArch(t_directorio *unDirectorio);

/**
 * @NAME: dirConSubdir
 * @DESC: pregunta al directorio si tiene subdirectorios
 */
int dirConSubdir(t_directorio *unDirectorio);

/**
 * @NAME: dirVacio
 * @DESC: pregunta al directorio si esta vacio, sin archivos y sin directorios
 */
int dirVacio(t_directorio *unDirectorio) ;

/**
 * @NAME: dirConSoloArch
 * @DESC: si el directorio solo contiene archivos en su interior
 */
int dirConSoloArch(t_directorio *unDirectorio);

/**
 * @NAME: eliminarDirectorioVacio
 * @DESC: elimina un directorio vacio, sacandolo de la lista de directorios y destruyendolo
 */
void eliminarDirectorioVacio(t_directorio *directorioAEliminar);

/**
 * @NAME: eliminarSubArchivoDeDir
 * @DESC: elimina un archivo perteneciente al directorio enviado por parametro, y al eliminar el archivo se encarga de eliminar por referencias
 */
void eliminarSubArchivoDeDir(t_directorio *unDirectorio);

/**
 * @NAME: archivoEstaActivo
 * @DESC: hace la logica para ver que un archivo tenga al menos una copia de cada bloque de archivo
 */
void archivoEstaActivoPorLogica(t_archivo *unArchivo);

/**
 * @NAME: archivoActivoPorFlag
 * @DESC: consulta el flag activo de un archivo
 */
int archivoActivoPorFlag(t_archivo *unArchivo);

/**
 * @NAME: nodoElegido
 * @DESC: setea nodo elegido por algoritmo distribucion, o devuele -1 de error
 */
int nodoElegido(t_list *nodosOrdenados, t_nodo **nodoActual, int *posicion);

/**
 * @NAME: mandarBloquesANodos
 * @DESC: manda bloques a los nodos
 */
int mandarBloquesANodos(char* data, int* cantidadBolquesEnviados,
		t_list** listaDeBolques) ;

/**
 * @NAME: buscarArchPorPadreYNombre
 * @DESC: busca un archivo con padre y nombre
 */
t_archivo *buscarArchPorPadreYNombre(int padre,char *nombre);

/**
 * @NAME: separarIpPuerto
 * @DESC: separa un ipPuerto enviado por parametro en una ip retornada y un puerto modificado
 */
char* separarIpPuerto(char* ipPuerto,int * puerto);

#endif /* FUNCIONESFILESYSTEM_H_ */
