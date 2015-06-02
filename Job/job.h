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

t_config_job* leer_archivo_configuracion(char* arch){
	t_config *config;
	t_config_job* info_config;
	info_config=(t_config_job*)malloc(sizeof(t_config_job));
	printf("recibo este archivo de configuracion: ");
	printf(" %s\n",arch);
	config = config_create(arch);
	info_config->IP=strdup(config_get_string_value(config,"IP"));
	printf("conectando a IP:  %s\n",info_config->IP);
	info_config->PORT= config_get_int_value(config, "PORT");
	printf("en el Puerto: %d\n",info_config->PORT);
	info_config->MAPPER=strdup(config_get_string_value(config,"MAPPER"));
	printf("Este es el mapper:  %s\n",info_config->MAPPER);
	info_config->REDUCE=config_get_string_value(config,"REDUCE");
	printf("Este es el reduce:  %s\n",info_config->REDUCE);
	info_config->COMBINER=strdup(config_get_string_value(config,"COMBINER"));
	printf("Soprta Combiner?:  %s\n",info_config->COMBINER);
	info_config->archivo_resultado=strdup(config_get_string_value(config,"RESULTADO"));
	printf("Este es el archivo resultado:  %s\n",info_config->archivo_resultado);
//	char** archivos = config_get_array_value(config,"ARCHIVOS");
	info_config->archivos=config_get_array_value(config,"ARCHIVOS");
	int i=0;
	while(info_config->archivos[i]!='\0'){
//	   info_config->archivos[i]=strdup(archivos[i]);
	   printf("archivo[%d]=  %s\n",i,info_config->archivos[i]);
	   i++;
	}

	return info_config;
}

uint32_t cantidadArchivosToProcesar(t_solicitud* sol){
	uint32_t i=0;
	while(sol->archivos[i] != 0){
		i++;
	}
	return i;
}

uint32_t tamanioBufferSerializar(t_solicitud* sol) {
	int i=0;
	size_t tam = strlen(sol->archivo_resultado);
	tam += sizeof(sol->combiner);
	while(sol->archivos[i] != 0){
		tam+= strlen(sol->archivos[i]);
		i++;
	}
	return (uint32_t)tam++;
}
#endif /* JOB_H_ */

