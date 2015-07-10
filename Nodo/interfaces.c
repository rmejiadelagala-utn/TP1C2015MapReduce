/*
 * interfaces.c
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */
#include "interfaces.h"

#include <socketes/paquetesNodo.h>
    char *DATOS;

void* conexionFS(void* arg){


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
	infoNodo->ID = ptr->ID;

	presentarseAlFileSystem(infoNodo, socket);
	system("clear");

	uint32_t nrobloque;
	int protocolo;
	int recibido;
	int nroBloqueRecibido;
	int resultado;

	while ((recibido=recvall(ptr->socket,&protocolo,4))>0) {

		switch (protocolo){
		  case SET_BLOQUE:
			  nroBloqueRecibido = setBloqueDeFileSystem(socket, DATOS, BLKSIZE);//Si devuelve 0 es porque recibio todo
			  msync(ptr->ARCH_BIN,strlen(DATOS),MS_SYNC);
			  respuestaSetBloque(socket,nroBloqueRecibido);
			  printf("Almacené la informacion en el bloque numero %d correctamente.\n",nroBloqueRecibido);
			  fflush(stdout);
			  break;

		 case GET_BLOQUE:

			  resultado = getBloqueParaFileSystem(socket,DATOS,BLKSIZE);

		}


	}

	return 0;
}

void* conexionJobs(void* sockJobNodo){

	int sock_in = *(int*)sockJobNodo;
	int recibido;
	int i;
	int protocolo;
	char* script;
	char* archivoSalida;
	int nroBloque;
	int cantArchivosRecibidos;
	t_list* archivosRecibidos;
	while((recibido=recvall(sock_in,&protocolo,sizeof(int)))>1){
		switch(protocolo){
		case ORDER_MAP:
			//Dejo que el nodo conteste que todo esta bien por ahora, despues hay que hacer el map aca

			printf("Recibi una orden de map, esta todo OK.\n");
			script=recibirString(sock_in);
			recvall(sock_in,&nroBloque,sizeof(int));
			archivoSalida=strdup("tmp/");
			string_append(&archivoSalida,recibirString(sock_in));
			printf("El script recibido es %s\n",script);
			printf("El archivo de salida recibido es %s\n",archivoSalida);
			crearScriptMapper(script);
			char* dataAUX = malloc(BLKSIZE);
			memcpy(dataAUX,DATOS+nroBloque*BLKSIZE,strlen(DATOS+nroBloque*BLKSIZE));
			ejecutarMapper("/tmp/mapper.sh",archivoSalida,dataAUX);
			free(dataAUX);
			protocolo=RES_MAP;
			sendall(sock_in,&protocolo,sizeof(int));
			protocolo=OK_MAP;
			sendall(sock_in,&protocolo,sizeof(int));
			break;

		case ENVIO_ARCHIVOS_NODO_NODO:
			archivosRecibidos = list_create();
			cantArchivosRecibidos = recibirInt(sock_in);
			for(i=0;i<cantArchivosRecibidos;i++){
				guardarEnDisco(recibirArchivo(sock_in));
			}
			break;
		}
	}


	return 0;
}
