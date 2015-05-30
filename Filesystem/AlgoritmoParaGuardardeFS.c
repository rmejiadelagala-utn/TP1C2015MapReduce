/*/
#include"estructurasFileSystem.h"
//
void distribuirBloquesEnNodos(t_bloqueArch *bloquesEnArch, t_nodo *nodos){
	t_nodo *nodosOrdenados = ordenarPorMenorUso(nodos);//falta definir
	t_nodo *nodoInicial = nodosOrdenados;
	for ( ; bloquesEnArch->sgte != NULL ; bloquesEnArch = bloquesEnArch->9sgte ){
		//Creo que el primer campo del for no es necesario ya que *nodosOrdenas es algo interno de esta funcion, y *nodos No se toca
		for ( ;*bloquesEnArch.*copiaDelBloque.sgte != NULL ; *bloquesEnArch.*copiaDelBloque = *bloquesEnArch.*copiaDelBloque.sgte ){
			*bloquesEnArch.*copiaDelBloque.ip = *nodosOrdenados.ip;
			*bloquesEnArch.*copiaDelBloque.puerto = *nodosOrdenados.puerto;
			*bloquesEnArch.*copiaDelBloque.numeroDeBloqueEnNodo = enviarBloque(*nodosOrdenados,*bloquesEnArch.info);//Tengo que enviar los datos no la estructura!!!
			//EnviarBloque(INFO) devuelve donde lo guardo el nodo, también podría hacerse que el FS le diga donde guardarlo
			*nodosOrdenados = *nodosOrdenados.sgte;
			if (nodosOrdenados == NULL){
				nodosOrdenados = nodoInicial;
			}
		}
	
	}

}

void enviarBloques(t_nodo *nodosOrdenados,t_info info){
	//ACA SE ENVIA LA INFORMACION DEL BLOQUE
}
t_nodo *ordenarPorMenorUso(t_nodo *nodos){
	t_nodo nodoInicial = *nodos;
	t_nodo nodoInicialOrdenado = new ^ptrn;
	//ETC ETC ETC
	return *nodoInicialOrdenado;
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
S
