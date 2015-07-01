#include "envioDeMensajes.h"
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <ifaddrs.h>
#include <string.h>

//sendall y recvall aseguran que se mande/reciba toda la informacion

int sendall(int socket, void *mensaje, size_t tamanio){

	    int total = 0;        // how many bytes we've sent
	    int bytesleft = tamanio; // how many we have left to send
	    int bytesSent;

	    while(total < tamanio) {
	        bytesSent = send(socket, mensaje+total, bytesleft, 0);
	        if (bytesSent == -1) { break; }
	        total += bytesSent;
	        bytesleft -= bytesSent;
	    }

	    tamanio = total; // return number actually sent here

	    return bytesSent==-1?-1:0; // return -1 on failure, 0 on success

}

int recvall(int socket, void *mensaje, size_t tamanio){
	 	 	 int total = 0;        // how many bytes we've received
		    int bytesleft = tamanio; // how many we have left to receive
		    int bytesRead;

		    while(total < tamanio) {
		        bytesRead = recv(socket, mensaje+total, bytesleft, 0);
		        if (bytesRead == -1) { break; }
		        if (bytesRead == 0) {break;}
		        total += bytesRead;
		        bytesleft -= bytesRead;
		    }

		    tamanio = total; // return number actually received here

		    return bytesRead; // return -1 on failure, 0 on node disconnection
}

//PROTOCOLOS

//Primitivas

t_buffer* crearBuffer(){
	t_buffer* buffer=malloc(sizeof(t_buffer));
	buffer->data=0;
	buffer->tamanio=0;
	return buffer;
}

t_buffer* crearBufferConProtocolo(int protocolo){
	t_buffer* buffer = crearBuffer();
	bufferAgregarInt(buffer,protocolo);
	return buffer;
}

void liberarBuffer(t_buffer* buffer){
	free(buffer->data);
	free(buffer);
}

void bufferAgregarInt(t_buffer* buffer, int unInt){
	int tamanioAnterior=buffer->tamanio;
	buffer->tamanio+=sizeof(int);
	buffer->data = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,&unInt,sizeof(int));
}

void bufferAgregarChar(t_buffer* buffer, char unChar){
	int tamanioAnterior=buffer->tamanio;
	buffer->tamanio+=sizeof(char);
	buffer->data = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,&unChar,sizeof(char));
}

void bufferAgregarString(t_buffer* buffer,char* unString, int tamanio){
	int largoString = tamanio;
	bufferAgregarInt(buffer,largoString);

	int tamanioAnterior=buffer->tamanio;

	buffer->tamanio+= largoString;
	buffer->data = realloc(buffer->data,buffer->tamanio);

	memcpy((buffer->data + tamanioAnterior),unString,largoString);

}

char* recibirString(int socket){
	int tamanioString;
	recvall(socket,&tamanioString,sizeof(int));
	char* string=malloc(tamanioString);
	recvall(socket,string,tamanioString);
	return string;
}

int recibirIntEnOrden(int socket, uint32_t *numero) {
	int resultado;

	if ((resultado = recv(socket, numero, sizeof(uint32_t), 0)) == -1) {
		//mostrar_error(-1, "Error recieving");
		return -1;
	}
	*numero = ntohl(*numero);

	return resultado;
}

uint32_t recibirProtocoloEnOrden(int socket) {
	uint32_t prot;

	int resultado = recibirIntEnOrden(socket,&prot);

	if (resultado == 0)
		return DISCONNECTED;

	if (resultado == -1)
		return -1;

	return prot;
}


//Enviar buffer

int enviarBuffer(t_buffer* buffer, int socket){
	int resultado= sendall(socket,buffer->data,buffer->tamanio);
	liberarBuffer(buffer);
	return resultado;
}

//Serializar mensajes

//Nodo
	//A FileSystem
	int presentarseAlFileSystem(t_nodoParaFS* infoNodo, int socket){
		int tamanioAEnviar = sizeof(t_nodoParaFS) + 4;
		void* buffer = malloc(tamanioAEnviar);
		int protocolo = CONEXION_NODO_A_FS;
		memcpy(buffer,&protocolo,4);
		memcpy(buffer+4,infoNodo,sizeof(t_nodoParaFS));
		return sendall(socket, buffer, tamanioAEnviar);
	}
	int enviarBloqueAFileSystem(int socket, char* dataBin, int funcionARealizar){
		t_buffer* buffer = crearBufferConProtocolo(GET_BLOQUE);
		bufferAgregarInt(buffer,funcionARealizar);
		bufferAgregarString(buffer,dataBin,strlen(dataBin)+1);
		return enviarBuffer(buffer,socket);
	}

//FileSystem
	//A Nodo
	int enviarBloqueANodo(int socket, int numeroDeBloque, char* dataBloque,int comienzoBloque, int tamanio){
		t_buffer* buffer = crearBufferConProtocolo(SET_BLOQUE);
		bufferAgregarInt(buffer,numeroDeBloque);
		bufferAgregarString(buffer,dataBloque+comienzoBloque,tamanio);
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}
	int pedirBloqueANodo(int socket, int numeroDeBloque, int protocoloDeRegreso){
		t_buffer* buffer = crearBufferConProtocolo(GET_BLOQUE);
		bufferAgregarInt(buffer,protocoloDeRegreso);
		bufferAgregarInt(buffer,numeroDeBloque);
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}
	//A Marta
	int actualizarIdIpPuertoEnMarta(int socket, t_registro_id_ipPuerto* unRegistro){
		int tamanioAEnviar = sizeof(t_registro_id_ipPuerto) + 4;
		void* buffer = malloc(tamanioAEnviar);
		int protocolo = MARTA_ACTUALIZA_EL_REGISTRO;
		memcpy(buffer,&protocolo,4);
		memcpy(buffer+4,unRegistro,sizeof(t_registro_id_ipPuerto));
		return sendall(socket, buffer, tamanioAEnviar);
	}
	int martaSeCayoUnNodo(int socket, int id){
		t_buffer* buffer = crearBufferConProtocolo(MARTA_SE_CAYO_UN_NODO);
		bufferAgregarInt(buffer,id);
		return enviarBuffer(buffer,socket);
	}
	int enviarCopiasAMarta(int socket, t_list* copias){
		int protocolo=ENVIO_BLOQUEARCH_A_MARTA;
		sendall(socket,&protocolo,sizeof(int));

		int cantidad = list_size(copias);
		sendall(socket,&cantidad,sizeof(int));

		void enviarCopia(t_bloqueEnNodo* unBloque){
			printf("ID bloque:%d\n",unBloque->id);
			fflush(stdout);
			sendall(socket,unBloque,sizeof(t_bloqueEnNodo));
		}

		list_iterate(copias,(void*)enviarCopia);
		return 1; //TODO VALIDACIONES
	}
//Marta
	//A Filesystem
	int presentarseMartaAlFileSystem(int socket){ //TODO posiblemente haya que agregar alguna funcionalidad mas
			t_buffer* buffer = crearBufferConProtocolo(CONEXION_MARTA_A_FS);
			return enviarBuffer(buffer,socket);
		}
	int dameBloqueArchFS(int socket,char *nombreArchivo , int padre, int numeroBloqueArch){
		printf("entro a dameBloqueArh:D\n" );
		fflush(stdout);
		t_buffer *buffer = crearBufferConProtocolo(ENVIO_BLOQUEARCH_A_MARTA);
		bufferAgregarString(buffer, nombreArchivo, strlen(nombreArchivo)+1);
		bufferAgregarInt(buffer,padre);
		bufferAgregarInt(buffer, numeroBloqueArch);
		printf("socket antes de enviar es %d :D\n", socket);
		fflush(stdout);
		return enviarBuffer(buffer, socket);
		return 1;
	}
	int dameListaArchFS(int socket,char **archivos, int cantArchivos){
		t_buffer* buffer = crearBufferConProtocolo(DAME_LISTA_DE_ARCHIVOS_FS);
		bufferAgregarInt(buffer,cantArchivos);
		int i;
		for(i=0;i<cantArchivos;i++) bufferAgregarString(buffer,archivos[i],strlen(archivos[i]));
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}
//Job
	//A Nodo
	int enviarMapperANodo(int socket, char* mapper, int nroDeBloqueNodo, char* nombreArchivoTmp ){
		t_buffer* buffer = crearBufferConProtocolo(CONEXION_JOB_A_NODO);
		bufferAgregarInt(buffer,ORDER_MAP);
        bufferAgregarString(buffer,mapper,strlen(mapper)+1);
		bufferAgregarInt(buffer,nroDeBloqueNodo);
        bufferAgregarString(buffer,nombreArchivoTmp,strlen(nombreArchivoTmp)+1);
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}

//Deserializar mensajes

//FileSystem
	//De Nodo
	t_nodoParaFS* conocerAlNodo(int socket){
		t_nodoParaFS* unNodo = malloc(sizeof(t_nodoParaFS));
		recvall(socket, unNodo, sizeof(t_nodoParaFS));
		return unNodo;
	}
	int recibirBloqueDeNodo(int socket, void* buffer){
		int tamanio;
		recvall(socket,&tamanio,4);
		*(char**)buffer = malloc(tamanio+2);
		int resultado = recvall(socket,*(char**)buffer,tamanio);
		return resultado;

	}
	//De Marta
	t_bloqueDeArchPedido* recibirPedidoDeBloqueArch(int socket){
		t_bloqueDeArchPedido* bloque = malloc(sizeof(t_bloqueDeArchPedido));
		int tamanio;
		int padre;
		int numeroBloqueArch;
		recvall(socket, &tamanio,sizeof(int));
		bloque->nombreArch = malloc(tamanio);
		recvall(socket,bloque->nombreArch,tamanio);
		recvall(socket,&bloque->padre,sizeof(int));
		recvall(socket,&bloque->numeroDeBloqueArch,sizeof(int));
		return bloque;
	}
	t_list* recibirPedidoListaArchivos(int socket){
		int cantArchivos;
		recvall(socket,&cantArchivos,sizeof(int));
		int i;
		t_list* archivosPedidos=list_create();
		for(i=0;i<cantArchivos;i++){
			char* archivoPedido = recibirString(socket);
			list_add(archivosPedidos,archivoPedido);
		}
		return archivosPedidos;
	}
//Nodo
	//De FileSystem
	int setBloqueDeFileSystem(int socket, char* dataBin, int block_size){
		int tamanio;
		int nroBloque;
		recvall(socket,&nroBloque,4);
		recvall(socket,&tamanio,4);
		recvall(socket,dataBin+(block_size*nroBloque),tamanio);
		memset(dataBin+(block_size*nroBloque)+tamanio,'\n',1);
		memset(dataBin+(block_size*nroBloque)+tamanio+1,'\0',1);
		return nroBloque;
	}
	int respuestaSetBloque(int socket, int resultado){

		t_buffer* buffer = crearBufferConProtocolo(RTA_SET_BLOQUE);
		bufferAgregarInt(buffer,resultado);
		return enviarBuffer(buffer,socket);
	}
	int getBloqueParaFileSystem(int socket,char* dataBin, int block_size){
		int protocoloDeRegreso;
		recvall(socket,&protocoloDeRegreso,4);
		int nroBloque;
		recvall(socket,&nroBloque,4);
		return enviarBloqueAFileSystem(socket, dataBin+(nroBloque*block_size),protocoloDeRegreso);
	}
//Marta
	//De Filesystem
	t_registro_id_ipPuerto* recibirRegistroNodo(int socket){
		t_registro_id_ipPuerto* unRegistro = malloc(sizeof(t_registro_id_ipPuerto));
		recvall(socket, unRegistro, sizeof(t_registro_id_ipPuerto));
		return unRegistro;
	}
	void recibirBloqueArchFS(int socketAuxiliar,t_list* copiasDeBloque){ //TODO retornar un resultado
		int cantidadDeCopias;
		recvall(socketAuxiliar,&cantidadDeCopias,sizeof(int));
		printf("\n\nCantidad de copias: %d\n\n",cantidadDeCopias);
		fflush(stdout);
		int i;
		for(i=0;i<cantidadDeCopias;i++){
			t_bloqueEnNodo* copiaDeBloque=malloc(sizeof(t_bloqueEnNodo));
			recvall(socketAuxiliar,copiaDeBloque,sizeof(t_bloqueEnNodo));
			printf("ID de copia: %d\n",copiaDeBloque->id);
			fflush(stdout);
			list_add(copiasDeBloque,copiaDeBloque);
		}
	}
//Job
	//De Marta
	t_ordenMap* recibirOrdenMapDeMarta(int sockMarta){
		t_ordenMap* ordenMap = malloc(sizeof(t_ordenMap));
		uint32_t temp_file_len;
		recvall(sockMarta,&(ordenMap->id_map),sizeof(uint32_t));
		recvall(sockMarta,&(ordenMap->id_nodo),sizeof(uint32_t));
		recvall(sockMarta,&(ordenMap->ip_nodo),sizeof(uint32_t));
		recvall(sockMarta,&(ordenMap->puerto_nodo),sizeof(uint32_t));
		recvall(sockMarta,&(ordenMap->block),sizeof(uint32_t));
		recvall(sockMarta,&temp_file_len,sizeof(uint32_t));
		ordenMap->temp_file_name= malloc(temp_file_len);
		recvall(sockMarta,ordenMap->temp_file_name,temp_file_len);
		return ordenMap;
	}
