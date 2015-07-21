/*
 * interaccionJobs.h
 *
 *  Created on: 11/7/2015
 *      Author: utnso
 */

#ifndef INTERACCIONJOBS_H_
#define INTERACCIONJOBS_H_

#include "MaRTA.h"

typedef char* t_filename;

typedef struct {
	t_filename* archivos;
	t_filename archivo_resultado;
	uint32_t combiner;
	int cantArchivos;
} t_solicitud;

void *interaccionJobs(void* sock_ptr);

t_solicitud* deserealizarSolicitudDeJob(int sockCliente);
t_InfoJob adaptarSolicitudAInfoJob(t_solicitud solicitud);

#endif /* INTERACCIONJOBS_H_ */
