#ifndef CONSOLAFS_H_
#define CONSOLAFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <string.h>
#include <socketes/envioDeMensajes.h>
#include "fsystem.h"
#include "funcionesFileSystem.h"

int consola();
void leerComando(char *command, int maximoLargo);
void freeSeparaciones(char ** separaciones);
void formatearMDFS();
void eliminarArchivo(char *archivo);
void renombrarArchivo(char *archivo, char *nuevoNombre);
void moverArchivo(char *archivo, char* padre);
void crearDirectorio(char *nomDirectorio);
void eliminarDirectorio(char *directorio);
void renombrarDirectorio(char *directorio, char* nuevoNombre);
void moverDirectorio(char *directorio, char* padre);
void copiarAMDFS(char *archivo, char *archivoACopiar);
void copiarAFS(char *archivo);
void solicitarMD5(char *archivo);
void verBloque(char *bloque, char* numeroDeBloque);
void eliminarBloque(char *archivo, char* numeroBloque);
void copiarBloque(char *archivo, char* numeroBloque, char* idNodo);
void levantarNodo(char *nodo);
void eliminarNodo(char *nodo);
void help();
void cd(char *nombreDirectorio);
void ls();

//Auxiliares
void validarDirectorioYEjecutar(char* unArch,void (*funcion)(void*));
void validarArchivoYEjecutar(char* unArch,void (*funcion)(void*));
int comprobarParametros(int cantParametros, char** parametros);
t_list *directoriosVisiblesDesdeActual(void);
t_list *archivosVisiblesDesdeActual(void);
int string_to_int(char* string);
int verBloqueDeArchivo(t_archivo *unArchivo, int nroBloque);
int funcionCopiarBloque(t_archivo *unArchivo, int nroBloque, int id);
int descargarArchivo(t_archivo *unArchivo);
int detectarError(void* cosaAChequear,int (*condicion)(void*),char* mensaje);
int esNull(void* algo);
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
