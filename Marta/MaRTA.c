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


int main(int argc, char *argv[]) {

	t_config *config;
	char *c;
	int serv; //socket servidor

	if (argc != 2){
		printf("Error: Se esperaba un parametro \nUso: ./Marta config.cfg\n");
		return 1;
	}
	c=argv[1];
	config=config_create(c);
	serv = crearServer(config_get_int_value(config, "PORT"));
	return 0;
}

