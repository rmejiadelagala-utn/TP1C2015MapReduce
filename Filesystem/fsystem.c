/*
 * fsystem.c
 *
 *  Created on: 09/5/2015
 *      Author: logonzalez
 */

#include "fsystem.h"
#include "consolaFS.h"
#include"estructurasFileSystem.h"
#include<commons/collections/list.h>
#include"funcionesFileSystem.h"
#include "CUnit/Basic.h"
#include <unistd.h>


int inicializar() {
	directorioUser = malloc(sizeof(t_directorio));

	directorioUser->index = 1;
	strcpy(directorioUser->nombre, "user");
	directorioUser->padre = 0;

	return 0;
}

int limpiar() {
	//list_clean(listaDirectorio);
	return 0;
}

void test_crear_directorio_nuevo_y_contar_directorios_totales() {
	listaDirectorio = list_create();
	list_add(listaDirectorio, directorioUser);

	crearDirectorioDadoPadreYNom("juan", 1, listaDirectorio);
	CU_ASSERT_EQUAL(listaDirectorio->elements_count, 2);
}

void test_crear_directorio_nuevo_y_verificar_su_indice() {
	listaDirectorio = list_create();
	list_add(listaDirectorio, directorioUser);

	crearDirectorioDadoPadreYNom("juan", 1, listaDirectorio);

	t_directorio *dirAux = list_get(listaDirectorio, 1);

	CU_ASSERT_EQUAL(dirAux->index, 2);
}

void agregar_tests() {
	CU_pSuite pSuite = CU_add_suite("Direcorio", inicializar, limpiar);
	CU_add_test(pSuite,
			"test crear directorio nuevo y ver que tiene un elemento mas",
			test_crear_directorio_nuevo_y_contar_directorios_totales);
	CU_add_test(pSuite,
			"test crear directorio nuevo y ver que tiene el indice correcto",
			test_crear_directorio_nuevo_y_verificar_su_indice);
}

//interaccionFSNodo es la funcion que va a ejecutar cada hilo que esta en conexion con los nodos
void *interaccionFSNodo(void*);


int main() {
	t_list *listaArchivos = list_create();
	t_list *listaNodos = list_create();
	t_list *listaDirectorios = list_create();
	//probando funcion de mostrar listas

	t_directorio *directorioHome = nuevoDirectorio(1, "home", 0);
	t_directorio *directorioMedia = nuevoDirectorio(2, "media", 0);
	t_directorio *directorioFotos = nuevoDirectorio(3, "fotos", 2);

	list_add(listaDirectorios, directorioHome);
	list_add(listaDirectorios, directorioMedia);
	list_add(listaDirectorios, directorioFotos);

	mostrarLista(listaDirectorios, (void*) mostrarDirectorio);

	//Muestro nodos
	t_nodo *nodoA = nuevoNodo("192.168.0.1:80", 1000);
	t_nodo *nodoB = nuevoNodo("192.168.0.2:127", 20000);
	t_nodo *nodoC = nuevoNodo("192.168.0.3.243", 65000);

	list_add(listaNodos, nodoA);
	list_add(listaNodos, nodoB);
	list_add(listaNodos, nodoC);

	mostrarLista(listaNodos, (void*) mostrarNodo);

	//muestro archivos

	t_bloqueEnNodo *copiaBloqueA1C1 = nuevoBloqueEnNodo("copiaBloqueA1C1", 11);
	t_bloqueEnNodo *copiaBloqueA1C2 = nuevoBloqueEnNodo("copiaBloqueA1C2", 12);
	t_bloqueEnNodo *copiaBloqueA1C3 = nuevoBloqueEnNodo("copiaBloqueA1C3", 13);
	t_list *copiasBloqueA1 = list_create();
	list_add(copiasBloqueA1, copiaBloqueA1C1);
	list_add(copiasBloqueA1, copiaBloqueA1C2);
	list_add(copiasBloqueA1, copiaBloqueA1C3);



	t_bloqueEnNodo *copiaBloqueA2C1 = nuevoBloqueEnNodo("copiaBloqueA2C1", 21);
	t_bloqueEnNodo *copiaBloqueA2C2 = nuevoBloqueEnNodo("copiaBloqueA2C2", 22);
	t_bloqueEnNodo *copiaBloqueA2C3 = nuevoBloqueEnNodo("copiaBloqueA2C3", 23);
	t_list *copiasBloqueA2 = list_create();
	list_add(copiasBloqueA2, copiaBloqueA2C1);
	list_add(copiasBloqueA2, copiaBloqueA2C2);
	list_add(copiasBloqueA2, copiaBloqueA2C3);

	t_bloqueEnNodo *copiaBloqueA3C1 = nuevoBloqueEnNodo("copiaBloqueA3C1", 31);
	t_bloqueEnNodo *copiaBloqueA3C2 = nuevoBloqueEnNodo("copiaBloqueA3C2", 32);
	t_bloqueEnNodo *copiaBloqueA3C3 = nuevoBloqueEnNodo("copiaBloqueA3C3", 33);
	t_list *copiasBloqueA3 = list_create();
	list_add(copiasBloqueA3, copiaBloqueA3C1);
	list_add(copiasBloqueA3, copiaBloqueA3C2);
	list_add(copiasBloqueA3, copiaBloqueA3C3);

	//mostrarLista(copiasBloqueA1, (void*) mostrarBloqueEnNodo);
	//mostrarLista(copiasBloqueA2, (void*) mostrarBloqueEnNodo);
//	mostrarLista(copiasBloqueA3, (void*) mostrarBloqueEnNodo);

	t_bloqueArch *bloqueArchivoA1 = nuevoBloqueArchivo(copiasBloqueA1);
	t_bloqueArch *bloqueArchivoA2 = nuevoBloqueArchivo(copiasBloqueA2);
	t_bloqueArch *bloqueArchivoA3 = nuevoBloqueArchivo(copiasBloqueA3);

	t_list * bloquesDeArchivoA = list_create();

	list_add(bloquesDeArchivoA, bloqueArchivoA1);
	list_add(bloquesDeArchivoA, bloqueArchivoA2);
	list_add(bloquesDeArchivoA, bloqueArchivoA3);


	 t_archivo *archivoA = nuevoArchivo("Archivo A", 1, 3000, bloquesDeArchivoA,
			1);

	 list_add(listaArchivos, archivoA);

	 mostrarLista(listaArchivos, (void*) mostrarArchivo);
	 //fin de prueba de funciond de mostrar listas
	 system("clear");

	 char* path = "ConfigFS.cfg";

	 config = config_create(path);

	 printf("*************** SE INICIA EL PROCESO FILESYSTEM ***************\n");
	 //	log_info(logFS, "*************** SE INICIA EL FILESYSTEM *************");
	 fd_set master;
	 fd_set read_fds;


	 listaNodo = list_create();

	 pthread_t consola_hilo;
	 if (pthread_create(&consola_hilo, NULL, consola, (void*) listaNodo) < 0) {
	 perror("could not create thread");
	 return 1;
	 }
	 //POR ALGUN MOTIVO ESTO NO ANDA:
	 //int puerto = config_get_int_value(config, "PUERTO_FS");


	 crearServerMultiHilo(8888,interaccionFSNodo);

	 //Probando el agregar test

	 /*CU_initialize_registry();

	 agregar_tests();

	 CU_basic_set_mode(CU_BRM_VERBOSE);
	 CU_basic_run_tests();
	 CU_cleanup_registry();

	 return CU_get_error();*/
	//Acá termina lo de los test en el main
	//Comenté esto porque me tiraba error
	/*

	 */
	//return 0;
//Comente la otra parte por si al final nos decidimos por usar la opcion con select,
//pero para ahora probar multihilo para sincronizar la lista de nodos.
	/*   int servFS, fdmax, i, sin_size, nbytes;
	 int newSock;

	 struct sockaddr_in their_addr;

	 FD_ZERO(&master);
	 FD_ZERO(&read_fds);

	 servFS = crearServer(config_get_int_value(config, "PUERTO_FS"));

	 FD_SET(servFS, &master);
	 fdmax = servFS;
	 char* mje;

	 for (;;) {
	 read_fds = master;
	 if (select(fdmax + 1, &read_fds, NULL, NULL, NULL ) == -1) {
	 perror("select");
	 exit(1);
	 }

	 for (i = 0; i <= fdmax; i++) {
	 if (FD_ISSET(i, &read_fds)) { //
	 //printf("FD_ISSET %i\n",i);
	 if (i == servFS) {
	 //conexion nueva
	 sin_size = sizeof(struct sockaddr_in);
	 //					newSock = malloc(sizeof(int));

	 if ((newSock = accept(servFS, (struct sockaddr*) &their_addr, (socklen_t *) &sin_size)) < 0) {
	 perror("accept");
	 exit(1);
	 } else {
	 FD_SET(newSock, &master); // agregar al conjunto maestro
	 if (newSock > fdmax) {   // actualizar el maximo
	 fdmax = newSock;
	 }
	 }
	 }else{

	 mje=malloc(sizeof(mje));
	 if ((nbytes = recv(i, mje, sizeof(mje), 0)) > 0) {

	 printf("%s\n", mje);
	 }

	 }
	 }
	 }

	 } //Fin de for;;*/

//close(servFS);
	list_destroy_and_destroy_elements(listaArchivos, (void*) liberarArchivo);
	list_destroy_and_destroy_elements(listaNodos, (void*) liberarNodo);
	list_destroy_and_destroy_elements(listaDirectorios,
			(void*) liberarDirectorio);
	return 0;
}

void *interaccionFSNodo(void* sock_ptr) {
	int sock_desc = *(int*) sock_ptr;
	char infoDeNodo[BUFFERSIZE];
	int read_size;

	list_add(listaNodo,sock_desc);
	//Receive a reply from the server
	read_size = recv((int)sock_desc, infoDeNodo, 5000, 0);

	while (read_size > 0) {
		printf("%s \n", infoDeNodo);
		memset(infoDeNodo, 0, sizeof(infoDeNodo));
		send(list_get(listaNodo,4),"Nodo, dame tu bloque\n",strlen("Nodo, dame tu bloque\n"),0);
		//Limpia el buffer de los mensajes que le manda ese nodo
		read_size = recv((int)sock_desc, infoDeNodo, 5000, 0);
	}
	if (read_size == 0) {
		printf("Nodo desconectado.\n");
	}
	if (read_size < 0) {
		printf("Error.");
	}
	close(sock_desc);
	return 0;
}

