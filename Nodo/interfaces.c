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

	printf("Nodo Conectado al Filesystem\n");

	tam_disco = obtener_tamanio_disco(ptr->ARCH_BIN);
	int cant_bloques = tam_disco / BLKSIZE;

	DATOS = mapeo_disco(ptr->ARCH_BIN);

	t_nodoParaFS* nodo = malloc(sizeof(t_nodoParaFS));
	nodo->IP_NODO = inet_addr (ptr->IP_NODO);
	nodo->PUERTO_NODO = ptr->PUERTO_NODO;
	nodo->NODO_NEW = (ptr->NODO_NEW == 's' || ptr->NODO_NEW == 'S');


	//nodo->CANT_BLOQUES = cant_bloques;

	//Formato de Mensaje-> IP:PUERTO:CANTBLOQUES:NUEVO

	t_mensaje* mensaje_fs = malloc(sizeof(t_mensaje));
	uint32_t nrobloque;
	int bytesLeidos;

	while ((bytesLeidos=recibir_mensaje(ptr->socket, &mensaje_fs)) > 0) {

		switch (mensaje_fs->id){
		  case SET_BLOQUE:
			  printf("Recibiendo mensaje SET_BLOQUE del Filesystem\n");
			  printf("Lei %d bytes\n",bytesLeidos);
			  int tamanioDatos_fs = bytesLeidos-sizeof(int)-sizeof(char);
			  char * datosRecibidos_fs;// = malloc(tamanioDatos_fs);
			  datosRecibidos_fs=strdup(mensaje_fs->info);
			  char* nroBloqueString=malloc(1);
			  memcpy(nroBloqueString,mensaje_fs->info,1);
			  printf("El string que recibi es: %s\n",nroBloqueString);
			  nrobloque=atoi(nroBloqueString);

			  printf("El numero de bloque es: %d\n",nrobloque);
			  //datosRecibidos_fs++;
			  printf("Recibi esto: %s\n",datosRecibidos_fs);
			  printf("El tamaño recibido fue: %d\n",tamanioDatos_fs-1 );

			  //DATOS=malloc(tamanioDatos_fs-1);
			  printf("%d\n",obtenerDirBloque(nrobloque));
			  //memset(DATOS + obtenerDirBloque(nrobloque),'\0', BLKSIZE );
			  memcpy(DATOS + obtenerDirBloque(nrobloque), datosRecibidos_fs, tamanioDatos_fs-1);

			  printf("El bin.data va a tener: %s\n",DATOS);
			  printf("Y pesa: %d\n",strlen(DATOS));

			  free(mensaje_nodo->info);
			  free(mensaje_nodo);
			  free(stream);
			  munmap(DATOS,sizeof(DATOS));
			  mensaje_nodo=malloc(sizeof(t_mensaje));
			  mensaje_nodo->id = SET_BLOQUE_OK;
			  mensaje_nodo->tipo = 'N';
			  mensaje_nodo->info = string_new();

			  stream = empaquetar_mensaje(mensaje_nodo);
			  if (enviar_mensaje(ptr->socket, stream->data, stream->length) > 0)
			  		printf("Enviando mensaje SET_BLOQUE_OK a Filesystem\n");

			  // Para que deje grabado la info en Data.bin
			  printf("imprimi al archivo");

		  break;

		  case GET_BLOQUE:
			  printf("Recibiendo mensaje GET_BLOQUE del Filesystem\n");
			  char * datos_fs = strdup(mensaje_fs->info);

			  nrobloque = atoi(datos_fs);

			  DATOS = string_new();
			  DATOS = mapeo_disco(ptr->ARCH_BIN);

			  free(mensaje_nodo->info);
			  free(mensaje_nodo);
			  free(stream);
			  free(stream->data);

			  stream=malloc(sizeof(t_stream));
			  mensaje_nodo=malloc(sizeof(t_mensaje));
			  mensaje_nodo->id = GET_BLOQUE_OK;
			  mensaje_nodo->tipo = 'N';

			  mensaje_nodo->info=malloc(BLKSIZE);
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

