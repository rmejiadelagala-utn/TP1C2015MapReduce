/*
 * MaRTA.c
 *
 *  Created on: 26/4/2015
 *      Author: rmejia
 */
#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<socketes/servidor.h>
#include <pthread.h>
#include "atencionJobs.h"

t_config *config;

int main(int argc, char *argv[]) {


	char *c;
	int serv; //socket servidor
	pthread_t thr;

	if (argc != 2){
		printf("Error: Se esperaba un parametro \nUso: ./Marta config.cfg\n");
		return 1;
	}
	c=argv[1];
	config=config_create(c);
	serv = crearServer(config_get_int_value(config, "PORT"));
	pthread_create(&thr, NULL, atencionJobs, (void*) &serv );
	pthread_join(thr,NULL);

	printf("me voy\n");
	return 0;
}

