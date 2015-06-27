/*
 * creacionDeHilos.c
 *
 *  Created on: 24/6/2015
 *      Author: rmejia
 */

#include "job.h"

void* hilo_mapper (void* arg_thread){
	t_arg_hilo_map ordenToNodo;
	t_ordenMap ordenMapper;
	ordenToNodo= *((t_arg_hilo_map*)arg_thread);
	int sockMarta,sockNodo, ip_nodo, puerto_nodo,block,res;
	int resOper; //resultado de la operación de mapper
	char* tmp_file_name;
	char* codigoMapper;
	sockMarta=ordenToNodo.sockMarta;
	codigoMapper=strdup(ordenToNodo.rutinaMapper);
	ordenMapper=*(ordenToNodo.ordenMapper);
	tmp_file_name=strdup(ordenMapper.temp_file_name);
	block=ordenMapper.block;
	ip_nodo=ordenMapper.ip_nodo;
	puerto_nodo=ordenMapper.puerto_nodo;

	//Me conecto a nodo
	//para que pueda funcionar creo esta variable ip, ya que me la estan
	//pasando como entero y necesito para mis funciones un char
	char* ip_nodo_char="127.0.0.1";
	sockNodo= crearCliente(ip_nodo_char,puerto_nodo);
	//Enviamos rutina mapper a Nodo
	res=enviarMapperANodo(sockNodo,codigoMapper,block,tmp_file_name);
	if(res<0){
		printf("todo mal, no pude enviar mapper a Nodo: %d", ip_nodo);
		exit(-1);
	}
	//recibir resultado de la Operacion mapper desde el Nodo
/*	resOper=recibirResultadoFromNodo(sockNodo);
	responderOrdenMapAMarta(sockMarta,ordenToNodo.ordenMapper,resOper);
	*/
	return NULL;
}

void crearHiloMapper(int sockMarta, char* codMapper) {
	pthread_t thread_map;
	t_ordenMap *ordenMapper;
	t_arg_hilo_map* arg_thread;
	ordenMapper=recibirOrdenMapDeMarta(sockMarta);
	arg_thread=(t_arg_hilo_map*)malloc(sizeof(t_arg_hilo_map));
	arg_thread->sockMarta=sockMarta;
	arg_thread->rutinaMapper=strdup(codMapper);
	arg_thread->ordenMapper=ordenMapper;

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
