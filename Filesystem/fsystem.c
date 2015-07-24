/*
 * fsystem.c
 *
 *  Created on: 09/5/2015
 *      Author: logonzalez
 */

#include "fsystem.h"

void *interaccionFSNodo(void*);

int main() {

	t_registro_id_ipPuerto* registroVacio = malloc(sizeof(t_registro_id_ipPuerto));
	sem_init(&consola_sem, 0, 0);
	sem_init(&escuchar_sem, 0, 0);
	sem_init(&resultadoJob_sem, 0, 0);
	sem_init(&escuchar_sem2, 0, 0);
	pthread_mutex_init(&listaDeNodos, NULL);
	pthread_mutex_init(&listaDeRegistros, NULL);
	mdfs_logger = log_create("filesystem.log", "MDFS", 1, log_level_from_string("TRACE"));
	mdfs_sync_logger = log_create("mdfs_sync.log", "MDFS", 0, log_level_from_string("TRACE"));
	cargarPersistencia();
	// Register signal and signal handler
	signal(SIGINT, signal_callback_handler);

	system("clear");

	char* path = "ConfigFS.cfg";

	config = config_create(path);

	printf("*************** SE INICIA EL PROCESO FILESYSTEM ***************\n");
	//fd_set master;
	//fd_set read_fds;

	listaNodo = list_create();

	pthread_t consola_hilo;
	if (pthread_create(&consola_hilo, NULL, consola, (void*) listaNodo) < 0) {
		perror("could not create thread");
		return 1;
	}

	crearServerMultiHilo(config_get_int_value(config, "PUERTO_FS"), interaccionFSNodo);

//close(servFS);
	list_destroy_and_destroy_elements(listaArchivos, (void*) liberarArchivo);
	list_destroy_and_destroy_elements(listaNodos, (void*) liberarNodo);
	list_destroy_and_destroy_elements(listaDirectorios, (void*) liberarDirectorio);
	list_destroy_and_destroy_elements(listaRegistrosIDIP, (void*) free);
	//free(copias);
	return 0;
}

void levantarArchivoAMemoriaYDistribuirANodos(char* pathLocal, char* nombreArchivo, int padre) {
	int local_fd;
	int envioNodoCorrectamente = 1;
	int cantidadBolquesEnviados = 0;
	struct stat file_stat;
	char *data;
	t_list* listaDeBloques = list_create();
	t_archivo *archivoNuevo;

	pthread_mutex_lock(&listaDeNodos);
	log_info(mdfs_sync_logger,"lock listaDeNodos");
	int estaActivo(t_nodo* unNodo) {
		return unNodo->activo;
	}
	t_list* nodosActivos = list_filter(listaNodos, estaActivo);
	int esMenorA3(t_list* nodosActivos) {
		return list_size(nodosActivos) < 3;
	}
	if (detectarError(nodosActivos, esMenorA3, "No hay suficientesNodos activos\n")) {
		pthread_mutex_unlock(&listaDeNodos);
		log_info(mdfs_sync_logger,"unlock listaDeNodos");
		return;
	}
	pthread_mutex_unlock(&listaDeNodos);
	log_info(mdfs_sync_logger,"unlock listaDeNodos");
	if (pathLocal != NULL) {
		if ((local_fd = open(pathLocal, O_RDONLY)) != -1) {

			fstat(local_fd, &file_stat);
			data = mmap((caddr_t) 0, file_stat.st_size, PROT_READ, MAP_SHARED, local_fd, 0);
			if (data == (caddr_t) (-1)) {
				perror("mmap");
				exit(1);

			}
			if (hayLugarEnLosNodos(data) == 0) {
				//Acá se pone a mandar bloques de arch a nodos y demás
				log_info(mdfs_logger, "Comenzando a enviar los bloques a los nodos");
				envioNodoCorrectamente = mandarBloquesANodos(data, &cantidadBolquesEnviados, &listaDeBloques);

				if (close(local_fd) == -1)
					perror("close");

				if (envioNodoCorrectamente != -1) {	//si se mando correctamente
					//agregar a la lista de archivos global el archivo nuevo,
					//con su correspondiente listaDeBloques, nombre, padre, tamanio,
					//y estado. (Esto es a las estructuras lógicas)

					archivoNuevo = nuevoArchivo(nombreArchivo, padre, string_length(data), listaDeBloques, 1);

					list_add(listaArchivos, archivoNuevo);
					log_info(mdfs_logger, "El archivo %s fue copiado correctamente.", nombreArchivo);
					//	printf("El archivo %s fue copiado correctamente.\n", nombreArchivo);
				} else {
					log_error(mdfs_logger, "error al enviar a nodos.");
					//		printf("error al enviar a nodos\n");
				}

				munmap(data, file_stat.st_size);
			} else {
				munmap(data, file_stat.st_size);
				log_warning(mdfs_logger,
						"No hay nodos disponibles con espacio para copiar el archivo deseado, se recomienda contratar el servicio PREMIUM FULL HD con nodos ilimitados");
			}								//else de hayLugarEnLosNodos

		} else {
			log_error(mdfs_logger, "Error al abrir el archivo.");
			//	printf("Error al abrir el archivo\n");
		}
	} else {
		log_warning(mdfs_logger, "upload: falta un operando.");
		//	printf("upload: falta un operando\n");
	}
}

void *interaccionFSNodo(void* sock_ptr) {

	int tamanioBloqueRecibido;

	int tamanioPosta;
	int esMarta = 0;
	int socket = *(int*) sock_ptr;
	int protocolo;
	int recibido;
	int id;
	int resultado;
	t_nodoParaFS* infoNodo;
	t_nodo* nodo;
	t_nodo* nodoViejo;
	int respuestaSetBloque;
	void* buffer;
	t_bloqueDeArchPedido* infoBloquePedido;
	t_registro_id_ipPuerto* unRegistro = malloc(sizeof(t_registro_id_ipPuerto));
	int condicionDeConexionNodo;
	t_list* copias;
	t_list* listaArchivosPedidos;
	char* archivoFinal;
	int nodoArchivoFinal;
	while ((recibido = recvall(socket, &protocolo, 4)) > 0) {
		switch (protocolo) {
		case CONEXION_NODO_A_FS:
			infoNodo = conocerAlNodo(socket);
			//ipPuerto = strdup(strcat(strcat(inet_ntoa(infoNodo->IP_NODO),":"),string_itoa(infoNodo->PUERTO_NODO)));
			id = infoNodo->ID;
			condicionDeConexionNodo = infoNodo->NODO_NEW + 2 * ((nodo = buscarNodoPorId(id, listaNodos)) != NULL);
			switch (condicionDeConexionNodo) {

			case NODO_NUEVO_Y_NO_TENGO_SU_ID:
				nodo = nuevoNodo(id, (infoNodo->CANT_BLOQUES) * BLOCK_SIZE);

				pthread_mutex_lock(&listaDeNodos);
				log_info(mdfs_sync_logger,"lock listaDeNodos");
				list_add(listaNodos, nodo);
				pthread_mutex_unlock(&listaDeNodos);
				log_info(mdfs_sync_logger,"unlock listaDeNodos");

				unRegistro->id = id;
				unRegistro->ip = infoNodo->IP_NODO;
				unRegistro->puerto = infoNodo->PUERTO_NODO;

				pthread_mutex_lock(&listaDeRegistros);
				log_info(mdfs_sync_logger,"lock listaDeRegistros");
				list_add(listaRegistrosIDIP, unRegistro);
				//	actualizarRegistro(unRegistro, infoNodo->IP_NODO, infoNodo->PUERTO_NODO);
				pthread_mutex_unlock(&listaDeRegistros);
				log_info(mdfs_sync_logger,"unlock listaDeRegistros");

				break;

			case NODO_NUEVO_Y_TENGO_SU_ID:
				free(unRegistro);
				nodo = nuevoNodo(id, (infoNodo->CANT_BLOQUES) * BLOCK_SIZE);
				nodoViejo = buscarNodoPorId(id, listaNodos);

				pthread_mutex_lock(&listaDeRegistros);
				log_info(mdfs_sync_logger,"lock listaDeRegistros");
				unRegistro = buscarRegistroPorId(id);
				//	actualizarRegistro(unRegistro, infoNodo->IP_NODO, infoNodo->PUERTO_NODO);
				unRegistro->ip = infoNodo->IP_NODO;
				unRegistro->puerto = infoNodo->PUERTO_NODO;
				pthread_mutex_unlock(&listaDeRegistros);
				log_info(mdfs_sync_logger,"unlock listaDeRegistros");

				eliminarNodoYReferencias(nodoViejo, listaNodos, listaArchivos);

				pthread_mutex_lock(&listaDeNodos);
				log_info(mdfs_sync_logger,"lock listaDeNodos");
				list_add(listaNodos, nodo);
				pthread_mutex_unlock(&listaDeNodos);
				log_info(mdfs_sync_logger,"unlock listaDeNodos");

				break;
			case NODO_VIEJO_Y_NO_TENGO_SU_ID:
				free(unRegistro);
				log_error(mdfs_logger, "Error, no se conocía a este nodo");
				//	printf("Error, no se conocía a este nodo.\n");
				//	fflush(stdout);
				break;
			case NODO_VIEJO_Y_TENGO_SU_ID:
				free(unRegistro);

				pthread_mutex_lock(&listaDeRegistros);
				log_info(mdfs_sync_logger,"lock listaDeRegistros");
				unRegistro = buscarRegistroPorId(id);
				//if (!verificarRegistro(unRegistro, infoNodo->IP_NODO, infoNodo->PUERTO_NODO)) {	//Cambió su IP o Puerto
				//	actualizarRegistro(unRegistro, infoNodo->IP_NODO, infoNodo->PUERTO_NODO);
				unRegistro->ip = infoNodo->IP_NODO;
				unRegistro->puerto = infoNodo->PUERTO_NODO;
				//	}
				pthread_mutex_unlock(&listaDeRegistros);
				log_info(mdfs_sync_logger,"unlock listaDeRegistros");

				break;
			}

			if (nodo)
				nodo->socket = socket;
			//free(infoNodo);
			break;
		case RTA_SET_BLOQUE:
			if (recvall(socket, &respuestaSetBloque, 4) <= 0)
				log_error(mdfs_logger, "Hubo un problema al escribir el archivo.");
			//		printf("Hubo un problema al escribir el archivo.\n");
			else if (respuestaSetBloque <= 0)
				log_error(mdfs_logger, "Hubo un problema al escribir el archivo.");
			//	printf("Hubo un problema al escribir el archivo.\n");
			break;
		case GET_BLOQUE:
			recibido = recvall(socket, &protocolo, 4);
			switch (protocolo) {
			case COPIAR_ARCHIVO_A_FS_LOCAL:
				tamanioBloqueRecibido = recibirBloqueDeNodo(socket, (void*) &buffer);

				write(fileno(archivoReconstruido), buffer, tamanioBloqueRecibido);
				free(buffer);
				sem_post(&semaforo);
				log_info(mdfs_sync_logger,"post semaforo");
				break;
			case VER_BLOQUE_NODO:
				sem_post(&consola_sem);
				log_info(mdfs_sync_logger,"post consola_sem");
				//La consola se encarga del resto
				sem_wait(&escuchar_sem);
				log_info(mdfs_sync_logger,"wait escuchar_sem");
				break;
			case COPIAR_BLOQUE_NODO:
				sem_post(&consola_sem);
				log_info(mdfs_sync_logger,"post consola_sem");
				//La consola se encarga del resto
				sem_wait(&escuchar_sem);
				log_info(mdfs_sync_logger,"wait escuchar_sem");
				break;
			}
			break;
		case CONEXION_MARTA_A_FS:
			esMarta = 1;
			log_info(mdfs_logger, "Hola Marta");
//			printf("Hola Marta!\n");
			protocolo = MARTA_ACTUALIZA_EL_REGISTRO;
			fflush(stdout);
			socketDeMarta = socket;
			void actualizarAMarta(t_registro_id_ipPuerto* unRegistro) {
				actualizarIdIpPuertoEnMarta(socketDeMarta, unRegistro);
			}

			pthread_mutex_lock(&listaDeRegistros);
			log_info(mdfs_sync_logger,"lock listaDeRegistros");
			t_list* registrosActivos = list_filter(listaRegistrosIDIP, (void*) nodoEstaActivo);
			list_iterate(registrosActivos, (void*) actualizarAMarta);
			list_destroy(registrosActivos);
			pthread_mutex_unlock(&listaDeRegistros);
			log_info(mdfs_sync_logger,"unlock listaDeRegistros");

			//TODO esta bien
			//Qué bien, me alegro entonces!!!
			break;
		case COPIATE_RESULTADO:
			printf("Me piden que guarde el ultimo archivo\n");
			fflush(stdout);
			copiarResultadoAFS(socket);
			break;
		case NODO_DAME_ARCHIVO_A_FS:
			sem_post(&resultadoJob_sem);
			log_info(mdfs_sync_logger,"post resultadoJob_sem");
			sem_wait(&escuchar_sem);
			log_info(mdfs_sync_logger,"wait escuchar_sem");
			break;
		case ENVIO_BLOQUEARCH_A_MARTA:
			infoBloquePedido = recibirPedidoDeBloqueArch(socket);
			//	copias = list_create();
			log_info(mdfs_logger, "nombreArch: %s\n", infoBloquePedido->nombreArch);
			resultado = encontrarCopias(infoBloquePedido->nombreArch, infoBloquePedido->padre, infoBloquePedido->numeroDeBloqueArch,
					&copias);
			if (resultado == -1) {
				log_warning(mdfs_logger, "No existe un Arch con ese Padre Y Nombre");
				//		("No existe un Arch con ese Padre Y Nombre \n");
			} else if (resultado == -2) {
				log_warning(mdfs_logger, "No existe ese numero De Bloque En Archivo");
				//		printf("No existe ese numero De Bloque En Archivo \n");
			} else {
				log_info(mdfs_logger, "Voy a entrar a enviar copia al socket %d\n", socket);
				//	printf("Voy a entrar a enviar copia al socket %d\n",socket);
				int suNodoEstaActivo(t_bloqueEnNodo* copiaDeBloque) {
					t_nodo* unNodo = buscarNodoPorId(copiaDeBloque->id, listaNodos);
					return unNodo->activo;
				}
				t_list * copiasConNodoActivo = list_filter(copias, (void*) suNodoEstaActivo);
				enviarCopiasAMarta(socket, copiasConNodoActivo);
				//	list_destroy_and_destroy_elements(copias,(void*)liberarBloqueEnNodo);
				//	free(copias);
				//	list_destroy_and_destroy_elements(copiasConNodoActivo,(void*)liberarBloqueEnNodo);
				free(copiasConNodoActivo);

			}

			break;
		case DAME_LISTA_DE_ARCHIVOS_FS:
			sendall(socket, &protocolo, sizeof(int)); //Le respondo con el mismo protocolo
			listaArchivosPedidos = recibirPedidoListaArchivos(socket); //IRA UN MALLOC ANTES DE ESTO?
			void mandarCantidadDeBloquesDeArchivo(char* unArchivo) {
				enviarCantBloquesDeArch(unArchivo, socket);
			}
			list_iterate(listaArchivosPedidos, (void*) mandarCantidadDeBloquesDeArchivo);
			list_destroy_and_destroy_elements(listaArchivosPedidos, free);
			break;
		}
	}

	if (recibido == 0) {
		if (nodo) {
			nodo->socket = -1;
			nodo->activo = 0;
			log_info(mdfs_logger, "Nodo desconectado.");
			//		printf("Nodo desconectado.\n");
			martaSeCayoUnNodo(socketDeMarta, id);
		} else if (esMarta) {
			log_info(mdfs_logger, "Marta no esta, Marta se fue");
			//	printf("Marta desconectada.\n");
		}
	}
	if (recibido < 0) {
		//	printf("Error.");
		log_error(mdfs_logger, "Error al comunicarse con el FileSytem");
	}
//free(unRegistro);

	close(socket);
	return 0;
}

