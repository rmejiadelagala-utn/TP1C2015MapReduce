 /*
 estructuras del fileSystem con funciones para inicializarlas

 */
 //Archivo
   struct t_archivo
  {
      char[255] nombre;
      int padre;
      float tamanio;
      t_bloquesArch *bloquesArch;
      int estado;
      int numeroDeBloqueEnArchivo;
  } ;


  struct t_bloquesArch
  {
  	t_bloquesEnNodo *copiasDeBloque;
  } ;

    struct t_bloquesEnNodo
  {
      t_ip ip;//a desarrollar
      t_puerto puerto;//a desarrolar
      int numeroDeBloqueEnNodo;

  };
