/*
 * atencionJobs.c
 *
 *  Created on: 17/5/2015
 *      Author: utnso
 */

#include"atencionJobs.h"

typedef char* t_filename;

typedef struct {
	t_filename* archivos;
	t_filename  archivo_resultado;
	uint32_t   combiner;
} t_solicitud;

void* atencionJobs(void* sock){
	int serv= *(int*)sock;
	int fdmax, i,newSock;
	int sin_size, nbytes;
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
				else {
                    // gestionar datos de un cliente
					buff=(char*)malloc(sizeof(uint32_t));
                    if ((nbytes = recv(i, buff, sizeof(uint32_t), 0)) <= 0) {
                        // error o conexión cerrada por el cliente
                        if (nbytes == 0) {
                            // conexión cerrada
                            printf("selectserver: socket %d cerró conexión\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // eliminar del conjunto maestro
                    } else {
                        // tenemos datos de algún cliente
                        // ahora lo muestro por stdout pero debo deserealizar y usar la data
                        deserealizar(buff,i);
                        FD_CLR(i, &master); // eliminar del conjunto maestro, seguira la atencion con hilo
                    }
				}
			}
		}
	}
	free(buff);
}

void deserealizar(char* buffer, int sockCliente) {
	char* payload;
	int nbytes;
	uint32_t tam_payload;
	memcpy(&tam_payload,buffer,sizeof(uint32_t));
	printf("tam_payload: %d\n", tam_payload);
	payload=(char*)malloc(tam_payload);
	if((nbytes=recv(sockCliente,payload,tam_payload,0))<=0) {
        // error o conexión cerrada por el cliente
        if (nbytes == 0) {
            // conexión cerrada
            printf("selectserver: socket %d cerró conexión\n", sockCliente);
        } else {
            perror("recv");
        }
        close(sockCliente); // bye! tal vez aca no tenga que cerrarlo sino en la funcion que me llamó.
	}
	else {
		int i=0;
		t_solicitud solicitud;
		uint32_t long_arch;
		uint32_t cant_arch; //cantidad de archivos a procesar
		int cursor;
		memcpy(&cant_arch,payload+sizeof(uint32_t),sizeof(uint32_t));
		printf("cant_arch: %s\n",cant_arch);
		cursor=2*sizeof(uint32_t);
		while (i<cant_arch){
			memcpy( &long_arch,payload + cursor	, sizeof(uint32_t));
			//solicitud.archivos[i]=malloc(long_arch+1);
			cursor+=sizeof(uint32_t);
			memcpy(solicitud.archivos[i],payload + cursor,  long_arch);
			cursor+=long_arch;
			i++;
		}
		memcpy(&long_arch ,payload + cursor,sizeof(uint32_t));
		cursor+=sizeof(uint32_t);
		memcpy(solicitud.archivo_resultado,payload + cursor, long_arch );
		cursor+=long_arch;
		memcpy(&solicitud.combiner,payload + cursor,  sizeof(uint32_t));
		printf("Mostrando archivo_resultado deserealizado\n");
		printf("%s\n",solicitud.archivo_resultado);
	}

}
