/*
 * fsystem.c
 *
 *  Created on: 09/5/2015
 *      Author: logonzalez
 */

#include "fsystem.h"

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
	listaArchivos = list_create();
	listaNodos = list_create();
	listaDirectorios = list_create();
	//probando funcion de mostrar listas
	t_directorio *directorioRoot = nuevoDirectorio(1, "Root", 0);
	t_directorio *directorioHome = nuevoDirectorio(3, "home", 1);
	t_directorio *directorioMedia = nuevoDirectorio(2, "media", 1);
	t_directorio *directorioFotos = nuevoDirectorio(10, "fotos", 2);

	list_add(listaDirectorios, directorioRoot);
	list_add(listaDirectorios, directorioHome);
	list_add(listaDirectorios, directorioMedia);
	list_add(listaDirectorios, directorioFotos);



	//Muestro nodos
	t_nodo *nodoA = nuevoNodo("127.0.0.1:80A", 10);
	t_nodo *nodoB = nuevoNodo("127.0.0.1:12B", 20);
	t_nodo *nodoC = nuevoNodo("127.0.0.1:243C", 60);

	nodoA->cantidadBloquesOcupados = 1;
	int *a=malloc(sizeof(int));
	*a=1;
	queue_push(nodoA->bloquesLiberados,a);/*
	nodoC->cantidadBloquesOcupados = 3;
	nodoB->cantidadBloquesOcupados = 3;
*/
	list_add(listaNodos, nodoA);
	list_add(listaNodos, nodoB);
	list_add(listaNodos, nodoC);

//	mostrarLista(listaNodos, (void*) mostrarNodo);

//muestro archivos

	t_bloqueEnNodo *copiaBloqueA1C1 = nuevoBloqueEnNodo("127.0.0.1:80A", 11);
	t_bloqueEnNodo *copiaBloqueA1C2 = nuevoBloqueEnNodo("127.0.0.1:12B", 12);
	t_bloqueEnNodo *copiaBloqueA1C3 = nuevoBloqueEnNodo("127.0.0.1:243C", 13);
	t_list *copiasBloqueA1 = list_create();
	list_add(copiasBloqueA1, copiaBloqueA1C1);
	list_add(copiasBloqueA1, copiaBloqueA1C2);
	list_add(copiasBloqueA1, copiaBloqueA1C3);

	t_bloqueEnNodo *copiaBloqueA2C1 = nuevoBloqueEnNodo("127.0.0.1:80A", 21);
	t_bloqueEnNodo *copiaBloqueA2C2 = nuevoBloqueEnNodo("127.0.0.1:12B", 22);
	t_bloqueEnNodo *copiaBloqueA2C3 = nuevoBloqueEnNodo("127.0.0.1:243C", 23);
	t_list *copiasBloqueA2 = list_create();
	list_add(copiasBloqueA2, copiaBloqueA2C1);
	list_add(copiasBloqueA2, copiaBloqueA2C2);
	list_add(copiasBloqueA2, copiaBloqueA2C3);

	t_bloqueEnNodo *copiaBloqueA3C1 = nuevoBloqueEnNodo("127.0.0.1:80A", 31);
	t_bloqueEnNodo *copiaBloqueA3C2 = nuevoBloqueEnNodo("127.0.0.1:12B", 32);
	t_bloqueEnNodo *copiaBloqueA3C3 = nuevoBloqueEnNodo("127.0.0.1:243C", 33);
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

	t_bloqueEnNodo *copiaBloqueB1C1 = nuevoBloqueEnNodo("127.0.0.1:80A", 41);
	t_list *copiasBloqueB1 = list_create();
	list_add(copiasBloqueB1, copiaBloqueB1C1);
	t_bloqueArch *bloqueArchivoB1 = nuevoBloqueArchivo(copiasBloqueB1);
	t_list *bloquesDeArchivoB = list_create();
	list_add(bloquesDeArchivoB, bloqueArchivoB1);


	t_archivo *archivoA = nuevoArchivo("ArchivoA", 2, 3000, bloquesDeArchivoA,1);
	t_archivo *archivoB = nuevoArchivo("ArchivoB", 3, 3000, bloquesDeArchivoB,1);


	list_add(listaArchivos, archivoA);
	list_add(listaArchivos, archivoB);


//	formatear(&listaNodos, &listaArchivos, &listaDirectorios);
//	renombrarArchivoPorNombre("Archivo A","Archivo con nombre cambiado",listaArchivos);
//	moverArchivoPorNombreYPadre("Archivo A", listaArchivos, listaDirectorios, 10);
//	crearDirectorioDadoPadreYNom("Directorio Nuevo", 1,listaDirectorios);
//	eliminarNodoDeLista(nodoA, listaNodos);
	mostrarLista(listaDirectorios, (void*) mostrarDirectorio);
	mostrarLista(listaArchivos, (void*) mostrarArchivo);
	mostrarLista(listaNodos, (void*) mostrarNodo);
//	eliminarArchivoYreferencias(archivoA,listaArchivos, listaNodos);
//	eliminarReferencias(nodoA, listaArchivos);
//	eliminarNodoYReferencias(nodoB, listaNodos,listaArchivos);
//	renombrarDirectorioConNombre("pepito",directorioHome);
//	moverDirectorioConPadre(8,directorioHome);



//160 lugar de trabajo de juanchi
/*
	t_list *copiasbloques1 = list_create();
	t_list * copiasbloques2 = list_create();
	t_list * copiasbloques3 = list_create();
	t_bloqueArch *bloquearch1 = nuevoBloqueArchivo(copiasbloques1);
	t_bloqueArch *bloquearch2 = nuevoBloqueArchivo(copiasbloques2);
	t_bloqueArch *bloquearch3 = nuevoBloqueArchivo(copiasbloques3);
	t_list *bloquesDeArchivo  = list_create();
list_add(bloquesDeArchivo,bloquearch1);
list_add(bloquesDeArchivo,bloquearch2);
list_add(bloquesDeArchivo,bloquearch3);
	distribuirBloquesEnNodos(bloquesDeArchivo,listaNodos);
	mostrarLista(listaNodos, (void*) mostrarNodo);
	mostrarLista(bloquesDeArchivo, (void*) mostrarBloqueArch);
	list_destroy_and_destroy_elements(bloquesDeArchivo,(void*)liberarBloqueArch);
//176 fin	lugar de trabajo de juanchi*/
//177 lugar de trabajo de juanchi
/*
 * 	       |->|home|--------->(ArchivoB)
 * 		   |
 *  |root|-|  			|--->(ArchivoA)
 * 		   |			|
 * 		   |->|Media|---|--->|fotos|
 *
 */
/*
	printf("dirVacio False %d\n",dirVacio(directorioHome));
	printf("dirVacio False %d\n",dirVacio(directorioMedia));
	printf("dirVacio True %d\n",dirVacio(directorioFotos));

	printf("dirConSoloArch True %d\n",dirConSoloArch(directorioHome));
	printf("dirConSoloArch False %d\n",dirConSoloArch(directorioMedia));
	printf("dirConSoloArch False %d\n",dirConSoloArch(directorioFotos));

	printf("dirConSubdir es false %d\n",dirConSubdir(directorioFotos));
	printf("dirConSubdir es false %d\n",dirConSubdir(directorioHome));
	printf("dirConSubdir es true %d\n",dirConSubdir(directorioMedia));
	*/

//	eliminarDirectorioYContenido(directorioRoot);
//	eliminarNodoYReferencias(nodoC, listaNodos,listaArchivos);

//	printf("False %d\n",strcmp(copiaBloqueB1C1->ipPuerto,nodoA->ipPuerto)==0);
	printf("Despues de la accion\n\n\n");
	mostrarLista(listaNodos, (void*) mostrarNodo);
	mostrarLista(listaArchivos, (void*) mostrarArchivo);
/*
mostrarLista(listaDirectorios, (void*) mostrarDirectorio);
	mostrarLista(listaArchivos, (void*) mostrarArchivo);
	mostrarLista(listaNodos, (void*) mostrarNodo);
*/













	//195 fin	lugar de trabajo de juanchi

//	mostrarArchivo(buscarArchPorNombre("Archivo A", listaArchivos));
//	mostrarDirectorio(buscarDirPorNombre("home",listaDirectorios));
//	mostrarNodo(buscarNodoPorIpPuerto("192.168.0.1:80A",listaNodos));
//	mostrarLista(listaDirectorios, (void*) mostrarDirectorio);
//	mostrarLista(listaNodos, (void*) mostrarNodo);
//	mostrarLista(listaArchivos, (void*) mostrarArchivo);
/*

	//fin de prueba de funciond de mostrar listas
	 system("clear");

	 char* path = "ConfigFS.cfg";

	 config = config_create(path);

	 printf("*************** SE INICIA EL PROCESO FILESYSTEM ***************\n");
	 //	log_info(logFS, "*************** SE INICIA EL FILESYSTEM *************");
	 //fd_set master;
	 //fd_set read_fds;


	 listaNodo = list_create();

	 pthread_t consola_hilo;
	 if (pthread_create(&consola_hilo, NULL, consola, (void*) listaNodo) < 0) {
	 perror("could not create thread");
	 return 1;
	 }



	 crearServerMultiHilo(config_get_int_value(config, "PUERTO_FS"),interaccionFSNodo);
*/
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
	t_nodo *nodo = list_get(listaNodos,0);
	nodo->socket=sock_desc;
	//Receive a reply from the server

	read_size = recv((int) sock_desc, infoDeNodo, 100, 0);
	while (read_size > 0) {
		printf("%s \n", infoDeNodo);
		//Limpia el buffer de los mensajes que le manda ese nodo
		if(infoDeNodo[0]=='0'){
		printf("Leyo el 0.\n");
		recv((int) sock_desc, infoDeNodo, 1, 0);
		int tamanio = infoDeNodo[0]-'0';
		printf("Leyo el tamanio:%d.\n",tamanio);
		recv((int) sock_desc, infoDeNodo, tamanio, 0);
		printf("Leyo el mensaje.\n");
	//	char* texto = malloc(strlen(infoDeNodo)+1);
	//	strcpy(texto,infoDeNodo);
		write(fileno(archivoReconstruido),infoDeNodo,tamanio);
		printf("Escribio el mensaje.\n");
		write(fileno(archivoReconstruido),'\0',1);
		sem_post(&semaforo);
		}
		read_size= recv((int) sock_desc, infoDeNodo, 1, 0);
	}
	if (read_size == 0) {
		printf("Nodo desconectado.\n");
	}
	if (read_size<0) {
		printf("Error.");
	}
	close(sock_desc);
	return 0;
}

