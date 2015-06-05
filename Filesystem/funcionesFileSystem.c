/*
 * funcionesFileSystem.c
 *
 *  Created on: 31/5/2015
 *      Author: JuanRios
 */
#include<stdbool.h>
#include"estructurasFileSystem.h"
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "fsystem.h"
#include"funcionesFileSystem.h"

typedef struct {
	char nombre[255];
	bool (*mismoNombreArch)(void*);
} t_closureCondicion;

static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente);
static int buscarPosicionEnListaDadoUnArchivo(t_list *lista, t_archivo *archivo);
static bool existeEseIndiceComoPadre(t_list *listaDirectorios, int padre);
static int indiceNuevo(t_list *listaDirectorio);
static int obtenerArchivo(char *nombreArchivo, char* path);
//
/*
 t_list* divideArchivoEnBloques(char* pathArch){

 int fd_a = -1, i;
 uint32_t tamanio_arch;

 if ((fd_a = open(pathArch, O_RDWR)) == -1)
 err(1, "FS: Error al abrir archivo (open)");

 struct stat bufa;

 stat(pathArch, &bufa);
 tamanio_arch = bufa.st_size;

 int cant_bloques = (tamanio_arch / BLKSIZE) + (tamanio_arch % BLKSIZE != 0);

 t_list * bloquesArchivo = list_create();
 t_bloquesEnArch * elem;

 for (i = 0; i < cant_bloques; i++) {

 //		elem->nombre = pathArch; -> Nombre del Archivo
 elem->nroBloqueArch = i;
 list_add(bloquesArchivo, elem);

 }
 return bloquesArchivo;
 }
 */
void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos) {
	//variables auxiliares para la funcion
	t_bloqueEnNodo *bloqueEnNodo;
	t_nodo *nodoActual;
	t_bloqueArch *bloqueArch;
	int i; //lo pongo aca porque si lo pongo adentro del for me tira error
	int j;
	int k = 0;
	int *aux;
	int posicionEnNodo;
	t_list *copiasDeBloque;
	t_list *copiasDeBloqueAUX;
	t_list *nodosOrdenados = list_create(); //Esto se hace para poder trabajar multihilo sino varios hilos me tocan el puntero de referencia de la lista de nodos y sonaste
	//fin de variables auxs

	list_add_all(nodosOrdenados, nodos);//Agrega todos los elementos de la segunda lista en la primera
	list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);//falta definir ordenarPorMenorUso()

	for (i = 0; i < list_size(bloquesEnArch); i++) {
		//Creo que el primer campo del for no es necesario ya que *nodosOrdenas es algo interno de esta funcion, y *nodos No se toca
		copiasDeBloque = list_create();

		for (j = 0; j < 2; j++) {
			nodoActual = list_get(nodosOrdenados, k);
			if (queue_is_empty(nodoActual->bloquesLiberados)) {
				posicionEnNodo = nodoActual->cantidadBloquesOcupados + 1;
			} else {
				aux = queue_pop(nodoActual->bloquesLiberados);
				posicionEnNodo = *aux;
			}
			nodoActual->cantidadBloquesOcupados++;
			bloqueEnNodo = nuevoBloqueEnNodo(nodoActual->ipPuerto,
					posicionEnNodo);
			list_add(copiasDeBloque, bloqueEnNodo);
			k++;
			if (list_size(nodosOrdenados) == k) {
				k = 0;
			}
			bloqueArch = (list_get(bloquesEnArch, i + 1));
			//cargarCopiasABloqueDeArch(bloqueArch,copiasDeBloque);
			copiasDeBloqueAUX = bloqueArch->copiasDeBloque;
			list_add_all(copiasDeBloqueAUX, copiasDeBloque);
		}

	}
}
void activarNodoReconectado(t_nodo *nodoABuscar, t_list *listaNodos) {
	int i;
	t_nodo *nodoActual;
	for (i = 0; i < list_size(listaNodos); i++) {
		nodoActual = list_get(listaNodos, i + 1);
		if (!strcmp(nodoABuscar->ipPuerto, nodoActual->ipPuerto)) {
			nodoActual->activo = 1;
			i = list_size(listaNodos);	//corto el ciclo como un campeon

		}
	}

}
bool esNodoNuevo(t_nodo *nodoABuscar, t_list *listaNodos) {
	bool mismosNodos(t_nodo *nodoDeLista) {
		return (!strcmp(nodoABuscar->ipPuerto, nodoDeLista->ipPuerto));
	}

	return list_any_satisfy(listaNodos, (bool*) mismosNodos);
}
//elimina el nodo y los bloques de copia que cada archivo contaba en ese nodo
void eliminarNodoDeLista(t_nodo *nodoAEliminar, t_list *listaNodos) {
	bool mismosNodos(t_nodo *nodoDeLista) {
		return (!strcmp(nodoAEliminar->ipPuerto, nodoDeLista->ipPuerto));
	}

	list_remove_by_condition(listaNodos, (bool*) mismosNodos);
}
void eliminarReferencias(t_nodo *nodoAEliminar, t_list *archivos) {

	bool copiaEstaEnNodo(t_bloqueEnNodo *copiaDeBloque) {
		return (!strcmp(nodoAEliminar->ipPuerto, copiaDeBloque->ipPuerto));
	}

	void _list_elements2(t_list *copiasDeBloques) {
		list_remove_by_condition(copiasDeBloques, (bool*) copiaEstaEnNodo);
	}

	void _list_elements1(t_archivo *unArchivo) {
		t_list *bloquesDeArchivo = (unArchivo->bloquesDeArch);
		list_iterate(bloquesDeArchivo, (void*) _list_elements2);
	}

	list_iterate(archivos, (void*) _list_elements1);
}
void eliminarNodoYRerencias(t_nodo *nodoAEliminar, t_list *listaNodos,
		t_list *archivos) {
	eliminarNodoDeLista(nodoAEliminar, listaNodos);
	eliminarReferencias(nodoAEliminar, archivos);

}

void* eliminarArchivoPorNombre(char nombreBuscado[255], t_list *listaArchivos) {
	int archivoConNombreBuscado(t_archivo *unArchivo) {
		return strcmp(nombreBuscado, unArchivo->nombre);
	}

	return list_remove_by_condition(listaArchivos,
			(void*) archivoConNombreBuscado);
}

void formatear(t_list **listaNodos, t_list **listaArchivos,
		t_list **listaDirectorios) {
	list_destroy_and_destroy_elements(*listaArchivos, (void*) liberarArchivo);
	list_destroy_and_destroy_elements(*listaNodos, (void*) liberarNodo);
	list_destroy_and_destroy_elements(*listaDirectorios,
			(void*) liberarDirectorio);
	*listaNodos = list_create();
	*listaArchivos = list_create();
	*listaDirectorios = list_create();
}

void renombrarArchivoPorNombre(char *nombreBuscado, char *nuevoNombre,
		t_list *listaArchivos) {

	int archivoConNombreBuscado(t_archivo *unArchivo) {
		return !strcmp(nombreBuscado, unArchivo->nombre);
	}

	//Busco cual es el archivo a modificar, dado el nombre
	t_archivo *archivoAModificar = list_find(listaArchivos,
			(void*) archivoConNombreBuscado);
	if (!archivoAModificar) {
		printf("el archivo %s no se encuentra en el sistema", nombreBuscado);
	} else {
		//genero un archivo nuevo, que va a ser el anterior con un nuevo nombre
		free(archivoAModificar->nombre);
		archivoAModificar->nombre = malloc(strlen(nuevoNombre) + 1);
		strcpy(archivoAModificar->nombre, nuevoNombre);
	}
}

//Esto de recibir al padre no esta del todo bien. Debería poder mover archivo
//no por número de padre del directorio al que se quiere mover, sino por la
//ruta a la que quiere moverse. Esta ruta sería una cadena de nombre de
//directorio separados por /. Eso me daría el padre del directorio al que quiero
//moverme.
void moverArchivoPorNombreYPadre(char *nombreBuscado, t_list *listaArchivos,
		t_list *listaDirectorios, int padre) {

	if (!existeEseIndiceComoPadre(listaDirectorios, padre)) {
		printf("el directorio a donde mover no existe\n", nombreBuscado);
	} else {

		int archivoConNombreBuscado(t_archivo *unArchivo) {
			return !strcmp(nombreBuscado, unArchivo->nombre);
		}
		t_archivo *archivoAModificarPadre;

		archivoAModificarPadre = list_find(listaArchivos,
				(void*) archivoConNombreBuscado);

		if (!archivoAModificarPadre) {
			printf("el archivo %s no se encuentra en el sistema",
					nombreBuscado);
		} else {
			archivoAModificarPadre->padre = padre;
		}
	}
}

void crearDirectorioDadoPadreYNom(char *nombre, int padre,
		t_list *listaDirectorio) {

	//Verifico que no se pase de los 1024 directorios permitidos
	if (list_size(listaDirectorio) > 1024) {
		printf("No se pueden cargar mas de 1024\n");
	} else {
		//genero el nuevo directorio con sus datos, y le pongo el indice del ultimo + 1
		t_directorio *directorioNuevo = nuevoDirectorio(
				indiceNuevo(listaDirectorio), nombre, padre);
		list_add(listaDirectorio, directorioNuevo);
		printf("El directorio se creo bien\n");
	}
}

//Este se podría hacer dado nombre, pero preferí así porque el índice es único.
//Si no, se reutiliza esta función para hacerla por nombre, ruta o lo que sea.
void eliminarDirectorioDadoElIndice(int indice, t_list *listaDirectorio) {

	int directorioConIndiceBuscado(t_directorio *directorio) {
		return indice == directorio->index;
	}

	if (list_remove_by_condition(listaDirectorio,
			(void*) directorioConIndiceBuscado) == NULL) {
		//tira error de que no lo encontró en la lista.
	}

	//Me faltó considerar que además del directorio borrado, debe borrar a
	//los hijos del mismo, ya que no tienen un padre. Y así sucesivamente.

	//borrarDescendientesDe(indice, listaDirectorio);

}

/*
 void enviarBloques(t_nodo *nodosOrdenados,t_info info){
 //ACA SE ENVIA LA INFORMACION DEL BLOQUE
 }
 //interaccion
 //se conecta el unNodo
 if (esNuevoNodo(unNodo)){
 agregarALista(*nodo,unNodo);
 }
 else {
 activarNodo(unNodo);
 }

 //llega un archivo nuevo
 //Se carga el archivo en las estructuras y se distribuye el archivo en bloques

 //Una vez terminado se distribuyen los bloques en los nodos
 */
/*
 t_fileSystem formatear(t_fileSystem fileSystem) {
 //Poner vacía la lista de directorios del FS completa
 list_clean(fileSystem.directorio);
 //Poner vacía de la lista de nodos todos los nodos asginados al FS
 list_clean(fileSystem.nodo);
 //Poner vacía de la lista de archivos todos los archivos que tenga el FS
 list_clean(fileSystem.archivo);

 return fileSystem;
 }
 */
/*
 void renombrarArchivo(char *nomArchivo, char *nuevoNombreArchivo,
 t_fileSystem fileSystem) {
 //Debo buscar el archivo de nombre nomArchivo y reemplazarlo por el
 //nuevo nombre
 int posicionEnListaArchivos = buscarArchivoPorNombre(nomArchivo,
 fileSystem.archivo);

 //Creo un nuevo archivo, igual al anterior pero con el nombre nuevo
 t_archivo nuevoArchivo = fileSystem.archivo[posicionEnListaArchivos];
 nuevoArchivo.nombre = nuevoNombreArchivo;

 if (posicionEnListaArchivos == -1) {
 //No encontró ese archivo, debería devolver un mensaje de error
 } else {
 list_replace(fileSystem.archivo, posicionEnListaArchivos, nuevoArchivo);
 }
 }
 */
/********* PRIVATE FUNCTIONS **************/

static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente) {
	return dataSiguiente->cantidadBloquesOcupados
			< data->cantidadBloquesOcupados;
}

static int buscarPosicionEnListaDadoUnArchivo(t_list *listaArchivos,
		t_archivo *archivo) {
	int posDondeReemplazar = -1;
	//Busco la posición en la lista de este archivo que debo modificar
	int i = 0;
	for (i = 0; i < listaArchivos->elements_count; i++) {
		if (list_get(listaArchivos, i) == archivo) {
			posDondeReemplazar = i;
		}
	}
	return posDondeReemplazar;
}

static int indiceNuevo(t_list *listaDirectorio) {
	int nuevoIndice = -1;

	int i = 0;
	for (i = 0; i < listaDirectorio->elements_count; i++) {
		t_directorio *dirAuxiliar;

		dirAuxiliar = list_get(listaDirectorio, i);
		if (dirAuxiliar->index != (i + 1)) {
			nuevoIndice = i + 1;

			i = listaDirectorio->elements_count;		//Para salir a lo rambo
		}
	}

	if (nuevoIndice == -1) {
		nuevoIndice = listaDirectorio->elements_count + 1;
	}

	return nuevoIndice;
}

static bool existeEseIndiceComoPadre(t_list *listaDirectorios, int padre) {

	bool existePadre(t_directorio *directorio) {
		return (directorio->index == padre);
	}

	return list_any_satisfy(listaDirectorios, (bool*) existePadre);
}
static int obtenerArchivo(char *nombreArchivo, char* path) {
	t_archivo *archivoEncontrado;
	int nombreCoincide(t_archivo *unArchivo) {
		return unArchivo->nombre == nombreArchivo;
	}
	if ((archivoEncontrado = list_find(listaArchivo, (void *) nombreCoincide))
			== NULL) {
		puts("Archivo no encontrado.");
		return 0;
	}
	if (!archivoEncontrado->estado) {
		puts("Archivo no está disponible.");
		return 0;
	}
	int noEsNull(void* unBloque) {
		return unBloque != NULL;
	}
	int obtenerBloque(t_bloqueArch *bloqueDeArchivo) {
		t_bloqueEnNodo *bloque = list_find(bloqueDeArchivo->copiasDeBloque,
				(void*) noEsNull);
		int ipPuertoCoincide(t_nodo *unNodo) {
			return unNodo->ipPuerto == bloque->ipPuerto;
		}
		t_nodo *nodoEncontrado = list_find(listaNodo, (void*) ipPuertoCoincide);
		//HAY QUE CREAR EL CAMPO SOCKET EN NODO
		/*int socket_desc = nodoEncontrado.socket;
		 send(socket_desc,1,sizeOf(1),0);
		 int tamanioData;
		 recv(socket_desc, &tamanioData,1,0);
		 char **data = malloc(&tamanioData);
		 recv(socket_desc, *data, tamanioData, 0);
		 puts(*data);*/
		//FALTA ESCRIBIR EN ARCHIVO EN VEZ DE MOSTRAR POR PANTALLA
	}
	list_iterate(archivoEncontrado->bloquesDeArch, (void *) obtenerBloque);

}
