
#ifndef ENVIODEMENSAJES_H_
#define ENVIODEMENSAJES_H_
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct {
	int tamanio;
	char* data;

}t_buffer;

//Estructuras del Nodo
typedef struct {
	struct in_addr IP_NODO;
	uint16_t PUERTO_NODO;
	char NODO_NEW;
	int CANT_BLOQUES;
	int ID;
}__attribute__ ((__packed__)) t_nodoParaFS;

typedef struct {
	int id;
	uint16_t puerto;
	struct in_addr ip;
}__attribute__ ((__packed__)) t_registro_id_ipPuerto;

typedef struct {
	int padre;
	int numeroDeBloqueArch;
	char* nombreArch;

} t_bloqueDeArchPedido;

// estructuras para Job
typedef struct {
 	uint32_t id_map;
 	uint32_t id_nodo;
 	uint32_t ip_nodo;
 	uint32_t puerto_nodo;
 	uint32_t block;
 	char* temp_file_name;
 } t_ordenMap;


//sendall y recvall aseguran que se mande/reciba toda la informacion

int sendall(int socket, void *mensaje, size_t tamanio);

int recvall(int socket, void *mensaje, size_t tamanio);

//PROTOCOLOS

enum protocolos {
	CONEXION_NODO_A_FS, CONEXION_MARTA_A_FS, SET_BLOQUE, GET_BLOQUE, RTA_SET_BLOQUE, DISCONNECTED,
	MAP_OK, NODO_NOT_FOUND, ORDER_MAP, MARTA_ACTUALIZA_EL_REGISTRO, ENVIO_BLOQUEARCH_A_MARTA,
	ORDER_REDUCE, FIN_OPERACION,CONEXION_JOB_A_NODO
};

//Primitivas

t_buffer* crearBuffer();

t_buffer* crearBufferConProtocolo(int protocolo);

void liberarBuffer(t_buffer* buffer);

void bufferAgregarInt(t_buffer* buffer, int unInt);

void bufferAgregarChar(t_buffer* buffer, char unChar);

void bufferAgregarString(t_buffer* buffer,char* unString, int tamanio);

void buffer_add_string(t_buffer* self, char *string_to_add);

int recibirIntEnOrden(int socket, uint32_t *numero);

uint32_t recibirProtocoloEnOrden(int socket);

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

	//A Marta
	int actualizarIdIpPuertoEnMarta(int socket, t_registro_id_ipPuerto* unRegistro);

//Marta
	//A FileSystem

	int presentarseMartaAlFileSystem(int socket);
	
	int dameBloqueArchFS(int socket,char *nombreArchivo , int padre, int numeroBloqueArch);
	
//Job
	//A Nodo
	int enviarMapperANodo(int socket, char* mapper, int nroDeBloqueNodo, char* nombreArchivoTmp );

//Deserializar mensajes

//FileSystem
	//De Nodo
	t_nodoParaFS* conocerAlNodo(int socket);

	int recibirBloqueDeNodo(int socket, void* buffer);

//Nodo
	//De FileSystem
	int setBloqueDeFileSystem(int socket, char* dataBin, int block_size);

	int respuestaSetBloque(int socket, int resultado);

	int getBloqueParaFileSystem(int socket,char* dataBin, int block_size);

 //Job
 	//De Marta
 	t_ordenMap* recibirOrdenMapDeMarta(int sockMarta);
#endif
