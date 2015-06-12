/*
 * interfaces.c
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */
#include "interfaces.h"
#include "paquetesNodo.h"

void* conexionFS(void* arg){

    char *DATOS;
	uint32_t tam_disco;
	t_hilofs* ptr;
	ptr = (t_hilofs*) arg;

	printf("Nodo Conectado al Filesystem");

	tam_disco = obtener_tamanio_disco(ptr->ARCH_BIN);
	int cant_bloques = tam_disco / BLKSIZE;

	DATOS = mapeo_disco(ptr->ARCH_BIN);

	t_mensaje* mensaje_nodo = malloc(sizeof(t_mensaje));

	mensaje_nodo->id = CONEXION_FS;
	mensaje_nodo->tipo = 'N';
	mensaje_nodo->info = string_new();

	//Formato de Mensaje-> IP:PUERTO:CANTBLOQUES:NUEVO
	string_append(&mensaje_nodo->info, ptr->IP_NODO);
	string_append(&mensaje_nodo->info, ":");
	string_append(&mensaje_nodo->info, string_itoa(ptr->PUERTO_NODO));
	string_append(&mensaje_nodo->info, ":");
	string_append(&mensaje_nodo->info, string_itoa(cant_bloques));
	string_append(&mensaje_nodo->info, ":");
	string_append(&mensaje_nodo->info, ptr->NODO_NEW);

/*	strcpy(mensaje->info, ptr->IP_NODO);
	strcat(mensaje->info, ":");
	strcat(mensaje->info, string_itoa(ptr->PUERTO_NODO));
	strcat(mensaje->info, string_itoa(cant_bloques));*/

	t_stream *stream = empaquetar_mensaje(mensaje_nodo);

	if (enviar_mensaje(ptr->socket, stream->data, stream->length) > 0)
		printf("Enviando mensaje a Filesystem\n");

	t_mensaje* mensaje_fs = malloc(sizeof(t_mensaje));
	uint32_t nrobloque;

	while ((recibir_mensaje(ptr->socket, mensaje_fs)) > 0) {

		switch (mensaje_fs->id){
		  case SET_BLOQUE:
			  printf("Recibiendo mensaje SET_BLOQUE del Filesystem\n");
			  char ** data_fs = string_split(mensaje_fs->info, ":");
			  nrobloque = atoi(data_fs[0]);

			  memset(DATOS + obtenerDirBloque(nrobloque),'\0', BLKSIZE );
			  memcpy(DATOS + obtenerDirBloque(nrobloque), data_fs[1], BLKSIZE);

			  free(mensaje_nodo->info);
			  free(mensaje_nodo);
			  free(stream);

			  mensaje_nodo->id = SET_BLOQUE_OK;
			  mensaje_nodo->tipo = 'N';
			  mensaje_nodo->info = string_new();

			  stream = empaquetar_mensaje(mensaje_nodo);
			  if (enviar_mensaje(ptr->socket, stream->data, stream->length) > 0)
			  		printf("Enviando mensaje SET_BLOQUE_OK a Filesystem\n");

			  munmap(DATOS,sizeof(DATOS));// Para que deje grabado la info en Data.bin

		  break;

		  case GET_BLOQUE:
			  printf("Recibiendo mensaje GET_BLOQUE del Filesystem\n");
			  char * datos_fs = string_new();

			  strcpy(datos_fs, mensaje_fs->info);
			  nrobloque = atoi(datos_fs);

			  DATOS = string_new();
			  DATOS = mapeo_disco(ptr->ARCH_BIN);

			  free(mensaje_nodo->info);
			  free(mensaje_nodo);
			  free(stream);

			  mensaje_nodo->id = GET_BLOQUE_OK;
			  mensaje_nodo->tipo = 'N';

			  memcpy(mensaje_nodo->info,DATOS + obtenerDirBloque(nrobloque), BLKSIZE);
			  stream = empaquetar_mensaje(mensaje_nodo);

			  if (enviar_mensaje(ptr->socket, stream->data, stream->length) > 0)
			  		printf("Enviando mensaje GET_BLOQUE_OK a Filesystem\n");

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

		  break;

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

