#include "servidor.h"

int crearServer(uint16_t puerto) {
	int socketServidor;
	struct sockaddr_in my_addr;
	int yes = 1;

	if ((socketServidor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket listener");
		exit(1);
	}

	setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(puerto);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(socketServidor, (struct sockaddr*) &my_addr,
			sizeof(struct sockaddr_in)) != 0) {
		perror("bind");
		exit(1);
	}

	if (listen(socketServidor, 20) != 0) {
		perror("listen");
		exit(1);
	}/*el socket se pone en escucha en el momento que haces el listen().
	 Eso lo haces una sola vez al principio de tu programa.*/

	printf("Escuchando en %d\n", ntohs(my_addr.sin_port));

	return socketServidor;
}

int crearCliente(char *ip, uint16_t port) {
	int sock;
	struct sockaddr_in dest;

	//conexion con el kernel
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr.s_addr = inet_addr(ip);

	if (connect(sock, (struct sockaddr*) &dest, sizeof(dest)) == (-1)) {
		return -1;
	}

	printf("Conectado con server en %s:%d\n", ip, port);
	fflush(stdout);
	return sock;
}

int enviar(int socket, int id, void* payload, int length) {
	printf("hola");
	int ret;
	header_t head;
	head.id = (uint8_t) id;
	head.length = (uint16_t) length;

	char* buff = malloc(length + sizeof(head));
	memcpy(buff, &head, sizeof(header_t));
	memcpy(buff + sizeof(header_t), payload, length);

	if ((ret = send(socket, buff, sizeof(header_t) + length, 0)) < 0) {
		perror("dentro de socketes: error en el send del paquete");
		exit(1);
	}
	free(buff);

	return ret;
}

int recibir(int socket, int* id, char** payload) {
	header_t head;
	int nbytes;
	*payload = NULL; //preguntar porque tengo que inicializar esto en NULL

	if ((nbytes = recv(socket, &head, sizeof(head), 0)) <= 0) {
		// error o conexion cerrada por el cliente
		if (nbytes == 0) {
			// conexion cerrada
			printf("socketes: socket %d hung up\n", socket);
		} else {
			perror("socketes: error en el recv del paquete");
			exit(1);
		}
		close(socket); // bye!
	} else {
		*id = head.id;
		*payload = malloc(head.length);
		if (recv(socket, *payload, head.length, 0) > 0) {
			printf("socketes: paquete recibido de %d [%d|%d|%s]\n", socket, head.id,head.length, *payload);
		} else {
			perror("socketes: error al leer el payload ");
			exit(1);
		}
	}
	/**
	 * el free del segemento que reservo con malloc, buff, no se va a poder liberar aca.
	 * Se va a tener que liberar afuera de esta funcion, por ejemplo:
	 *
	 * 		int id;
	 * 		char* payload;
	 * 		recibir(soket,&id,&payload);
	 *
	 * 		free(payload);
	 *
	 */
	//free(buff);
	return nbytes;
}
