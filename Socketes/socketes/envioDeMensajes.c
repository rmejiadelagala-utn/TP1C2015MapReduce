#include "envioDeMensajes.h"

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
		        total += bytesRead;
		        bytesleft -= bytesRead;
		    }

		    tamanio = total; // return number actually received here

		    return bytesRead==-1?-1:0; // return -1 on failure, 0 on success
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

void bufferAgregarInt(t_buffer* buffer, int unInt){
	int tamanioAnterior=buffer->tamanio;
	buffer->tamanio+=sizeof(int);
	buffer = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,&unInt,sizeof(int));
}

void bufferAgregarChar(t_buffer* buffer, char unChar){
	int tamanioAnterior=buffer->tamanio;
	buffer->tamanio+=sizeof(char);
	buffer = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,&unChar,sizeof(char));
}

void bufferAgregarString(t_buffer* buffer,char* unString){
	int tamanioAnterior=buffer->tamanio;
	int largoString = strlen(unString)+1;
	bufferAgregarInt(buffer, largoString);
	buffer->tamanio+= largoString;
	buffer = realloc(buffer->data,buffer->tamanio);
	memcpy(buffer->data + tamanioAnterior,unString,largoString);
}



//Enviar buffer

int enviarBuffer(t_buffer* buffer, int socket){
	int tamanioMensaje = buffer->tamanio + sizeof(buffer->tamanio);
	int offset = 0;
	void* mensajeAEnviar = malloc(tamanioMensaje);
	memcpy(mensajeAEnviar+offset,buffer->tamanio,sizeof(buffer->tamanio));
	offset+=sizeof(buffer->tamanio);
	memcpy(mensajeAEnviar+offset,buffer->data,buffer->tamanio);
	int resultado= sendall(socket,mensajeAEnviar,tamanioMensaje);
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

//FileSystem
	//A Nodo
	int enviarBloqueANodo(int socket, int numeroDeBloque, char* dataBloque){
		t_buffer* buffer = crearBufferConProtocolo(SET_BLOQUE);
		bufferAgregarInt(buffer,numeroDeBloque);
		bufferAgregarString(buffer,dataBloque);
		enviarBuffer(buffer,socket);
	}


//Deserializar mensajes

//FileSystem
	//De Nodo
	t_nodoParaFS* conocerAlNodo(int socket){
		t_nodoParaFS* unNodo;
		recvall(socket, unNodo, sizeof(t_nodoParaFS));
		return unNodo;
	}
//Nodo
	//De FileSystem
	int setBloqueDeFileSystem(int socket, int* nroBloque, char* dataBin, int block_size){
		int tamanio;
		recvall(socket,nroBloque,4);
		recvall(socket,&tamanio,4);
		return recvall(socket,dataBin+(block_size*nroBloque),tamanio);
	}
