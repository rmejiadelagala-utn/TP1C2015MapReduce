/*
 * fsystem.c
 *
 *  Created on: 09/5/2015
 *      Author: logonzalez
 */

#include "fsystem.h"


int main (){

	system("clear");

	char* path = "/home/utnso/ConfigFS.cfg";

	config = config_create(path);

    printf("*************** SE INICIA EL PROCESO FILESYSTEM ***************\n");
//	log_info(logFS, "*************** SE INICIA EL FILESYSTEM *************");
	fd_set master;
    fd_set read_fds;

    int servFS, fdmax, i, sin_size, nbytes;
    int newSock;

    struct sockaddr_in their_addr;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	servFS = crearServer(config_get_int_value(config, "PUERTO_FS"));

	FD_SET(servFS, &master);
	fdmax = servFS;
	char* mje;

	for (;;) {
		read_fds = master;
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL ) == -1) {
			perror("select");
			exit(1);
		}

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { //
				//printf("FD_ISSET %i\n",i);
				if (i == servFS) {
					//conexion nueva
					sin_size = sizeof(struct sockaddr_in);
//					newSock = malloc(sizeof(int));

					if ((newSock = accept(servFS, (struct sockaddr*) &their_addr, (socklen_t *) &sin_size)) < 0) {
						perror("accept");
						exit(1);
					} else {
						FD_SET(newSock, &master); // agregar al conjunto maestro
						 if (newSock > fdmax) {   // actualizar el maximo
							 fdmax = newSock;
						 }
					}
				}else{

					mje=malloc(sizeof(mje));
					if ((nbytes = recv(i, mje, sizeof(mje), 0)) > 0) {

						printf("%s\n", mje);
					}

				}
			}
		}

	} //Fin de for;;

	close(servFS);

	return 0;
}
