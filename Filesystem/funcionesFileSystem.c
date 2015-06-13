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
t_list *listaArchivos;
t_list *listaNodos;
t_list *listaDirectorios;

static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente);
static int buscarPosicionEnListaDadoUnArchivo(t_list *lista, t_archivo *archivo);
static bool existeEseIndiceComoPadre(t_list *listaDirectorios, int padre);
static int indiceNuevo(t_list *listaDirectorio);
static char* nodoIpPuerto(t_nodo *unNodo);
static char* dirNombre(t_directorio *unDir);
static char* archNombre(t_archivo *unArch);
static void *buscarEnListaPorStrKey(t_list *lista, char *key,
		char *keyGetter(void*));
static void *buscarEnListaPorIntKey(t_list *lista, int key,
		int *keyGetter(void*));
static void recorrerCopiasDeUnArch(t_archivo *unArchivo,
		void (*accionACopia)(t_bloqueEnNodo*));
static void disminuirNodo(t_bloqueEnNodo *copia);
static int dirPadre(t_directorio *unDir);
static int dirIndex(t_directorio *unDir);
static int archPadre(t_archivo *unArch);
static void lista_remove_and_destroy_by_condition(t_list *self,
bool (*condition)(void*), void (*element_destroyer)(void*));
static bool tieneLugar(t_nodo *unNodo);
//
/*
 void darFormatoAlArchivo(char* path) {

 }
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

int BLOCK_SIZE =2* 1024 * 1024;//Probar con menos
int CANT_COPIAS = 3;
/*
 * //FIXME a la hora de tener un archivo mayor a 20MB o que el BLOCK_SIZE sea menor estalla
 * hay 2 motivos para que estalle, uno no encontro un barra n en el bloque por lo que debería devolver error
 * e imprimir por pantalla que el archivo esta mal formateado. Y el otro es cuando de verdad esta buscando /n
 * y hay un /n tambien tira segment fault. Por lo cual es importante arreglarlo
 */

int mandarBloquesANodos(char* data, int* cantidadBolquesEnviados,

		t_list** listaDeBloques) {

	int i, fin = 0;
	int comienzoDeBloque = 0, finDeBloque;
	t_bloqueArch *bloqueDeArchivo;
//	t_list *nodosOrdenados = list_create();
	t_nodo *nodoActual;
	int posicionEnNodo;
	int *aux;
	t_bloqueEnNodo *bloqueEnNodo;
	int ultimoIndiceDelData = string_length(data) - 1;//Juanchi dice que -1 no va

	*cantidadBolquesEnviados = 0;

	while (!fin) {
		bloqueDeArchivo = malloc(sizeof(t_bloqueArch));
		bloqueDeArchivo->copiasDeBloque = list_create();
		finDeBloque = comienzoDeBloque + BLOCK_SIZE;

		if (finDeBloque > ultimoIndiceDelData) {

			finDeBloque = ultimoIndiceDelData;
			fin = 1;
		}	//Sale si ya no hay bloques

		while (data[finDeBloque] != '\n'){
			finDeBloque--;
		}
		//Acá tengo el final del bloque dado, y también donde empieza

		//ordenar lista nodo por cantidad de bloques usados-->sale nodosOrdenados
		t_list *nodosOrdenados = list_create();
		list_add_all(nodosOrdenados, listaNodos);//Agrega todos los elementos de la segunda lista en la primera
		list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);
		int k = 0;
		//Acá distribuye las copias dado el algoritmo de dstribucion
		for (i = 0; i < CANT_COPIAS; i++) {
			//--Mandar este bloque al nodo que corresponda--

			//nodoActual = nodoElegdoYConLugar(nodosOrdenados);
			if (nodoElegido(nodosOrdenados, &nodoActual,&k) != -1) {
				//salio bien el elegir nodo, estando en nodoActual

				//cargarEnListaArchivoElNodo(nodoElegido);
				if (queue_is_empty(nodoActual->bloquesLiberados)) {
					posicionEnNodo = nodoActual->cantidadBloquesOcupados + 1;
				} else {
					aux = queue_pop(nodoActual->bloquesLiberados);
					posicionEnNodo = *aux;
					free(aux);
				}
				nodoActual->cantidadBloquesOcupados++;
				bloqueEnNodo = nuevoBloqueEnNodo(nodoActual->ipPuerto,
						posicionEnNodo);
/*
 * nodo: nodoActual
 * socket: nodoActual->socket
 * info: eee masomenos, buscala en memoria
 */

				//TODO enviarBloqueDeDatosA(nodoElegido, incicio y fin de bloque);

				//termino de agregar a la lista de archivos, la info nueva del bloque
				list_add(bloqueDeArchivo->copiasDeBloque, bloqueEnNodo);//algo malo puede pasar
				printf("agrego un bloque\n");
			} else {
				printf("No hay nodos disponibles\n");
				return -1;
			}
			k++;
		}
		//cambiar el bloque start al siguiente y agrega el Bloque a la lista de bloques
		list_add(*listaDeBloques, bloqueDeArchivo);
		(*cantidadBolquesEnviados)++;
		comienzoDeBloque = finDeBloque + 1;
		printf("agrego el bloque a la lista de bloques\n");
		list_destroy(nodosOrdenados);
	}
	return 1;
}

int nodoElegido(t_list *nodosOrdenados, t_nodo **nodoActual, int *posicion) {
	int z = *posicion;
	int fin = 0;

	*nodoActual = list_get(nodosOrdenados, *posicion);

	while (!tieneLugar(*nodoActual) && fin != 0) {
		*posicion = *posicion + 1;//con el ++ se quejaba eclipse
		if (z == *posicion) {
			fin = 1;
			return -1;
		}
		if (list_size(nodosOrdenados) == *posicion) {
			*posicion = 1;
		}
		*nodoActual = list_get(nodosOrdenados, *posicion);
		printf("%d ---- %d\n", *posicion, z);
	}
	printf("%d\t",*posicion);
	return 0;
}

void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos) {//Probada :D
	//variables auxiliares para la funcion
	t_bloqueEnNodo *bloqueEnNodo;
	t_nodo *nodoActual;
	t_bloqueArch *bloqueArch;
	int i;
	int j;
	int k = 0;
	int *aux;
	int posicionEnNodo;
	t_list *copiasDeBloqueAUX;
	t_list *nodosOrdenados = list_create();
	//fin de variables auxs

	list_add_all(nodosOrdenados, nodos);//Agrega todos los elementos de la segunda lista en la primera
	list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);

	for (i = 0; i < list_size(bloquesEnArch); i++) {

		for (j = 0; j < 3; j++) {
			nodoActual = list_get(nodosOrdenados, k);
			if (queue_is_empty(nodoActual->bloquesLiberados)) {
				posicionEnNodo = nodoActual->cantidadBloquesOcupados + 1;
			} else {
				aux = queue_pop(nodoActual->bloquesLiberados);
				posicionEnNodo = *aux;
				free(aux);
			}
			nodoActual->cantidadBloquesOcupados++;
			bloqueEnNodo = nuevoBloqueEnNodo(nodoActual->ipPuerto,
					posicionEnNodo);
			k++;
			if (list_size(nodosOrdenados) == k) {
				k = 0;
			}
			bloqueArch = (list_get(bloquesEnArch, i));
			copiasDeBloqueAUX = bloqueArch->copiasDeBloque;
			list_add(copiasDeBloqueAUX, bloqueEnNodo);
		}
	}
	list_destroy(nodosOrdenados);
}

void eliminarDirectorioYContenido(t_directorio *directorioAEliminar) { //probada
	t_directorio *unDirectorio = directorioAEliminar;

	void eliminarDirRecursivamente(t_directorio *unDirectorio) {

		if (dirVacio(unDirectorio) && unDirectorio != directorioAEliminar) {
			t_directorio *dirPadre = buscarDirPorIndex(unDirectorio->padre);
			eliminarDirectorioVacio(unDirectorio);
			eliminarDirRecursivamente(dirPadre);
		} else if (dirConSoloArch(unDirectorio)) {
			eliminarSubArchivoDeDir(unDirectorio);
			eliminarDirRecursivamente(unDirectorio);
		} else if (dirConSubdir(unDirectorio)) { //el dir tiene subDir entonces entra a los subdirs
			t_directorio *subDir = dameUnSubdir(unDirectorio);
			eliminarDirRecursivamente(subDir);
		} else {	//esta vacio y es el directorio a Eliminar
			eliminarDirectorioVacio(directorioAEliminar);//en este caso directorioAEliminar y unDirectorio son iguales y esta vacio
			printf("Se elimino todo el directorio con su contenido\n");
		}

	}
	eliminarDirRecursivamente(unDirectorio);
}
//funciones auxiliares de eliminar recursivamente, todas probadas
void eliminarSubArchivoDeDir(t_directorio *unDirectorio) {
	t_archivo *subArchivo = buscarArchPorPadre(unDirectorio->index);
	eliminarArchivoYreferencias(subArchivo, listaArchivos, listaNodos);
}

void eliminarDirectorioVacio(t_directorio *directorioAEliminar) {
	int directorioConIndiceBuscado(t_directorio *directorio) {
		return directorioAEliminar->index == directorio->index;
	}
	list_remove_and_destroy_by_condition(listaDirectorios,
			(void*) directorioConIndiceBuscado, (void*) liberarDirectorio);
}
int dirConSoloArch(t_directorio *unDirectorio) {
	return dameUnSubArch(unDirectorio) != NULL
			&& dameUnSubdir(unDirectorio) == NULL;
}
int dirVacio(t_directorio *unDirectorio) {
	return dameUnSubArch(unDirectorio) == NULL
			&& dameUnSubdir(unDirectorio) == NULL;
}
int dirConSubdir(t_directorio *unDirectorio) {
	return dameUnSubdir(unDirectorio) != NULL;
}
t_directorio *dameUnSubdir(t_directorio *unDirectorio) {
	return buscarDirPorPadre(unDirectorio->index);
}
t_archivo *dameUnSubArch(t_directorio *unDirectorio) {
	return buscarArchPorPadre(unDirectorio->index);
}
//fin de funciones auxiliares de eliminar recursivamente
//Funciones de busqueda
t_nodo *buscarNodoPorIpPuerto(char *ipPuerto, t_list *listaNodos) {	//probada
	t_nodo *nodo = buscarEnListaPorStrKey(listaNodos, ipPuerto,
			(char*) nodoIpPuerto);
	return nodo != NULL ? nodo : NULL;
}
t_directorio *buscarDirPorNombre(char *nombre, t_list *listaDirectorios) {//probada
	t_archivo *dir = buscarEnListaPorStrKey(listaDirectorios, nombre,
			(char*) dirNombre);
	return dir != NULL ? dir : NULL;
}
t_directorio *buscarDirPorIndex(int index) { //probada
	t_archivo *dir = buscarEnListaPorIntKey(listaDirectorios, index,
			(int*) dirIndex);
	return dir != NULL ? dir : NULL;
}
t_directorio *buscarDirPorPadre(int padre) { //probada
	t_archivo *dir = buscarEnListaPorIntKey(listaDirectorios, padre,
			(int*) dirPadre);
	return dir != NULL ? dir : NULL;
}
t_archivo *buscarArchPorNombre(char *nombre, t_list *listaArchivos) { //probada
	t_archivo *arch = buscarEnListaPorStrKey(listaArchivos, nombre,
			(char*) archNombre);
	return arch != NULL ? arch : NULL;
}
t_archivo *buscarArchPorPadre(int padre) { //probada
	t_archivo *arch = buscarEnListaPorIntKey(listaArchivos, padre,
			(int*) archPadre);
	return arch != NULL ? arch : NULL;
}
//Fin de funciones de busqueda
t_directorio *encontrarDirectorioHijo(t_list *listaDirectorios,
		t_directorio *directorioPadre) {
	return list_find(listaDirectorios, ( {bool esPadre(t_directorio* unDir)
				{	return esHijo(directorioPadre,unDir);}esPadre;}));
}

void eliminarArchivoYreferencias(t_archivo *unArchivo, t_list *listaArchivos, //probada
		t_list *listaNodos) {
	recorrerCopiasDeUnArch(unArchivo, (void*) disminuirNodo);
	eliminarArchivoDeLista(unArchivo, listaArchivos);

}
void eliminarArchivoDeLista(t_archivo *unArchivo, t_list *listaArchivos) { //probada
	bool archivoConNombreBuscado(t_archivo *archivoDeLista) {
		return (strcmp(archivoDeLista->nombre, unArchivo->nombre) == 0);
	}
	return list_remove_and_destroy_by_condition(listaArchivos,
			(bool*) archivoConNombreBuscado, (void*) liberarArchivo);
}
void activarNodoReconectado(t_nodo *nodoABuscar, t_list *listaNodos) { //probada
	int i;
	t_nodo *nodoActual;
	for (i = 0; i < list_size(listaNodos); i++) {
		nodoActual = list_get(listaNodos, i);
		if (!strcmp(nodoABuscar->ipPuerto, nodoActual->ipPuerto)) {
			activarNodo(nodoActual);
			i = list_size(listaNodos);	//corto el ciclo como un campeon

		}
	}

}
//XXX creo que no hace falta esta función porque el nodo cuando se conecta indíca si es nuevo o no
bool esNodoNuevo(t_nodo *nodoABuscar, t_list *listaNodos) {	//probada
	bool mismosNodos(t_nodo *nodoDeLista) {
		return (!strcmp(nodoABuscar->ipPuerto, nodoDeLista->ipPuerto));
	}

	return !list_any_satisfy(listaNodos, (bool*) mismosNodos);
}
//elimina el nodo y los bloques de copia que cada archivo contaba en ese nodo

void eliminarNodoYReferencias(t_nodo *nodoAEliminar, t_list *listaNodos, //probada
		t_list *archivos) {
	eliminarReferencias(nodoAEliminar, archivos);
	eliminarNodoDeLista(nodoAEliminar, listaNodos);

}

void eliminarNodoDeLista(t_nodo *nodoAEliminar, t_list *listaNodos) { //probada
	bool mismosNodos(t_nodo *nodoDeLista) {
		return (!strcmp(nodoAEliminar->ipPuerto, nodoDeLista->ipPuerto));
	}

	list_remove_and_destroy_by_condition(listaNodos, (bool*) mismosNodos,
			(void*) liberarNodo);
}
void eliminarReferencias(t_nodo *nodoAEliminar, t_list *archivos) { //probada

	bool copiaEstaEnNodo(t_bloqueEnNodo *copiaDeBloque) {
		return (strcmp(nodoAEliminar->ipPuerto, copiaDeBloque->ipPuerto) == 0);
	}

	void _list_elements2(t_bloqueArch *bloqueDeArch) {
		if (list_is_empty(bloqueDeArch->copiasDeBloque)) {
			//hacerNada
		} else {
			lista_remove_and_destroy_by_condition(bloqueDeArch->copiasDeBloque, //cambiado a funcion lista... para que no explote
					(bool*) copiaEstaEnNodo, (void*) liberarBloqueEnNodo);
		}

	}

	void _list_elements1(t_archivo *unArchivo) {
		list_iterate(unArchivo->bloquesDeArch, (void*) _list_elements2);
	}

	list_iterate(archivos, (void*) _list_elements1);
}

void formatear(t_list **listaNodos, t_list **listaArchivos,	//probada
		t_list **listaDirectorios) {
	list_destroy_and_destroy_elements(*listaArchivos, (void*) liberarArchivo);
	list_destroy_and_destroy_elements(*listaNodos, (void*) liberarNodo);
	list_destroy_and_destroy_elements(*listaDirectorios,
			(void*) liberarDirectorio);
	*listaNodos = list_create();
	*listaArchivos = list_create();
	*listaDirectorios = list_create();
}

void renombrarArchivoPorNombre(char *nombreBuscado, char *nuevoNombre, //probada
		t_list *listaArchivos) {

	int archivoConNombreBuscado(t_archivo *unArchivo) {
		return !strcmp(nombreBuscado, unArchivo->nombre);
	}

//Busco cual es el archivo a modificar, dado el nombre
	t_archivo *archivoAModificar = list_find(listaArchivos,
			(void*) archivoConNombreBuscado);
	if (!archivoAModificar) {
		printf("el archivo %s no se encuentra en el sistema\n", nombreBuscado);
	} else {
		//genero un archivo nuevo, que va a ser el anterior con un nuevo nombre
		free(archivoAModificar->nombre);
		archivoAModificar->nombre = malloc(strlen(nuevoNombre) + 1);
		strcpy(archivoAModificar->nombre, nuevoNombre);
		printf("el archivo %s fue renombrado a %s\n", nombreBuscado,
				nuevoNombre);
	}
}

//Esto de recibir al padre no esta del todo bien. Debería poder mover archivo
//no por número de padre del directorio al que se quiere mover, sino por la
//ruta a la que quiere moverse. Esta ruta sería una cadena de nombre de
//directorio separados por /. Eso me daría el padre del directorio al que quiero
//moverme.
//Respuesta a ramiro: Meeeh, esto lo puede hacer el adapter
void moverArchivoPorNombreYPadre(char *nombreBuscado, t_list *listaArchivos, //probada
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
			printf("el archivo %s no se encuentra en el sistema\n",
					nombreBuscado);
		} else {
			printf("el archivo %s fue movido al directorio con indice %d\n",
					nombreBuscado, padre);
			archivoAModificarPadre->padre = padre;
		}
	}
}

void crearDirectorioDadoPadreYNom(char *nombre, int padre, //probada
		t_list *listaDirectorio) {
	int i;
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

void renombrarDirectorioConNombre(char *nombre, t_directorio *unDirectorio) { //probada
	free(unDirectorio->nombre);
	unDirectorio->nombre = malloc(strlen(nombre) + 1);
	strcpy(unDirectorio->nombre, nombre);
	printf("Directorio fue renombrado correctamente\n");
}
void moverDirectorioConPadre(int padre, t_directorio *unDirectorio) { //probada
	if (!buscarDirPorIndex(padre)) {
		printf("No existe el indice a donde se desea mover\n");
	} else {
		unDirectorio->padre = padre;
		printf("El directorio fue movido correctamente\n");
	}
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
void archivoEstaActivoPorLogica(t_archivo *unArchivo) {
	bool* _tieneAlMenosUnaCopia(t_bloqueArch *unBloqueArch) {
		return list_is_empty(unBloqueArch->copiasDeBloque);
	}

	if (list_all_satisfy(unArchivo->bloquesDeArch,
			(bool*) _tieneAlMenosUnaCopia)) {
		unArchivo->estado = 1;
	} else {
		unArchivo->estado = 0;
	}
}

int archivoActivoPorFlag(t_archivo *unArchivo) {
	return unArchivo->estado;
}

/******************************************/
/********* PRIVATE FUNCTIONS **************/
/******************************************/
static void lista_remove_and_destroy_by_condition(t_list *self,
bool (*condition)(void*), void (*element_destroyer)(void*)) {
	void* data = list_remove_by_condition(self, condition);
	if (data)
		element_destroyer(data);
}
static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente) {
	return dataSiguiente->cantidadBloquesOcupados
			> data->cantidadBloquesOcupados;
}
static void *buscarEnListaPorStrKey(t_list *lista, char *key,
		char *keyGetter(void*)) {
	bool _comparacion(void* data) {
		return (strcmp(keyGetter(data), key)) == 0;
	}

	return list_find(lista, (bool*) _comparacion);
}
static void *buscarEnListaPorIntKey(t_list *lista, int key,
		int *keyGetter(void*)) {
	bool _comparacion(void* data) {
		return keyGetter(data) == key;
	}

	return list_find(lista, (bool*) _comparacion);
}
static char* nodoIpPuerto(t_nodo *unNodo) {
	return unNodo->ipPuerto;
}
static char* dirNombre(t_directorio *unDir) {
	return unDir->nombre;
}
static int dirPadre(t_directorio *unDir) {
	return unDir->padre;
}
static int dirIndex(t_directorio *unDir) {
	return unDir->index;
}
static int archPadre(t_archivo *unArch) {
	return unArch->padre;
}
static char* archNombre(t_archivo *unArch) {
	return unArch->nombre;
}

//XXX posible eliminacion
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
	int i;
	for (i = 2; i <= 1024 && buscarDirPorIndex(i); i++)
		;
	return i;

	/*
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

	 return nuevoIndice;*/
}

static bool existeEseIndiceComoPadre(t_list *listaDirectorios, int padre) {

	bool existePadre(t_directorio *directorio) {
		return (directorio->index == padre);
	}

	return list_any_satisfy(listaDirectorios, (bool*) existePadre);
}

//XXX posible eliminacion
int obtenerArchivo(t_archivo *archivo) {
	if (!(archivo->estado)) {
		printf("Archivo no está disponible.");
		return -1;
	}
	int noEsNull(void* unBloque) {
		return unBloque != NULL;
	}
	int obtenerBloque(t_bloqueArch *bloqueDeArchivo) {
		t_bloqueEnNodo *bloque = list_find(bloqueDeArchivo->copiasDeBloque,
				(void*) noEsNull);
		int ipPuertoCoincide(t_nodo *unNodo) {
			return !strcmp(unNodo->ipPuerto, bloque->ipPuerto);
		}
	t_nodo *nodoEncontrado = list_find(listaNodos, (void*) ipPuertoCoincide);
	if(!nodoEncontrado) return -1;
	t_mensaje *mensaje = malloc(sizeof(t_mensaje));
	mensaje->id=104;
	mensaje->tipo='5';
	mensaje->info = strdup(string_itoa(bloque->numeroDeBloqueEnNodo));
	t_stream *stream = empaquetar_mensaje(mensaje);
	int result;
	result=send(nodoEncontrado->socket,&stream->length,sizeof(int),0);
	if (result<=0) return result;
	send(nodoEncontrado->socket,stream->data,stream->length,0);
	if (result<=0) return result;
	sem_wait(&semaforo);

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
	list_iterate(archivo->bloquesDeArch, (void *) obtenerBloque);
	return 1;
}

static void recorrerCopiasDeUnArch(t_archivo *unArchivo,
		void (*accionACopia)(t_bloqueEnNodo*)) {
	void _recorrerCopias(t_bloqueArch *bloqueArchivo) {
		list_iterate(bloqueArchivo->copiasDeBloque, (void*) accionACopia);
	}
	list_iterate(unArchivo->bloquesDeArch, (void*) _recorrerCopias);
}

static void disminuirNodo(t_bloqueEnNodo *copia) {
	t_nodo *nodo = buscarNodoPorIpPuerto(copia->ipPuerto, listaNodos);
	nodo->cantidadBloquesOcupados = nodo->cantidadBloquesOcupados - 1;
	int *numero = malloc(sizeof(int));
	*numero = copia->numeroDeBloqueEnNodo;
	queue_push(nodo->bloquesLiberados, numero);
}

static bool tieneLugar(t_nodo *unNodo) {
	return unNodo->tamanio / BLOCK_SIZE >= unNodo->cantidadBloquesOcupados;
}
