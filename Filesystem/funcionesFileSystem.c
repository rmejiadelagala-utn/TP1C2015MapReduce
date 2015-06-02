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
#include <string.h>

typedef struct {
	char nombre[255];
	bool (*mismoNombreArch)(void*);
} t_closureCondicion;

static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente);
static int buscarPosicionEnListaDadoUnArchivo(t_list *lista, t_archivo *archivo);
//

void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos) {
	//variables auxiliares para la funcion
	t_bloqueEnNodo bloqueEnNodo;
	t_nodo *nodoActual;
	t_bloqueEnNodo *bloqueEnNodoP;
	t_bloqueArch *bloqueArch;
	int i; //lo pongo aca porque si lo pongo adentro del for me tira error
	int j;
	int k;
	t_list *copiasDeBloque;
	t_list *copiasDeBloqueAUX;
	t_list *nodosOrdenados = list_create(); //Esto se hace para poder trabajar multihilo sino varios hilos me tocan el puntero de referencia de la lista de nodos y sonaste
	//fin de variables auxs

	list_add_all(nodosOrdenados, nodos);//Agrega todos los elementos de la segunda lista en la primera
	list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);//falta definir ordenarPorMenorUso()

	for (i = 0; i < list_size(bloquesEnArch); i++) {
		//Creo que el primer campo del for no es necesario ya que *nodosOrdenas es algo interno de esta funcion, y *nodos No se toca
		copiasDeBloque = list_create();

		for (j = 0; j < 2; j++) {
			nodoActual = list_get(nodosOrdenados, k);
			nodoActual->cantidadBloquesOcupados++;
			bloqueEnNodo = nuevoBloqueEnNodo(nodoActual->ip, nodoActual->puerto,
					nodoActual->cantidadBloquesOcupados);/*
					 ***FIXME***
					 Ahora que escribo esto para ustedes me doy cuenta que acá va a haber un bug porque al eliminar un archivo
					 esta variable va a disminuir, y puede que los bloquesEnNodo que elimine no sean los últimos sino los primeros,
					 entonces estaría pisando bloquesEnNodo valido.Posible solución sería meter una variable más en algún lugar
					 bloqueEnNodoP = &bloqueEnNodo;//Esto porque nuevoBloqueEnNodo retorna una estructura y para el list_add me pide un puntero al dato*/
			bloqueEnNodoP = &bloqueEnNodo;
			list_add(copiasDeBloque, bloqueEnNodoP);
			k++;
			if (list_size(nodosOrdenados) == k) {
				k = 0;
			}
			bloqueArch = (list_get(bloquesEnArch, i + 1));
			//cargarCopiasABloqueDeArch(bloqueArch,copiasDeBloque);
			copiasDeBloqueAUX = &bloqueArch->copiasDeBloque;
			list_add_all(copiasDeBloqueAUX, copiasDeBloque);
		}

	}
}
void activarNodoReconectado(t_nodo *nodoABuscar, t_list *listaNodos) {
	int i;
	t_nodo *nodoActual;
	for (i = 0; i < list_size(listaNodos); i++) {
		nodoActual = list_get(listaNodos, i + 1);
		if ((nodoABuscar->ip == nodoActual->ip)
				&& (nodoABuscar->puerto == nodoActual->puerto)) {
			nodoActual->activo = 1;
			i = list_size(listaNodos);	//corto el ciclo como un campeon

		}
	}

}
bool esNodoNuevo(t_nodo *nodoABuscar, t_list *listaNodos) {
	bool mismosNodos(t_nodo *nodoDeLista) {
		return (nodoABuscar->ip == nodoDeLista->ip)
				&& (nodoABuscar->puerto == nodoDeLista->puerto);
	}

	return list_any_satisfy(listaNodos, (bool*) mismosNodos);
}
//elimina el nodo y los bloques de copia que cada archivo contaba en ese nodo
void eliminarNodoDeLista(t_nodo *nodoAEliminar, t_list *listaNodos) {
	bool mismosNodos(t_nodo *nodoDeLista) {
		return (nodoAEliminar->ip == nodoDeLista->ip)
				&& (nodoAEliminar->puerto == nodoDeLista->puerto);
	}

	list_remove_by_condition(listaNodos, (bool*) mismosNodos);
}
void eliminarReferencias(t_nodo *nodoAEliminar, t_list *archivos) {

	bool copiaEstaEnNodo(t_bloqueEnNodo *copiaDeBloque) {
		return (nodoAEliminar->ip == copiaDeBloque->ip)
				&& (nodoAEliminar->puerto == copiaDeBloque->puerto);
	}

	void _list_elements2(t_list *copiasDeBloques) {
		list_remove_by_condition(copiasDeBloques, (bool*) copiaEstaEnNodo);
	}

	void _list_elements1(t_archivo *unArchivo) {
		t_list *bloquesDeArchivo = &(unArchivo->bloquesDeArch);
		list_iterate(bloquesDeArchivo, (void*) _list_elements2);
	}

	list_iterate(archivos, (void*) _list_elements1);
}
void eliminarNodoYRerencias(t_nodo *nodoAEliminar, t_list *listaNodos,
		t_list *archivos) {
	eliminarNodoDeLista(nodoAEliminar, listaNodos);
	eliminarReferencias(nodoAEliminar, archivos);

}

void* eliminarArchivoPorNombre(char nombreBuscado[255], t_list *listaArchivos) {
	int archivoConNombreBuscado(t_archivo *unArchivo) {
		return strcmp(nombreBuscado, unArchivo->nombre);
	}

	return list_remove_by_condition(listaArchivos,
			(void*) archivoConNombreBuscado);
}

void formatear(t_list *listaNodos, t_list *listaArchivos,
		t_list *listaDirectorios) {
	list_clean(listaArchivos);
	list_clean(listaNodos);
	list_clean(listaDirectorios);
}

t_list* renombrarArchivoPorNombre(char nombreBuscado[255],
		char nuevoNombre[255], t_list *listaArchivos) {
	t_archivo *archivoAModificar;
	int posDondeReemplazar = -1;

	int archivoConNombreBuscado(t_archivo *unArchivo) {
		return strcmp(nombreBuscado, unArchivo->nombre);
	}

	//Busco cual es el archivo a modificar, dado el nombre
	archivoAModificar = list_find(listaArchivos,
			(void*) archivoConNombreBuscado);

	if (archivoAModificar == NULL) {
		//tirar error de que no se encuentra ese archivo
	}

	//busco la posicion en la lista, donde se encuantra el archivo con ese nombre
	posDondeReemplazar = buscarPosicionEnListaDadoUnArchivo(listaArchivos,
			archivoAModificar);

	//genero un archivo nuevo, que va a ser el anterior con un nuevo nombre
	t_archivo *archivoModificado;
	archivoModificado = archivoAModificar;
	strcpy(archivoModificado->nombre, nuevoNombre);

	//reemplazo en la posicion donde se hallaba el original en la lista,
	//el nuevo archivo con solo el nombre cambiado
	list_replace(listaArchivos, posDondeReemplazar, archivoModificado);

	//retorno la lista de archivos del FS ya modificada
	return listaArchivos;
}

//Esto de recibir al padre no esta del todo bien. Debería poder mover archivo
//no por número de padre del directorio al que se quiere mover, sino por la
//ruta a la que quiere moverse. Esta ruta sería una cadena de nombre de
//directorio separados por /. Eso me daría el padre del directorio al que quiero
//moverme.
t_list* moverArchivoPorNombreYPadre(char nombreBuscado[255],
		t_list *listaArchivos, int padre) {

	int archivoConNombreBuscado(t_archivo *unArchivo) {
		return strcmp(nombreBuscado, unArchivo->nombre);
	}

	int posEnLista = -1;
	t_archivo *archivoAModificarPadre;

	archivoAModificarPadre = list_find(listaArchivos,
			(void*) archivoConNombreBuscado);

	posEnLista = buscarPosicionEnListaDadoUnArchivo(listaArchivos,
			archivoAModificarPadre);

	archivoAModificarPadre->padre = padre;

	list_replace(listaArchivos, posEnLista, archivoAModificarPadre);

	return listaArchivos;
}

t_list* crearDirectorioDadoPadreYNom(char nombre[255], int padre,
		t_list *listaDirectorio) {

	//Verifico que no se pase de los 1024 directorios permitidos
	if (list_size(listaDirectorio) > 1024) {
		//tira error de que supera máxima cant de directorios permitidos
	} else {

		//genero el nuevo directorio con sus datos, y le pongo el indice del ultimo + 1
		t_directorio *nuevoDirectorio = malloc(sizeof(t_directorio));

		//Esto del índice que asigna tiene un ERROR. No debe asignar eso, sino
		//El índice que le sigue al último valor de índice.
		//Esto pasa porque usamos el índice en la estructura y no el de la lista
		//Y cuando elimino un directorio, los demás índices no cambian
		nuevoDirectorio->index = listaDirectorio->elements_count + 1;
		strcpy(nuevoDirectorio->nombre, nombre);
		nuevoDirectorio->padre = padre;

		list_add(listaDirectorio, nuevoDirectorio);

	}
	return listaDirectorio;
}

//Este se podría hacer dado nombre, pero preferí así porque el índice es único.
//Si no, se reutiliza esta función para hacerla por nombre, ruta o lo que sea.
t_list* eliminarDirectorioDadoElIndice(int indice, t_list *listaDirectorio) {

	int directorioConIndiceBuscado(t_directorio *directorio) {
		return indice == directorio->index;
	}

	if (list_remove_by_condition(listaDirectorio,
			(void*) directorioConIndiceBuscado) == NULL) {
		//tira error de que no lo encontró en la lista.
	}

	return listaDirectorio;
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
/*
 t_fileSystem formatear(t_fileSystem fileSystem) {
 //Poner vacía la lista de directorios del FS completa
 list_clean(fileSystem.directorio);
 //Poner vacía de la lista de nodos todos los nodos asginados al FS
 list_clean(fileSystem.nodo);
 //Poner vacía de la lista de archivos todos los archivos que tenga el FS
 list_clean(fileSystem.archivo);

 return fileSystem;
 }
 */
/*
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
 */
/********* PRIVATE FUNCTIONS **************/

static bool ordenarPorMenorUso(t_nodo *data, t_nodo *dataSiguiente) {
	return dataSiguiente->cantidadBloquesOcupados
			< data->cantidadBloquesOcupados;
}

static int buscarPosicionEnListaDadoUnArchivo(t_list *listaArchivos,
		t_archivo *archivo) {
	int posDondeReemplazar = -1;
	//Busco la posición en la lista de este archivo que debo modificar
	int i = 0;
	for (i = 0; i < listaArchivos->elements_count; i++) {
		if (list_get(listaArchivos, i) == archivo) {
			posDondeReemplazar = i;
		}
	}
	return posDondeReemplazar;
}
