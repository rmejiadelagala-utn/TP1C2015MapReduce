/*
 * funcionesMaRTA.c
 *
 *  Created on: 22/6/2015
 *      Author: utnso
 */
#include "funcionesMaRTA.h"

static int registroID(t_registro_id_ipPuerto *unRegistro);
static void *buscarEnListaPorIntKey(t_list *lista, int key, int *keyGetter(void*));

t_registro_id_ipPuerto* buscarRegistroPorId(int id) {
	t_registro_id_ipPuerto* unReg = buscarEnListaPorIntKey(listaRegistrosIDIP, id, (int*) registroID);
	return unReg != NULL ? unReg : NULL;
}

t_list* obtenerIDyCantBloquesDeArchivosDelFS(char** archivos,int cantidadArchivos){
	dameListaArchFS(socketDeFS,archivos,cantidadArchivos);
	sem_wait(&funcionesMarta);
	t_list* listaArchivos = list_create();
	int i;
	for (i=0;i<cantidadArchivos;i++){
		t_InfoArchivo* infoArchivo= malloc(sizeof(t_InfoArchivo));
		recvall(socketDeFS,&(infoArchivo->cantidadDeBloques),sizeof(int));
		list_add(listaArchivos,infoArchivo);
	}
	return listaArchivos;
	sem_post(&interaccionFS);
}

/******************************************/
/********* PRIVATE FUNCTIONS **************/
/******************************************/
static int registroID(t_registro_id_ipPuerto *unRegistro) {
	return unRegistro->id;
}

static void *buscarEnListaPorIntKey(t_list *lista, int key,
		int *keyGetter(void*)) {
	bool _comparacion(void* data) {
		return keyGetter(data) == key;
	}

	return list_find(lista, (bool*) _comparacion);
}
