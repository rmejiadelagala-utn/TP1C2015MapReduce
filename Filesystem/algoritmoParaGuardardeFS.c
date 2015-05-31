	/*
#include"estructurasFileSystem.h"
#include"funcionesDeEstructurasFileSystem.h"
#include<commons/collections/list.h>
#include<stdlib.h>
//
bool ordenarPorMenorUso(t_nodo *data,t_nodo *dataSiguiente){
	return dataSiguiente->cantidadBloquesOcupados < data->cantidadBloquesOcupados;
}

void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos){
	t_list *nodosOrdenados = list_create();//Esto se hace para poder trabajar multihilo sino varios hilos me tocan el puntero de referencia de la lista de nodos y sonaste
	list_add_all(nodosOrdenados, nodos);//Agrega todos los elementos de la segunda lista en la primera
	list_sort(nodosOrdenados, (void*) ordenarPorMenorUso);//falta definir ordenarPorMenorUso()
	t_list *nodoInicial = nodosOrdenados;
	t_list *bloqueEnArchCursor = bloquesEnArch;
	int i;//lo pongo aca porque si lo pongo adentro del for me tira error
	int j;
	int k = 0;
	for (i = 0; i<(bloquesEnArch->elements_count) ; i++ ){
		//Creo que el primer campo del for no es necesario ya que *nodosOrdenas es algo interno de esta funcion, y *nodos No se toca
		t_list *copiasDeBloque = list_create();
		t_bloqueArch bloqueArch = list_get(bloquesEnArch, i + 1);

		for (j = 0 ;j<2 ; j++){
			t_nodo nodoActual = list_get(nodosOrdenados,k);
			nodoActual->cantidadBloquesOcupados++;
			t_bloqueEnNodo bloqueEnNodo = nuevoBloqueEnNodo(nodoActual->ip, nodoActual->puerto, nodoActual->cantidadBloquesOcupados);
			list_add(copiasDeBloque);
			k++;
			if (list_size(nodosOrdenados) == k){
				k = 0;
			}
			bloqueArch->copiaDeBloque=list_get(copiasDeBloque,1);
		}
	
	}

}


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
