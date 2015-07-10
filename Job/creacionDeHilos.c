/*
 * creacionDeHilos.c
 *
 *  Created on: 24/6/2015
 *      Author: rmejia
 */

#include "job.h"
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/config.h>
#include<socketes/servidor.h>
#include<socketes/envioDeMensajes.h>
#include<stdbool.h>
#include <pthread.h>

int recibirResultadoFromNodo(int sockNodo){
		uint32_t recibido, protocolo, rptaNodoAJob;
		if((recibido=recvall(sockNodo,&protocolo,sizeof(uint32_t)))<0){
			return -1;
		}
		if(protocolo==RES_MAP){
			recvall(sockNodo,&rptaNodoAJob,sizeof(uint32_t));
		}else {
			printf("no entiendo el protocolo, usa: RES_MAP");
		}
		return rptaNodoAJob;

	}

int responderOrdenMapAMarta(int sockMarta,t_ordenMap ordenMapper, int resOper){
	int result_envio;
	t_buffer* buffer = crearBuffer();//crearBufferConProtocolo(RES_MAP);
	if(resOper==OK_MAP){
		bufferAgregarInt(buffer,OK_MAP);
	} else
		bufferAgregarInt(buffer,NODO_NOT_FOUND);
	bufferAgregarInt(buffer,ordenMapper.id_map);
	//bufferAgregarInt(buffer,ordenMapper.id_nodo);
	result_envio=enviarBuffer(buffer,sockMarta);
	return result_envio;
}

void* hilo_mapper (void* arg_thread){

	t_arg_hilo_map ordenToNodo;
	t_ordenMap ordenMapper;
	ordenToNodo= *((t_arg_hilo_map*)arg_thread);
	int sockMarta,sockNodo, puerto_nodo,block,res,envioRes;
	int resOper; //resultado de la operación de mapper
	char* tmp_file_name;
	char* codigoMapper;
	char* ip_nodo_char;
	sockMarta=ordenToNodo.sockMarta;
	printf("El path del codigo mapper es %s\n",ordenToNodo.pathMapper);
	codigoMapper=subirCodigoDeMapper(ordenToNodo.pathMapper);
	ordenMapper=*(ordenToNodo.ordenMapper);
	tmp_file_name=strdup(ordenMapper.temp_file_name);
	block=ordenMapper.block;
	puerto_nodo=ordenMapper.puerto_nodo;
	struct in_addr addr;
	addr.s_addr=ordenMapper.ip_nodo;
	ip_nodo_char= inet_ntoa(addr);
	if ((sockNodo= crearCliente(ip_nodo_char,puerto_nodo))<0){
		void* buffer = crearBufferConProtocolo(NODO_NOT_FOUND);
		int protocolo=NODO_NOT_FOUND;
		printf("\n\n\nProtocolo que le mando a marta: %d\n\n\n",protocolo);
		enviarBuffer(buffer,sockMarta);
		return -1;
	}
	//Enviamos rutina mapper a Nodo
	fflush(stdout);
	res=enviarMapperANodo(sockNodo,codigoMapper,block,tmp_file_name);
	if(res<0){
		printf("todo mal, no pude enviar mapper a Nodo: %d", ip_nodo_char);
		exit(-1);
	}
	//recibir resultado de la Operacion mapper desde el Nodo
	resOper=recibirResultadoFromNodo(sockNodo);
	if(resOper==OK_MAP){
		printf("mapper id= %d, terminó OK, aviso a Marta\n",ordenMapper.id_map);
	} else {
		printf("mapper id= %d, falló, aviso a Marta de esto\n",ordenMapper.id_map);
	}
	//re-enviar a marta resultado de la operacion, recibida de Nodo
	envioRes=responderOrdenMapAMarta(sockMarta,ordenMapper,resOper);
	if(envioRes<0){
		printf("no pude enviar la respuesta a marta, algo pasó\n");
	}
	return NULL;
}

void crearHiloMapper(int sockMarta, char* pathMapper) {
	pthread_t thread_map;
	t_ordenMap *ordenMapper;
	t_arg_hilo_map* arg_thread;
	ordenMapper=recibirOrdenMapDeMarta(sockMarta);
	//Muestro lo que recibo de marta
	printf("Muestro lo que recibo de marta\n");
	printf("id_map:%d\n",ordenMapper->id_map);
	printf("id_nodo:%d\n",ordenMapper->id_nodo);
	printf("ip_nodo:%d\n",ordenMapper->ip_nodo);
	struct in_addr addr;
	addr.s_addr=ordenMapper->ip_nodo;
	printf("muestro ip en formato de numeros y puntos: %s\n",inet_ntoa(addr));
	printf("puerto_nodo:%d\n",ordenMapper->puerto_nodo);
	printf("tmp_arch: %s\n",ordenMapper->temp_file_name);
	arg_thread=(t_arg_hilo_map*)malloc(sizeof(t_arg_hilo_map));
	arg_thread->sockMarta=sockMarta;
	arg_thread->pathMapper=strdup(pathMapper);
	int tamanioDeLaOrden = sizeof(t_ordenMap) + strlen(ordenMapper->temp_file_name);
	arg_thread->ordenMapper=malloc(tamanioDeLaOrden);
	arg_thread->ordenMapper=ordenMapper;

	pthread_create (&thread_map, NULL, hilo_mapper, (void*)arg_thread);
	return ;
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
