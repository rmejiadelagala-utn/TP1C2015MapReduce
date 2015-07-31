#include "interaccionJobs.h"

void *interaccionJobs(void* sock_ptr) {


	log_info(marta_sync_logger, "lock conexionFS");
	int sockCliente = *(int*) sock_ptr;

	t_solicitud solicitud = deserealizarSolicitudDeJob(sockCliente);

	t_InfoJob info_job = adaptarSolicitudAInfoJob(solicitud);

	t_list* listaDeArchivos = obtenerIDyCantBloquesDeArchivosDelFS(
			info_job.pathsDeArchivos, info_job.cantArchivos);

	if (!listaDeArchivos) {
		enviarError(sockCliente);
		log_error(marta_logger, "Se produjo un error con el job");
		close(sockCliente);
		pthread_mutex_unlock(&conexionFS);
		log_info(marta_sync_logger, "unlock conexionFS");
		return NULL;
	}

	if (planificarTodosLosMaps(info_job, listaDeArchivos, listaTemporal,
			sockCliente) < 0) {
		log_error(marta_logger, "Error fatal en el map.");
		return NULL;
	}

	void mostrarListaTemporal(t_MapTemporal* unMapTemporal) {
		log_info(marta_logger,
				"bloqueOrigen: %d; idArchivoOrigen: %d; idMapTemporal: %d; idNodo: %d; path:%s \n",
				unMapTemporal->bloqueOrigen, unMapTemporal->idArchivoOrigen,
				unMapTemporal->idMapTemporal, unMapTemporal->id_nodo,
				unMapTemporal->path);
	}

	list_iterate(listaTemporal, (void*) mostrarListaTemporal);

	char* archivoResultado;
	int idNodoArchFinal;

	if (archivoResultado = planificarTodosLosReduce(info_job, listaTemporal,
			&idNodoArchFinal, sockCliente)) {
		printf("Envio el archivo %s al filesystem", archivoResultado);
		fflush(stdout);
		copiarAMDFS(archivoResultado, idNodoArchFinal, socketDeFS);

	}

	void mostrarCarga(t_CargaNodo* unaCargaNodo) {

		printf("Nodo: %i ---> Carga: %i\n", unaCargaNodo->id_nodo,
				unaCargaNodo->cantidadOperacionesEnCurso);

		fflush(stdout);
	}

	list_iterate(cargaNodos, (void*) mostrarCarga);

	close(sockCliente);

	return NULL;

}

t_solicitud deserealizarSolicitudDeJob(int sockCliente) {
	int i;

	t_solicitud *coso=malloc(sizeof(t_solicitud));
	t_solicitud solicitud=*coso;
	solicitud.cantArchivos = recibirInt(sockCliente);

	log_info(marta_logger, "La cantidad de archivos recibidos es de %d",
			solicitud.cantArchivos);


	solicitud.archivos = malloc(sizeof(int)*solicitud.cantArchivos);

	for (i = 0; i < solicitud.cantArchivos; i++) {
		solicitud.archivos[i] = recibirString(sockCliente);

	}

	solicitud.archivo_resultado = recibirString(sockCliente);
	solicitud.combiner = recibirInt(sockCliente);

	log_info(marta_logger, "Mostrare los datos del paquete deserealizados\n");
	log_info(marta_logger, "Combiner: %d", solicitud.combiner);
	log_info(marta_logger, "Cantidad de archivos: %d", solicitud.cantArchivos);
	for (i = 0; i < solicitud.cantArchivos; i++)
		log_info(marta_logger, "Archivo a trabajar: %s\n",
				solicitud.archivos[i]);
	log_info(marta_logger, "Archivo resultado: %s",
			solicitud.archivo_resultado);

	return solicitud;
}

t_InfoJob adaptarSolicitudAInfoJob(t_solicitud solicitud) {

	t_InfoJob info_job;

	info_job.combiner = solicitud.combiner;

	pthread_mutex_lock(&contadorJobs);
	log_info(marta_sync_logger, "lock contadorJobs");

	info_job.idJob = contadorDeIdJob;

	contadorDeIdJob++;

	pthread_mutex_unlock(&contadorJobs);
	log_info(marta_sync_logger, "unlock contadorJobs");

	info_job.pathDeResultado = solicitud.archivo_resultado;

	int i;

	info_job.pathsDeArchivos = malloc(sizeof(char*) * solicitud.cantArchivos);

	for (i = 0; i < solicitud.cantArchivos; i++) {

		info_job.pathsDeArchivos[i] = solicitud.archivos[i];

	}

	return info_job;
}

int copiarAMDFS(char* elArchivo, int idNodoArchFinal, int socket) {
	t_buffer* buffer = crearBufferConProtocolo(COPIATE_RESULTADO);

	bufferAgregarString(buffer, elArchivo, strlen(elArchivo) + 1);
	bufferAgregarInt(buffer, idNodoArchFinal);
	return enviarBuffer(buffer, socket);
}
