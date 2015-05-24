#ifndef SERVIDORMULTIHILO_H_
#define SERVIDORMULTIHILO_H_
#define BUFFERSIZE 1000

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

void *recibir_handler(void*);
void *enviar_handler(void*);
int crearServerMultiHilo(uint16_t, void*);
#endif /* SERVIDORMULTIHILO_H_ */
