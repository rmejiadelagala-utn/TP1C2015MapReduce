#include<stdio.h>
#include<sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
	int tamanio;
	char* data;

} t_buffer;


//Estructuras del Nodo
typedef struct {
	int IP_NODO;
	uint16_t PUERTO_NODO;
	char NODO_NEW;
	int CANT_BLOQUES;
} __attribute__ ((__packed__)) t_nodoParaFS;


//sendall y recvall aseguran que se mande/reciba toda la informacion

int sendall(int socket, void *mensaje, size_t tamanio);

int recvall(int socket, void *mensaje, size_t tamanio);

//PROTOCOLOS

//Primitivas

t_buffer* crearBuffer();

void bufferAgregarInt(t_buffer* buffer, int unInt);

void bufferAgregarString(t_buffer* buffer,char* unString);

//Enviar buffer

int enviarBuffer(t_buffer* buffer, int socket);

int enviarBufferConProtocolo(t_buffer* buffer, int socket, int protocolo);

//Serializar mensajes

//Nodo-Salida
	//A FileSystem
	int presentarseAlFileSystem(t_nodoParaFS* infoNodo);
