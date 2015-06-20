/*
 * interaccionMarta.c
 *
 *  Created on: 19/6/2015
 *      Author: JuanchiRios
 */
#include "interaccionMarta.h"


int encontrarCopias(char* nombre,int directorioIndex, int numeroDeBloqueEnArchivo, t_list **copias){
	t_archivo *archivo = buscarArchPorPadreYNombre(directorioIndex, nombre);
	if(archivo == NULL){
		return -1;
	}
	else {
		t_bloqueArch *UnbloqueArch = list_get(archivo->bloquesDeArch,numeroDeBloqueEnArchivo);
		if (UnbloqueArch == NULL){
			return -2;
		}
		else{
			*copias = UnbloqueArch->copiasDeBloque;
			return 0;
		}

	}

}
