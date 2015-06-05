/*
 * job.c
 *
 *  Created on: 25/4/2015
 *      Author: utnso
 */

#include"job.h"
#include"funcionesJob.h"

int main(int argc,char *argv[]) {

	char *c;
	int sock;
	t_config_job* info_config;
	t_solicitud solicitud;

	info_config=(t_config_job*)malloc(sizeof(t_config_job));
	if(argc!=2){
		printf("Error: Uso: ./job path_archconfig\n");
		return 1;
	}
	c=argv[1];
	info_config=leer_archivo_configuracion(c);
//	Por ahora comento lo del socket, ya que voy a usar el puntero a t_config_job	
	sock= crearCliente(info_config->IP,info_config->PORT);
	printf("se creo socket cliente con nro: %d\n",sock);
//	Armar la estructura solicitud
	printf("voy a armar solicitud\n");
	solicitud.archivos=info_config->archivos;
	solicitud.archivo_resultado=info_config->archivo_resultado;
	if (!strcmp(info_config->COMBINER,"NO")){
		solicitud.combiner=0;
	} else {
		if (!strcmp(info_config->COMBINER,"SI")){
			solicitud.combiner=1;
		} else {
			printf("el valor ingresado para el combiner es incorrecto\n");
			printf("me las tomo, bye!\n");
			exit(1);
		}
	}
	serializer_y_send_solicitud(sock, &solicitud);
	printf("termine de enviar solicitud a MaRTA\n");
	printf("iniciare mapper y reducers");
	recv_y_respuesta_operaciones();
	free(info_config);
return 0;
}

