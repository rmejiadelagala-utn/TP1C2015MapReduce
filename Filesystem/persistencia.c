/*
 * persistencia.c
 *
 *  Created on: 9/7/2015
 *      Author: utnso
 */
#include "persistencia.h"

static void escribirCola(FILE * fp,t_queue *cola);

//Funciones para guardar estructuras basicas

void guardarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	fwrite(&bloqueEnNodo->id, sizeof(bloqueEnNodo->id), 1, fpArch);
	fwrite(&bloqueEnNodo->numeroDeBloqueEnNodo, sizeof(bloqueEnNodo->numeroDeBloqueEnNodo), 1, fpArch);
}
void cargarBloqueEnNodo() {
	t_bloqueEnNodo *unBloqueEnNodo = malloc(sizeof(t_bloqueEnNodo)); //nueva estructura
	fread(&unBloqueEnNodo->id, sizeof(unBloqueEnNodo->id), 1, fpArch);
	fread(&unBloqueEnNodo->numeroDeBloqueEnNodo, sizeof(unBloqueEnNodo->numeroDeBloqueEnNodo), 1, fpArch);
	mostrarBloqueEnNodo(unBloqueEnNodo);
}

void guardarNodo(t_nodo *unNodo) {
	fwrite(&unNodo->id, sizeof(unNodo->id), 1, fpNodos);
	fwrite(&unNodo->tamanio, sizeof(unNodo->tamanio), 1, fpNodos);
	fwrite(&unNodo->socket, sizeof(unNodo->socket), 1, fpNodos);
	fwrite(&unNodo->cantidadBloquesOcupados, sizeof(unNodo->cantidadBloquesOcupados), 1, fpNodos);
	fwrite(&unNodo->activo, sizeof(unNodo->activo), 1, fpNodos);
	//fwrite(&(queue_size(unNodo->bloquesLiberados)), sizeof(int), 1, fpNodos);
	//FIXME posiblemente, para mi va como la de arriba pero si pongo así el bastardo me tira error
	fwrite(queue_size(unNodo->bloquesLiberados), sizeof(int), 1, fpNodos);
	escribirCola(fpNodos,unNodo->bloquesLiberados);
}
void cargarNodo() {
	t_nodo *unNodo = malloc(sizeof(t_nodo)); //nueva estructura
	unNodo->bloquesLiberados = queue_create();
	int i;
	int *numero = malloc(sizeof(int));
	fread(&unNodo->id, sizeof(unNodo->id), 1, fpNodos);
	fread(&unNodo->tamanio, sizeof(unNodo->tamanio), 1, fpNodos);
	fread(&unNodo->socket, sizeof(unNodo->socket), 1, fpNodos);
	fread(&unNodo->cantidadBloquesOcupados, sizeof(unNodo->cantidadBloquesOcupados), 1, fpNodos);
	fread(&unNodo->activo, sizeof(unNodo->activo), 1, fpNodos);
	fread(&i, sizeof(int), 1, fpNodos);
	while(i){
		fread(numero, sizeof(int), 1, fpNodos);
		queue_push(unNodo->bloquesLiberados, numero);
		i--;
	}
	list_add(listaNodos,unNodo);
}

void guardarDirectorio(t_directorio *unDir) {
	fwrite(&unDir->index, sizeof(unDir->index), 1, fpDir);
	fwrite(strlen(unDir->nombre)+1, sizeof(int), 1, fpDir);
	fwrite(&unDir->nombre, strlen(unDir->nombre)+1, 1, fpDir);
	fwrite(&unDir->padre, sizeof(unDir->padre), 1, fpDir);
}
void cargarDirectorio() {
	t_directorio *unDir = malloc(sizeof(t_directorio)); //nueva estructura
	int length;
	//asignacion de valores a la estructura
	fread(&unDir->index, sizeof(unDir->index), 1, fpArch);
	fread(&length, sizeof(int), 1, fpArch);
	unDir->nombre = malloc(length);
	fread(&unDir->nombre, length, 1, fpArch);
	fread(&unDir->padre, sizeof(unDir->padre), 1, fpDir);
	list_add(listaDirectorios, unDir);
}
/*
void mostrarBloqueArch(t_bloqueArch *bloqueArch) {
	fprintf(fpArch, "%d\n", list_size(bloqueArch->copiasDeBloque));
	list_iterate((bloqueArch->copiasDeBloque), (void*) mostrarBloqueEnNodo);
}

void mostrarArchivo(t_archivo *unArchivo) {
	printf("Estado: %d\n", unArchivo->estado);
	printf("Nombre: %s\n", unArchivo->nombre);
	printf("Padre: %d\n", unArchivo->padre);
	printf("Tamanio: %f\n", unArchivo->tamanio);
	list_iterate((unArchivo->bloquesDeArch), (void*) mostrarBloqueArch);
}

void mostrarNombreArchivo(t_archivo *unArchivo){
	printf("Archivo:\t");
	printf("%s\n",unArchivo->nombre);
}

void mostrarLista(t_list *unaLista, void (*shower)(void*)) {
	list_iterate(unaLista, shower);
}*/



























/******************************************/
/********* PRIVATE FUNCTIONS **************/
/******************************************/
static void escribirCola(FILE * fp,t_queue *cola){
	void escribirEntero(int entero){
		fwrite(&entero, sizeof(int), 1, fp);
	}
	if(!queue_is_empty(cola)) list_iterate(cola->elements,(void*) escribirEntero);
}
