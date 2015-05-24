/*
 * fsystem.c
 *
 *  Created on: 09/5/2015
 *      Author: logonzalez
 */

#include "fsystem.h"
#include "consolaFS.h"

//interaccionFSNodo es la funcion que va a ejecutar cada hilo que esta en conexion con los nodos
void *interaccionFSNodo(void*);
int listaIDNodo[20];
int main (){

	system("clear");

	char* path = "/home/utnso/ConfigFS.cfg";

	config = config_create(path);

    printf("*************** SE INICIA EL PROCESO FILESYSTEM ***************\n");
//	log_info(logFS, "*************** SE INICIA EL FILESYSTEM *************");
	fd_set master;
    fd_set read_fds;

    pthread_t consola_hilo;
    if( pthread_create( &consola_hilo , NULL ,  consola , NULL) < 0)
                        {
                            perror("could not create thread");
                            return 1;
                        }

    crearServerMultiHilo(config_get_int_value(config, "PUERTO_FS"),interaccionFSNodo);

    return 0;

//Comente la otra parte por si al final nos decidimos por usar la opcion con select,
//pero para ahora probar multihilo para sincronizar la lista de nodos.

 /*   int servFS, fdmax, i, sin_size, nbytes;
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

	return 0;*/
}

void *interaccionFSNodo(void* sock_ptr)
{

	int sock_desc = *(int*)sock_ptr;
    char infoDeNodo[BUFFERSIZE];
    int read_size;

        //Receive a reply from the server
        while( read_size=recv(sock_desc , infoDeNodo , 5000 , 0) > 0)
        {
            printf("%s \n",infoDeNodo);
            memset(infoDeNodo,0,sizeof(infoDeNodo)); //Limpia el buffer de los mensajes que le manda ese nodo
        }
        if(read_size==0){
        	printf("Nodo desconectado.\n");
        }
        if(read_size<0){
        	printf("Error.");
        }
        close(sock_desc);
        return 0;
}
