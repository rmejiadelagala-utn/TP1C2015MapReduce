#include "interaccionJobs.h"

void *interaccionJobs(void* sock_ptr) {



	pthread_mutex_lock(&conexionFS);
	int sockCliente = *(int*) sock_ptr;

	t_solicitud* solicitud = deserealizarSolicitudDeJob(sockCliente);

<<<<<<< HEAD
	if(solicitud==NULL){
		log_error(marta_logger,"Se produjo un error con el job");
		close(sockCliente);
		pthread_mutex_unlock(&conexionFS);
		return NULL;
	}

	t_InfoJob info_job = adaptarSolicitudAInfoJob(*solicitud);
=======
	t_InfoJob info_job = adaptarSolicitudAInfoJob(solicitud);
>>>>>>> parent of d096ed3... Empece a agregar retornos en las recepciones de mensajes para detectar errores.

	t_list* listaDeArchivos = obtenerIDyCantBloquesDeArchivosDelFS(info_job.pathsDeArchivos,
			info_job.cantArchivos);

	if(!listaDeArchivos){
		enviarError(sockCliente);
		log_error(marta_logger,"Se produjo un error con el job");
		close(sockCliente);
		pthread_mutex_unlock(&conexionFS);
		return NULL;
	}

	planificarTodosLosMaps(info_job, listaDeArchivos, listaTemporal, sockCliente);

	void mostrarListaTemporal(t_MapTemporal* unMapTemporal) {
		log_info(marta_logger,"bloqueOrigen: %d; idArchivoOrigen: %d; idMapTemporal: %d; idNodo: %d; path:%s \n",
				unMapTemporal->bloqueOrigen, unMapTemporal->idArchivoOrigen,
				unMapTemporal->idMapTemporal, unMapTemporal->id_nodo, unMapTemporal->path);
	}

	list_iterate(listaTemporal, (void*) mostrarListaTemporal);

	char* archivoResultado;

	if(archivoResultado=planificarTodosLosReduce(info_job, listaTemporal, sockCliente)){
		printf("Envio el archivo %s al filesystem",archivoResultado);
		fflush(stdout);
		copiarAMDFS(archivoResultado,socketDeFS);

	}



	close(sockCliente);

	return NULL;

}



<<<<<<< HEAD
t_solicitud* deserealizarSolicitudDeJob(int sockCliente){
		int i;

		t_solicitud* solicitud=malloc(sizeof(t_solicitud));

		if((solicitud->cantArchivos = recibirInt(sockCliente))<0)return NULL;
=======
t_solicitud deserealizarSolicitudDeJob(int sockCliente){
	int i;

		t_solicitud solicitud;
		solicitud.cantArchivos = recibirInt(sockCliente);
>>>>>>> parent of d096ed3... Empece a agregar retornos en las recepciones de mensajes para detectar errores.

		log_info(marta_logger,"La cantidad de archivos recibidos es de %d",solicitud->cantArchivos);

		solicitud->archivos = malloc(solicitud->cantArchivos);

		for (i = 0; i < solicitud->cantArchivos; i++) {

<<<<<<< HEAD
			if(recibirStringEn(sockCliente,&(solicitud->archivos[i]))<0)return NULL;

		}

		if(recibirStringEn(sockCliente,&(solicitud->archivo_resultado))<0) return NULL;
		if((solicitud->combiner = recibirInt(sockCliente))<0) return NULL;
=======
			solicitud.archivos[i] = recibirString(sockCliente);

		}

		solicitud.archivo_resultado=recibirString(sockCliente);
		solicitud.combiner = recibirInt(sockCliente);
>>>>>>> parent of d096ed3... Empece a agregar retornos en las recepciones de mensajes para detectar errores.

		log_info(marta_logger,"Mostrare los datos del paquete deserealizados\n");
		log_info(marta_logger,"Combiner: %d",solicitud->combiner);
		log_info(marta_logger,"Cantidad de archivos: %d", solicitud->cantArchivos);
		for(i=0;i<solicitud->cantArchivos;i++) log_info(marta_logger,"Archivo a trabajar: %s\n", solicitud->archivos[i]);
		log_info(marta_logger,"Archivo resultado: %s", solicitud->archivo_resultado);

		return solicitud;
}

t_InfoJob adaptarSolicitudAInfoJob(t_solicitud solicitud) {

	t_InfoJob info_job;

	info_job.combiner = solicitud.combiner;

	pthread_mutex_lock(&contadorJobs);

	info_job.idJob = contadorDeIdJob;

	info_job.cantArchivos = solicitud.cantArchivos;

	contadorDeIdJob++;

	pthread_mutex_unlock(&contadorJobs);

	info_job.pathDeResultado = strdup(solicitud.archivo_resultado);

	int i;

	info_job.pathsDeArchivos = malloc(sizeof(char*) * solicitud.cantArchivos);

	for (i = 0; i < info_job.cantArchivos; i++) {

		info_job.pathsDeArchivos[i] = strdup(solicitud.archivos[i]);

	}

	return info_job;
}


int copiarAMDFS(char* elArchivo, int socket) {
	t_buffer* buffer = crearBufferConProtocolo(COPIATE_RESULTADO);

	char** nombreArchivoSeparadoPorGuionBajo;

	nombreArchivoSeparadoPorGuionBajo = string_split(elArchivo, "_");

	char* stringIdNodo = nombreArchivoSeparadoPorGuionBajo[3];

	int idNodo = atoi(stringIdNodo);

	bufferAgregarString(buffer, elArchivo, strlen(elArchivo)+1);
	bufferAgregarInt(buffer, idNodo);
	return enviarBuffer(buffer,socket);
}
