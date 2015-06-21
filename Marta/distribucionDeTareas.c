/*
 * distribucionDeTareas.c
 *
 *  Created on: 9/6/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "estructurasMaRTA.h"
#include "socketes/envioDeMensajes.h"

pthread_mutex_t mutexListaNodo;
t_list* cargaNodos;

void liberarDestinoMap(t_DestinoMap* self);
void liberarMapPendiente(t_MapPendiente* self);
int recibirResultadoDeMap(int sockjob, t_ResultadoMap* resultadoMap);

void liberarMapPendiente(t_MapPendiente* self) {
	liberarDestinoMap(self->map_dest);
	free(self);
}

void liberarDestinoMap(t_DestinoMap* self) {
	free(self->temp_file_name);
	free(self);
}

void liberarCopiaDeBloque(t_CopiaDeBloque* self) {
	free(self);
}

//planifica un map de los que tiene que hacer el job
t_DestinoMap* planificarMap(t_InfoJob infoDeJob, uint32_t idArchivo,
		uint32_t numeroDeBloque, uint32_t* ultimoIDMap) {

	t_CopiaDeBloque* selected_copy;
	t_DestinoMap* self;

	t_list* block_copies = list_create();

	/*
	//TODO pone en block_copies la info de en que nodos estan las copias de ese bloqArch
	if (buscarBloquesEnFS(infoDeJob, idArchivo, numeroDeBloque, block_copies) <= 0) {
		//log_error(paranoid_log, "No se pudieron localizar las copias del Archivo: %i, Bloque: %i", idArchivo, block_number);
		list_destroy_and_destroy_elements(block_copies, (void *) free_block_copy);
		return NULL;
	}*/

	//Selecciona la copia de bloque primera
	 //TODO seleccionar por alguna planifición la mejor copia, y no la primera
	selected_copy = list_get(block_copies, 0);

	self = malloc(sizeof(t_DestinoMap));
	self->id_map = ++(*ultimoIDMap);
	self->id_nodo = selected_copy->id_nodo;
	self->ip_nodo = selected_copy->ip_nodo;
	self->puerto_nodo = selected_copy->puerto_nodo;
	self->block = selected_copy->block;
	self->temp_file_name = string_from_format("map_%i_%i.temp", infoDeJob.idJob, self->block);

	list_destroy_and_destroy_elements(block_copies, (void *) liberarCopiaDeBloque);

	return self;
}


int ordenarMapAJob(t_DestinoMap* destinoDeMap, int socket) {

	int result;

	//mostrar_map_dest(destinoDeMap);

	t_buffer* map_order = crearBufferConProtocolo(ORDER_MAP);

	bufferAgregarInt(map_order, destinoDeMap->id_map);
	bufferAgregarInt(map_order, destinoDeMap->id_nodo);
	bufferAgregarInt(map_order, destinoDeMap->ip_nodo);
	bufferAgregarInt(map_order, destinoDeMap->puerto_nodo);
	bufferAgregarInt(map_order, destinoDeMap->block);
	buffer_add_string(map_order, destinoDeMap->temp_file_name);

	result = enviarBuffer(socket, map_order);

	if (result < 0) {
		//log_error(paranoid_log, "No se Pudo enviar la Orden de Map al Job");
		printf("No se Pudo enviar la Orden de Map al Job");
	}

	return result;
}

int recibirResultadoDeMap(int sockjob, t_ResultadoMap* resultadoMap) {

	resultadoMap->prot = recibirProtocoloEnOrden(sockjob);

	switch (resultadoMap->prot) {
	case MAP_OK:
		//log_info(paranoid_log, "Map Realizado con Exito");
		printf("Map realizado con exito");
		break;

	case NODO_NOT_FOUND:
		//log_warning(paranoid_log, "No se encontró el NODO donde mapear");
		printf("No se encontró el nodo donde mapear");
		break;

	case DISCONNECTED:
		//log_error(paranoid_log, "Job se Desconectó de forma inesperada");
		printf("Job se desconectó de forma inesperada");
		return 0;
		break;

	case -1:
		//log_error(paranoid_log, "No se pudo recibir el resultado del Map");
		printf("No se pudo recibir el resultado del Map");
		return -1;
		break;

	default:
		//log_error(paranoid_log, "Protocolo Inesperado %i (MaRTA PANIC!)", result_map->prot);
		printf("Protocolo Inesperado %i", resultadoMap->prot);
		return -1;
		break;
	}

	return recibirIntEnOrden(sockjob, &(resultadoMap->id_map));
}

void agregarMapPendiente(t_list* mapsPendientes, t_InfoArchivo* infoArchivo,
		uint32_t numeroDeBloque, t_DestinoMap* destinoMap) {

	t_MapPendiente* mapPendiente;

	t_CargaNodo* cargaNodo;

	mapPendiente = malloc(sizeof(t_MapPendiente));
	mapPendiente->file = infoArchivo;
	mapPendiente->block = numeroDeBloque;
	mapPendiente->map_dest = destinoMap;

	list_add(mapsPendientes, mapPendiente);

	int encuentraNodo(t_CargaNodo* carga_nodo) {
		return carga_nodo->id_nodo == destinoMap->id_nodo;
	}

	pthread_mutex_lock(&mutexListaNodo);

	cargaNodo = list_find(cargaNodos, (void *) encuentraNodo);

	if (cargaNodo == NULL) {
		cargaNodo = malloc(sizeof(t_CargaNodo));
		cargaNodo->id_nodo = destinoMap->id_nodo;
		cargaNodo->cantidadOperacionesEnCurso = 1;

		list_add(cargaNodos, cargaNodo);
	} else {
		(cargaNodo->cantidadOperacionesEnCurso)++;
	}

	//cierro el mutex
	pthread_mutex_unlock(&mutexListaNodo);

}

void borrarMapPendiente(t_list* mapsPendientes, uint32_t idMap,
		t_list* listaTemporal, uint32_t* ultimoIdTemporal) {

	int encuentraMapPendiente(t_MapPendiente* mapPendiente) {
		return mapPendiente->map_dest->id_map == idMap;
	}

	t_MapPendiente* mapPendiente = list_find(mapsPendientes, (void *) encuentraMapPendiente);
	t_CargaNodo* cargaNodo;

	int seEncuetraNodo(t_CargaNodo* carga_nodo) {
		return carga_nodo->id_nodo == mapPendiente->map_dest->id_nodo;
	}

	t_MapTemporal* temp_map = malloc(sizeof(t_MapTemporal));

	temp_map->id_temp = ++(*ultimoIdTemporal);
	temp_map->path = strdup(mapPendiente->map_dest->temp_file_name);
	temp_map->id_nodo = mapPendiente->map_dest->id_nodo;
	temp_map->id_file_origin = mapPendiente->file->idArchivo;
	temp_map->block_origin = mapPendiente->block;

	list_add(listaTemporal, temp_map);

	pthread_mutex_lock(&mutexListaNodo);

	cargaNodo = list_find(cargaNodos, (void *) seEncuetraNodo);

	if (cargaNodo == NULL) {
		printf("No está nodo en lista de cargas");
	} else {
		(cargaNodo->cantidadOperacionesEnCurso)--;
	}

	pthread_mutex_unlock(&mutexListaNodo);

	list_remove_and_destroy_by_condition(mapsPendientes, (void *) encuentraMapPendiente, (void *) liberarMapPendiente);
}

//la función que planifica todos los map de un job determinado.
int planificarTodosLosMaps(t_InfoJob info_job, t_list* listaDeArchivos,
		t_list* ListaTemporal, int sockjob) {

	t_list* listaMapsPendientes = list_create();
	t_MapPendiente* mapPendiente;
	t_DestinoMap* destinoMap;

	int resultado = 1;
	int i, j, cantidadDeArchivos = list_size(listaDeArchivos);

	t_InfoArchivo* infoArchivo;
	uint32_t ultimoIDMap = 0;
	uint32_t ultimoIDTemporal = 0;

	for (i = 0; i < cantidadDeArchivos; i++) {
		infoArchivo = list_get(listaDeArchivos, i);

		for (j = 0; j < infoArchivo->cantidadDeBloques; j++) {

			//Aca dada la información que le manda job, la info del primer
			//archivo de la lista de archivos a mapear, elige el destino del map
			//Al que debe mandar hacer ese map
			destinoMap = planificarMap(info_job, infoArchivo->idArchivo, j, &ultimoIDMap);

			//Si obtiene un destino, le ordena al job realizar el map en ese destino
			resultado = (destinoMap != NULL) ? ordenarMapAJob(destinoMap, sockjob) : -2;

			if (resultado > 0) {
				//agrega a la lista de maps pendientes ese map, con la info
				//del archivo a mapear, la posicion y el destino

				agregarMapPendiente(listaMapsPendientes, infoArchivo, j, destinoMap);

			} else {
				//elemina todos los maps pendientes, porque no se puede realizar
				//este job
				list_destroy_and_destroy_elements(listaMapsPendientes, (void *) liberarMapPendiente);
				return -1;
			}
		}
	}

	t_ResultadoMap resultadoDeMap;

	int encuentreMapPendiente(t_MapPendiente* pending_map) {
		return pending_map->map_dest->id_map == resultadoDeMap.id_map;
	}

	while (!list_is_empty(listaMapsPendientes)) {
		//se queda esperando los resultados de los job mientras que la lista
		//de pendientes no esté vacía

		//recibe el resultado de un map por parte del job
		resultado = recibirResultadoDeMap(sockjob, &resultadoDeMap);

		//En base al resultado, realiza las tareas que le corresponden
		if (resultado > 0) {

			switch (resultadoDeMap.prot) {
			case MAP_OK:
				borrarMapPendiente(listaMapsPendientes, resultadoDeMap.id_map, ListaTemporal, &ultimoIDTemporal);
				break;

			case NODO_NOT_FOUND:
				mapPendiente = list_find(listaMapsPendientes, (void *) encuentreMapPendiente);

				liberarDestinoMap(mapPendiente->map_dest);
				mapPendiente->map_dest = planificarMap(info_job, mapPendiente->file->idArchivo, mapPendiente->block, &ultimoIDMap);

				resultado = (mapPendiente->map_dest != NULL) ? ordenarMapAJob(mapPendiente->map_dest, sockjob) : -2;

				if (resultado <= 0) {
					list_destroy_and_destroy_elements(listaMapsPendientes, (void *) liberarMapPendiente);
					return -1;
				}
				break;
			}

		} else {
			list_destroy_and_destroy_elements(listaMapsPendientes, (void *) liberarMapPendiente);
			return -1;
		}
	}

	//termina toda la planificacion, destruyendo lo que quedo de las listas pendientes
	//de map
	list_destroy_and_destroy_elements(listaMapsPendientes, (void *) liberarMapPendiente);
	return 1;
}
