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
	        if (bytesSent == 0) { break; }
	        total += bytesSent;
	        bytesleft -= bytesSent;
	    }

	    tamanio = total; // return number actually sent here

	    return bytesSent;

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

void bufferAgregarString(t_buffer* buffer,char* unString, uint32_t tamanio){
	int largoString = tamanio;
	bufferAgregarInt(buffer,largoString);

	uint32_t tamanioAnterior=buffer->tamanio;

	buffer->tamanio+= largoString;
	buffer->data = realloc(buffer->data,buffer->tamanio);

	memcpy((buffer->data + tamanioAnterior),unString,largoString);

}

char* recibirString(int socket){
	int tamanioString;
	if(recvall(socket,&tamanioString,sizeof(int))<1) return NULL;
	char* string;
	string =malloc(sizeof(char)*tamanioString);
	if(recvall(socket,string,tamanioString)<1) return NULL;
	return string;
}

void recibirStringEn(int socket, char** stringReceptor){

	int tamanioString;

	tamanioString=recibirInt(socket);

	*stringReceptor = malloc(tamanioString);

	recvall(socket,*stringReceptor,tamanioString);

}

int recibirInt(int socket){
	int entero;
	if(recvall(socket,&entero,sizeof(int))<1) return -1;
	return entero;
}

void enviarError(int socket){
	int error=-1;
	sendall(socket,&error,sizeof(int));
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

int enviarProtocolo(int protocolo,int socket){
	void* buffer = crearBuffer();
	bufferAgregarInt(buffer,protocolo);
	return enviarBuffer(buffer,socket);
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
	int enviarBloqueAFileSystem(int socket, char* dataBin, int funcionARealizar, int tamanioBloque){
		t_buffer* buffer = crearBufferConProtocolo(GET_BLOQUE);
		bufferAgregarInt(buffer,funcionARealizar);
		bufferAgregarString(buffer,dataBin,tamanioBloque);
		return enviarBuffer(buffer,socket);
	}

//FileSystem
	//A Nodo
	int enviarUltimoBloqueANodo(int socket, int numeroDeBloque, char* dataBloque,uint32_t comienzoBloque, uint32_t tamanio){
		t_buffer* buffer = crearBufferConProtocolo(SET_ULTIMO_BLOQUE);
		bufferAgregarInt(buffer,numeroDeBloque);
		bufferAgregarString(buffer,dataBloque+comienzoBloque,tamanio);
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}

	int enviarBloqueANodo(int socket, int numeroDeBloque, char* dataBloque,uint32_t comienzoBloque, uint32_t tamanio){
		t_buffer* buffer = crearBufferConProtocolo(SET_BLOQUE);
		bufferAgregarInt(buffer,numeroDeBloque);
		bufferAgregarString(buffer,dataBloque+comienzoBloque,tamanio);
		int resultado = enviarBuffer(buffer,socket);
		if(!resultado) close(socket);
		return resultado;
	}
	int pedirBloqueANodo(int socket, int numeroDeBloque, int protocoloDeRegreso, int tamanioBloque){
		t_buffer* buffer = crearBufferConProtocolo(GET_BLOQUE);
		bufferAgregarInt(buffer,protocoloDeRegreso);
		bufferAgregarInt(buffer,numeroDeBloque);
		bufferAgregarInt(buffer,tamanioBloque);
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}
	//A Marta
	int actualizarIdIpPuertoEnMarta(int socket, t_registro_id_ipPuerto* unRegistro){
		if(socket>0){
		fflush(stdout);
		int tamanioAEnviar = sizeof(t_registro_id_ipPuerto) + 4;
		void* buffer = malloc(tamanioAEnviar);
		int protocolo = MARTA_ACTUALIZA_EL_REGISTRO;
		memcpy(buffer,&protocolo,4);
		memcpy(buffer+4,unRegistro,sizeof(t_registro_id_ipPuerto));
		return sendall(socket, buffer, tamanioAEnviar);}
		return -1;
	}
	int martaSeCayoUnNodo(int socket, int id){
		t_buffer* buffer = crearBufferConProtocolo(MARTA_SE_CAYO_UN_NODO);
		bufferAgregarInt(buffer,id);
		return enviarBuffer(buffer,socket);
	}
	int enviarCopiasAMarta(int socket, t_list* copias){
		int resultado=1;
		int protocolo=ENVIO_BLOQUEARCH_A_MARTA;
		int enviado = sendall(socket,&protocolo,sizeof(int));
		if(enviado<=0) return enviado;

		int cantidad = list_size(copias);
		enviado=sendall(socket,&cantidad,sizeof(int));
		if(enviado<=0) return enviado;

		void enviarCopia(t_bloqueEnNodo* unBloque){
			enviado = sendall(socket,unBloque,sizeof(t_bloqueEnNodo));
			if(enviado<=0) resultado=enviado;
		}

		list_iterate(copias,(void*)enviarCopia);
		return resultado; //TODO VALIDACIONES
	}
//Marta
	//A Filesystem
	int presentarseMartaAlFileSystem(int socket){ //TODO posiblemente haya que agregar alguna funcionalidad mas
			t_buffer* buffer = crearBufferConProtocolo(CONEXION_MARTA_A_FS);
			return enviarBuffer(buffer,socket);
		}
	int dameBloqueArchFS(int socket,char *nombreArchivo, int padre, int numeroBloqueArch){
		printf("Voy a agregar el string %s\n",nombreArchivo);
		fflush(stdout);
		t_buffer *buffer = crearBufferConProtocolo(ENVIO_BLOQUEARCH_A_MARTA);
		bufferAgregarString(buffer, nombreArchivo, strlen(nombreArchivo)+1);
		bufferAgregarInt(buffer,padre);
		bufferAgregarInt(buffer, numeroBloqueArch);
		return enviarBuffer(buffer, socket);
		return 1;
	}
	int dameListaArchFS(int socket,char **archivos, int cantArchivos){
		t_buffer* buffer = crearBufferConProtocolo(DAME_LISTA_DE_ARCHIVOS_FS);
		bufferAgregarInt(buffer,cantArchivos);
		int i;
		for(i=0;i<cantArchivos;i++){
			printf("Pido el archivo %s",archivos[i]);
			bufferAgregarString(buffer,archivos[i],strlen(archivos[i])+1);
		}

		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}
//Job
	//A Nodo
	int enviarMapperANodo(int socket, char* mapper, int nroDeBloqueNodo, uint32_t tamanioBloque, char* nombreArchivoTmp, int tamanioScript ){
		//t_buffer* buffer = crearBufferConProtocolo(CONEXION_JOB_A_NODO);
		//bufferAgregarInt(buffer,ORDER_MAP);
		t_buffer* buffer = crearBufferConProtocolo(ORDER_MAP);
        bufferAgregarString(buffer,mapper,tamanioScript);
		bufferAgregarInt(buffer,nroDeBloqueNodo);
		bufferAgregarInt(buffer,tamanioBloque);
        bufferAgregarString(buffer,nombreArchivoTmp,strlen(nombreArchivoTmp)+1);
		int resultado = enviarBuffer(buffer,socket);
		return resultado;
	}

//Deserializar mensajes

//FileSystem
	//De Nodo
	t_nodoParaFS* conocerAlNodo(int socket){
		t_nodoParaFS* unNodo = malloc(sizeof(t_nodoParaFS));
		if(recvall(socket, unNodo, sizeof(t_nodoParaFS))<1){
			free(unNodo);
			return NULL;
		}
		return unNodo;
	}
	int recibirBloqueDeNodo(int socket, void* buffer){
		int tamanio;
		if(recvall(socket,&tamanio,4)<1) return -1;
		*(char**)buffer = malloc(tamanio);
		int resultado = recvall(socket,*(char**)buffer,tamanio);
		if(resultado<1) return -1;
		return tamanio;

	}
	//De Marta
	t_bloqueDeArchPedido* recibirPedidoDeBloqueArch(int socket){
		t_bloqueDeArchPedido* bloque = malloc(sizeof(t_bloqueDeArchPedido));
		int tamanio;
		int padre;
		int numeroBloqueArch;
		if(recvall(socket, &tamanio,sizeof(int))<1) return NULL;
		bloque->nombreArch = malloc(tamanio);
		if(recvall(socket,bloque->nombreArch,tamanio)<1){
			free(bloque->nombreArch);
			return NULL;
		}
		if(recvall(socket,&bloque->padre,sizeof(int))<1) return NULL;
		if(recvall(socket,&bloque->numeroDeBloqueArch,sizeof(int))<1) return NULL;
		return bloque;
	}
	t_list* recibirPedidoListaArchivos(int socket){
		char* archivoPedido;
		int cantArchivos;
		if(recvall(socket,&cantArchivos,sizeof(int))<1) return NULL;
		int i;
		t_list* archivosPedidos=list_create();
		for(i=0;i<cantArchivos;i++){
			if(( archivoPedido = recibirString(socket))==NULL){
				list_destroy_and_destroy_elements(archivosPedidos,free);
				return NULL;
			}
			list_add(archivosPedidos,archivoPedido);
		}
		return archivosPedidos;
	}
//Nodo
	//De FileSystem
	int setBloqueDeFileSystem(int socket, char* dataBin, int block_size){
		int tamanio;
		int nroBloque;
		if(recvall(socket,&nroBloque,4)<1){
			printf("Recibi mal el numero de bloque\n");
			return -1;
		}
		if(recvall(socket,&tamanio,4)<1){
			printf("Recibi mal el tamaño de bloque\n");
			return -1;
		}
		int tamanioEnKB = tamanio/1024;
		printf("Recibo un bloque de %dKiB\n",tamanioEnKB);
		return recvall(socket,dataBin+(block_size*nroBloque),tamanio);

	}
	int respuestaSetBloque(int socket, int resultado){

		t_buffer* buffer = crearBufferConProtocolo(RTA_SET_BLOQUE);
		bufferAgregarInt(buffer,resultado);
		return enviarBuffer(buffer,socket);
	}
	int getBloqueParaFileSystem(int socket,char* dataBin, int block_size){
		int protocoloDeRegreso;
		if(recvall(socket,&protocoloDeRegreso,4)<1) return -1;
		int nroBloque;
		if(recvall(socket,&nroBloque,4)<1) return -1;
		int tamanioBloque;
		if(recvall(socket,&tamanioBloque,4)<1) return -1;
		return enviarBloqueAFileSystem(socket, dataBin+(nroBloque*block_size),protocoloDeRegreso,tamanioBloque);
	}
//Marta
	//De Filesystem
	t_registro_id_ipPuerto* recibirRegistroNodo(int socket){
		t_registro_id_ipPuerto* unRegistro = malloc(sizeof(t_registro_id_ipPuerto));
		if(recvall(socket, unRegistro, sizeof(t_registro_id_ipPuerto))<1){
			free(unRegistro);
			return NULL;
		}
		return unRegistro;
	}
	int recibirBloqueArchFS(int socketAuxiliar,t_list* copiasDeBloque){ //TODO retornar un resultado
		int cantidadDeCopias;
		if(recvall(socketAuxiliar,&cantidadDeCopias,sizeof(int))<1) return -1;
		if(cantidadDeCopias==-1 || cantidadDeCopias==0) return -1;
		fflush(stdout);
		int i;
		for(i=0;i<cantidadDeCopias;i++){
			t_bloqueEnNodo* copiaDeBloque=malloc(sizeof(t_bloqueEnNodo));
			if(recvall(socketAuxiliar,copiaDeBloque,sizeof(t_bloqueEnNodo))<1)return -1;
			fflush(stdout);
			list_add(copiasDeBloque,copiaDeBloque);
		}
		return 1;
	}
//Job
	//De Marta
	t_ordenMap* recibirOrdenMapDeMarta(int sockMarta){
		t_ordenMap* ordenMap = malloc(sizeof(t_ordenMap));
		uint32_t temp_file_len;
		int r1 = recvall(sockMarta,&(ordenMap->id_map),sizeof(uint32_t));
		int r2 = recvall(sockMarta,&(ordenMap->id_nodo),sizeof(uint32_t));
		int r3 = recvall(sockMarta,&(ordenMap->ip_nodo),sizeof(uint32_t));
		int r4 = recvall(sockMarta,&(ordenMap->puerto_nodo),sizeof(uint32_t));
		int r5 = recvall(sockMarta,&(ordenMap->block),sizeof(uint32_t));
		int r6 = recvall(sockMarta,&(ordenMap->block_size),sizeof(uint32_t));
		int r7 = recvall(sockMarta,&temp_file_len,sizeof(uint32_t));
		ordenMap->temp_file_name= malloc(temp_file_len);
		int r8 = recvall(sockMarta,ordenMap->temp_file_name,temp_file_len);
		ordenMap->temp_file_name[temp_file_len]='\0';//cierro el string
		if((r1<1)||(r2<1)||(r3<1)||(r4<1)||(r5<1)||(r6<1)||(r7<1)||(r8<1)){
			free(ordenMap);
			return NULL;
		}
		return ordenMap;
	}
