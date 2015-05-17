/*
 * atencionJobs.c
 *
 *  Created on: 17/5/2015
 *      Author: utnso
 */

#include<stdio.h>
#include"atencionJobs.h"

void* atencionJobs(void* sockServ){
	int* p= (int*)sockServ;
	printf("el valor del socket es: %d\n",*p);
/*
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
	fdmax = servProg;
*/
}

