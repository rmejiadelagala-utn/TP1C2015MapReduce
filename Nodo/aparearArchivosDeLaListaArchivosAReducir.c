/*
 * aparearArchivosDeLaListaArchivosAReducir.c
 *
 *  Created on: 14/7/2015
 *      Author: rmejia
 */
#include "nodo.h"
#include "interfaces.h"
#include "estructurasNodo.h"
#include "funcionesNodo.h"
#include <stdbool.h>


void destruir_RegistroArch(t_RegistroArch* registroArch){
	free(registroArch->registro);
	free(registroArch);
	return;
}

/*
 * Abro el archivo que recibo x parametro y devuelvo el filedescriptor
 * es responsabilidad de la funcion que llama el cerrar el fd
 * OJO esta dificil cerrarlos.!!!!
 */
int abrirArchivoLocal(char* nombreArch){
	int fd;
	char pathNombreArchivo = string_new();
	string_append(&pathNombreArchivo, "/tmp/");
	string_append(&pathNombreArchivo, nombreArch);
	fd=open(pathNombreArchivo,O_RDONLY);
	return fd;
}

int abrirArchivoRemoto(t_archivoAReducir* unArchivoAReducir ){
	int sockNodo;
	char* ip_nodo_char;
	struct in_addr addr;
	addr.s_addr=unArchivoAReducir->ipNodo;
	ip_nodo_char= inet_ntoa(addr);
	sockNodo= crearCliente(ip_nodo_char,unArchivoAReducir->puertoNodo);
	/*enivar solicitud, usar protocolo ENVIO_ARCHIVO_NODO_NODO pero corregir la funcionalidad
	 * en el case swith ya que no se recibe un archivo si no que esta parte se abre y se
	 * empezara a solicitar registro x registro*/
	enviarSolicitudDeArchivoRemoto(sockNodo,unArchivoAReducir->nombreArch);
	return sockNodo;
}


/*
 * Sea un fd simple o socket, lo leo con syscall: read().
 */
t_RegistroArch* getRegistroDeArchivo(int fd){
	t_RegistroArch* registroArch=(	t_RegistroArch*)malloc(sizeof(t_RegistroArch));
	int numbytes;
	char buffer;
	registroArch->socket=fd;
	registroArch->registro=string_new();
	while ((numbytes = read(fd,&buffer , sizeof(char))) > 0){
		if (buffer!='\n'){
			string_append(&(registroArch->registro), &buffer);
		} else {
			string_append(&(registroArch->registro), "\0");
			break;
		}
	  }
	if(numbytes==0){
		printf("recibi EOF de %d\n", fd);
		return NULL;
	} else if (numbytes <0){
		printf("Error: no se pudo leer de %d\n", fd);
		perror("read");
		return NULL;
	}
	return registroArch;
}


/* Funcion: actulizar la lista de registrosArchivos, destruir el registro seleccionado
 * que ya fue enviado al pipe del reduce y ya fue procesado, y obtener el nuevo
 * regitro del archivo correspondiente y agregarlo a la lista
 */
void actualizarListaDeRegistros(t_list* Lista_reg){
	t_RegistroArch* nuevoRegistro, regYaProcesado;
	//pedir el registro siguiente del archivo, del regYaProcesado->socket
/*	nuevoRegistro=getRegistroDeArchivo(regYaProcesado->socket);
	* explota porque regYaProcesado no tiene informacion, no esta inicilizado ni malloceado y
	* estas queriendo entrar a una estructura sin mallocear y sin valor
	*/
	//eliminar de la lista el regYaProcesado, que siempre es el primero
	regYaProcesado=list_remove(Lista_reg, 0);

	//insertar el nuevo registro obtenido en la lista de registros.
	if(nuevoRegistro != NULL) {
		list_add(Lista_reg,nuevoRegistro);
	} else {
		printf("No se recibio mas datos desde el fd: %d, lo cerramos\n",regYaProcesado->socket);
		close(regYaProcesado->socket);
	}
	destruir_RegistroArch(regYaProcesado);
//	return;
}

/* *************************************************************************************
 * Funcion:
 *  Toma un archivo a reducir, abre los archivos si son locales y si son
 *  remotos se conecta a los nodos_remotos para pedir los archivos.
 *  En cualquiera de los casos  les pide el primer registro.
 *  Lo importante aca es que los archivos, ya quedan abiertos.
 *
 ***************************************************************************************/
t_RegistroArch* getFirstRowsFromArchivosAReducir(t_archivoAReducir* unArchivoAReducir ){

	t_RegistroArch* unRegistroArch;
    if(esLocal(unArchivoAReducir)){
    	int fdLocal=abrirArchivoLocal(unArchivoAReducir->nombreArch);
    	unRegistroArch=getRegistroDeArchivo(fdLocal);
    } else {
    	int sockRemoto=abrirArchivoRemoto(unArchivoAReducir);
    	unRegistroArch=getRegistroDeArchivo(sockRemoto);
    }
    return unRegistroArch;
}

int aparearArchivosDeLaListaArchivosAReducir(t_list* archivosAReducir){
	t_list* Lista_reg; //lista de Registros a aparear
	t_RegistroArch* rowSeleccionado;
	/*Obtengo los primeros registros de cada archivo, remoto o local */
	Lista_reg=list_map(archivosAReducir,(void*)getFirstRowsFromArchivosAReducir);
	bool _registroArchivo_menor(t_RegistroArch* reg, t_RegistroArch* regMenor){
		return (strcmp(reg->registro,regMenor->registro)<=0);
	}
	while( !list_is_empty(Lista_reg)){
		//siempre ordeno la lista de menor a mayor, y el primer elemento es el que va al pipe
		list_sort(Lista_reg, (void*) _registroArchivo_menor);
		//rowSeleccionado=apareoDeRegistros(Lista_reg);
		rowSeleccionado=list_get(Lista_reg,0);

		enviarRowAlPipeDelReduce(rowSeleccionado->registro);//aca me falta el pipe como parametro.

		// eliminar de la lista el registro procesado y obtener el registro sgte del mismo archivo
		actualizarListaDeRegistros(Lista_reg);
	}
	return 0;
}
