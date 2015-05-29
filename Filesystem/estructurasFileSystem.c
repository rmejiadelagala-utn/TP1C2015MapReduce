 /*
 estructuras del fileSystem

 */
 //estructura FileSytem
 struct t_fileSystem
   {
   	t_archivo *archivo;
   	t_nodo *nodo;
   	t_directorio *directorio;
  }fileSystem ;


 //estructura Archivo
struct t_archivo
  {
      char[255] nombre;
      int padre;
      float tamanio;
      t_bloqueArch *bloqueArch;
      int estado;
      int numeroDeBloqueEnArchivo;
  } ;


struct t_bloqueArch
  {
  	t_bloqueEnNodo *copiaDeBloque;
    t_info info;//informacion del archivo
  } ;

struct t_bloqueEnNodo
  {
      char[255] ip;//quizas conviene en un futuro que
      int puerto;//ip y puerto sean un solo char
      int numeroDeBloqueEnNodo;

  };
  //estructura nodo

struct t_nodo
  {
  	char[255] ip;//quizas conviene en un futuro que
  	int puerto;//ip y puerto sean un solo char
  	int tamanio;
  	int cantidadBloquesOcupados;
  	int activo;//para saber si el bloque se cayo o esta activo
  };

//estructura tabla de directorios
struct t_directorio
  {
  	int index;
  	char[255] nombre;
  	int padre;
  };  