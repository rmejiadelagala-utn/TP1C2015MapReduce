/*
 * funcionesFS.c
 *
 *  Created on: 19/7/2015
 *      Author: utnso
 */

#include "funcionesFS.h"


static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente);
static void setBloquencio(t_nodo* nodo);
static t_queue* duplicarColaInt(t_queue* cola);
static t_nodo* duplicarNodo(t_nodo* unNodo);
static t_list* duplicarListaNodo();
static int nodoSeleccionado(t_list *nodosOrdenados, t_nodo **nodoActual, int *posicion);
static bool tieneLugar(t_nodo *unNodo);
static int estaActivo(t_nodo* unNodo);

int hayLugarEnLosNodos(char *data) {

log_info(mdfs_logger,"Verificando si hay espacio en los nodos");

int i, fin = 0;
int comienzoDeBloque = 0, finDeBloque;
t_nodo *nodoActual;
int ultimoIndiceDelData = string_length(data)-1;//Juanchi dice que -1 no va

t_list *nodosOrdenados = list_create();
pthread_mutex_lock(&listaDeNodos);
//	list_add_all(nodosOrdenados, listaNodos);//Agrega todos los elementos de la segunda lista en la primera
nodosOrdenados = list_filter(listaNodos,(void*) estaActivo);
list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);
t_list* listaNodosAux = duplicarListaNodo(nodosOrdenados);
pthread_mutex_unlock(&listaDeNodos);

	while (!fin) {

		finDeBloque = comienzoDeBloque + BLOCK_SIZE;

		if (finDeBloque > ultimoIndiceDelData) {

			finDeBloque = ultimoIndiceDelData;
			fin = 1;
		}	//Sale si ya no hay bloques


		while (data[finDeBloque] != '\n' && (!fin)){
			finDeBloque--;
		}

		//Acá tengo el final del bloque dado, y también donde empieza

		//ordenar lista nodo por cantidad de bloques usados-->sale nodosOrdenados
		list_sort(listaNodosAux, (void*) ordenarPorMenorUso);

		int k = 0;
		//Acá distribuye las copias dado el algoritmo de dstribucion
		for (i = 0; i < CANT_COPIAS; i++) {
			//--Mandar este bloque al nodo que corresponda--

			//nodoActual = nodoElegdoYConLugar(nodosOrdenados);
			if (nodoSeleccionado(listaNodosAux, &nodoActual,&k) == 0) {//XXX
				//salio bien el elegir nodo, estando en nodoActual
			setBloquencio(nodoActual);

			} else {
//no hay nodos disponibles
				list_destroy_and_destroy_elements(listaNodosAux,(void*)liberarNodo);
				return -1;
			}
			k++;
		}
		//cambiar el bloque start al siguiente y agrega el Bloque a la lista de bloques
		comienzoDeBloque = finDeBloque + 1;

	}
	list_destroy_and_destroy_elements(listaNodosAux,(void*)liberarNodo);
	list_destroy(nodosOrdenados);
	return 0;
}




static void setBloquencio(t_nodo* nodo){
	if (!queue_is_empty(nodo->bloquesLiberados)) {
		queue_pop(nodo->bloquesLiberados);
	}
	nodo->cantidadBloquesOcupados++;
}



static t_list* duplicarListaNodo() {
	t_list* listaDuplicada = list_create();

	void duplicarAgregarNodo(t_nodo *unNodo){
		t_nodo *nodoDuplicado = duplicarNodo(unNodo);
		list_add(listaDuplicada,nodoDuplicado);
	}


	list_iterate(listaNodos,(void*)duplicarAgregarNodo);
	return listaDuplicada;
}

static t_nodo* duplicarNodo(t_nodo* unNodo) {
	t_nodo *nodoDuplicado = malloc(sizeof(t_nodo));
	nodoDuplicado->activo = unNodo->activo;
	nodoDuplicado->bloquesLiberados = duplicarColaInt(unNodo->bloquesLiberados);
	nodoDuplicado->cantidadBloquesOcupados = unNodo->cantidadBloquesOcupados;
	nodoDuplicado->id = unNodo->id;
	nodoDuplicado->socket = unNodo->socket;
	nodoDuplicado->tamanio = unNodo->tamanio;
	return nodoDuplicado;
}

static t_queue* duplicarColaInt(t_queue* cola) {
	t_queue* colaDuplicada = queue_create();

	void agregarEnteros(int *numero) {//XXX ojito
		queue_push(colaDuplicada, numero);
	}

	list_iterate(cola->elements, (void*)agregarEnteros);
	return colaDuplicada;
}



static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente) {
	return dataSiguiente->cantidadBloquesOcupados
			> data->cantidadBloquesOcupados;
}

static int nodoSeleccionado(t_list *nodosOrdenados, t_nodo **nodoActual, int *posicion) {
	*nodoActual = list_get(nodosOrdenados, *posicion);
	int fin = 0;
	while (list_size(nodosOrdenados)>= 3 && !fin) {
		*nodoActual = list_get(nodosOrdenados, *posicion);
		if (!tieneLugar(*nodoActual)){
			list_remove_and_destroy_element(nodosOrdenados, *posicion,(void*)liberarNodo);
			*posicion = *posicion - 1;
		}
		else {
			fin = 1;
		}
		*posicion = *posicion + 1;//con el ++ se quejaba eclipse
	}
	if(list_size(nodosOrdenados)< 3) return -1;
	*posicion = *posicion - 1;
	return 0;
}

static bool tieneLugar(t_nodo *unNodo) {
	return unNodo->tamanio > (unNodo->cantidadBloquesOcupados * BLOCK_SIZE + BLOCK_SIZE);
}

static int estaActivo(t_nodo* unNodo){
	return unNodo->activo;
}
