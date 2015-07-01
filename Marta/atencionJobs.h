/*
 * atencionJobs.h
 *
 *  Created on: 17/5/2015
 *      Author: utnso
 */

#ifndef ATENCIONJOBS_H_
#define ATENCIONJOBS_H_

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <sys/socket.h>

#include<socketes/servidor.h>
#include<socketes/estructuras.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "MaRTA.h"


void* atencionJobs(void*);


#endif /* ATENCIONJOBS_H_ */
