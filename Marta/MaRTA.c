/*
 * MaRTA.c
 *
 *  Created on: 26/4/2015
 *      Author: rmejia
 */

#include "MaRTA.h"

t_config *config;

int main(int argc, char *argv[]) {

	listaRegistrosIDIP = list_create();
	char *c;
	int serv; //socket servidor
	int cliente; //socket cliente del FS
	pthread_t thr;
	pthread_t thr_fs;

	if (argc != 2){
		printf("Error: Se esperaba un parametro \nUso: ./Marta config.cfg\n");
		return 1;
	}
	c=argv[1];
	config=config_create(c);
	serv = crearServer(config_get_int_value(config, "PORT")); //Jobs
	cliente = crearCliente(config_get_string_value(config,"IP_FS"),config_get_int_value(config,"PUERTO_FS")); //FS
	pthread_create(&thr, NULL, atencionJobs, (void*) &serv );
	pthread_create(&thr_fs, NULL, interaccionMartaFS, (void*) &cliente );
	pthread_join(thr,NULL);
	pthread_join(thr_fs,NULL);

	list_destroy_and_destroy_elements(listaRegistrosIDIP,(void*)free);
	printf("Me voy.\n");
	return 0;
}

