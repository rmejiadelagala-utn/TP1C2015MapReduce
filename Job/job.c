/*
 * job.c
 *
 *  Created on: 25/4/2015
 *      Author: utnso
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/config.h>
#include<socketes/servidor.h>
#include"job.h"

int main(int argc,char *argv[]) {

	char *c;
	int sock;
	t_config_job* info_config;

	info_config=(t_config_job*)malloc(sizeof(t_config_job));
	if(argc!=2){
		printf("Error: Uso: ./job path_archconfig\n");
		return 1;
	}
	c=argv[1];
	info_config=leer_archivo_configuracion(c);
//	Por ahora comento lo del socket, ya que voy a usar el puntero a t_config_job	
	sock= crearCliente(info_config->IP,info_config->PORT);
	free(info_config);
	return 0;
}

