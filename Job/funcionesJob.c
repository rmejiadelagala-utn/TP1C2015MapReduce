/*
 * funcionesJob.c
 *
 *  Created on: 4/6/2015
 *      Author: rmejia
 */

# include "job.h"

t_config_job* leer_archivo_configuracion(char* arch){
	t_config *config;
	t_config_job* info_config;
	info_config=(t_config_job*)malloc(sizeof(t_config_job));
	printf("recibo este archivo de configuracion: ");
	printf(" %s\n",arch);
	config = config_create(arch);
	info_config->IP=strdup(config_get_string_value(config,"IP"));
	printf("conectando a IP:  %s\n",info_config->IP);
	info_config->PORT= config_get_int_value(config, "PORT");
	printf("en el Puerto: %d\n",info_config->PORT);
	info_config->MAPPER=strdup(config_get_string_value(config,"MAPPER"));
	printf("Este es el mapper:  %s\n",info_config->MAPPER);
	info_config->REDUCE=config_get_string_value(config,"REDUCE");
	printf("Este es el reduce:  %s\n",info_config->REDUCE);
	info_config->COMBINER=strdup(config_get_string_value(config,"COMBINER"));
	printf("Soprta Combiner?:  %s\n",info_config->COMBINER);
	info_config->archivo_resultado=strdup(config_get_string_value(config,"RESULTADO"));
	printf("Este es el archivo resultado:  %s\n",info_config->archivo_resultado);
//	char** archivos = config_get_array_value(config,"ARCHIVOS");
	info_config->archivos=config_get_array_value(config,"ARCHIVOS");
	int i=0;
	printf("estos son los archivos que recibo para procesar:\n");
	while(info_config->archivos[i]!='\0'){
//	   info_config->archivos[i]=strdup(archivos[i]);
	   printf("archivo[%d]=  %s\n",i,info_config->archivos[i]);
	   i++;
	}

	return info_config;
}

uint32_t cantidadArchivosToProcesar(t_solicitud* sol){
	uint32_t i=0;
	while(sol->archivos[i] != 0){
		i++;
	}
	return i;
}

/********************************************************************************************
 *              |______________________p_a_q_u_e_t_e________________________________________|
 *  tam_paquete | cant_arch | tam_  | nomb_ | tam_  |nomb_ |...| tam_arch |arch_  | conbiner |
 *              | _a_proc   | arch1 | arch1 | arch2 |arch2 |   |  result  |result |          |
 ********************************************************************************************/

uint32_t tamanioBufferSerializar(t_solicitud* sol) {
	int i=0;
	size_t tam;
	tam=sizeof(uint32_t);//reserva 4bytes para el campo tam_arch_result
	tam+= strlen(sol->archivo_resultado);//reserva tam_arch_result bytes
	tam += sizeof(sol->combiner); //reserva 4 bytes para el campo combiner
	while(sol->archivos[i] != 0){
		tam+=sizeof(uint32_t); //reserva 4 bytes para el campo tam_arch1,2,3 etc
		tam+= strlen(sol->archivos[i]);// reserva tam_arch bytes para el campo nombre_arch
		i++;
	}
	return (uint32_t)tam;
}

void recv_y_respuesta_operaciones(void) {
	getchar();
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
	free(payload);
}

t_solicitud cargarEstructuraSolicitud(t_config_job* info_config){
	t_solicitud solicitud;
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
	return solicitud;
}

t_rutinas cargarEstructuraRutinas(t_config_job* config_job){
	t_rutinas rutinas;
	rutinas.MAPPER=strdup(config_job->MAPPER);
	rutinas.REDUCE=strdup(config_job->REDUCE);
	return rutinas;
}
