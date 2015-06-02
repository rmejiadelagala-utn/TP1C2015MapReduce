/*
 * job.c
 *
 *  Created on: 25/4/2015
 *      Author: utnso
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/config.h>
#include<socketes/servidor.h>
#include"job.h"

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
	printf("%s\n",solicitud.archivos[1]);
	solicitud.archivo_resultado=info_config->archivo_resultado;
	solicitud.combiner=strcmp(info_config->COMBINER,"NO")?0:1;
	printf("%d\n",solicitud.combiner);
	serializer_y_send_solicitud(sock, &solicitud);

	free(info_config);
	getchar();
return 0;
}


void serializer_y_send_solicitud(int sock, t_solicitud* solicitud) {
//  Armar Paquete a enviar

/*<-------------------------------- p a y l o a d ----------------------------------------->
 *
 ********************************************************************************************
 *              |______________________p_a_q_u_e_t_e________________________________________|
 *  tam_paquete | cant_arch | tam_  |arch1 | tam_  |arch2 |...| tam_arch |arch_  | conbiner |
 *              | _a_proc   | arch1 |      | arch2 |      |   |  result  |result |          |
 ********************************************************************************************
 *
 |<--uint32_t-->|<-----------------------tam_paquete -------------------------------------->|
 *
 */

	printf("voy a armar paquete\n");
	char* payload;
	uint32_t long_arch, cursor, i, nbytes;
	uint32_t tam_paquete;

	tam_paquete = tamanioBufferSerializar(solicitud);
	payload=(char*)malloc(tam_paquete + sizeof(uint32_t));

	memcpy(payload,&tam_paquete,sizeof(uint32_t));
	uint32_t cant_arch=cantidadArchivosToProcesar(solicitud);
	memcpy(payload+sizeof(uint32_t),&cant_arch,sizeof(uint32_t));
	i=0;
	cursor=2*sizeof(uint32_t);
	while (solicitud->archivos[i] != 0){
			long_arch=strlen(solicitud->archivos[i]);
			memcpy(payload + cursor, &long_arch	, sizeof(uint32_t));
			cursor+=sizeof(uint32_t);
			memcpy(payload + cursor , solicitud->archivos[i], long_arch);
			cursor+=long_arch;
			i++;
		}
	long_arch=strlen(solicitud->archivo_resultado);
	memcpy(payload + cursor,&long_arch ,sizeof(uint32_t));
	cursor+=sizeof(uint32_t);
	memcpy(payload + cursor, solicitud->archivo_resultado,strlen(solicitud->archivo_resultado) );
	cursor+=strlen(solicitud->archivo_resultado);
	memcpy(payload + cursor, &solicitud->combiner, sizeof(uint32_t));

	if ((nbytes=send(sock, payload,tam_paquete+sizeof(uint32_t) , 0)) < 0) {
					perror("error en el send del paquete a MaRTA");
					exit(1);
	}
}

