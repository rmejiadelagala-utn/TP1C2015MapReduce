/*
 funciones para inicializar las estructuras del fileSystem
 */

#include"estructurasFileSystem.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
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

t_bloqueEnNodo *nuevoBloqueEnNodo(char *ipPuerto, int numeroDeBloqueEnNodo) {
	t_bloqueEnNodo *unBloqueEnNodo = malloc(sizeof(t_bloqueEnNodo)); //nueva estructura
	unBloqueEnNodo->ipPuerto = malloc(strlen(ipPuerto) + 1);
	//asignacion de valores a la estructura
	strcpy(unBloqueEnNodo->ipPuerto, ipPuerto);
	unBloqueEnNodo->numeroDeBloqueEnNodo = numeroDeBloqueEnNodo;
	//devuelvo la estructura
	return unBloqueEnNodo;
}

//funciones para la estructura nodo
t_nodo *nuevoNodo(char *ipPuerto, int tamanio) {
	t_nodo *unNodo = malloc(sizeof(t_nodo));  //nueva estructura
	unNodo->ipPuerto = malloc(strlen(ipPuerto) + 1);
	//asignacion de campos a la estructura
	strcpy(unNodo->ipPuerto, ipPuerto);
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

//Funciones Para liberar
void liberarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	free(bloqueEnNodo->ipPuerto);
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
	free(unNodo->ipPuerto);
	free(unNodo);
}

void liberarDirectorio(t_directorio *unDirectorio) {
	free(unDirectorio->nombre);
	free(unDirectorio);
}

//Funciones para mostrar estructuras por pantalla

void mostrarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	printf("\tIpPuerto: %s\n", bloqueEnNodo->ipPuerto);
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

void mostrarColaDeInt(t_nodo *unNodo) {
	if (queue_is_empty(unNodo->bloquesLiberados)) {
		printf("No hay bloques liberados en el medio en el nodo\n\n");
	} else {
		int *primeroAux = queue_peek(unNodo->bloquesLiberados);
		int *aux = queue_pop(unNodo->bloquesLiberados);
		printf("BloqueLiberado: %d\n", *primeroAux);
		queue_push(unNodo->bloquesLiberados, primeroAux);
		while (queue_peek(unNodo->bloquesLiberados) != primeroAux) {
			aux = queue_pop(unNodo->bloquesLiberados);
			queue_push(unNodo->bloquesLiberados, aux);
			printf("BloqueLiberado: %d\n\n", *aux);
		}
	}
}

void mostrarNodo(t_nodo *unNodo) {
	printf("Activo: %d\n", unNodo->activo);
	printf("CantidadBloquesOcupados: %d\n", unNodo->cantidadBloquesOcupados);
	printf("IpPuerto: %s\n", unNodo->ipPuerto);
	printf("Tamanio: %f\n", unNodo->tamanio);
	mostrarColaDeInt(unNodo);
}

void mostrarDirectorio(t_directorio *unDirectorio) {
	printf("Index: %d\n", unDirectorio->index);
	printf("Nombre: %s\n", unDirectorio->nombre);
	printf("Padre: %d\n\n", unDirectorio->padre);
}
void mostrarLista(t_list *unaLista, void (*shower)(void*)) {
	list_iterate(unaLista, shower);
}
