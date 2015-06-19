/*
 * interaccionMarta.h
 *
 *  Created on: 19/6/2015
 *      Author: JuanchiRios
 */

#ifndef INTERACCIONMARTA_H_
#define INTERACCIONMARTA_H_


#include <stdio.h>
#include"funcionesFileSystem.h"


/**
 * @NAME: encontrarCopias
 * @DESC: encuentra las copias de un bloque de archivo de un bloque de archivo de un archivo que es buscado por el directorio padre y el nombre del archivo
 */
int encontrarCopias(char* nombre,int directorioIndex, int numeroDeBloqueEnArchivo, t_list **copias);

#endif /* INTERACCIONMARTA_H_ */
