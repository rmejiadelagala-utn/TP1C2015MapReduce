#include "interaccionMartaFS.h"
void* interaccionMartaFS(void* sock){
	int socket= *(int*)sock;
	int recibido;
	int protocolo;
	t_registro_id_ipPuerto* registroRecibido = malloc(sizeof(t_registro_id_ipPuerto));
	t_registro_id_ipPuerto* registroACambiar = malloc(sizeof(t_registro_id_ipPuerto));
	presentarseMartaAlFileSystem(socket);
	int id;
	t_CargaNodo* cargaNodo;
	int coincideIDNodo(t_CargaNodo* cargaNodo) {
		return cargaNodo->id_nodo == id;
	}
	int coincideIDRegistro(t_registro_id_ipPuerto* unRegistro) {
		return unRegistro->id == id;
	}
	void mostrarCargaNodo(t_CargaNodo* cargaNodo){
		log_info(marta_logger,"ID del Nodo: %d. Su carga actual es: %d\n",cargaNodo->id_nodo, cargaNodo->cantidadOperacionesEnCurso);
	}
	void mostrarRegistro(t_registro_id_ipPuerto* unRegistro){
		log_info(marta_logger,"ID: %d.--------------IP: %s. Puerto: %d",unRegistro->id,inet_ntoa(unRegistro->ip),unRegistro->puerto);
	}
	while ((recibido=recvall(socket,&protocolo,4))>0) {
		switch (protocolo) {

		case MARTA_ACTUALIZA_EL_REGISTRO:
			log_info(marta_logger,"Se recibio el mensaje para actualizar registroIDIP");
			registroRecibido = recibirRegistroNodo(socket);
			id=registroRecibido->id;
			list_add(listaRegistrosIDIP,registroRecibido);
			cargaNodo=malloc(sizeof(t_CargaNodo));
			cargaNodo->id_nodo = id;
			cargaNodo->cantidadOperacionesEnCurso = 0;
			list_add(cargaNodos,cargaNodo);
	//		printf("\nNodos:\n");
	//		list_iterate(cargaNodos,(void*)mostrarCargaNodo);
			printf("\nTabla de nodos:\n");
			list_iterate(listaRegistrosIDIP,(void*)mostrarRegistro);
			break;
		case MARTA_SE_CAYO_UN_NODO:
			recvall(socket,&id,sizeof(int));

			pthread_mutex_lock(&mutexListaNodo);

			list_remove_and_destroy_by_condition(cargaNodos,(void*)coincideIDNodo,(void*)free);

			pthread_mutex_unlock(&mutexListaNodo);

			list_remove_and_destroy_by_condition(listaRegistrosIDIP,(void*)coincideIDRegistro,(void*)free);
//			printf("Nodos:\n");
//			list_iterate(cargaNodos,(void*)mostrarCargaNodo);
			printf("Tabla de nodos:\n");
			list_iterate(listaRegistrosIDIP,(void*)mostrarRegistro);
			break;
		case ENVIO_BLOQUEARCH_A_MARTA:
			sem_post(&funcionesMarta);
			log_info(marta_logger,"El protocolo es %d",protocolo);
			log_info(marta_logger,"Despertate marta.\n");
			sem_wait(&interaccionFS);
			break;
		case DAME_LISTA_DE_ARCHIVOS_FS:
			sem_post(&funcionesMarta);

			sem_wait(&interaccionFS);
		}
	}

}
