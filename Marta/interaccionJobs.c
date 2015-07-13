#include "interaccionJobs.h"

void *interaccionJobs(void* sock_ptr) {

	pthread_mutex_lock(&conexionFS);

	int sockCliente = *(int*) sock_ptr;

	t_solicitud solicitud = deserealizarSolicitudDeJob(sockCliente);

	t_InfoJob info_job = adaptarSolicitudAInfoJob(solicitud);

	t_list* listaDeArchivos = obtenerIDyCantBloquesDeArchivosDelFS(info_job.pathsDeArchivos,
			info_job.cantArchivos);

	if(!listaDeArchivos){
		enviarError(sockCliente);
		printf("Se produjo un error con el job\n");
		return NULL;
	}

	planificarTodosLosMaps(info_job, listaDeArchivos, listaTemporal, sockCliente);

	void mostrarListaTemporal(t_MapTemporal* unMapTemporal) {
		printf("bloqueOrigen: %d; idArchivoOrigen: %d; idMapTemporal: %d; idNodo: %d; path:%s \n",
				unMapTemporal->bloqueOrigen, unMapTemporal->idArchivoOrigen,
				unMapTemporal->idMapTemporal, unMapTemporal->id_nodo, unMapTemporal->path);
	}

	list_iterate(listaTemporal, (void*) mostrarListaTemporal);

	close(sockCliente);

	return NULL;

}



t_solicitud deserealizarSolicitudDeJob(int sockCliente){
	int i;

		int tamanioDeDatosRecibidos = recibirInt(sockCliente); //Este dato ahora es inutil

		t_solicitud solicitud;
		solicitud.cantArchivos = recibirInt(sockCliente);


		for (i = 0; i < solicitud.cantArchivos; i++) {

			recibirStringEn(sockCliente,&solicitud.archivos[i]);

		}


		recibirStringEn(sockCliente,&solicitud.archivo_resultado);
		solicitud.combiner = recibirInt(sockCliente);

		printf("Mostrare los datos del paquete deserealizados\n");
		printf("Combiner: %d\n",solicitud.combiner);
		printf("Cantidad de archivos: %d\n", solicitud.cantArchivos);
		for(i=0;i<solicitud.cantArchivos;i++) printf("Archivo a trabajar: %s\n", solicitud.archivos[i]);
		printf("Archivo resultado: %s\n", solicitud.archivo_resultado);

		return solicitud;
}

t_InfoJob adaptarSolicitudAInfoJob(t_solicitud solicitud) {

	t_InfoJob info_job;

	info_job.combiner = solicitud.combiner;

	pthread_mutex_lock(&contadorJobs);

	info_job.idJob = contadorDeIdJob;

	contadorDeIdJob++;

	pthread_mutex_unlock(&contadorJobs);

	info_job.pathDeResultado = solicitud.archivo_resultado;

	int i;

	info_job.pathsDeArchivos = malloc(sizeof(char*) * solicitud.cantArchivos);

	for (i = 0; i < solicitud.cantArchivos; i++) {

		info_job.pathsDeArchivos[i] = solicitud.archivos[i];

	}

	return info_job;
}
