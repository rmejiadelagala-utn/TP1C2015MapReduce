#ifndef CONSOLAFS_H_
#define CONSOLAFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <string.h>

int consola();
void leerComando(char *command, int maximoLargo);
void freeSeparaciones(char ** separaciones);
void formatearMDFS();
void eliminarArchivo(char *archivo);
void renombrarArchivo(char *archivo);
void moverArchivo(char *archivo);
void crearDirectorio(char *directorio);
void eliminarDirectorio(char *directorio);
void renombrarDirectorio(char *directorio);
void moverDirectorio(char *directorio);
void copiarAMDFS(char *archivo);
void copiarAFS(char *archivo, t_list*);
void solicitarMD5(char *archivo);
void verBloque(char *bloque);
void eliminarBloque(char *bloque);
void copiarBloque(char* bloque);
void levantarNodo(char *nodo);
void eliminarNodo(char *nodo);
void help();
void cd(char *nombreDirectorio);
void ls();

//Auxiliares
t_list *directoriosVisiblesDesdeActual(void);
t_list *archivosVisiblesDesdeActual(void);
//

enum entrada {
	FORMATEAR,
	ELIMINAR_ARCHIVO,
	RENOMBRAR_ARCHIVO,
	MOVER_ARCHIVO,
	CREAR_DIRECTORIO,
	ELIMINAR_DIRECTORIO,
	RENOMBRAR_DIRECTORIO,
	MOVER_DIRECTORIO,
	COPIAR_A_MDFS,
	COPIAR_A_FS,
	SOLICITAR_MD5,
	VER_BLOQUE,
	ELIMINAR_BLOQUE,
	COPIAR_BLOQUE,
	LEVANTAR_NODO,
	ELIMINAR_NODO,
	HELP,
	SALIR,
	CD,
	LS,
	COMANDO_INVALIDO,
	FALTA_PARAMETRO
};

#endif /* CONSOLAFS_H_ */
