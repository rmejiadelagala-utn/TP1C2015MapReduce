 /*
 funciones para inicializar las estructuras del fileSystem

 */

 //funciones para estructura Archivo
 t_archivo nuevoArchivo(char[255] nombre, int padre, float tamanio,
            t_bloquesArch *bloquesArch, int estado, int numeroDeBloqueEnArchivo)
{
  struct t_archivo unArchivo; /* definimos una nueva estructura llamado unArchivo */
 
  /* asignacion de  campos a la estructura */
  strcpy(unArchivo.nombre,nombre); //la forma de insertar valores a un tipo char en una estructura es usando strcpy de la libreria string.h
  unArchivo.padre = padre;
  unArchivo.tamanio = Archivo;
  unArchivo.*bloquesArch = *bloquesArch;
  unArchivo.estado = estructura;
  unArchivo.numeroDeBloqueEnArchivo = numeroDeBloqueEnArchivo;
  //devuelvo el archivo
  return unArchivo;
}


t_bloqueArch nuevoBloqueEnArchivo(t_bloqueEnNodo *copiaDeBloque){
  struct t_bloquesArch unBloqueEnArchivo;//nueva estructura
  //asignacion de campos a la estructura
  unBloqueEnArchivo.*copiaDeBloque = *copiaDeBloque;
  //devuelvo el bloqueDeArchivo
  return unBloqueEnArchivo;
}


t_bloqueEnNodo nuevoBloqueEnNodo(char[255] ip, int puerto, int numeroDeBloqueEnNodo){
  struct t_bloqueEnNodo unBloqueEnNodo;//nueva estructura
  //asignacion de valores a la estructura
  strcpy(unArchivo.ip,ip);
  unBloqueEnNodo.puerto = puerto;
  unBloqueEnNodo.numeroDeBloqueEnNodo = numeroDeBloqueEnNodo;
  //devuelvo la estructura
  return unBloqueEnNodo;
}



//funciones para la estructura nodo
t_nodo nuevoNodo(char[255] ip, int puerto, int tamanio){
  struct t_nodo unNodo;//nueva estructura 
  //asignacion de campos a la estructura
  strcpy(unNodo.ip,ip);
  unNodo.puerto = puerto;
  unNodo.tamanio = tamanio;
  unNodo.cantidadBloquesOcupados = 0;//SE SUPONE QUE SI EL NODO ES NUEVO NO TIENE BLOQUES ESCRITOS
  unNodo.activo = 1;//SE SUPONE QUE SI EL NODO SE ESTA CONECTANDO, ENTONCES ESTA ACTIVO
  //devuelvo la estructura
  return unNodo;
}

//funciones para la estructura tabla de directorios

t_directorio nuevoDirectorio(int index, char[255] nombre, int padre){
  struct t_directorio unDirectorio;
  //asignacion de campos a la estructura
  unDirectorio.index = index;
  strcpy(unDirectorio.index,index);
  unDirectorio.padre = padre;
  //devuelvo la estructura
  return unDirectorio;
}