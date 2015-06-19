#include "envioDeMensajes.h"

//sendall y recvall aseguran que se mande/reciba toda la informacion

int sendall(int socket, void *mensaje, size_t tamanio){
	return send(socket, mensaje, tamanio, 0);
}

int recvall(int socket, void *mensaje, size_t tamanio){
	return recv(socket, mensaje, tamanio, 0);
}

//PROTOCOLOS

//Primitivas

t_buffer* crearBuffer(){
	t_buffer* buffer=malloc(sizeof(t_buffer));
	buffer->data=0;
	buffer->tamanio=0;
	return buffer;
}

void bufferAgregarInt(t_buffer* buffer, int unInt){
	int tamanioAnterior=buffer->tamanio;
	buffer->tamanio+=sizeof(int);
	buffer = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,&unInt,sizeof(int));
}

void bufferAgregarString(t_buffer* buffer,char* unString){
	int tamanioAnterior=buffer->tamanio+1;
	int largoString = strlen(unString);
	buffer->tamanio+= largoString;
	buffer = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,unString,largoString);
}

//Enviar buffer

int enviarBuffer(t_buffer* buffer, int socket){
	return sendall(socket,buffer->data,buffer->tamanio);
}

int enviarBufferConProtocolo(t_buffer* buffer, int socket, int protocolo){
	bufferAgregarInt(buffer,protocolo);
	return enviarBuffer(buffer, socket);
}

//Serializar mensajes

//Nodo-Salida
	//A FileSystem
	int presentarseAlFileSystem(t_nodoParaFS* infoNodo){
		t_buffer* buffer = crearBuffer();

		infoNodo->IP_NODO;
		infoNodo->NODO_NEW;
		infoNodo->PUERTO_NODO;
	}
