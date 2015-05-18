/*
 * atencionJobs.c
 *
 *  Created on: 17/5/2015
 *      Author: utnso
 */


#include"atencionJobs.h"

void* atencionJobs(void* sock){
	int serv= *(int*)sock;
	int fdmax, i,newSock;
	int sin_size;
	printf("estoy en el hilo\n");
	printf("el valor del socket es: %d\n",serv);
	struct sockaddr_in their_addr;
	char* payload;
	char* buff;
	//char* prog_socket_str;

	fd_set master;
	fd_set read_fds;
	header_t head;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(serv, &master);
	fdmax = serv;
	for (;;) {
		read_fds = master; // copialo
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL ) == -1) {
			perror("select");
			exit(1);
		}
		for(i=0; i<=fdmax; i++){
			if (FD_ISSET(i, &read_fds)) { // tenemos datos!!
				//printf("FD_ISSET %i\n",i);
				if (i == serv) {
					//conexion nueva
					sin_size = sizeof(struct sockaddr_in);
					if ((newSock = accept(serv, (struct sockaddr*) &their_addr,
							(socklen_t *) &sin_size)) < 0) {
						perror("accept");
						exit(1);
					}else {
						FD_SET(newSock, &master); // aniadir al conjunto maestro
						if (newSock > fdmax) {    // actualizar el maximo
							fdmax = newSock;
						}
						printf("nuevo job en %s on socket %d\n", inet_ntoa(their_addr.sin_addr),
									 newSock);
					}
				}
			}
		}
	}

}

