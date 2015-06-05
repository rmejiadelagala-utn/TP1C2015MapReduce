/*
 * funcionesJob.h
 *
 *  Created on: 4/6/2015
 *      Author: rmejia
 */

#ifndef FUNCIONESJOB_H_
#define FUNCIONESJOB_H_

#include "job.h"

t_config_job* leer_archivo_configuracion(char* arch);
uint32_t cantidadArchivosToProcesar(t_solicitud* sol);
uint32_t tamanioBufferSerializar(t_solicitud* sol);
void serializer_y_send_solicitud(int sock, t_solicitud* solicitud);
void recv_y_respuesta_operaciones(void);

#endif /* FUNCIONESJOB_H_ */
