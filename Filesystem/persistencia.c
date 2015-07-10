/*
 * persistencia.c
 *
 *  Created on: 9/7/2015
 *      Author: utnso
 */
#include "persistencia.h"

static void escribirCola(t_queue *cola, FILE * fp);
static void leerCola(t_queue **cola, FILE * fp) ;
static void fwrite_str(char* string,FILE *fp);
static void fread_str(char** string,FILE *fp);

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
//persistencia para nodos
void guardarNodo(t_nodo *unNodo) {
	fwrite(&unNodo->id, sizeof(unNodo->id), 1, fpNodos);
	fwrite(&unNodo->tamanio, sizeof(unNodo->tamanio), 1, fpNodos);
	fwrite(&unNodo->socket, sizeof(unNodo->socket), 1, fpNodos);
	fwrite(&unNodo->cantidadBloquesOcupados, sizeof(unNodo->cantidadBloquesOcupados), 1, fpNodos);
	fwrite(&unNodo->activo, sizeof(unNodo->activo), 1, fpNodos);
	escribirCola(unNodo->bloquesLiberados, fpNodos);
}

t_nodo *cargarNodo() {
	t_nodo *unNodo = malloc(sizeof(t_nodo)); //nueva estructura
	unNodo->bloquesLiberados = queue_create();
	fread(&unNodo->id, sizeof(unNodo->id), 1, fpNodos);
	fread(&unNodo->tamanio, sizeof(unNodo->tamanio), 1, fpNodos);
	fread(&unNodo->socket, sizeof(unNodo->socket), 1, fpNodos);
	fread(&unNodo->cantidadBloquesOcupados, sizeof(unNodo->cantidadBloquesOcupados), 1, fpNodos);
	fread(&unNodo->activo, sizeof(unNodo->activo), 1, fpNodos);
	leerCola(&unNodo->bloquesLiberados, fpNodos);
	return unNodo;

}

void guardarListaNodos() {
	fpNodos = fopen("nodos", "w+");//era wb+
	list_iterate(listaNodos,(void*)guardarNodo);
	fclose(fpNodos);
}

void cargarListaNodos() {
	fpNodos = fopen("nodos", "r+");//probablemente en vez de r+ así si no existe lo crea
	t_nodo *unNodo;
	while(!feof(fpNodos)) {
		unNodo = cargarNodo();
		if (!feof(fpNodos)) list_add(listaNodos,unNodo);
		//esto lo tengo que hacer así porque el eof da true cuando se paso, entonces la ultima estructura podria estar mal. Por lo tanto no quiero agregarla a la lista
	}
	fclose(fpNodos);
}


//persistencia para directorios
void guardarDirectorio(t_directorio *unDir) {
	fwrite(&unDir->index, sizeof(unDir->index), 1, fpDir);
	fwrite_str(unDir->nombre,fpDir);
	fwrite(&unDir->padre, sizeof(unDir->padre), 1, fpDir);
}
t_directorio *cargarDirectorio() {
	t_directorio *unDir = malloc(sizeof(t_directorio)); //nueva estructura
	fread(&unDir->index, sizeof(unDir->index), 1, fpDir);
	fread_str(&unDir->nombre,fpDir);
	fread(&unDir->padre, sizeof(unDir->padre), 1, fpDir);
	return unDir;
}

void guardarListaDirectorios() {
	fpDir = fopen("directorios", "w");//era wb+
	list_iterate(listaDirectorios,(void*)guardarDirectorio);
	fclose(fpDir);
}

void cargarListaDirectorios() {
	fpDir = fopen("directorios", "r");//probablemente en vez de r+ así si no existe lo crea
	t_directorio *unDir;
	while(!feof(fpDir)) {
		unDir = cargarDirectorio();
		if (!feof(fpDir)) list_add(listaDirectorios,unDir);
		//esto lo tengo que hacer así porque el eof da true cuando se paso, entonces la ultima estructura podria estar mal. Por lo tanto no quiero agregarla a la lista
	}
	fclose(fpDir);
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
static void escribirCola(t_queue *cola, FILE * fp){
	int tamanioCola;
	void escribirEntero(int *entero){
		printf("%d\n",*entero);
		fflush(stdout);
		fwrite(entero, sizeof(int), 1, fp);
	}
	tamanioCola = queue_size(cola);
	fwrite(&tamanioCola, sizeof(int), 1, fpNodos);
	if(!queue_is_empty(cola)) list_iterate(cola->elements,(void*) escribirEntero);
}
static void leerCola(t_queue **cola, FILE * fp) {
	int *tamanioCola = malloc(sizeof(int));
	fread(tamanioCola, sizeof(int), 1, fpNodos);
	while(*tamanioCola){
		int *numero = malloc(sizeof(int));
		fread(numero, sizeof(int), 1, fpNodos);
		queue_push(*cola, numero);
		*tamanioCola = *tamanioCola - 1;
	}
	free(tamanioCola);
}

static void fwrite_str(char* string,FILE *fp) {
	int length =(strlen(string)+1);
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(string, sizeof(char), length, fp);
}
static void fread_str(char** string,FILE *fp) {
	int length;
	fread(&length, sizeof(int), 1, fpDir);
	*string = malloc(length);
	fread(*string, sizeof(char), length, fpDir);
}
