#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "estructuras.h"

int crearServer(uint16_t);
int crearCliente(char*,uint16_t);
int enviar(int,int,void*,int);
int recibir(int, int*, char**);

#endif /* SERVIDOR_H_ */
