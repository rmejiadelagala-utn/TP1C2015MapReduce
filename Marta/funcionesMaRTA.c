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
