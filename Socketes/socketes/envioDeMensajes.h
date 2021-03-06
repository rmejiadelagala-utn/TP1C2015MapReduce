
#ifndef ENVIODEMENSAJES_H_
#define ENVIODEMENSAJES_H_
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <commons/collections/list.h>


typedef struct {
	uint32_t tamanio;
	char* data;

}t_buffer;
//estructuras interaccion Marta-Fs
typedef struct {
	int id;
	int numeroDeBloqueEnNodo;
	uint32_t tamanioBloque;
}__attribute__ ((__packed__)) t_bloqueEnNodo;

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
 	uint32_t block_size;
 } t_ordenMap;


//sendall y recvall aseguran que se mande/reciba toda la informacion

int sendall(int socket, void *mensaje, size_t tamanio);

int recvall(int socket, void *mensaje, size_t tamanio);

//PROTOCOLOS

enum protocolos {
	CONEXION_NODO_A_FS, CONEXION_MARTA_A_FS, SET_BLOQUE, SET_ULTIMO_BLOQUE, GET_BLOQUE, RTA_SET_BLOQUE, DISCONNECTED,
	NODO_NOT_FOUND, ORDER_MAP, MARTA_ACTUALIZA_EL_REGISTRO, ENVIO_BLOQUEARCH_A_MARTA,
	MARTA_SE_CAYO_UN_NODO,ORDER_REDUCE, FIN_OPERACION,CONEXION_JOB_A_NODO,RES_MAP,RES_REDUCE,
	OK_MAP,OK_REDUCE, NOTOK_MAP,NOTOK_REDUCE,COPIAR_ARCHIVO_A_FS_LOCAL,VER_BLOQUE_NODO,
	COPIAR_BLOQUE_NODO, DAME_LISTA_DE_ARCHIVOS_FS, ENVIO_ARCHIVOS_NODO_NODO, NODO_ESTAS, COPIATE_RESULTADO, NODO_DAME_ARCHIVO_A_FS
};

//Primitivas

t_buffer* crearBuffer();

t_buffer* crearBufferConProtocolo(int protocolo);

void liberarBuffer(t_buffer* buffer);

void bufferAgregarInt(t_buffer* buffer, int unInt);

void bufferAgregarChar(t_buffer* buffer, char unChar);

void bufferAgregarString(t_buffer* buffer,char* unString, uint32_t tamanio);

char* recibirString(int socket);

void recibirStringEn(int socket, char** stringReceptor);

int recibirInt(int socket);

void enviarError(int socket);

int recibirIntEnOrden(int socket, uint32_t *numero);

uint32_t recibirProtocoloEnOrden(int socket);

//Enviar buffer

int enviarBuffer(t_buffer* buffer, int socket);

int enviarProtocolo(int protocolo,int socket);

//Serializar mensajes

//Nodo
	//A FileSystem
	int presentarseAlFileSystem(t_nodoParaFS* infoNodo, int socket);

	int enviarBloqueAFileSystem(int socket, char* dataBin, int funcionARealizar, int tamanioBloque);

//FileSystem
	//A Nodo

	int enviarBloqueANodo(int socket, int numeroDeBloque, char* dataBloque,uint32_t comienzoBloque, uint32_t tamanio);

	int pedirBloqueANodo(int socket, int numeroDeBloque, int protocoloDeRegreso, int tamanioBloque);

	//A Marta
	int enviarCopiasAMarta(int socket, t_list* copias);

	int actualizarIdIpPuertoEnMarta(int socket, t_registro_id_ipPuerto* unRegistro);

	int martaSeCayoUnNodo(int socket, int id);

//Marta
	//A FileSystem
	int presentarseMartaAlFileSystem(int socket);
	
	int dameBloqueArchFS(int socket,char *nombreArchivo , int padre, int numeroBloqueArch);
	
	int dameListaArchFS(int socket,char **archivos, int cantArchivos);

//Job
	//A Nodo
	int enviarMapperANodo(int socket, char* mapper, int nroDeBloqueNodo, uint32_t block_size, char* nombreArchivoTmp, int tamanioMapper );

//Deserializar mensajes

//FileSystem
	//De Nodo
	t_nodoParaFS* conocerAlNodo(int socket);

	int recibirBloqueDeNodo(int socket, void* buffer);

	//De Marta
	t_bloqueDeArchPedido* recibirPedidoDeBloqueArch(int socket);

	t_list* recibirPedidoListaArchivos(int socket);

//Nodo
	//De FileSystem
	int setBloqueDeFileSystem(int socket, char* dataBin, int block_size);

	int respuestaSetBloque(int socket, int resultado);

	int getBloqueParaFileSystem(int socket,char* dataBin, int block_size);

 //Job
 	//De Marta
 	t_ordenMap* recibirOrdenMapDeMarta(int sockMarta);

 //Marta
 	//De Filesystem
 	t_registro_id_ipPuerto* recibirRegistroNodo(int socket);

 	int recibirBloqueArchFS(int socketAuxiliar,t_list* copiasDeBloque);
#endif
