/*
 * job.c
 *
 *  Created on: 25/4/2015
 *      Author: utnso
 */

#include"job.h"

int main(int argc,char *argv[]) {

	char *c;
	int sockMarta,result;
	t_config_job* info_config;
	t_solicitud* solicitud;
	t_rutinas* rutinas;

	solicitud =(t_solicitud*)malloc(sizeof(t_solicitud));
	rutinas =(t_rutinas*)malloc(sizeof(t_rutinas));
	info_config=(t_config_job*)malloc(sizeof(t_config_job));

	if(argc!=2){
		printf("Error: Uso: ./job path_archconfig\n");
		return 1;
	}
	c=argv[1];
	info_config=leer_archivo_configuracion(c);
//	Por ahora comento lo del socket, ya que voy a usar el puntero a t_config_job
	sockMarta= crearCliente(info_config->IP,info_config->PORT);
	printf("se creo socket cliente con nro: %d\n",sockMarta);
//	Armar la estructura solicitud para enviar a MaRTA
	*solicitud=cargarEstructuraSolicitud(info_config);
//	Enviar solicitud de procesamiento a MaRTA
	serializer_y_send_solicitud(sockMarta, solicitud);
	printf("termine de enviar solicitud a MaRTA\n");
	printf("iniciare mapper y reducers\n");
//	cargar estructura rutinas para procesar ordenes de MaRTA
	*rutinas=cargarEstructuraRutinas(info_config);
	fflush(stdout);
/*	recepciono las ordenes de MaRTA y mando los hilos al Nodo
 	hasta que se me indique que finalizo todas las operaciones */
	result=procesarOrdenesDeMarta(sockMarta,rutinas);
	free(info_config);
	free(solicitud);
	free(rutinas);
return result;
}

