/*
 * funcionesFileSystem.c
 *
 *  Created on: 31/5/2015
 *      Author: JuanRios
 */
#include<stdbool.h>
#include"estructurasFileSystem.h"
#include<commons/collections/list.h>
#include <stdlib.h>

static bool ordenarPorMenorUso(t_nodo *data,t_nodo *dataSiguiente);

//
//variables auxiliares para la funcion
	t_bloqueEnNodo bloqueEnNodo;
	t_nodo *nodoActual;
	t_bloqueEnNodo *bloqueEnNodoP;
	t_bloqueArch *bloqueArch;
	int i;//lo pongo aca porque si lo pongo adentro del for me tira error
	int j;
	int k;
	t_list *copiasDeBloque;
	t_list *copiasDeBloqueAUX;


void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos){
	t_list *nodosOrdenados = list_create();//Esto se hace para poder trabajar multihilo sino varios hilos me tocan el puntero de referencia de la lista de nodos y sonaste
	list_add_all(nodosOrdenados, nodos);//Agrega todos los elementos de la segunda lista en la primera
	list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);//falta definir ordenarPorMenorUso()

	for (i = 0; i<list_size(bloquesEnArch) ; i++ ){
		//Creo que el primer campo del for no es necesario ya que *nodosOrdenas es algo interno de esta funcion, y *nodos No se toca
		copiasDeBloque = list_create();

		for (j = 0 ;j<2 ; j++){
			nodoActual = list_get(nodosOrdenados,k);
			nodoActual->cantidadBloquesOcupados++;
			bloqueEnNodo = nuevoBloqueEnNodo(nodoActual->ip, nodoActual->puerto, nodoActual->cantidadBloquesOcupados);/*
			***FIXME***
			Ahora que escribo esto para ustedes me doy cuenta que acá va a haber un bug porque al eliminar un archivo
			esta variable va a disminuir, y puede que los bloquesEnNodo que elimine no sean los últimos sino los primeros, 
			entonces estaría pisando bloquesEnNodo valido.Posible solución sería meter una variable más en algún lugar
			bloqueEnNodoP = &bloqueEnNodo;//Esto porque nuevoBloqueEnNodo retorna una estructura y para el list_add me pide un puntero al dato*/
			list_add(copiasDeBloque,bloqueEnNodoP);
			k++;
			if (list_size(nodosOrdenados) == k){
				k = 0;
			}
			bloqueArch = (list_get(bloquesEnArch, i + 1));
			//cargarCopiasABloqueDeArch(bloqueArch,copiasDeBloque);
			copiasDeBloqueAUX = &bloqueArch->copiasDeBloque;
			list_add_all(copiasDeBloqueAUX,copiasDeBloque);
		}

	}
}

/*
void enviarBloques(t_nodo *nodosOrdenados,t_info info){
	//ACA SE ENVIA LA INFORMACION DEL BLOQUE
}
//interaccion
//se conecta el unNodo
if (esNuevoNodo(unNodo)){
	agregarALista(*nodo,unNodo);
}
else {
	activarNodo(unNodo);
}

//llega un archivo nuevo
//Se carga el archivo en las estructuras y se distribuye el archivo en bloques

//Una vez terminado se distribuyen los bloques en los nodos
*/

t_fileSystem formatear(t_fileSystem fileSystem) {
	//Poner vacía la lista de directorios del FS completa
	list_clean(fileSystem.directorio);
	//Poner vacía de la lista de nodos todos los nodos asginados al FS
	list_clean(fileSystem.nodo);
	//Poner vacía de la lista de archivos todos los archivos que tenga el FS
	list_clean(fileSystem.archivo);

	return fileSystem;
}

void eliminarArchivo(char *nomArchivo, t_fileSystem fileSystem) {
	int posicionEnListaArchivos = buscarArchivoPorNombre(nomArchivo,
			fileSystem.archivo);

	if (posicionEnListaArchivos == -1) {
		//No encontró ese archivo, debería devolver un mensaje de error
	} else {
		list_remove(fileSystem.archivo, posicionEnListaArchivos);
	}
}

void renombrarArchivo(char *nomArchivo, char *nuevoNombreArchivo,
		t_fileSystem fileSystem) {
	//Debo buscar el archivo de nombre nomArchivo y reemplazarlo por el
	//nuevo nombre
	int posicionEnListaArchivos = buscarArchivoPorNombre(nomArchivo,
			fileSystem.archivo);

	//Creo un nuevo archivo, igual al anterior pero con el nombre nuevo
	t_archivo nuevoArchivo = fileSystem.archivo[posicionEnListaArchivos];
	nuevoArchivo.nombre = nuevoNombreArchivo;

	if (posicionEnListaArchivos == -1) {
		//No encontró ese archivo, debería devolver un mensaje de error
	} else {
		list_replace(fileSystem.archivo, posicionEnListaArchivos, nuevoArchivo);
	}
}

/********* PRIVATE FUNCTIONS **************/

static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente) {
	return dataSiguiente->cantidadBloquesOcupados
			< data->cantidadBloquesOcupados;
}

static int buscarArchivoPorNombre(char *nomArchivo, t_list* listaArchivo) {
	//En cosa de no encontrar, devuelve -1
	int tamanioListaArchivos = listaArchivo->elements_count;
	int posicionEnListaArchivos = -1;

	//Busca en la lista de archivos, aquel que tenga nombre *archivo y devuelve su posición
	for (int i = 0; i < tamanioListaArchivos; i++) {
		if (strcmp(listaArchivo[i].head->data->nombre, nomArchivo) == 0) {
			posicionEnListaArchivos = i;
		}
	}
	return posicionEnListaArchivos;
}

