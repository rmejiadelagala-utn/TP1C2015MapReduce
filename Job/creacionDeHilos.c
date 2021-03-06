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
#include <sys/stat.h>

int recibirResultadoFromNodo(int sockNodo) {
	uint32_t recibido, protocolo, rptaNodoAJob;
	recibido = recvall(sockNodo, &protocolo, sizeof(uint32_t));
	if (recibido == 0) {
		printf("Se desconectó un nodo\n");
		return NODO_NOT_FOUND;
	}
	if (recibido < 0) {
		printf("Se produjo un error con el nodo\n");
		return NODO_NOT_FOUND;
	}
	if (protocolo == RES_MAP || protocolo == RES_REDUCE) {
		recibido = recvall(sockNodo, &rptaNodoAJob, sizeof(uint32_t));
		if (recibido == 0) {
			printf("Se desconectó un nodo\n");
			return NODO_NOT_FOUND;
		}
		if (recibido < 0) {
			printf("Se produjo un error con el nodo\n");
			return NODO_NOT_FOUND;
		}
	} else {
		//printf("PROTOCOLO RECIBIDO: %d\n",protocolo);
		printf("no entiendo el protocolo, usa: RES_MAP");
	}
	return rptaNodoAJob;

}


/**********************************************************
 *
 *                   PARTE DEL MAPPER
 *
 **********************************************************/

int responderOrdenMapAMarta(int sockMarta,t_ordenMap ordenMapper, int resOper){
	int result_envio;
	t_buffer* buffer = crearBuffer();//crearBufferConProtocolo(RES_MAP);
	bufferAgregarInt(buffer,resOper);
	bufferAgregarInt(buffer,ordenMapper.id_map);
	bufferAgregarInt(buffer,ordenMapper.id_nodo);
	result_envio=enviarBuffer(buffer,sockMarta);
	return result_envio;
}

void* hilo_mapper (void* arg_thread){


	pthread_detach(pthread_self());
	t_arg_hilo_map ordenToNodo;
	t_ordenMap ordenMapper;
	ordenToNodo= *((t_arg_hilo_map*)arg_thread);
	int sockMarta,sockNodo, puerto_nodo,block,res,envioRes;
	int resOper; //resultado de la operación de mapper
	char* tmp_file_name;
	char* codigoMapper;
	char* ip_nodo_char;
	int block_size;
	sockMarta=ordenToNodo.sockMarta;
	codigoMapper=subirCodigoFromPathToBuffer(ordenToNodo.pathMapper);
	ordenMapper=*(ordenToNodo.ordenMapper);
	tmp_file_name=strdup(ordenMapper.temp_file_name);
	block=ordenMapper.block;
	block_size=ordenMapper.block_size;
	puerto_nodo=ordenMapper.puerto_nodo;
	struct in_addr addr;
	addr.s_addr=ordenMapper.ip_nodo;
	ip_nodo_char= inet_ntoa(addr);
	sockNodo= crearCliente(ip_nodo_char,puerto_nodo);
	if ((sockNodo= crearCliente(ip_nodo_char,puerto_nodo))<0){
		printf("No se pudo conectar al nodo %d\n",ordenMapper.id_nodo);
		fflush(stdout);
		void* buffer = crearBufferConProtocolo(NODO_NOT_FOUND);
		bufferAgregarInt(buffer,ordenMapper.id_map);
		bufferAgregarInt(buffer,ordenMapper.id_nodo);
		enviarBuffer(buffer,sockMarta);
		return NULL;
	}
	//Enviamos rutina mapper a Nodo

	res=enviarMapperANodo(sockNodo,codigoMapper,block,block_size,tmp_file_name,tamanioScript(ordenToNodo.pathMapper));

	if(res<0){
		printf("Todo mal, no pude enviar mapper a Nodo: %s\n", ip_nodo_char);
	}
	else {
		printf("Envie un map al nodo %d\n",ordenMapper.id_nodo);
		pthread_mutex_lock(&contadorMappersDebug_mutex);
		contadorMappersDebug++;
		printf("Llevo enviados %d maps \n",contadorMappersDebug);
		pthread_mutex_unlock(&contadorMappersDebug_mutex);
	}
	//recibir resultado de la Operacion mapper desde el Nodo
	resOper=recibirResultadoFromNodo(sockNodo);
	//printf("le mando a marta el protocolo %d\n", resOper);
	fflush(stdout);
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
	munmap(ordenToNodo.pathMapper,tamanioScript(ordenToNodo.pathMapper));

	close(sockNodo);

	return NULL;
}

void crearHiloMapper(int sockMarta, char* pathMapper) {
	pthread_t thread_map;
	t_ordenMap *ordenMapper;
	t_arg_hilo_map* arg_thread;
	ordenMapper=recibirOrdenMapDeMarta(sockMarta);
	//Muestro lo que recibo de marta
	fflush(stdout);
	struct in_addr addr;
	addr.s_addr=ordenMapper->ip_nodo;
	arg_thread=(t_arg_hilo_map*)malloc(sizeof(t_arg_hilo_map));
	arg_thread->sockMarta=sockMarta;
	arg_thread->pathMapper=strdup(pathMapper);
	int tamanioDeLaOrden = sizeof(t_ordenMap) + strlen(ordenMapper->temp_file_name);
	arg_thread->ordenMapper=malloc(tamanioDeLaOrden);
	arg_thread->ordenMapper=ordenMapper;

	pthread_create (&thread_map, NULL, hilo_mapper, (void*)arg_thread);
	return ;
}

/**********************************************************
 *
 *                   PARTE DEL REDUCE
 *
 **********************************************************/

int responderOrdenReduceAMarta(int sockMarta,t_ordenReduce ordenReduce, int resOper){
	int result_envio;
	t_buffer* buffer = crearBuffer();//crearBufferConProtocolo(RES_MAP);
	if(resOper==OK_REDUCE){
		bufferAgregarInt(buffer,OK_REDUCE);
	} else
		bufferAgregarInt(buffer,NODO_NOT_FOUND);
	bufferAgregarInt(buffer,ordenReduce.id_reduce);
	//bufferAgregarInt(buffer,ordenMapper.id_nodo);
	result_envio=enviarBuffer(buffer,sockMarta);
	return result_envio;
}

t_ordenReduce* recibirOrdenReduceDeMarta(int sockMarta){
		t_ordenReduce* ordenReduce = malloc(sizeof(t_ordenReduce));
		uint32_t cantArchAreducir;
		int i=0;
		uint32_t tamanioArch=0;
		recvall(sockMarta,&(ordenReduce->id_reduce),sizeof(uint32_t));
		recvall(sockMarta,&(ordenReduce->ip_nodo),sizeof(uint32_t));
		recvall(sockMarta,&(ordenReduce->puerto_nodo),sizeof(uint32_t));
		recvall(sockMarta,&(tamanioArch),sizeof(uint32_t));
		(ordenReduce->archResultado)=(char*)malloc(tamanioArch+1);
		recvall(sockMarta,(ordenReduce->archResultado),tamanioArch);
		recvall(sockMarta,&(ordenReduce->cantArchAreducir),sizeof(uint32_t));
		//reservo la cantidad necesaria de punteros t_nodoArchTmp
		ordenReduce->nodosArchTmp=(t_nodoArchTmp*)malloc(sizeof(t_nodoArchTmp*)*(ordenReduce->cantArchAreducir));
		while (i < (ordenReduce->cantArchAreducir)) {
			ordenReduce->nodosArchTmp[i]=malloc(sizeof(t_nodoArchTmp));
			recvall(sockMarta,&(ordenReduce->nodosArchTmp[i]->ip_nodo),sizeof(uint32_t));
			recvall(sockMarta,&(ordenReduce->nodosArchTmp[i]->puerto_nodo),sizeof(uint32_t));
			ordenReduce->nodosArchTmp[i]->archTmp=recibirString(sockMarta);
			i++;
		}
		return ordenReduce;
	}


int enviarReduceANodo(int sockNodo,char* codigoReduce, int cantArchivos,
					  t_nodoArchTmp** nodosArchTmp,char* archResultado, int tamanioScript){
	t_buffer* buffer = crearBufferConProtocolo(ORDER_REDUCE);
    bufferAgregarString(buffer,codigoReduce,tamanioScript);
    bufferAgregarInt(buffer,cantArchivos);
    int i;
    for(i=0;i < cantArchivos;i++){
    	fflush(stdout);
    	bufferAgregarInt(buffer,nodosArchTmp[i]->ip_nodo);
    	    	fflush(stdout);
    	bufferAgregarInt(buffer,nodosArchTmp[i]->puerto_nodo);
    	    	    	fflush(stdout);
    	bufferAgregarString(buffer,nodosArchTmp[i]->archTmp,strlen(nodosArchTmp[i]->archTmp)+1);
    	printf("Mando a crear el reduce:%s\n",nodosArchTmp[i]->archTmp);
    }

    fflush(stdout);
    bufferAgregarString(buffer,archResultado,strlen(archResultado)+1);
	int resultado = enviarBuffer(buffer,sockNodo);
	return resultado;
}


void* hilo_reduce (void* arg_thread){


	t_arg_hilo_reduce ordenToNodo;
	t_ordenReduce ordenReduce;
	ordenToNodo= *((t_arg_hilo_reduce*)arg_thread);//copio contenido de arg_thread a ordenToNodo
	int sockMarta,sockNodo, puerto_nodo,res,envioRes;
	int resOper; //resultado de la operación de reduce
	char* archResultado;
	char* codigoReduce;
	char* ip_nodo_char;
	sockMarta=ordenToNodo.sockMarta;
	codigoReduce=subirCodigoFromPathToBuffer(ordenToNodo.pathReduce);
	ordenReduce=*(ordenToNodo.ordenReduce);
	archResultado=strdup(ordenReduce.archResultado);
	puerto_nodo=ordenReduce.puerto_nodo;
	struct in_addr addr;
	addr.s_addr=ordenReduce.ip_nodo;
	ip_nodo_char= inet_ntoa(addr);
	if ((sockNodo= crearCliente(ip_nodo_char,puerto_nodo))<0){
		printf("No se pudo conectar al ip_nodo %d\n",ordenReduce.ip_nodo);
		fflush(stdout);
		void* buffer = crearBufferConProtocolo(NODO_NOT_FOUND);
		//En mapper enviaba el id_nodo, ahora no tengo ese dato mando ip_nodo
		bufferAgregarInt(buffer,ordenReduce.id_reduce);
		enviarBuffer(buffer,sockMarta);
		return -1;
	}
	//Enviamos rutina reduce a Nodo
	fflush(stdout);
	res=enviarReduceANodo(sockNodo,codigoReduce,ordenReduce.cantArchAreducir,ordenReduce.nodosArchTmp,archResultado,tamanioScript(ordenToNodo.pathReduce));
	if(res<0){
		printf("todo mal, no pude enviar reduce a Nodo: %s\n", ip_nodo_char);
		exit(-1);
	}
	//recibir resultado de la Operacion reduce desde el Nodo
	resOper=recibirResultadoFromNodo(sockNodo);
	if(resOper==OK_REDUCE){
		printf("reduce terminó OK, aviso a Marta\n");
	} else {
		printf("reduce falló, aviso a Marta de esto\n");
	}
	//re-enviar a marta resultado de la operacion, recibida de Nodo
	envioRes=responderOrdenReduceAMarta(sockMarta,ordenReduce,resOper);
	if(envioRes<0){
		printf("no pude enviar la respuesta a marta, algo pasó\n");
	}
	munmap(ordenToNodo.pathReduce,tamanioScript(ordenToNodo.pathReduce));

	close(sockNodo);

	pthread_exit(NULL);
}


void crearHiloReduce(int sockMarta, char* pathReduce) {
	pthread_t thread_reduce;
	t_ordenReduce* ordenReduce;
	t_arg_hilo_reduce* arg_thread=(t_arg_hilo_reduce*)malloc(sizeof(t_arg_hilo_reduce));
	ordenReduce=recibirOrdenReduceDeMarta(sockMarta);
	arg_thread->sockMarta=sockMarta;
	arg_thread->pathReduce=strdup(pathReduce);
	arg_thread->ordenReduce=ordenReduce;
	pthread_create (&thread_reduce, NULL, hilo_reduce,(void*)arg_thread);
	pthread_detach(thread_reduce);
	//free(ordenReduce); No puedo liberar aca sino me va a liberar antes de
	//free(arg_thread);  que termine el hilo, muy malo!!!!
	//capaz tenga que hacer un join_pthread. Averiguar!!!
	return;
}




int tamanioScript(char* pathScript){
	struct stat unStat;
	stat(pathScript,&unStat);
	return unStat.st_size;
}
