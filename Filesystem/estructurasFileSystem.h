 /*
 estructuras del fileSystem

 */
 //estructura FileSytem
#include<commons/collections/list.h>


 //estructura Archivo

typedef struct
  {
      char ip[255];//quizas conviene en un futuro que
      int puerto;//ip y puerto sean un solo char
      int numeroDeBloqueEnNodo;

  }t_bloqueEnNodo;


typedef struct
  {
    int numeroDeBloqueEnArchivo;
  	t_bloqueEnNodo *copiaDeBloque;
  }t_bloqueArch ;


typedef struct
  {
      char nombre[255];
      int padre;
      float tamanio;
      t_bloqueArch *bloqueArch;
      int estado;
  }t_archivo ;

  //estructura nodo

typedef struct
  {
  	char ip[255];//quizas conviene en un futuro que
  	int puerto;//ip y puerto sean un solo char
  	int tamanio;
  	int cantidadBloquesOcupados;
  	int activo;//para saber si el bloque se cayo o esta activo
  }t_nodo;

//estructura tabla de directorios
typedef struct
  {
  	int index;
  	char nombre[255];
  	int padre;
  }t_directorio;
//estructura del fileSystem
typedef struct
   {
	t_list *archivo;
	t_list *nodo;
	t_list *directorio;
  }t_fileSystem ;
