 /*
 estructuras del fileSystem

 */
 //estructura FileSytem



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
typedef struct
   {
   	t_archivo *archivo;
   	t_nodo *nodo;
   	t_directorio *directorio;
  }t_fileSystem ;
