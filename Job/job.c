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

int main(int argc,char *argv[]) {

	t_config *config;
	char *c;
	if(argc!=2){
		printf("Error: Uso: ./job path_archconfig\n");
		return 1;
	}
	c=argv[1];
	printf("recibo este archivo de configuracion: ");
	printf(" %s\n",c);
	config = config_create(c);
	printf("conectando a IP:  %s\n",config_get_string_value(config,"IP"));
	printf("en el Puerto: %d\n",config_get_int_value(config, "PORT"));
	return 0;
}

