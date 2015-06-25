/*
 * job.h
 *
 *  Created on: 15/5/2015
 *      Author: rmejia
 */

#ifndef JOB_H_
#define JOB_H_
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/config.h>
#include<socketes/servidor.h>
#include<socketes/envioDeMensajes.h>
#include<stdbool.h>

#define TRUE 1
#define FALSE 0

//esta estructura también va a estar replicada en MaRTA/atencion_Job.h con lo cual habría que ponerla en socketes/estructuras.h
typedef struct {
	int length;
	char* data;
}t_steam;



typedef char* t_filename;

typedef struct {
	t_filename* archivos;
	t_filename  archivo_resultado;
	uint32_t   combiner;
} t_solicitud;

typedef struct {
	char* IP;
	uint16_t PORT;
	char* MAPPER;
	char* REDUCE;
	char* COMBINER;
	char** archivos;
	char*  archivo_resultado;
} t_config_job;


#endif /* JOB_H_ */

