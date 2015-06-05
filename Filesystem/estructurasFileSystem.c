/*
 funciones para inicializar las estructuras del fileSystem
 */

#include"estructurasFileSystem.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
//funciones para estructura Archivo
/*
 t_archivo nuevoArchivo(char nombre[255], int padre, float tamanio,
 t_list bloquesDeArch, int estado)
 {
 t_archivo unArchivo; // definimos una nueva estructura llamado unArchivo
 
 // asignacion de  campos a la estructura
 strcpy(unArchivo.nombre,nombre); //la forma de insetar valores a un tipo char en una estructura es usando strcpy de la libreria string.h
 unArchivo.padre = padre;
 unArchivo.tamanio = tamanio;
 unArchivo.bloquesDeArch = bloquesDeArch;
 unArchivo.estado = estado;
 //devuelvo el archivo
 return unArchivo;
 }*/

t_bloqueEnNodo nuevoBloqueEnNodo(char ip[255], int puerto,
		int numeroDeBloqueEnNodo) {
	t_bloqueEnNodo unBloqueEnNodo; //nueva estructura
	//asignacion de valores a la estructura
	strcpy(unBloqueEnNodo.ip, ip);
	unBloqueEnNodo.puerto = puerto;
	unBloqueEnNodo.numeroDeBloqueEnNodo = numeroDeBloqueEnNodo;
	//devuelvo la estructura
	return unBloqueEnNodo;
}

//funciones para la estructura nodo
t_nodo nuevoNodo(char ip[255], int puerto, int tamanio) {
	t_nodo unNodo;  //nueva estructura
	//asignacion de campos a la estructura
	strcpy(unNodo.ip, ip);
	unNodo.puerto = puerto;
	unNodo.tamanio = tamanio;
	unNodo.cantidadBloquesOcupados = 0; //SE SUPONE QUE SI EL NODO ES NUEVO NO TIENE BLOQUES ESCRITOS
	unNodo.activo = 1; //SE SUPONE QUE SI EL NODO SE ESTA CONECTANDO, ENTONCES ESTA ACTIVO
	unNodo.bloquesLiberados = queue_create();
	//devuelvo la estructura
	return unNodo;
}

void activarNodo(t_nodo unNodo) {
	unNodo.activo = 1;
}
void desactivarNodo(t_nodo unNodo) {
	unNodo.activo = 0;
}
//funciones para la estructura tabla de directorios

t_directorio nuevoDirectorio(int index, char nombre[255], int padre) {
	t_directorio unDirectorio;
	//asignacion de campos a la estructura
	unDirectorio.index = index;
	strcpy(unDirectorio.nombre, nombre);
	unDirectorio.padre = padre;
	//devuelvo la estructura
	return unDirectorio;
}

//Funciones Para liberar
void liberarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	free(&bloqueEnNodo->ip);
	free(&bloqueEnNodo->numeroDeBloqueEnNodo);
	free(&bloqueEnNodo->puerto);
	free(bloqueEnNodo);
}

void liberarBloqueArch(t_bloqueArch *bloqueArch) {
	list_destroy_and_destroy_elements(&(bloqueArch->copiasDeBloque),
			(void*) liberarBloqueEnNodo);
	free(bloqueArch);
}

void liberarArchivo(t_archivo *unArchivo) {
	list_destroy_and_destroy_elements(&(unArchivo->bloquesDeArch),
			(void*) liberarBloqueArch);
	free(&unArchivo->estado);
	free(&unArchivo->nombre);
	free(&unArchivo->padre);
	free(&unArchivo->tamanio);
	free(unArchivo);
}

void liberarNodo(t_nodo *unNodo) {
	queue_destroy_and_destroy_elements(unNodo->bloquesLiberados, (void*) free);
	free(&unNodo->activo);
	free(&unNodo->cantidadBloquesOcupados);
	free(&unNodo->ip);
	free(&unNodo->puerto);
	free(&unNodo->tamanio);
	free(unNodo);
}

void liberarDirectorio(t_directorio *unDirectorio) {
	free(&unDirectorio->index);
	free(&unDirectorio->nombre);
	free(&unDirectorio->padre);
	free(unDirectorio);
}

//Funciones para mostrar estructuras por pantalla

void mostrarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	printf("\tIp: %s\n", bloqueEnNodo->ip);
	printf("\tPuerto: %d\n", bloqueEnNodo->puerto);
	printf("\tNumeroDeBloqueEnNodo %d\n", bloqueEnNodo->numeroDeBloqueEnNodo);
}

void mostrarBloqueArch(t_bloqueArch *bloqueArch) {
	printf("\n\n\n");
	list_iterate(&(bloqueArch->copiasDeBloque), (void*) mostrarBloqueEnNodo);
}

void mostrarArchivo(t_archivo *unArchivo) {
	printf("Estado: %d\n", unArchivo->estado);
	printf("Nombre: %s\n", unArchivo->nombre);
	printf("Padre: %d\n", unArchivo->padre);
	printf("Tamanio: %f\n", unArchivo->tamanio);
	list_iterate(&(unArchivo->bloquesDeArch), (void*) mostrarBloqueArch);
}

void mostrarColaDeInt(t_nodo *unNodo) {
	if (queue_is_empty(unNodo->bloquesLiberados)) {
		printf("No hay bloques liberados en el medio en el nodo");
	} else {
		int *primeroAux = queue_peek(unNodo->bloquesLiberados);
		int *aux = queue_pop(unNodo->bloquesLiberados);
		printf("BloqueLiberado: %d\n", *primeroAux);
		queue_push(unNodo->bloquesLiberados, primeroAux);
		while (queue_peek(unNodo->bloquesLiberados) != primeroAux) {
			aux = queue_pop(unNodo->bloquesLiberados);
			queue_push(unNodo->bloquesLiberados, aux);
			printf("BloqueLiberado: %d\n", *aux);
		}
	}
}

void mostrarNodo(t_nodo *unNodo) {
	printf("Activo: %d\n", unNodo->activo);
	printf("CantidadBloquesOcupados: %d\n", unNodo->cantidadBloquesOcupados);
	printf("Ip: %s\n", unNodo->ip);
	printf("Puerto: %d\n", unNodo->puerto);
	printf("Tamanio: %f\n", unNodo->tamanio);
	mostrarColaDeInt(unNodo);
}

void mostrarDirectorio(t_directorio *unDirectorio) {
	printf("Index: %d\n", unDirectorio->index);
	printf("Nombre: %s\n", unDirectorio->nombre);
	printf("Padre: %d\n", unDirectorio->padre);
}

