/*
 * interfaces.c
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */
#include "interfaces.h"

#include <socketes/paquetesNodo.h>


void* conexionFS(void* arg){

    char *DATOS;
	uint32_t tam_disco;
	t_hilofs* ptr;
	ptr = (t_hilofs*) arg;
	int socket = ptr->socket;
	printf("Nodo Conectado al Filesystem\n");

	tam_disco = obtener_tamanio_disco(ptr->ARCH_BIN);
	int cant_bloques = tam_disco / BLKSIZE;

	DATOS = mapeo_disco(ptr->ARCH_BIN);

	t_nodoParaFS* infoNodo = malloc(sizeof(t_nodoParaFS));
	infoNodo->IP_NODO.s_addr = inet_addr (ptr->IP_NODO);
	infoNodo->PUERTO_NODO = ptr->PUERTO_NODO;
	infoNodo->NODO_NEW = (ptr->NODO_NEW == 's' || ptr->NODO_NEW == 'S');
	infoNodo->CANT_BLOQUES = cant_bloques;

	presentarseAlFileSystem(infoNodo, socket);
	system("clear");

	uint32_t nrobloque;
	int protocolo;
	int recibido;
	int resultado;

	while ((recibido=recvall(ptr->socket,&protocolo,4))>0) {
		fflush(stdout);
		switch (protocolo){
		  case SET_BLOQUE:
			  resultado = setBloqueDeFileSystem(socket, DATOS, BLKSIZE);//Si devuelve 0 es porque recibio todo
			  msync(ptr->ARCH_BIN,BLKSIZE,0);
			  respuestaSetBloque(socket,resultado);
			  break;

		 case GET_BLOQUE:

			  resultado = getBloqueParaFileSystem(socket,DATOS);/*

			    memcpy(mensaje_nodo->info,DATOS + obtenerDirBloque(nrobloque), BLKSIZE);
			  stream = empaquetar_mensaje(mensaje_nodo);

			  printf("Voy a mandar el bloque %d\n",nrobloque);
			  send(ptr->socket, &stream->length, sizeof(int),0);

			  if (enviar_mensaje(ptr->socket, stream->data, stream->length) > 0)
				printf("Enviando mensaje a Filesystem\n");

	      break;

		  default: //Recibo mensaje GET_FILE del FS
			  printf("Recibiendo mensaje GET_FILE del Filesystem\n");
			  char * namefile = string_new();

			  strcpy(namefile, mensaje_fs->info);

			  free(mensaje_nodo->info);
			  free(mensaje_nodo);
			  free(stream);

			  mensaje_nodo->id = GET_FILE_OK;
			  mensaje_nodo->tipo = 'N';
			  mensaje_nodo->info = getFileContent(namefile, ptr->DIR_TEMP);

			  stream = empaquetar_mensaje(mensaje_nodo);

			  if (enviar_mensaje(ptr->socket, stream->data, stream->length) > 0)
				  printf("Enviando mensaje GET_FILE_OK a Filesystem\n");

		  break;*/

		}


	}

	return 0;
}

void* conexionJobs(void* sockJobNodo){

	int sock_in = *(int*)sockJobNodo;


	return 0;
}

/*
void* conexionNodos(void * sockNodo){

	return 0;
}
*/

