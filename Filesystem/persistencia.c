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
static char* fread_str(FILE *fp);
static void fwrite_list(t_list *subList, FILE *fp,void(*struct_writer)(void*));
static t_list* fread_list(FILE *fp,void*(*struct_reader)());


//Funciones para guardar estructuras basicas
//Persistencia para archivos


/*
void guardarListabloqueEnNodo(t_list *listaBloqueEnNodo) {
	int i = list_size(listaBloqueEnNodo);
	fwrite(&i, sizeof(int), 1, fpArch);
	list_iterate(listaBloqueEnNodo,(void*)guardarNodo);
}

t_list* cargarListabloqueEnNodo () {
	t_list* listaBloqueEnNodo = list_create();
	int length;
	fread(&length, sizeof(int), 1, fpDir);
	while(length) {
		t_bloqueEnNodo *unBloqueEnNodo = cargarBloqueEnNodo();
		list_add(listaBloqueEnNodo,unBloqueEnNodo);
		length--;
	}
	return listaBloqueEnNodo;
}*/


void guardarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) {
	fwrite(&bloqueEnNodo->id, sizeof(bloqueEnNodo->id), 1, fpArch);
	fwrite(&bloqueEnNodo->numeroDeBloqueEnNodo, sizeof(bloqueEnNodo->numeroDeBloqueEnNodo), 1, fpArch);
}

void guardarBloqueDeArch(t_bloqueArch *unBloqueDeArch) {
	fwrite_list(unBloqueDeArch->copiasDeBloque,fpArch,(void*)guardarBloqueEnNodo);
}

void guardarArchivo(t_archivo *unArchivo){
	fwrite(&unArchivo->estado, sizeof(unArchivo->estado), 1, fpArch);
	fwrite_str(unArchivo->nombre,fpArch);
	printf("nombre arch: %s\n",unArchivo->nombre);
	fflush(stdout);
	fwrite(&unArchivo->padre, sizeof(unArchivo->padre), 1, fpArch);
	fwrite(&unArchivo->tamanio, sizeof(unArchivo->tamanio), 1, fpArch);
	fwrite_list(unArchivo->bloquesDeArch,fpArch,(void*)guardarBloqueDeArch);

}

void guardarListaArchivos() {
	fpArch = fopen("archivos", "w");
	fwrite_list(listaArchivos, fpArch,(void*)guardarArchivo);
	fclose(fpArch);
}

t_bloqueEnNodo *cargarBloqueEnNodo() {
	t_bloqueEnNodo *unBloqueEnNodo = malloc(sizeof(t_bloqueEnNodo)); //nueva estructura
	fread(&unBloqueEnNodo->id, sizeof(unBloqueEnNodo->id), 1, fpArch);
	fread(&unBloqueEnNodo->numeroDeBloqueEnNodo, sizeof(unBloqueEnNodo->numeroDeBloqueEnNodo), 1, fpArch);
	return unBloqueEnNodo;
}

t_bloqueArch *cargarBloqueDeArch() {
	t_bloqueArch *unBloqueDeArch = malloc(sizeof(t_bloqueArch));
	unBloqueDeArch->copiasDeBloque = fread_list(fpArch,(void*)cargarBloqueEnNodo);
	return unBloqueDeArch;
}


t_archivo *cargarArchivo(){

	t_archivo *unArchivo = malloc(sizeof(t_archivo));
	printf("cargarArchivo\n");
	fflush(stdout);
	fread(&unArchivo->estado, sizeof(unArchivo->estado), 1, fpArch);
	printf("cargarArchivo estado %d\n",unArchivo->estado);
	fflush(stdout);
	unArchivo->nombre = fread_str(fpArch);
	printf("nombre :%s!!!!!!!!!!!!!!!\n",unArchivo->nombre);
	fflush(stdout);
	fread(&unArchivo->padre, sizeof(unArchivo->padre), 1, fpArch);
	fread(&unArchivo->tamanio, sizeof(unArchivo->tamanio), 1, fpArch);
	unArchivo->bloquesDeArch = fread_list(fpArch,(void*)cargarBloqueDeArch);
	return unArchivo;
}
void cargarListaArchivos() {
	fpArch = fopen("archivos", "r");
	listaArchivos = fread_list( fpArch,(void*)cargarArchivo);
	fclose(fpArch);
}
/*void guardarListaArchivos() {
	fpArch = fopen("archivos", "w+");//era wb+
	list_iterate(listaArchivos,(void*)guardarArchivo);
	fclose(fpArch);
}

void cargarListaArchivos() {
	fpArch = fopen("archivos", "r+");//probablemente en vez de r+ así si no existe lo crea
	t_archivo *unArchivo;
	while(!feof(fpArch)) {
		unArchivo = cargarArchivo();
		if (!feof(fpArch)) list_add(listaArchivos,unArchivo);
		//esto lo tengo que hacer así porque el eof da true cuando se paso, entonces la ultima estructura podria estar mal. Por lo tanto no quiero agregarla a la lista
	}
	fclose(fpArch);
}

*/



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
	fpNodos = fopen("nodos", "w");
	fwrite_list(listaNodos, fpNodos,(void*)guardarNodo);
	fclose(fpNodos);
}

void cargarListaNodos() {
	fpNodos = fopen("nodos", "r");
	listaNodos = fread_list( fpNodos,(void*)cargarNodo);
	fclose(fpNodos);
}
/*
void guardarListaNodos() {
	fpNodos = fopen("nodos", "w+");//era wb+
	int length;
	fwrite(&length, sizeof(length), 1, fpNodos);
	list_iterate(listaNodos,(void*)guardarNodo);
	fclose(fpNodos);
}

void cargarListaNodos() {
	fpNodos = fopen("nodos", "r+");//probablemente en vez de r+ así si no existe lo crea
	t_nodo *unNodo;
	int length;
	fwrite(&length, sizeof(length), 1, fpNodos);
	while(!feof(fpNodos)) {
		unNodo = cargarNodo();
		if (!feof(fpNodos)) list_add(listaNodos,unNodo);
		//esto lo tengo que hacer así porque el eof da true cuando se paso, entonces la ultima estructura podria estar mal. Por lo tanto no quiero agregarla a la lista
	}
	fclose(fpNodos);
}

*/
//persistencia para directorios
void guardarDirectorio(t_directorio *unDir) {
	fwrite(&unDir->index, sizeof(unDir->index), 1, fpDir);
	fwrite_str(unDir->nombre,fpDir);
	fwrite(&unDir->padre, sizeof(unDir->padre), 1, fpDir);
}
t_directorio *cargarDirectorio() {
	t_directorio *unDir = malloc(sizeof(t_directorio)); //nueva estructura
	fread(&unDir->index, sizeof(unDir->index), 1, fpDir);
	unDir->nombre = fread_str(fpDir);
	fread(&unDir->padre, sizeof(unDir->padre), 1, fpDir);
	return unDir;
}

void guardarListaDirectorios() {
	fpDir = fopen("directorios", "w");
	fwrite_list(listaDirectorios, fpDir,(void*)guardarDirectorio);
	fclose(fpDir);
}
void cargarListaDirectorios() {
	fpDir = fopen("directorios", "r");
	listaDirectorios = fread_list(fpDir,(void*)cargarDirectorio);
	fclose(fpDir);
}
//registros RegistrosIDIP

void guardarRegistroIDIP(t_registro_id_ipPuerto *unReg) {
	fwrite(&unReg->id, sizeof(unReg->id), 1, fpReg);
	fwrite(&unReg->ip.s_addr, sizeof(unReg->ip.s_addr), 1, fpReg);
	fwrite(&unReg->puerto, sizeof(unReg->puerto), 1, fpReg);
}
t_registro_id_ipPuerto *cargarRegistroIDIP() {
	t_registro_id_ipPuerto *unReg = malloc(sizeof(t_registro_id_ipPuerto)); //nueva estructura
	fread(&unReg->id, sizeof(unReg->id), 1, fpReg);
	fread(&unReg->ip.s_addr, sizeof(unReg->ip.s_addr), 1, fpReg);
	fread(&unReg->puerto, sizeof(unReg->puerto), 1, fpReg);
	return unReg;
}

void guardarListaRegistrosIDIP() {
	fpReg = fopen("registrosIDIP", "w");
	fwrite_list(listaRegistrosIDIP, fpReg,(void*)guardarRegistroIDIP);
	fclose(fpReg);
}
void cargarListaRegistrosIDIP() {
	fpReg = fopen("registrosIDIP", "r");
	listaRegistrosIDIP = fread_list(fpReg,(void*)cargarRegistroIDIP);
	fclose(fpReg);
}
//

void cargarPersistenciacfg() {
	fpNodos = fopen("nodos", "r");
	fclose(fpNodos);
	fpArch = fopen("archivos", "r");
	fclose(fpArch);
	fpDir = fopen("directorios", "r");
	fclose(fpDir);
}
//Funciones genéricas
/**********************************************************************/
/*********************** PRIVATE FUNCTIONS ****************************/
/**********************************************************************/
static void fwrite_list(t_list *list, FILE *fp,void(*struct_writer)(void*)) {
	int i = list_size(list
			 //mostrarLista(copiasBloqueA1, (void*) mostrarBloqueEnNodo);
			 //mostrarLista(copiasBloqueA2, (void*) mostrarBloqueEnNodo);
			 //	mostrarLista(copiasBloqueA3, (void*) mostrarBloqueEnNodo);
);
	fwrite(&i, sizeof(int), 1, fp);
	list_iterate(list,struct_writer);//quizas (void*) struct_writer/
}

static t_list* fread_list(FILE *fp,void*(*struct_reader)()) {
	t_list* list = list_create();
	int length;
	fread(&length, sizeof(int), 1, fp);
	while(length) {
		void *unElemento = struct_reader();//o *struct_writer();
		list_add(list, unElemento);
		length--;
	}
	 //mostrarLista(copiasBloqueA1, (void*) mostrarBloqueEnNodo);
	 //mostrarLista(copiasBloqueA2, (void*) mostrarBloqueEnNodo);
	 //	mostrarLista(copiasBloqueA3, (void*) mostrarBloqueEnNodo);

	return list;
}

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
static char* fread_str(FILE *fp) {
	int length;
	fread(&length, sizeof(int), 1, fp);
	char *string = malloc(length);
	fread(string, sizeof(char), length, fp);
	return string;
}
