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
#include "MaRTA.h"

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

t_CopiaDeBloque* elegirMejorNodoParaMap(t_list* copiasDeBloque) {
//	printf("La lista que me llego tiene tamanio: %d\n",

	t_CargaNodo* cargaNodo;
	t_CargaNodo* cargaNodoOtroBloque;
	uint32_t idNodoBloqueEvaluado;

	//la lista global cargaNodos tiene idNodo y sus cargas de trabajo en curso
	//la copiaDeBloque tiene el id del nodo donde está la copia de bloqArch necesaria

	//para todas las copias, selecciono aquella que su idNodo sea el que
	//menos carga de trabajo tenga actualmente

	int encuentraNodoDeId(t_CargaNodo* cargaNodo) {
		return cargaNodo->id_nodo == idNodoBloqueEvaluado;
	}

	bool compararNodosPorMenorCarga(t_CopiaDeBloque *unBloque,
			t_CopiaDeBloque *bloqueSiguiente) {

		idNodoBloqueEvaluado = unBloque->id_nodo;
		cargaNodo = list_find(cargaNodos, (void *) encuentraNodoDeId);

		idNodoBloqueEvaluado = bloqueSiguiente->id_nodo;
		cargaNodoOtroBloque = list_find(cargaNodos, (void *) encuentraNodoDeId);

		if ((!cargaNodo) || (!cargaNodoOtroBloque))
			return -1;

		return cargaNodo->cantidadOperacionesEnCurso
				< cargaNodoOtroBloque->cantidadOperacionesEnCurso;
	}

	//ordena las copiasDeBloque por los nodos que menos están trabajando
	pthread_mutex_lock(&mutexListaNodo);
	log_info(marta_sync_logger,"lock mutexListaNodo");

//	printf("Voy a ordenar las copias de bloque\n");
//	fflush(stdout);
	list_sort(copiasDeBloque, (void*) compararNodosPorMenorCarga);

	pthread_mutex_unlock(&mutexListaNodo);
	log_info(marta_sync_logger,"unlock mutexListaNodo");
	int i;
	t_CopiaDeBloque* copiaElegida;
	for (i = 0; (copiaElegida = list_get(copiasDeBloque, i)) == NULL; i++)
		;

	//elijo la primera de las copias de la lista ordena por menor carga.
	//t_CopiaDeBloque* copiaElegida = list_get(copiasDeBloque, 0);
	log_info(marta_logger, "La copia elegida tiene id %d\n",
			copiaElegida->id_nodo);
	return list_get(copiasDeBloque, 0);
}

int buscarBloquesEnFS(t_InfoJob infoDeJob, uint32_t idArchivo,
		uint32_t numeroDeBloque, t_list *copiasDeBloque) {

//	printf("El id del archivo es %d\n", idArchivo);
//	printf("El nombre del archivo es %s\n",
//			infoDeJob.pathsDeArchivos[idArchivo]);
//	printf("El numero de bloque es %d\n", numeroDeBloque);

	dameBloqueArchFS(socketDeFS, infoDeJob.pathsDeArchivos[idArchivo], 1,
			numeroDeBloque);
	sem_wait(&funcionesMarta);
	log_info(marta_sync_logger,"wait funcionesMarta");

	t_list* copias = list_create();
	if (recibirBloqueArchFS(socketDeFS, copias) < 0)
		return -1;
	void deBloqueEnNodoACopiaDeBloque(t_bloqueEnNodo* bloqueEnNodo) {
		t_CopiaDeBloque* copiaDeBloque = malloc(sizeof(t_CopiaDeBloque));
		copiaDeBloque->block = bloqueEnNodo->numeroDeBloqueEnNodo;
		copiaDeBloque->id_nodo = bloqueEnNodo->id;
		copiaDeBloque->size = bloqueEnNodo->tamanioBloque;
		list_add(copiasDeBloque, copiaDeBloque);
	}
	list_iterate(copias, (void*) deBloqueEnNodoACopiaDeBloque);
	//copiasDeBloque = list_map(copias,deBloqueEnNodoACopiaDeBloque);
	void mostrarBloque(t_CopiaDeBloque* unBloque) {
		//log_info(marta_logger, "ID bloque:%d\nNumero de bloque:%d",
		//		unBloque->id_nodo, unBloque->block);
	}
	list_iterate(copiasDeBloque, (void*) mostrarBloque);
//	printf("El tamaño de la lista es %d\n", list_size(copiasDeBloque));
//	fflush(stdout);
	list_destroy_and_destroy_elements(copias, free);
	sem_post(&interaccionFS);
	log_info(marta_sync_logger,"post interaccionFS");

	return 1;	//1 salió bien, <= 0 no lo encontró
}
//planifica un map de los que tiene que hacer el job
t_DestinoMap* planificarMap(t_InfoJob infoDeJob, uint32_t idArchivo,
		uint32_t numeroDeBloque, uint32_t* ultimoIDMap) {

	t_CopiaDeBloque* copiaSeleccionada;
	t_DestinoMap* self;

	t_list* copiasDeBloque = list_create();

	if (buscarBloquesEnFS(infoDeJob, idArchivo, numeroDeBloque, copiasDeBloque)
			<= 0) {
		log_warning(marta_logger,
				"No se encontraron las copias del Archivo: %i, Bloque: %i",
				idArchivo, numeroDeBloque);
		list_destroy_and_destroy_elements(copiasDeBloque,
				(void *) liberarCopiaDeBloque);
		return NULL;
	}
	log_info(marta_logger, "Voy a buscar la mejor copia\n");
	//Selecciona la copia de bloque que está en el nodo que menos trabajo tiene
	copiaSeleccionada = elegirMejorNodoParaMap(copiasDeBloque);

	//XXX ojo acá hay warning feo. Me parece que no estaría funcionando bien esto.
	t_registro_id_ipPuerto* unRegistro = buscarRegistroPorId(
			copiaSeleccionada->id_nodo);

	if (!unRegistro)
		return NULL;

	self = malloc(sizeof(t_DestinoMap));
	self->id_map = ++(*ultimoIDMap);
	self->id_nodo = copiaSeleccionada->id_nodo;
	self->ip_nodo = unRegistro->ip.s_addr;
	self->puerto_nodo = unRegistro->puerto;
	self->block = copiaSeleccionada->block;
	self->temp_file_name = string_from_format("map_%i_%i_%i.temp",
			infoDeJob.idJob, idArchivo, numeroDeBloque);
	self->block_size = copiaSeleccionada->size;

	list_destroy_and_destroy_elements(copiasDeBloque,
			(void *) liberarCopiaDeBloque);

	fflush(stdout);

	return self;

}

int rePlanificarMapsDeNodoMuerto(int idNodoMuerto, t_InfoJob infoJob,
		int ultimoIDMap, t_list* listaTemporal, t_list* listaMapsPendientes,
		t_list** nuevosDestinosDeMapsReplanificados) {

	t_list* listaReplanificacionPendientesMuertos;

	//tomar de la listaTemporal, aquellos maps que sean de idNodoMuerto.
	//A estos, mandarlos a planificar como se hace normalmente.
	//Guardar los mapTemporales replanificados a listaReplanificacionPendientesMuertos.

	bool temporalConIDNodoMuerto(t_MapTemporal* unMapTemporal) {
		return unMapTemporal->id_nodo == idNodoMuerto;
	}

	void eliminarTemporal(t_MapTemporal* unMapTemporal) {
		free(unMapTemporal->path);
		free(unMapTemporal);
	}

	//XXX ojo, puede que al eliminar la listaTemporal, se elimine la listaReplanifi
	listaReplanificacionPendientesMuertos = list_filter(listaTemporal,
			(void*) temporalConIDNodoMuerto);

	lista_remove_and_destroy_by_condition(listaTemporal,
			(void*) temporalConIDNodoMuerto, (void*) eliminarTemporal);

	void rePlanificar(t_MapTemporal* unMapTemporal) {
		t_DestinoMap* destinoMap;

		destinoMap = planificarMap(infoJob, unMapTemporal->idArchivoOrigen,
				unMapTemporal->bloqueOrigen, &ultimoIDMap);

		//XXX ojo con esta lista nuevos... muy raro.
		list_add(*nuevosDestinosDeMapsReplanificados, destinoMap);
	}

	list_iterate(listaReplanificacionPendientesMuertos, (void*) rePlanificar);

	list_destroy_and_destroy_elements(listaReplanificacionPendientesMuertos,
			(void*) eliminarTemporal);

	return 1;
}

int ordenarARealizarLosRePlanificados(
		t_list* nuevosDestinosDeMapsReplanificados, int socketJob) {

	void ordenarMap(t_DestinoMap* unDestino) {
		ordenarMapAJob(unDestino, socketJob);
	}

	list_iterate(nuevosDestinosDeMapsReplanificados, (void*) ordenarMap);

	return 1;
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
	bufferAgregarInt(map_order, destinoDeMap->block_size);
	bufferAgregarString(map_order, destinoDeMap->temp_file_name,
			strlen(destinoDeMap->temp_file_name) + 1);

	result = enviarBuffer(map_order, socket);

	if (result < 0) {
		log_warning(marta_logger, "No se Pudo enviar la Orden de Map al Job");
	} else {
		log_info(marta_logger, "Le mandé la orden al job");
	}
	return result;
}

int recibirResultadoDeMap(int sockjob, t_ResultadoMap* resultadoMap) {

	resultadoMap->prot = recibirInt(sockjob);

	switch (resultadoMap->prot) {
	case OK_MAP:
		log_info(marta_logger, "Map realizado con exito");
		break;

	case NODO_NOT_FOUND:
		log_warning(marta_logger, "No se encontró el nodo donde mapear");
		break;

	case 0:
		log_warning(marta_logger, "Job se desconectó de forma inesperada");
		return -1;
		break;

	case -1:
		log_warning(marta_logger, "No se pudo recibir el resultado del Map");
		return -1;
		break;

	default:
		log_warning(marta_logger, "Protocolo Inesperado %i",
				resultadoMap->prot);
		return -1;
		break;
	}

	resultadoMap->id_map = recibirInt(sockjob);
	return 1;
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
	log_info(marta_sync_logger,"lock mutexListaNodo");

	cargaNodo = list_find(cargaNodos, (void *) encuentraNodo);

	if (cargaNodo == NULL) {
		cargaNodo = malloc(sizeof(t_CargaNodo));
		cargaNodo->id_nodo = destinoMap->id_nodo;
		cargaNodo->cantidadOperacionesEnCurso = 1;

		list_add(cargaNodos, cargaNodo);
	} else {
		(cargaNodo->cantidadOperacionesEnCurso)++;
	}

	void mostrarCargaNodo(t_CargaNodo* unaCargaNodo) {

		printf("Nodo: %i ---> Carga: %i\n", unaCargaNodo->id_nodo,
				unaCargaNodo->cantidadOperacionesEnCurso);

		fflush(stdout);
	}

	printf("\n");
	list_iterate(cargaNodos, (void*) mostrarCargaNodo);

//cierro el mutex
	pthread_mutex_unlock(&mutexListaNodo);
	log_info(marta_sync_logger,"unlock mutexListaNodo");

}

void borrarMapPendiente(t_list* mapsPendientes, uint32_t idMap,
		t_list* listaTemporal, uint32_t* ultimoIdTemporal) {

	int encuentraMapPendiente(t_MapPendiente* mapPendiente) {
		return mapPendiente->map_dest->id_map == idMap;
	}

	t_MapPendiente* mapPendiente = list_find(mapsPendientes,
			(void *) encuentraMapPendiente);
	t_CargaNodo* cargaNodo;

	int seEncuetraNodo(t_CargaNodo* carga_nodo) {
		return carga_nodo->id_nodo == mapPendiente->map_dest->id_nodo;
	}

	t_MapTemporal* temp_map = malloc(sizeof(t_MapTemporal));

	temp_map->idMapTemporal = ++(*ultimoIdTemporal);
	temp_map->path = strdup(mapPendiente->map_dest->temp_file_name);
	temp_map->id_nodo = mapPendiente->map_dest->id_nodo;
	temp_map->idArchivoOrigen = mapPendiente->file->idArchivo;
	temp_map->bloqueOrigen = mapPendiente->block;

	list_add(listaTemporal, temp_map);

	pthread_mutex_lock(&mutexListaNodo);
	log_info(marta_sync_logger,"lock mutexListaNodo");

	cargaNodo = list_find(cargaNodos, (void *) seEncuetraNodo);

	if (cargaNodo == NULL) {
		printf("No está nodo en lista de cargas");
	} else {
		(cargaNodo->cantidadOperacionesEnCurso)--;
	}

	pthread_mutex_unlock(&mutexListaNodo);
	log_info(marta_sync_logger,"unlock mutexListaNodo");

	lista_remove_and_destroy_by_condition(mapsPendientes,
			(void *) encuentraMapPendiente, (void *) liberarMapPendiente);
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
			printf("Planifico el map para el archivo de id %d",
					infoArchivo->idArchivo);
			destinoMap = planificarMap(info_job, infoArchivo->idArchivo, j,
					&ultimoIDMap);

			//Si obtiene un destino, le ordena al job realizar el map en ese destino
			if (destinoMap) {
				resultado = ordenarMapAJob(destinoMap, sockjob);
			} else {
				int error = -1;
				sendall(sockjob, &error, sizeof(int));
				return -2;
			}

			if (resultado > 0) {
				//agrega a la lista de maps pendientes ese map, con la info
				//del archivo a mapear, la posicion y el destino

				agregarMapPendiente(listaMapsPendientes, infoArchivo, j,
						destinoMap);

			} else {

				//elemina todos los maps pendientes, porque no se puede realizar
				//este job
				list_destroy_and_destroy_elements(listaMapsPendientes,
						(void *) liberarMapPendiente);
				return -1;
			}
		}
	}

	log_info(marta_logger, "Envie todos los pedidos");
	pthread_mutex_unlock(&conexionFS);
	log_info(marta_sync_logger,"unlock conexionFS");

	t_ResultadoMap resultadoDeMap;
	t_list* nuevosDestinosDeMapsReplanificados;

	int encuentreMapPendiente(t_MapPendiente* pending_map) {
		return pending_map->map_dest->id_map == resultadoDeMap.id_map;
	}

	while (!list_is_empty(listaMapsPendientes)) {
		//se queda esperando los resultados de los job mientras que la lista
		//de pendientes no esté vacía

		//recibe el resultado de un map por parte del job
		resultado = recibirResultadoDeMap(sockjob, &resultadoDeMap);
		fflush(stdout);

		//En base al resultado, realiza las tareas que le corresponden
		if (resultado > 0) {

			switch (resultadoDeMap.prot) {
			case OK_MAP:
				borrarMapPendiente(listaMapsPendientes, resultadoDeMap.id_map,
						ListaTemporal, &ultimoIDTemporal);
				break;

			case NODO_NOT_FOUND:		//este sería el NOTOK_MAP
				log_warning(marta_logger, "ERROR AL REALIZAR UN MAP");
				fflush(stdout);

				mapPendiente = list_find(listaMapsPendientes,
						(void *) encuentreMapPendiente);

				if (mapPendiente)
					log_info(marta_logger, "El resultado del map no es null");
				else
					log_warning(marta_logger, "El resultado del map es null");

				liberarDestinoMap(mapPendiente->map_dest);

				mapPendiente->map_dest = planificarMap(info_job,
						mapPendiente->file->idArchivo, mapPendiente->block,
						&ultimoIDMap);

				//para agregar la cantidad de operaciones al replanificado
				int seEncuetraNodo(t_CargaNodo* carga_nodo) {
					return carga_nodo->id_nodo
							== mapPendiente->map_dest->id_nodo;
				}

				t_CargaNodo* cargaNodo = list_find(cargaNodos,
						(void *) seEncuetraNodo);

				if (cargaNodo == NULL) {
					printf("No está nodo en lista de cargas\n");
				} else {
					(cargaNodo->cantidadOperacionesEnCurso)++;
				}

				pthread_mutex_unlock(&planificarMapMutex);
				log_info(marta_sync_logger,"unlock planificarMapMutex");

				if (mapPendiente) {
					resultado = ordenarMapAJob(mapPendiente->map_dest, sockjob);
				} else {
					int error = -1;
					sendall(sockjob, &error, sizeof(int));
					return -2;
				}

				if (resultado <= 0) {
					log_info(marta_logger,
							" VOY A DESTRUIR TODOS LOS MAPS PENDIENTES");
					fflush(stdout);
					list_destroy_and_destroy_elements(listaMapsPendientes,
							(void *) liberarMapPendiente);
					return -1;
				}
				break;

			case NOTOK_MAP:

				log_warning(marta_logger, "NO SE ENCONTRO UN NODO");
				fflush(stdout);

				mapPendiente = list_find(listaMapsPendientes,
						(void *) encuentreMapPendiente);

				if (mapPendiente)
					log_info(marta_logger, "El resultado del map no es null");
				else
					log_warning(marta_logger, "El resultado del map es null");

				int tomarIDNodoDadoElIDMap(t_list* listaMapsPendientes,
						int idMap) {

					bool elIdMapDeSuMapDestEsidMap(t_MapPendiente* mapPendiente) {
						return mapPendiente->map_dest->id_map == idMap;
					}

					t_MapPendiente* mapPendienteAux = list_find(
							listaMapsPendientes,
							(void*) elIdMapDeSuMapDestEsidMap);

					return mapPendienteAux->map_dest->id_nodo;
				}

				int idNodoMuerto = tomarIDNodoDadoElIDMap(listaMapsPendientes,
						resultadoDeMap.id_map);

				rePlanificarMapsDeNodoMuerto(idNodoMuerto, info_job,
						ultimoIDMap, listaTemporal, listaMapsPendientes,
						&nuevosDestinosDeMapsReplanificados);

				pthread_mutex_unlock(&planificarMapMutex);
				log_info(marta_sync_logger,"unlock planificarMapMutex");

				ordenarARealizarLosRePlanificados(
						nuevosDestinosDeMapsReplanificados, sockjob);

				//todo falta poner mutex y retornar/agarrar errores
				break;
			}

		} else {
			list_destroy_and_destroy_elements(listaMapsPendientes,
					(void *) liberarMapPendiente);
			return -1;
		}
	}

//termina toda la planificacion, destruyendo lo que quedo de las listas pendientes
//de map
	list_destroy_and_destroy_elements(listaMapsPendientes,
			(void *) liberarMapPendiente);
	return 1;
}

int ipDeNodo(int idNodo) {

	t_registro_id_ipPuerto* registroIpPuerto = buscarRegistroPorId(idNodo);

	int ipDelNodo = registroIpPuerto->ip.s_addr;

	return ipDelNodo;
}

int puertoDeNodo(int idNodo) {

//XXX ver warning. No lo entiendo bien
	t_registro_id_ipPuerto* registroIpPuerto = buscarRegistroPorId(idNodo);

	int puertoDelNodo = registroIpPuerto->puerto;

	return puertoDelNodo;
}

int ordenarReduceAJob(t_DestinoReduce* destinoReduce, t_list* origenesDeReduce,
		int sockJob) {

	int resultado;

//mostrar_map_dest(destinoDeMap);
	t_buffer* reduce_order = crearBufferConProtocolo(ORDER_REDUCE);

//XXX decir si sse mandaono id_Reduce
	bufferAgregarInt(reduce_order, destinoReduce->id_reduce);
	bufferAgregarInt(reduce_order, destinoReduce->ip_nodo);
	bufferAgregarInt(reduce_order, destinoReduce->puerto_nodo);
	bufferAgregarString(reduce_order, destinoReduce->temp_file_name,
			strlen(destinoReduce->temp_file_name) + 1);
	bufferAgregarInt(reduce_order, list_size(origenesDeReduce));

	void bufferAgregarOrigenesReduce(t_OrigenReduce* unOrigenReduce) {

		//	printf("el id de nodo es %i\n", unOrigenReduce->id_nodo);
		//	printf("y el nombre del archivo es %s", unOrigenReduce->temp_file_name);
		//	fflush(stdout);

		bufferAgregarInt(reduce_order, puertoDeNodo(unOrigenReduce->id_nodo));
		bufferAgregarInt(reduce_order, ipDeNodo(unOrigenReduce->id_nodo));
		bufferAgregarString(reduce_order, unOrigenReduce->temp_file_name,
				strlen(unOrigenReduce->temp_file_name) + 1);
	}

	list_iterate(origenesDeReduce, (void*) bufferAgregarOrigenesReduce);

	resultado = enviarBuffer(reduce_order, sockJob);

	if (resultado < 0) {
		log_warning(marta_logger,
				"No se Pudo enviar la Orden de Reduce al Job");
	} else {
		log_info(marta_logger, "Le mandé la orden de Reduce al job");
	}
	return resultado;
}

int ordenarUltimoReduceAJob(t_DestinoReduce* destinoReduce,
		t_list* origenesDeReduce, int sockJob) {

	int resultado;

//mostrar_map_dest(destinoDeMap);
	t_buffer* reduce_order = crearBufferConProtocolo(ORDER_REDUCE);

//XXX decir si sse mandaono id_Reduce
	bufferAgregarInt(reduce_order, destinoReduce->id_reduce);
	bufferAgregarInt(reduce_order, destinoReduce->ip_nodo);
	bufferAgregarInt(reduce_order, destinoReduce->puerto_nodo);
	bufferAgregarString(reduce_order, destinoReduce->temp_file_name,
			strlen(destinoReduce->temp_file_name) + 1);
	bufferAgregarInt(reduce_order, list_size(origenesDeReduce));

	void bufferAgregarOrigenesReduce(t_DestinoReduce* unOrigenReduce) {

		//	printf("el id de nodo es %i\n", unOrigenReduce->id_nodo);
		//	printf("y el nombre del archivo es %s", unOrigenReduce->temp_file_name);
		//	fflush(stdout);

		bufferAgregarInt(reduce_order, puertoDeNodo(unOrigenReduce->id_nodo));
		bufferAgregarInt(reduce_order, ipDeNodo(unOrigenReduce->id_nodo));
		bufferAgregarString(reduce_order, unOrigenReduce->temp_file_name,
				strlen(unOrigenReduce->temp_file_name) + 1);
	}

	list_iterate(origenesDeReduce, (void*) bufferAgregarOrigenesReduce);

	resultado = enviarBuffer(reduce_order, sockJob);

	if (resultado < 0) {
		log_warning(marta_logger,
				"No se Pudo enviar la Orden de Reduce al Job");
	} else {
		log_info(marta_logger, "Le mandé la orden de Reduce al job");
	}
	return resultado;
}

int recibirResultadoDeReduce(int sockjob, t_ResultadoReduce* resultadoReduce) {

	resultadoReduce->prot = recibirInt(sockjob);

	switch (resultadoReduce->prot) {
	case OK_REDUCE:
		log_info(marta_logger, "Reduce realizado con exito");
		break;

	case NODO_NOT_FOUND:
		log_info(marta_logger, "No se encontró el nodo donde hacer reduce");
		break;

	case 0:
		log_warning(marta_logger, "Job se desconectó de forma inesperada");
		return -1;
		break;

	case -1:
		log_warning(marta_logger, "No se pudo recibir el resultado del Reduce");
		return -1;
		break;

	default:
		log_warning(marta_logger, "Protocolo Inesperado %i",
				resultadoReduce->prot);
		return -1;
		break;
	}

	resultadoReduce->id_reduce = recibirInt(sockjob);
	return 1;
}

void agregarReducePendiente(t_list* reducePendientes,
		t_DestinoReduce* destinoReduce) {

	t_ReducePendiente* reducePendiente;

	reducePendiente = malloc(sizeof(t_ReducePendiente));
	reducePendiente->numeroDeReducePendiente = destinoReduce->id_reduce;
	reducePendiente->map_dest = destinoReduce;

	list_add(reducePendientes, reducePendiente);

}

char* planificarTodosLosReduce(t_InfoJob infoJob, t_list* listaMapsTemporales,
		int sockJob) {
	char* archivoFinal;
	int idJobAlQueAplica = infoJob.idJob;
	uint32_t ultimoIDReduce = 0;

	t_list* listaIdNodosDondeAplicarReduce = list_create();

//FIXME esto es inecesario, pero queria usar el t_list y con esto lo soluciono por ahora
	typedef struct {
		int idNodo;
	} t_idNodo;

//funciones auxiliares
	int obtenerIdJobDelMapTemporal(t_MapTemporal* unMapTemporal) {

		char** archivoTemporalSeparadoPorGuionBajo;

		archivoTemporalSeparadoPorGuionBajo = string_split(unMapTemporal->path,
				"_");

		char* stringIdJob = archivoTemporalSeparadoPorGuionBajo[1];

		return atoi(stringIdJob);
	}

	bool temporalesDelJobTrabajado(t_MapTemporal* unMapTemporal) {
		return idJobAlQueAplica == obtenerIdJobDelMapTemporal(unMapTemporal);
	}

	int mejorNodoDondeAplicarReduceSinCombiner(t_list *mapsTemporales) { //XXX testear. Puede fallar feo
		int maxTemporalesPorNodo = 0;
		int idMejorNodo = -1;
		int cantidadDeApariciones = 0;

		void elegirNodoConMasTemporales(t_MapTemporal* unMapTemporal) {

			bool apareceNodoAEvaluar(t_MapTemporal* otroMapTemporal) {
				return otroMapTemporal->id_nodo == unMapTemporal->id_nodo;
			}

			cantidadDeApariciones = list_count_satisfying(mapsTemporales,
					(void*) apareceNodoAEvaluar);

			if (cantidadDeApariciones > maxTemporalesPorNodo) {
				idMejorNodo = unMapTemporal->id_nodo;
				maxTemporalesPorNodo = cantidadDeApariciones;
				cantidadDeApariciones = 0;
			}

		}

		list_iterate(mapsTemporales, (void*) elegirNodoConMasTemporales);

		return idMejorNodo; //si devuelve -1, no selecciono ningun nodo. Error de algoritmo
	}

	void tomarIdsNodo(t_MapTemporal* unMapTemporal) {

		t_idNodo* tIdNodo = malloc(sizeof(t_idNodo));
		tIdNodo->idNodo = unMapTemporal->id_nodo;

		bool estaEseNodoCargado(t_idNodo* idCargado) {
			return unMapTemporal->id_nodo == idCargado->idNodo;
		}

		if (!list_any_satisfy(listaIdNodosDondeAplicarReduce,
				(void*) estaEseNodoCargado)) {

			list_add(listaIdNodosDondeAplicarReduce, tIdNodo);

		}
	}

//FIXME Job necesitaría ip-Puerto del nodo con ese id, eso debería sacarlo MaRTA
//de sus listas de nodos. No se si eso se hace acá o en otro lado.
//Si es así, hay que agregar esos campos en t_DestinoReduce
	t_OrigenReduce* convertirAEstructuraNecesaria(t_MapTemporal* unMapTemporal) {

		t_OrigenReduce* origenReduce = malloc(sizeof(t_OrigenReduce));

		origenReduce->id_nodo = unMapTemporal->id_nodo;
		origenReduce->temp_file_name = unMapTemporal->path;

		return origenReduce;
	}
//fin funciones auxiliares

	t_list* mapsTemporalesDeLosArchivosDelJob = list_filter(listaMapsTemporales,
			(void*) temporalesDelJobTrabajado);

	if (infoJob.combiner == 1) {	//Con combiner

		//debo aplicar reduce dentro de cada nodo que tenga maps temporales
		//Es decir, debo agarrar y mandar a reducir todos las archivos tmp que tenga
		//cada uno de esos nodos. Una vez terminados todos estos reduce en cada nodo
		//hacer un reduce general con todos los tmp auxiliares de reduce generados

		//con esto cargo la lista listaIdNodosDondeAplicarReduce con los
		//distintos idNodo que estan involucrados en este reduce combiner
		list_iterate(mapsTemporalesDeLosArchivosDelJob, (void*) tomarIdsNodo);

		t_list* listaReducePendientes = list_create();
		t_list* destinosIntermedios = list_create();
		int idNodoDondeAplicarReduceFinal;

		while (!list_is_empty(listaIdNodosDondeAplicarReduce)) {
			t_list* mapsTemporalesDondeHacerReduceEnNodo;
			t_list* origenesReduceEnNodo;

			t_idNodo* idAux = list_get(listaIdNodosDondeAplicarReduce, 0);
			int resReduceEnNodo;

			bool destinosDelNodoAAplicarReduceLocal(
					t_MapTemporal* unMapTemporal) {

				return unMapTemporal->id_nodo == idAux->idNodo;
			}

			mapsTemporalesDondeHacerReduceEnNodo = list_filter(
					mapsTemporalesDeLosArchivosDelJob,
					(void*) destinosDelNodoAAplicarReduceLocal);

			origenesReduceEnNodo = list_map(
					mapsTemporalesDondeHacerReduceEnNodo,
					(void*) convertirAEstructuraNecesaria);

			//XXX ojo, malloc dentro de while. Al finalizar, hago free de la lista que los contiene
			t_DestinoReduce* destinoIntermedioReduce = malloc(
					sizeof(t_DestinoReduce));

			//XXX el nombre del reduce no se si siempre será único. supuestamente si
			destinoIntermedioReduce->id_reduce = ++(ultimoIDReduce);
			destinoIntermedioReduce->id_nodo = idAux->idNodo;
			destinoIntermedioReduce->ip_nodo = ipDeNodo(idAux->idNodo);
			destinoIntermedioReduce->puerto_nodo = puertoDeNodo(idAux->idNodo);
			destinoIntermedioReduce->temp_file_name = string_from_format(
					"reduce_intermedio_%i_%i.temp", infoJob.idJob,
					destinoIntermedioReduce->id_reduce);

			resReduceEnNodo = ordenarReduceAJob(destinoIntermedioReduce,
					origenesReduceEnNodo, sockJob);

			if (resReduceEnNodo > 0) {
				printf("Mandó orden Reduce Intermedio %s\n",
						destinoIntermedioReduce->temp_file_name);
				fflush(stdout);
			} else {
				printf("Falló la orden de reduce inermedio %s\n",
						destinoIntermedioReduce->temp_file_name);
				fflush(stdout);
				return NULL;
			}

			agregarReducePendiente(listaReducePendientes,
					destinoIntermedioReduce);

			list_add(destinosIntermedios, destinoIntermedioReduce);

			//asigno como responsable del reduce final a aquel nodo que sea el útlimo
			//que aplicó un reduce intermedio. Esto es así porque es más probable
			//que si ese acaba de ser exitoso, siga activo ¿?
			idNodoDondeAplicarReduceFinal = idAux->idNodo;

			void destruirTIdNodo(t_idNodo* unTIdNodo) {
				free(unTIdNodo);
			}

			list_remove_and_destroy_element(listaIdNodosDondeAplicarReduce, 0,
					(void*) destruirTIdNodo);

		}

		//muestro la lista de intermedios que tengo al salir del while
		void mostrar(t_DestinoReduce* unDestino) {
			printf("ID REDUCE: %d\n", unDestino->id_reduce);
			printf("ID NODO: %d\n", unDestino->id_nodo);
			printf("TEMP FILE NAME: %s\n", unDestino->temp_file_name);
			fflush(stdout);
		}

		list_iterate(destinosIntermedios, (void*) mostrar);

		printf("\nEnvie todos las ordenes de reduce intermedios\n");
		fflush(stdout);
		//ojo acá: pthread_mutex_unlock(&conexionFS);

		t_ResultadoReduce resultadoDeReduce;
		int resultado;

		bool encuentraReducePendiente(t_DestinoReduce* unDestinoReduce) {
			return unDestinoReduce->id_reduce == resultadoDeReduce.id_reduce;
		}

		void eliminarReducePendiente(t_DestinoReduce* unDestinoReduce) {
			free(unDestinoReduce->temp_file_name);
			free(unDestinoReduce);
		}

		void mostrarReducePendientes(t_ReducePendiente* reducePendiente) {
			printf("\nReduce pendiente: %i \n",
					reducePendiente->numeroDeReducePendiente);
			fflush(stdout);
		}

		//aca recibir resultado de reduce intermedios que se van haciendo
		while (!list_is_empty(listaReducePendientes)) {

			resultado = recibirResultadoDeReduce(sockJob, &resultadoDeReduce);

			list_iterate(listaReducePendientes,
					(void*) mostrarReducePendientes);
			fflush(stdout);

			//En base al resultado, realiza las tareas que le corresponden
			if (resultado > 0) {
				switch (resultadoDeReduce.prot) {
				case OK_REDUCE:

					log_info(marta_logger, "Resultado de reduce: %i",
							resultadoDeReduce.id_reduce);

					list_remove_by_condition(listaReducePendientes,
							(void*) encuentraReducePendiente);
					break;
				case NODO_NOT_FOUND:
					log_info(marta_logger, "NO SE ENCONTRO UN NODO");
					log_info(marta_logger, "CANCELO TODO EL JOB");

					//XXX no se si acá va el return NULL;
					return NULL;

					break;
				}

			} else {
				log_info(marta_logger,
						"Recibí resultado incorrecto, cancelo Job");
				/*list_destroy_and_destroy_elements(listaReducePendientes,
				 (void *) eliminarReducePendiente);
				 Puede ir list_destroy() solamente. Con esa funcionaría*/
				return NULL;
			}
		}

		printf("Despues de intermedios y antes de hacer finales\n");
		fflush(stdout);

		list_iterate(destinosIntermedios, (void*) mostrar);

		//Una vez realizados todos los reduce intermedios en los nodos
		//debo aplicar un reduce general y final sobre esos tmp de reduce intermedios
		//Sería como hacer un reduce sinCombiner en la lista tmp de reduce del job
		int resultadoReduceFinal;

		t_DestinoReduce* destinoFinalReduce = malloc(sizeof(t_DestinoReduce));

		//XXX el nombre del reduce no se si siempre será único. supuestamente si
		destinoFinalReduce->id_reduce = ++(ultimoIDReduce);
		destinoFinalReduce->id_nodo = idNodoDondeAplicarReduceFinal;
		destinoFinalReduce->ip_nodo = ipDeNodo(idNodoDondeAplicarReduceFinal);
		destinoFinalReduce->puerto_nodo = puertoDeNodo(
				idNodoDondeAplicarReduceFinal);
		/*destinoFinalReduce->temp_file_name = string_from_format(
				"reduce_final_%i_%i.temp", infoJob.idJob,
				destinoFinalReduce->id_nodo);*/
		destinoFinalReduce->temp_file_name = strdup(infoJob.pathDeResultado);

		resultadoReduceFinal = ordenarUltimoReduceAJob(destinoFinalReduce,
				destinosIntermedios, sockJob);

		if (resultadoReduceFinal > 0) {
			log_info(marta_logger, "Reduce con combiner enviado exitosamente");
		} else {
			log_info(marta_logger,
					"falló envío de la orden de reduce. Cancelo job");
			//todo mandar a borrar cosas y eso. No se bien que hacer acá
			return NULL;
		}

		//recibir el resultado del reduce final
		log_info(marta_logger,
				"Hice todos los reduce intermedios y mande a hacer el final");

		t_ResultadoReduce resultadoFinal;

		resultado = recibirResultadoDeReduce(sockJob, &resultadoFinal);

		//En base al resultado, realiza las tareas que le corresponden
		if (resultado > 0) {
			switch (resultadoFinal.prot) {
			case OK_REDUCE:
				log_info(marta_logger, "Reduce final realizado exitosamente");

				break;
			case NODO_NOT_FOUND:
				log_info(marta_logger, "NO SE ENCONTRO UN NODO");
				log_info(marta_logger,
						"CANCELO TODO EL JOB. LASTIMA ERA EL FINAL");
				return NULL;
				break;
			}

		} else {
			log_info(marta_logger,
					"Recibí resultado de orden a reduce incorrecto. Cancelo job");
			free(destinoFinalReduce);
			return NULL;
		}

		void destruirDestinoReduce(t_DestinoReduce* unDestinoReduce) {
			free(unDestinoReduce);
		}

		char* archivoFinal = strdup(destinoFinalReduce->temp_file_name);
		free(destinoFinalReduce);

		list_destroy_and_destroy_elements(destinosIntermedios,
				(void*) destruirDestinoReduce);

		return archivoFinal;

	} else //Sin combiner
	{
		int idNodoDondeAplicarReduce = mejorNodoDondeAplicarReduceSinCombiner(
				mapsTemporalesDeLosArchivosDelJob);

		//ahora le tengo que decir al job, que en el nodo idNodoDondeAplicarRedecu,
		//aplique reduce sobre nodo-tal, nodo-tal.Archtemp

		int resultado;
		t_list* origenesDeReduce; //lista de: (idNodo, archivoTemporal)

		origenesDeReduce = list_map(mapsTemporalesDeLosArchivosDelJob,
				(void*) convertirAEstructuraNecesaria);

		t_DestinoReduce* destinoReduce = malloc(sizeof(t_DestinoReduce));

		//XXX el nombre del reduce no se si siempre será único. supuestamente si
		destinoReduce->id_reduce = ++(ultimoIDReduce);
		destinoReduce->id_nodo = idNodoDondeAplicarReduce;
		destinoReduce->ip_nodo = ipDeNodo(idNodoDondeAplicarReduce);
		destinoReduce->puerto_nodo = puertoDeNodo(idNodoDondeAplicarReduce);
		/*destinoReduce->temp_file_name = string_from_format(
				"reduce_final_%i_%i.temp", infoJob.idJob,
				destinoReduce->id_nodo);*/
		destinoReduce->temp_file_name = strdup(infoJob.pathDeResultado);

		resultado = ordenarReduceAJob(destinoReduce, origenesDeReduce, sockJob);

		if (resultado > 0) {
			log_info(marta_logger, "Reduce sin combiner enviado exitosamente");
		} else {
			log_info(marta_logger,
					"falló envío de la orden de reduce. Cancelo job");
			//todo mandar a borrar cosas y eso. No se bien que hacer acá
			return NULL;
		}

		//recibir el resultado del reduce final
		log_info(marta_logger,
				"Hice todos los reduce intermedios y mande a hacer el final");

		t_ResultadoReduce resultadoFinal;

		resultado = recibirResultadoDeReduce(sockJob, &resultadoFinal);

		//En base al resultado, realiza las tareas que le corresponden
		if (resultado > 0) {
			switch (resultadoFinal.prot) {
			case OK_REDUCE:
				log_info(marta_logger,
						"Reduce final sin combiner realizado exitosamente");

				break;
			case NODO_NOT_FOUND:
				log_info(marta_logger, "NO SE ENCONTRO UN NODO");
				log_info(marta_logger, "CANCELO TODO EL JOB.");
				return NULL;

				break;
			}

		} else {
			log_info(marta_logger,
					"Recibí resultado de orden a reduce incorrecto. Cancelo job");
			free(destinoReduce);
			return NULL;
		}

		archivoFinal = strdup(destinoReduce->temp_file_name);

		free(destinoReduce);
		list_destroy_and_destroy_elements(origenesDeReduce, (void*) free);
	}

	/*list_destroy_and_destroy_elements(mapsTemporalesDeLosArchivosDelJob,
	 (void*) free);*/

	return archivoFinal;
}

