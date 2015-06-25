/*
 * procesarOrdenesDeMarta.c
 *
 *  Created on: 24/6/2015
 *      Author: rmejia
 */
#include "procesarOrdenesDeMarta.h"

int procesarOrdenesDeMarta(int sockMarta) {

    bool finOperacion=false;
    uint32_t protocolo, recibido; //de acuerdo al protocolo puede ser mapper o reduce

    while ( !finOperacion ||
            (recibido=recvall(sockMarta,&protocolo,sizeof(uint32_t)))>0) {

        switch (protocolo) {
        case ORDER_MAP:
            printf("Crear hilo mapper");
            break;
        case ORDER_REDUCE:
            printf("Crear hilo reducer");
            break;
        case FIN_OPERACION:
            printf("Se terminó de procesar todo");
            finOperacion=true;
            }
        }

        if (recibido == 0) {
            printf("Marta desconectado.\n");
        }
        if (recibido < 0) {
            printf("Error.");
        }
    return 0;
}

