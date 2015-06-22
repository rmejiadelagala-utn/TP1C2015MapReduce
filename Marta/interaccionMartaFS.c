#include "interaccionMartaFS.h"
void* interaccionMartaFS(void* sock){
	int socket= *(int*)sock;
	int recibido;
	int protocolo;
	t_registro_id_ipPuerto* registro = malloc(sizeof(t_registro_id_ipPuerto));

	while ((recibido=recvall(socket,&protocolo,4))>0) {

		switch (protocolo) {

		case MARTA_ACTUALIZA_EL_REGISTRO:
			registro = recibirRegistroNodo(socket);
			buscarRegistroPorId(registro->id);

		}

	}
}
