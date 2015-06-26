/*
 * creacionDeHilos.c
 *
 *  Created on: 24/6/2015
 *      Author: rmejia
 */

#include "job.h"
#include "string.h"

void* hilo_mapper (void* arg_thread){
	t_arg_hilo_map ordenToNodo= *((t_arg_hilo_map*)arg_thread);
	int sockNodo, ip_nodo, puerto_nodo,block;
	char* tmp_file_name;
	tmp_file_name=strdup(ordenToNodo.ordenMapper->temp_file_name);
	block=ordenToNodo.ordenMapper->block;
	ip_nodo=ordenToNodo.ordenMapper->ip_nodo;
	puerto_nodo=ordenToNodo.ordenMapper->puerto_nodo;
	//Me conecto a nodo
	//para que pueda funcionar creo esta variable ip, ya que me la estan
	//pasando como entero y necesito para mis funciones un char
	char* ip_nodo_char="127.0.0.1";
	sockNodo= crearCliente(ip_nodo_char,puerto_nodo);
	enviarMapperANodo(sockNodo, ordenToNodo.rutinaMapper,block,tmp_file_name);

	return NULL;
}

void crearHiloMapper(int sockMarta, char* rutinaMap) {
	pthread_t thread_map;
	t_arg_hilo_map* arg_thread;
	arg_thread=(t_arg_hilo_map*)malloc(sizeof(t_arg_hilo_map));
	arg_thread->sockMarta=sockMarta;
	arg_thread->rutinaMapper=strdup(rutinaMap);
	arg_thread->ordenMapper=recibirOrdenMapDeMarta(sockMarta);

	pthread_create (&thread_map, NULL, &hilo_mapper, (void*)arg_thread);
}

/*
void* hilo_reduce (void* arg){
	*((int*)arg)=1; //el contenido a lo que apunta arg es =1, es solo para probar
}

void crearHiloReduce(int sockMarta) {
	pthread_t thread_reduce;
//	pthread_create (&thread_reduce, NULL, &hilo_reduce, NULL);
}
*/
