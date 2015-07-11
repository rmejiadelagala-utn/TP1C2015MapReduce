/*
 * MaRTA.c
 *
 *  Created on: 26/4/2015
 *      Author: rmejia
 */

#include "MaRTA.h"



int main(/*int argc, char *argv[]*/) {
	t_list* copiasDeBloque = list_create();
	cargaNodos = list_create();
	listaRegistrosIDIP = list_create();
	//char *c;
	int serv; //socket servidor
	pthread_t thr;
	pthread_t thr_fs;
	sem_init(&funcionesMarta,0,0);
	sem_init(&interaccionFS,0,0);
	listaTemporal = list_create();
	contadorDeIdJob=0;


	/*if (argc != 2){
		printf("Error: Se esperaba un parametro \nUso: ./Marta config.cfg\n");
		return 1;
	}
	c=argv[1];*/
	config=config_create("../marta.cfg");
	//serv = crearServer(config_get_int_value(config, "PORT")); //Jobs con select
	socketDeFS = crearCliente(config_get_string_value(config,"IP_FS"),config_get_int_value(config,"PUERTO_FS")); //FS
	printf("EL VALOR DEL SOCKET DEL FILESYSTEM ES %d",socketDeFS);
	fflush(stdout);
	//pthread_create(&thr, NULL, atencionJobs, (void*) &serv ); con select
	pthread_create(&thr_fs, NULL, interaccionMartaFS, (void*) &socketDeFS );
	crearServerMultiHilo(config_get_int_value(config,"PORT"),interaccionJobs);



	testear();

	pthread_join(thr,NULL);
	pthread_join(thr_fs,NULL);

	list_destroy_and_destroy_elements(listaRegistrosIDIP,(void*)free);
	printf("Me voy.\n");
	return 0;
}

