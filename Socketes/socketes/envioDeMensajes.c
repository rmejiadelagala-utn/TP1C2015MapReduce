#include<stdio.h>
#include<sys/socket.h>


int ssend(int socket, void *mensaje, size_t tamanio){
	return send(socket, mensaje, tamanio, 0);
}
int srecv(int socket, void *mensaje, size_t tamanio){
	return recv(socket, mensaje, tamanio, 0);
}
