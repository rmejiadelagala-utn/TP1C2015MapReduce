#include "interaccionJobs.h"

typedef char* t_filename;

typedef struct {
	t_filename* archivos;
	t_filename archivo_resultado;
	uint32_t combiner;
} t_solicitud;

void *interaccionJobs(void* sock_ptr) {
	int sockCliente = *(int*) sock_ptr;
	char* buffer = (char*) malloc(sizeof(uint32_t));
	recvall(sockCliente, buffer, sizeof(uint32_t));
	char* payload;
	int nbytes;
	uint32_t tam_payload;
	memcpy(&tam_payload, buffer, sizeof(uint32_t));
	printf("tam_payload: %d\n", tam_payload);
	//payload tiene los datos sin el tamaño total
	payload = (char*) malloc(tam_payload);
	if ((nbytes = recvall(sockCliente, payload, tam_payload)) <= 0) {
		// error o conexión cerrada por el cliente
		if (nbytes == 0) {
			// conexión cerrada
			printf("selectserver: socket %d cerró conexión\n", sockCliente);
		} else {
			perror("recv");
		}
		close(sockCliente); // bye! tal vez aca no tenga que cerrarlo sino en la funcion que me llamó.
	} else {
		int i = 0;
		t_solicitud solicitud;
		size_t long_arch;
		uint32_t cant_arch; //cantidad de archivos a procesar
		int cursor;
		char* bufftmp;
		//el primer campo de payload es cant_arch a procesar
		printf("Mostrare los datos del paquete deserealizados\n");
		memcpy(&cant_arch, payload, sizeof(uint32_t));
		printf("cant_arch: %d\n", cant_arch);
		cursor = 1 * sizeof(uint32_t);
		solicitud.archivos = malloc(sizeof(char*) * cant_arch);
		while (i < cant_arch) {
			memcpy(&long_arch, (payload + cursor), sizeof(uint32_t));
			cursor += sizeof(uint32_t);
			bufftmp = malloc(long_arch);
			bufftmp[long_arch] = '\0';
			memcpy(bufftmp, payload + cursor, long_arch);
			solicitud.archivos[i] = strdup(bufftmp);
			printf("solicitud_archivo: %s\n", solicitud.archivos[i]);
			cursor += long_arch;
			i++;
		}
		memcpy(&long_arch, (payload + cursor), sizeof(uint32_t));
		cursor += sizeof(uint32_t);
		solicitud.archivo_resultado = malloc(long_arch + 1);
		memset(solicitud.archivo_resultado, '\0', long_arch + 1);
		memcpy(solicitud.archivo_resultado, (payload + cursor), long_arch);
		cursor += long_arch;
		memcpy(&solicitud.combiner, payload + cursor, sizeof(uint32_t));
		printf("%s\n", solicitud.archivo_resultado);
		//Pido al FileSystem los bloques en los que esta el archivo
		t_InfoJob info_job;
		info_job.combiner = solicitud.combiner;
		info_job.idJob = contadorDeIdJob;
		contadorDeIdJob++;
		info_job.pathDeResultado = strdup(solicitud.archivo_resultado);
		int j;
		//debo reservar memoria para punteros a char
		info_job.pathsDeArchivos = malloc(sizeof(char*) * cant_arch);
		for (j = 0; j < cant_arch; j++) {
			info_job.pathsDeArchivos[j] = (char*) malloc(
					strlen(solicitud.archivos[j]) + 1);
			info_job.pathsDeArchivos[j] = strdup(solicitud.archivos[j]);
		}

		t_list* listaDeArchivos = obtenerIDyCantBloquesDeArchivosDelFS(
				solicitud.archivos, cant_arch);
		void mostrarCantidadDeBloquesPorArchivo(t_InfoArchivo* unaInfoArchivo) {
			printf("Recibi un archivo con %d bloques.\n",
					unaInfoArchivo->cantidadDeBloques);
			fflush(stdout);
		}
		printf("La lista de archivos es de tamaño %d\n",
				list_size(listaDeArchivos));
		list_iterate(listaDeArchivos,
				(void*) mostrarCantidadDeBloquesPorArchivo);
		planificarTodosLosMaps(info_job, listaDeArchivos, listaTemporal,
				sockCliente);

		void mostrarListaTemporal(t_MapTemporal* unMapTemporal) {
			printf(
					"bloqueOrigen: %d; idArchivoOrigen: %d; idMapTemporal: %d; idNodo: %d; path:%s \n",
					unMapTemporal->bloqueOrigen, unMapTemporal->idArchivoOrigen,
					unMapTemporal->idMapTemporal, unMapTemporal->id_nodo,
					unMapTemporal->path);
		}
		list_iterate(listaTemporal, (void*) mostrarListaTemporal);

		free(bufftmp);
	}

}
