/*
 funciones para inicializar las estructuras del fileSystem
 */

#include"estructurasFileSystem.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

static void mostrarEntero(int *numero);
static void mostrarColaDeInt(t_queue *cola);
//funciones para estructura Archivo

t_archivo *nuevoArchivo(char *nombre, int padre, float tamanio,
		t_list *bloquesDeArch, int estado) {

	t_archivo *unArchivo = malloc(sizeof(t_archivo)); // definimos una nueva estructura llamado unArchivo
	unArchivo->nombre = malloc(strlen(nombre) + 1);
	// asignacion de  campos a la estructura
	strcpy(unArchivo->nombre, nombre); //la forma de insetar valores a un tipo char en una estructura es usando strcpy de la libreria string.h
	unArchivo->padre = padre;
	unArchivo->tamanio = tamanio;
	unArchivo->bloquesDeArch = bloquesDeArch;
	unArchivo->estado = estado;
	//devuelvo el archivo
	return unArchivo;
}
t_bloqueArch *nuevoBloqueArchivo(t_list *copiasDeBloques) {
	t_bloqueArch *bloqueArchivo = malloc(sizeof(t_bloqueArch));
	bloqueArchivo->copiasDeBloque = copiasDeBloques;
	return bloqueArchivo;
}

t_bloqueEnNodo *nuevoBloqueEnNodo(int id, int numeroDeBloqueEnNodo) {
	t_bloqueEnNodo *unBloqueEnNodo = malloc(sizeof(t_bloqueEnNodo)); //nueva estructura
	unBloqueEnNodo->id = id;
	//asignacion de valores a la estructura
	unBloqueEnNodo->numeroDeBloqueEnNodo = numeroDeBloqueEnNodo;
	//devuelvo la estructura
	return unBloqueEnNodo;
}

//funciones para la estructura nodo
t_nodo *nuevoNodo(int id, int tamanio) {
	t_nodo *unNodo = malloc(sizeof(t_nodo));  //nueva estructura
	//asignacion de campos a la estructura
	unNodo->id = id;
	unNodo->tamanio = tamanio;
	unNodo->cantidadBloquesOcupados = 0; //SE SUPONE QUE SI EL NODO ES NUEVO NO TIENE BLOQUES ESCRITOS
	unNodo->activo = 1; //SE SUPONE QUE SI EL NODO SE ESTA CONECTANDO, ENTONCES ESTA ACTIVO
	unNodo->bloquesLiberados = queue_create();
	//devuelvo la estructura
	return unNodo;
}

void activarNodo(t_nodo *unNodo) {
	unNodo->activo = 1;
}
void desactivarNodo(t_nodo *unNodo) {
	unNodo->activo = 0;
}

t_nodo* string_to_nodo(char* infoDeNodo,int socket){
			char** mensajePartido = string_split(infoDeNodo, ":");
			char* ipPuerto = strcat(strcat(mensajePartido[0], ":"),	mensajePartido[1]);
			t_nodo* nodo = nuevoNodo(ipPuerto, 50);
			nodo->socket = socket;
			return nodo;
}
//funciones para la estructura tabla de directorios

t_directorio *nuevoDirectorio(int index, char *nombre, int padre) {
	t_directorio *unDirectorio = malloc(sizeof(t_directorio));
	unDirectorio->nombre = malloc(strlen(nombre) + 1);
	//asignacion de campos a la estructura
	unDirectorio->index = index;
	strcpy(unDirectorio->nombre, nombre);
	unDirectorio->padre = padre;
	//devuelvo la estructura
	return unDirectorio;
}

int esHijo(t_directorio *dirHijo, t_directorio *dirPadre){
	return dirHijo->padre==dirPadre->index;
}

//Funciones Para liberar
void liberarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	free(bloqueEnNodo);
}

void liberarBloqueArch(t_bloqueArch *bloqueArch) {
	list_destroy_and_destroy_elements(bloqueArch->copiasDeBloque,
			(void*) liberarBloqueEnNodo);
	free(bloqueArch);
}

void liberarArchivo(t_archivo *unArchivo) {
	list_destroy_and_destroy_elements((unArchivo->bloquesDeArch),
			(void*) liberarBloqueArch);
	free(unArchivo->nombre);
	free(unArchivo);
}


void liberarNodo(t_nodo *unNodo) {
	queue_destroy_and_destroy_elements(unNodo->bloquesLiberados, (void*) free);
	free(unNodo);
}

void liberarDirectorio(t_directorio *unDirectorio) {
	free(unDirectorio->nombre);
	free(unDirectorio);
}

//Funciones para mostrar estructuras por pantalla

void mostrarRegistro(t_registro_id_ipPuerto *registro) {
	printf("\tId del Nodo: %d\n", registro->id);
}

void mostrarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	printf("\tId del Nodo: %d\n", bloqueEnNodo->id);
	printf("\tNumeroDeBloqueEnNodo %d\n\n", bloqueEnNodo->numeroDeBloqueEnNodo);
}

void mostrarBloqueArch(t_bloqueArch *bloqueArch) {
	printf("\n\n\n");
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

void mostrarNodo(t_nodo *unNodo) {
	printf("Activo: %d\n", unNodo->activo);
	printf("CantidadBloquesOcupados: %d\n", unNodo->cantidadBloquesOcupados);
	printf("ID: %d\n", unNodo->id);
	printf("Tamanio: %f\n", unNodo->tamanio);
	mostrarColaDeInt(unNodo->bloquesLiberados);
}

void mostrarDirectorio(t_directorio *unDirectorio) {
	printf("Index: %d\n", unDirectorio->index);
	printf("Nombre: %s\n", unDirectorio->nombre);
	printf("Padre: %d\n\n", unDirectorio->padre);
}

void mostrarNombreDirectorio(t_directorio *unDirectorio){
	printf("%d\t\t",unDirectorio->index);
	printf("%s\n",unDirectorio->nombre);
}

void mostrarLista(t_list *unaLista, void (*shower)(void*)) {
	list_iterate(unaLista, shower);
}

/******************************************/
/********* PRIVATE FUNCTIONS **************/
/******************************************/

static void mostrarEntero(int *numero) {
	printf("%d\t", *numero);
}
static void mostrarColaDeInt(t_queue *cola) {
	if (queue_is_empty(cola)) {
		printf("No hay bloques liberados en el medio en el nodo");
	} else {/*
	 int *primeroAux = malloc(sizeof(int));
	 int *aux = malloc(sizeof(int));
	 primeroAux = queue_peek(cola);
	 aux = queue_pop(cola);
	 printf("BloquesLiberados: %d\t", *aux);
	 queue_push(cola, primeroAux);
	 while (queue_peek(cola) != primeroAux) {
	 aux = queue_pop(cola);
	 printf("%d\t", *aux);
	 queue_push(cola, aux);

	 }
	 free(primeroAux);
	 free (aux);*/
		printf("BloquesLiberados:\t");
		mostrarLista(cola->elements, (void*) mostrarEntero);
	}
	printf("\n\n");
}
