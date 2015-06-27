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
		printf("ID del Nodo: %d.\n",cargaNodo->id_nodo);
	}
	void mostrarRegistro(t_registro_id_ipPuerto* unRegistro){
		printf("ID: %d.--------------IP: %s. Puerto: %d\n",unRegistro->id,inet_ntoa(unRegistro->ip),unRegistro->puerto);
	}
	pthread_mutex_lock(&mutexConexionFS);
	while ((recibido=recvall(socket,&protocolo,4))>0) {
		switch (protocolo) {
		fflush(stdout);
		case MARTA_ACTUALIZA_EL_REGISTRO:
			printf("Se recibio el mensaje.\n");
			registroRecibido = recibirRegistroNodo(socket);
			id=registroRecibido->id;
			list_add(listaRegistrosIDIP,registroRecibido);
			cargaNodo=malloc(sizeof(t_CargaNodo));
			cargaNodo->id_nodo = id;
			cargaNodo->cantidadOperacionesEnCurso = 0;
			list_add(cargaNodos,cargaNodo);
			printf("\nNodos:\n");
			list_iterate(cargaNodos,(void*)mostrarCargaNodo);
			printf("\nTabla de nodos:\n");
			list_iterate(listaRegistrosIDIP,(void*)mostrarRegistro);
			fflush(stdout);
			break;
		case MARTA_SE_CAYO_UN_NODO:
			recvall(socket,&id,sizeof(int));
			list_remove_and_destroy_by_condition(cargaNodos,(void*)coincideIDNodo,(void*)free);
			list_remove_and_destroy_by_condition(listaRegistrosIDIP,(void*)coincideIDRegistro,(void*)free);
			printf("Nodos:\n");
			list_iterate(cargaNodos,(void*)mostrarCargaNodo);
			printf("Tabla de nodos:\n");
			list_iterate(listaRegistrosIDIP,(void*)mostrarRegistro);
			fflush(stdout);
			break;
		}
	}

}
void* recibirBloque(void* sock){
			printf("entro a recibirBloque :D\n");
			fflush(stdout);
			int socket= *(int*)sock;
			presentarseMartaAlFileSystem(socket);
			presentarseMartaAlFileSystem(socket);
			dameBloqueArchFS(socket,"unArchivo",1,2);
			presentarseMartaAlFileSystem(socket);
			presentarseMartaAlFileSystem(socket);
			presentarseMartaAlFileSystem(socket);
			presentarseMartaAlFileSystem(socket);
			 //FIXME deshardcodear
			t_list* copiasDeBloque=list_create();
			recibirBloqueArchFS(socket,copiasDeBloque);
			void mostrarBloque(t_bloqueEnNodo* unBloque){
				printf("ID bloque:%d\nNumero de bloque:%d\n",unBloque->id,unBloque->numeroDeBloqueEnNodo);
				fflush(stdout);
			}
			list_iterate(copiasDeBloque,(void*)mostrarBloque);
			return 1;
		}
