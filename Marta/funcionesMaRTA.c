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
	log_info(marta_sync_logger,"wait funcionesMarta");
	fflush(stdout);
	t_list* listaArchivos = list_create();
	int i;
	int huboError=0;
	for (i=0;i<cantidadArchivos;i++){
		t_InfoArchivo* infoArchivo= malloc(sizeof(t_InfoArchivo));
		recvall(socketDeFS,&(infoArchivo->cantidadDeBloques),sizeof(int));
		if(infoArchivo->cantidadDeBloques==-1){
			huboError=1;
			log_error(marta_logger,"No se encontro el archivo %s.",archivos[i]);
		}
		else	log_info(marta_logger,"La cantidad de bloques recibidos fue %d.",infoArchivo->cantidadDeBloques);
		infoArchivo->idArchivo=i;
		list_add(listaArchivos,infoArchivo);
	}
	sem_post(&interaccionFS);
	log_info(marta_sync_logger,"post interaccionFS");
	if(huboError) return NULL;
	return listaArchivos;
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
