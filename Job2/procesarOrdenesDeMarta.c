/*
 * procesarOrdenesDeMarta.c
 *
 *  Created on: 24/6/2015
 *      Author: rmejia
 */

#include "job.h"

int procesarOrdenesDeMarta(int sockMarta, t_rutinas* rutinas) {

    bool finOperacion=false;
    uint32_t protocolo;
	int recibido; //de acuerdo al protocolo puede ser mapper o reduce

    while ( !finOperacion &&
            (recibido=recvall(sockMarta,&protocolo,sizeof(uint32_t)))>0) {

        switch (protocolo) {
        case ORDER_MAP:
            printf("Crear hilo mapper\n");
            fflush(stdout);
            crearHiloMapper(sockMarta, rutinas->MAPPER );
            break;
        case ORDER_REDUCE:
            printf("Crear hilo reducer\n");
            crearHiloReduce(sockMarta,rutinas->REDUCE);
            break;
        case FIN_OPERACION:
            printf("Se terminó de procesar todos los archivos solicitados\n");
            finOperacion=true;
        	break;
        case -1:
        	printf("Hubo un error");
        	exit(1);
        	break;
        }
    }

        if (recibido == 0) {
            printf("Marta desconectado.\n");
            return 2;
        }
        if (recibido < 0) {
            printf("Error.");
            return 1;
        }
    return 0;
}

