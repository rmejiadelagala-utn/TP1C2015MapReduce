/*
 * interfaces.c
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */
#include "interfaces.h"


#include <socketes/paquetesNodo.h>
char *DATOS;

void* conexionFS(void* arg) {

	uint32_t tam_disco;
	t_hilofs* ptr;
	ptr = (t_hilofs*) arg;
	int socket = ptr->socket;
	log_info(nodo_logger,"Nodo Conectado al Filesystem");

	tam_disco = obtener_tamanio_disco(ptr->ARCH_BIN);
	int cant_bloques = tam_disco / BLKSIZE;

	DATOS = mapeo_disco(ptr->ARCH_BIN);

	t_nodoParaFS* infoNodo = malloc(sizeof(t_nodoParaFS));
	infoNodo->IP_NODO.s_addr = inet_addr(ptr->IP_NODO);
	infoNodo->PUERTO_NODO = ptr->PUERTO_NODO;
	infoNodo->NODO_NEW = (ptr->NODO_NEW == 's' || ptr->NODO_NEW == 'S');
	infoNodo->CANT_BLOQUES = cant_bloques;
	infoNodo->ID = ptr->ID;

	presentarseAlFileSystem(infoNodo, socket);
	system("clear");

	uint32_t nrobloque;
	int protocolo;
	int recibido;
	int nroBloqueRecibido;
	int resultado;
	char* archivoFinal;
	while ((recibido = recvall(ptr->socket, &protocolo, 4)) > 0) {

		switch (protocolo) {
		case SET_BLOQUE:
			recibido = setBloqueDeFileSystem(socket, DATOS, BLKSIZE); //Si devuelve 0 es porque recibio todo
			msync(ptr->ARCH_BIN, strlen(DATOS), MS_SYNC);
			respuestaSetBloque(socket, recibido);
			fflush(stdout);
			break;

		case GET_BLOQUE:
			log_info(nodo_logger,"me pidieron un bloque");
			resultado = getBloqueParaFileSystem(socket, DATOS, BLKSIZE);

			break;

		case NODO_ESTAS:
			break;

		case NODO_DAME_ARCHIVO_A_FS:
			printf("Me piden el archivo\n");
			archivoFinal=recibirString(socket);
			if(enviarArchivoPedido(archivoFinal,socket)<0) printf("Hubo un error al enviar el archivo\n");
			printf("Envie el archivo\n");
			break;

		}

	}

	return 0;
}

void* conexionJobs(void* sockJobNodo) {

	pthread_detach(pthread_self());

	int sock_in = *(int*) sockJobNodo;
	int recibido;
	int i;
	int protocolo;
	char* script;
	char* archivoSalida;
	int nroBloque;
	int cantArchivosRecibidos;
	int numeroMapActual;
	int numeroReduceActual;
	t_list* archivosRecibidos;
	char* nombreScript;
	char* nomArchSalida;
	int cantArchReduce;
	t_list* archivosAReducir;
	uint32_t tamanioBloque;
	FILE* bloqueAMapear;
	char* archivoPedido;
	struct stat datosArch;
	t_archivoAReducir* archivoRecibido;
	FILE* fileArchivoPedido;
	char* dataArchivoPedido;
	void* buffer;
	int resultado;

	while ((recibido = recvall(sock_in, &protocolo, sizeof(int))) > 1) {
		switch (protocolo) {
		case ORDER_MAP:
			//Dejo que el nodo conteste que todo esta bien por ahora, despues hay que hacer el map aca

			log_info(nodo_logger,"Recibi una orden de map, esta todo OK.");
			script = recibirString(sock_in);
			nroBloque = recibirInt(sock_in);
			tamanioBloque = recibirInt(sock_in);
			archivoSalida = strdup("/tmp/");
			nomArchSalida = recibirString(sock_in);
			string_append(&archivoSalida, nomArchSalida);
			log_info(nodo_logger,"El archivo de salida recibido es %s", archivoSalida);

			pthread_mutex_lock(&numeroMap);
			log_info(nodo_sync_logger,"lock numeroMap");
			nombreScript = strdup("tmp/");
			string_append(&nombreScript,string_itoa(arch_config->ID));
			string_append(&nombreScript,"mapper");
			string_append(&nombreScript, string_itoa(numeroDeMap));
			string_append(&nombreScript, ".sh");
			numeroMapActual = numeroDeMap;
			crearScriptMapper(script, nombreScript);
			numeroDeMap++;
			pthread_mutex_unlock(&numeroMap);
			log_info(nodo_sync_logger,"unlock numeroMap");

			char* dataAUX = malloc(tamanioBloque);
			memcpy(dataAUX, DATOS + (nroBloque * BLKSIZE), tamanioBloque);
			log_info(nodo_logger,"antes de ejecutar %s", nombreScript);
			resultado=ejecutarMapper(nombreScript, archivoSalida, dataAUX);
			log_info(nodo_logger,"ejecute %s", nombreScript);
			free(script);
			free(dataAUX);
			free(nombreScript);
			free(nomArchSalida);
			free(archivoSalida);
			enviarProtocolo(RES_MAP,sock_in);
			if(resultado>0){
				log_info(nodo_logger,"El map nº %d fue exitoso.", numeroMapActual);
				enviarProtocolo(OK_MAP,sock_in);
			}
			else{
				log_info(nodo_logger,"El map nº %d tuvo un error.", numeroMapActual);
				enviarProtocolo(NOTOK_MAP,sock_in);
			}
			pthread_exit(NULL);
			break;

		case ENVIO_ARCHIVOS_NODO_NODO:
			archivoPedido=recibirString(sock_in);
			int fileArchivoPedido = open(archivoPedido, O_RDONLY);
			//FILE* fileArchivoPedido = fopen(archivoPedido,"a+");
			stat(archivoPedido,&datosArch);



			if ((dataArchivoPedido = (char *) mmap(0, datosArch.st_size, PROT_READ, MAP_PRIVATE, fileArchivoPedido, 0)) == MAP_FAILED) {
					log_error(nodo_logger,"Error al iniciar el mapeo de disco. '%s' ", strerror(errno));
					close(fileArchivoPedido);
					exit(1);
				}

			buffer = crearBuffer();
			fflush(stdout);
			bufferAgregarString(buffer,dataArchivoPedido,datosArch.st_size);
			enviarBuffer(buffer,sock_in);
			break;

		case ORDER_REDUCE:
			log_info(nodo_logger,"Me llego una orden reduce");
			archivosAReducir = list_create();
			script = recibirString(sock_in);
			cantArchReduce = recibirInt(sock_in);
			archivoSalida = strdup("/tmp/");
			log_info(nodo_logger,"Voy a recibir una lista de archivos de tamaño %d",cantArchReduce);
			for (i = 0; i < cantArchReduce; i++) {
				list_add(archivosAReducir,recibirArchReduce(sock_in) );
			}
			log_info(nodo_logger,"Recibi la lista de archivos.\n");
			nomArchSalida = recibirString(sock_in);
			log_info(nodo_logger,"El archivo de salida recibido es %s\n", nomArchSalida);
			fflush(stdout);

			pthread_mutex_lock(&numeroReduce);
			log_info(nodo_sync_logger,"lock numeroReduce");
			nombreScript = strdup("tmp/");
			string_append(&nombreScript,string_itoa(arch_config->ID));
			string_append(&nombreScript,"reduce");
			string_append(&nombreScript, string_itoa(numeroDeReduce));
			string_append(&nombreScript, ".sh");
			string_append(&archivoSalida, nomArchSalida);
			numeroReduceActual = numeroDeReduce;
			crearScriptReduce(script, nombreScript);
			numeroDeReduce++;
			pthread_mutex_unlock(&numeroReduce);
			log_info(nodo_sync_logger,"unlock numeroReduce");

			resultado = ejecutarReduce(nombreScript, archivoSalida, archivosAReducir,numeroReduceActual);

			enviarProtocolo(RES_REDUCE,sock_in);
			if(resultado>0)	enviarProtocolo(OK_REDUCE,sock_in);
			else enviarProtocolo(NOTOK_REDUCE,sock_in);
			break;

		}
	}

	return 0;
}

t_archivoAReducir* nuevoArchReduce(int ip, int puerto, char* nombre) {
	t_archivoAReducir* unArch = malloc(sizeof(t_archivoAReducir));
	unArch->ipNodo = ip;
	unArch->puertoNodo = puerto;
	unArch->nombreArch = strdup(nombre);
	return unArch;
}

t_archivoAReducir* recibirArchReduce(int socket) {
	t_archivoAReducir* unArch = malloc(sizeof(t_archivoAReducir));
	printf("Recibo puerto nodo\n");
	unArch->puertoNodo = recibirInt(socket);
	printf("Recibo ip nodo\n");
	unArch->ipNodo = recibirInt(socket);

	unArch->nombreArch = recibirString(socket);
	printf("Recibo nombre archivo:%s\n",unArch->nombreArch);
	return unArch;
}

int enviarArchivoPedido(char* archivo, int sock_in) {
	char* archivoPedido=strdup("/tmp/");
	string_append(&archivoPedido,archivo);
	t_buffer* buffer;
	char* dataArchivoPedido;

	struct stat datosArch;

	printf("Voy a abrir el archivo\n");
	fflush(stdout);

	int fileArchivoPedido = open(archivoPedido, O_RDONLY);
	//FILE* fileArchivoPedido = fopen(archivoPedido,"a+");
	stat(archivoPedido, &datosArch);

	printf("Mapeo el archivo\n");
	fflush(stdout);
	if ((dataArchivoPedido = (char *) mmap(0, datosArch.st_size, PROT_READ, MAP_PRIVATE,
			fileArchivoPedido, 0)) == MAP_FAILED) {
		;
		log_error(nodo_logger, "Error al iniciar el mapeo de disco. '%s' ", strerror(errno));
		close(fileArchivoPedido);
		exit(1);
	}
	printf("Creo el buffer\n");
	fflush(stdout);

	buffer = crearBufferConProtocolo(NODO_DAME_ARCHIVO_A_FS);
	fflush(stdout);
	bufferAgregarString(buffer, dataArchivoPedido, datosArch.st_size);
	return enviarBuffer(buffer, sock_in);
}
