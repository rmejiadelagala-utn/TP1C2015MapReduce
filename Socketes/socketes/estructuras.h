#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdint.h>

/**
 * HEADER de los paquetes.
 * Los paquetes que enviamos siempre son de tamano variable
 * De modo que necesitamos dos recv para leerlos.
 * Primero leemos el header, ahi nos dice el tamanio que tenemos
 * que leer en el segundo recv para completar la lectura del total
 * del paquete.
 * El header debe ser siempre debe tener esta estructura y tamano.
 */
typedef struct {
	uint8_t id;
	uint16_t length;
} header_t;

/****
 *
 * Aca definiremos la estructura del paquete a enviar
 *
 ****/

/****
 *
 * Si necesitamos alguna otra estructura tambien, iremos viendo
 *
 ***/

#endif /* ESTRUCTURAS_H_ */
