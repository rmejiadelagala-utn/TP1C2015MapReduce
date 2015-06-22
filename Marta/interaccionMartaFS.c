#include "interaccionMartaFS.h"
void* interaccionMartaFS(void* sock){
	int socket= *(int*)sock;
	int recibido;
	int protocolo;
	t_registro_id_ipPuerto* registroRecibido = malloc(sizeof(t_registro_id_ipPuerto));
	t_registro_id_ipPuerto* registroACambiar = malloc(sizeof(t_registro_id_ipPuerto));
	presentarseMartaAlFileSystem(socket);


	while ((recibido=recvall(socket,&protocolo,4))>0) {

		switch (protocolo) {

		case MARTA_ACTUALIZA_EL_REGISTRO:
			printf("Se recibio el mensaje.\n");
			registroRecibido = recibirRegistroNodo(socket);
			registroACambiar = buscarRegistroPorId(registroRecibido->id);
			if(registroACambiar!=NULL) printf("El registro a cambiar tiene esta info:\nID:%d\nIP:%s\nPuerto:%d\n",registroACambiar->id,inet_ntoa(registroACambiar->ip),registroACambiar->puerto);
			else printf("El nodo no fue encontrado\n");
			printf("El registro recibido tiene esta info:\nID:%d\nIP:%s\nPuerto:%d\n",registroRecibido->id,inet_ntoa(registroRecibido->ip),registroRecibido->puerto);
			if(registroACambiar == NULL) list_add(listaRegistrosIDIP,registroRecibido);
			else registroACambiar = registroRecibido;
			if(registroACambiar!=NULL) printf("El registro ahora tiene esta info:\nID:%d\nIP:%s\nPuerto:%d\n",registroACambiar->id,inet_ntoa(registroACambiar->ip),registroACambiar->puerto);
			fflush(stdout);


		}

	}
}
