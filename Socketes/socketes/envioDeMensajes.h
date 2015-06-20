
#ifndef ENVIODEMENSAJES_H_
#define ENVIODEMENSAJES_H_
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
	struct in_addr IP_NODO;
	uint16_t PUERTO_NODO;
	char NODO_NEW;
	int CANT_BLOQUES;
}__attribute__ ((__packed__)) t_nodoParaFS;


//sendall y recvall aseguran que se mande/reciba toda la informacion

int sendall(int socket, void *mensaje, size_t tamanio);

int recvall(int socket, void *mensaje, size_t tamanio);

//PROTOCOLOS

enum protocolos {
	CONEXION_NODO_A_FS,SET_BLOQUE, GET_BLOQUE, RTA_SET_BLOQUE
};

//Primitivas

t_buffer* crearBuffer();

t_buffer* crearBufferConProtocolo(int protocolo);

void liberarBuffer(t_buffer* buffer);

void bufferAgregarInt(t_buffer* buffer, int unInt);

void bufferAgregarChar(t_buffer* buffer, char unChar);

void bufferAgregarString(t_buffer* buffer,char* unString, int tamanio);

//Enviar buffer

int enviarBuffer(t_buffer* buffer, int socket);

//Serializar mensajes

//Nodo
	//A FileSystem
	int presentarseAlFileSystem(t_nodoParaFS* infoNodo, int socket);

	int enviarBloqueAFileSystem(int socket, char* dataBin);

//FileSystem
	//A Nodo

	int enviarBloqueANodo(int socket, int numeroDeBloque, char* dataBloque,int comienzoBloque, int tamanio);

	int pedirBloqueANodo(int socket, int numeroDeBloque);
//Deserializar mensajes

//FileSystem
	//De Nodo
	t_nodoParaFS* conocerAlNodo(int socket);

	int recibirBloqueDeNodo(int socket, void** buffer);

//Nodo
	//De FileSystem
	int setBloqueDeFileSystem(int socket, char* dataBin, int block_size);

	int respuestaSetBloque(int socket, int resultado);

	getBloqueParaFileSystem(int socket,char* dataBin);
#endif
