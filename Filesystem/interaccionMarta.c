/*
 * interaccionMarta.c
 *
 *  Created on: 19/6/2015
 *      Author: JuanchiRios
 */
#include "interaccionMarta.h"
#include <stdio.h>

int encontrarCopias(char* nombre,int directorioIndex, int numeroDeBloqueEnArchivo, t_list **copias){
	t_archivo *archivo = buscarArchPorPadreYNombre(nombre, directorioIndex);
	if(archivo == NULL){
		return -1;
	}
	else {
		*copias = list_get(archivo->bloquesDeArch,numeroDeBloqueEnArchivo);
		return 0;
	}

}
