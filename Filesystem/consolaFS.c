#include "consolaFS.h"
#include "funcionesFileSystem.h"
#include "fsystem.h"

t_directorio *directorioActual;
char *direccion;
int consola(void* unListaNodo) {
	directorioActual = nuevoDirectorio(1, "root", 0);
	list_add(listaDirectorios,directorioActual);
	direccion = string_duplicate(directorioActual->nombre);
	t_list *listaNodo = unListaNodo;
	// // lo defino como un char** porque necesito tener un "array"
	//con todas las cadenas, donde la primera es el comando y las demas los parametros
	char **comandoSeparado;
	char comando[100];
	char *comandosValidos[20];
	int exit = 0;
	//Incializo los comandos en un array para despues poder hacer
	//el switch y llamar a la funcion correspondiente
	comandosValidos[0] = string_duplicate("formatear");
	comandosValidos[1] = string_duplicate("eliminarArchivo");
	comandosValidos[2] = string_duplicate("renombrarArchivo");
	comandosValidos[3] = string_duplicate("moverArchivo");
	comandosValidos[4] = string_duplicate("crearDirectorio");
	comandosValidos[5] = string_duplicate("eliminarDirectorio");
	comandosValidos[6] = string_duplicate("renombrarDirectorio");
	comandosValidos[7] = string_duplicate("moverDirectorio");
	comandosValidos[8] = string_duplicate("copiarAMDFS");
	comandosValidos[9] = string_duplicate("copiarAFS");
	comandosValidos[10] = string_duplicate("solicitarMD5");
	comandosValidos[11] = string_duplicate("verBloque");
	comandosValidos[12] = string_duplicate("EliminarBloque");
	comandosValidos[13] = string_duplicate("copiarBloque");
	comandosValidos[14] = string_duplicate("levantarNodo");
	comandosValidos[15] = string_duplicate("eliminarNodo");
	comandosValidos[16] = string_duplicate("help");
	comandosValidos[17] = string_duplicate("salir");
	comandosValidos[18] = string_duplicate("cd");
	comandosValidos[19] = string_duplicate("ls");
	int i;

	printf("\nBienvenido a la consola: \n");

	do {
		printf("Socketes:~%s>",direccion);
		int entrada = COMANDO_INVALIDO; //Lo asigno por defecto en caso de que la entrada no coincida con ninguno de los comandos
		leerComando(comando, 100);


		//Cuando recibo el comando lo separo en comando-parametros (1 por ahora)
		comandoSeparado = string_split(comando," ");

		//Una vez que lo separe reviso si la primera parte de las separaciones comandoSeparado[0]
		//se corresponde con alguno de los comandos validos para asignar a entrada
		for (i = 0; i < 20; i++) {
			if (string_equals_ignore_case(comandoSeparado[0],
					comandosValidos[i])) {
				entrada = i;
			}
		}
		//Si el comando es uno de aquellos que necesitan parametros, valida el parametro antes de ejecutarlo
		if(entrada==CD && (comandoSeparado[1] == NULL)) entrada = FALTA_PARAMETRO;
		switch (entrada) {
		case FORMATEAR:
			if(comprobarParametros(0,comandoSeparado)==1)
			formatearMDFS();
			break;
		case ELIMINAR_ARCHIVO:
			if(comprobarParametros(1,comandoSeparado)==1)
			eliminarArchivo(comandoSeparado[1]);
			break;
		case RENOMBRAR_ARCHIVO:
			if(comprobarParametros(2,comandoSeparado)==1)
			renombrarArchivo(comandoSeparado[1],comandoSeparado[2]);
			break;
		case MOVER_ARCHIVO:
			if(comprobarParametros(2,comandoSeparado)==1)
			moverArchivo(comandoSeparado[1],comandoSeparado[2]);
			break;
		case CREAR_DIRECTORIO:
			if(comprobarParametros(1,comandoSeparado)==1)
			crearDirectorio(comandoSeparado[1]);
			break;
		case ELIMINAR_DIRECTORIO:
			if(comprobarParametros(1,comandoSeparado)==1)
			eliminarDirectorio(comandoSeparado[1]);
			break;
		case RENOMBRAR_DIRECTORIO:
			if(comprobarParametros(2,comandoSeparado)==1)
			renombrarDirectorio(comandoSeparado[1],comandoSeparado[2]);
			break;
		case MOVER_DIRECTORIO:
			if(comprobarParametros(2,comandoSeparado)==1)
			moverDirectorio(comandoSeparado[1],comandoSeparado[2]);
			break;
		case COPIAR_A_MDFS:
			if(comprobarParametros(1,comandoSeparado)==1)
			copiarAMDFS(comandoSeparado[1]);
			break;
		case COPIAR_A_FS:
			if(comprobarParametros(1,comandoSeparado)==1)
			copiarAFS(comandoSeparado[1]);
			break;
		case SOLICITAR_MD5:
			if(comprobarParametros(1,comandoSeparado)==1)
			solicitarMD5(comandoSeparado[1]);
			break;
		case VER_BLOQUE:
			if(comprobarParametros(1,comandoSeparado)==1)
			verBloque(comandoSeparado[1]);
			break;
		case ELIMINAR_BLOQUE:
			if(comprobarParametros(1,comandoSeparado)==1)
			eliminarBloque(comandoSeparado[1]);
			break;
		case COPIAR_BLOQUE:
			if(comprobarParametros(1,comandoSeparado)==1)
			copiarBloque(comandoSeparado[1]);
			break;
		case LEVANTAR_NODO:
			if(comprobarParametros(1,comandoSeparado)==1)
			levantarNodo(comandoSeparado[1]);
			break;
		case ELIMINAR_NODO:
			if(comprobarParametros(1,comandoSeparado)==1)
			eliminarNodo(comandoSeparado[1]);
			break;
		case HELP:
			if(comprobarParametros(0,comandoSeparado)==1)
			help();
			break;
		case SALIR:
			if(comprobarParametros(0,comandoSeparado)==1)
			printf("Gracias, vuelvan pronto.\n");
			exit = 1;
			break;
		case CD:
			if(comprobarParametros(1,comandoSeparado)==1)
			cd(comandoSeparado[1]);
			break;
		case LS:
			if(comprobarParametros(0,comandoSeparado)==1)
			ls();
			break;
		case COMANDO_INVALIDO:
			printf("Comando Invalido\n");
			break;

		}
			//para evitar que halla perdidas de memoria, esta funcion va liberando uno por uno los strings en comandoSeparado
		freeSeparaciones(comandoSeparado);


	} while (!exit);
	for (i = 0; i < 20; i++) free(comandosValidos[i]);
	free(direccion);
	return 0;
}

void leerComando(char *lectura, int maximoLargo) {

	fgets(lectura, maximoLargo, stdin);
	//Reemplazo \n por \0
	if ((strlen(lectura)>0)&&(lectura[strlen(lectura)-1]=='\n')) lectura[strlen(lectura)-1]=0;
	//Borro los ultimos espacios;
	int i;
	for(i=strlen(lectura);(i>0)&& (lectura[--i]==' ');) lectura[i]=0;
	//Si borre todo el texto pongo un placholder
	if(!lectura[0]) lectura[0]='\n';

}

//asigna los strings en un auxiliar para ir liberandolos uno por uno
void freeSeparaciones(char ** separaciones) {
	char **separacionesAux = separaciones;

	while (*separacionesAux != NULL) {
		free(*separacionesAux);
		separacionesAux++;
	}

	free(separaciones);
}

//Todas estas excepto el help son las que vamos a tener que ir desarrollando cuando hagamos el FileSystem
void formatearMDFS() {
	//Formateo las 3 listas
	formatear(&listaNodos,&listaArchivos,&listaDirectorios);
	//Vuelvo a crear el root
	directorioActual = nuevoDirectorio(1, "root", 0);
	list_add(listaDirectorios,directorioActual);
	//Actualizo el string que dice donde estoy parado
	free(direccion);
	direccion = string_duplicate(directorioActual->nombre);

	printf("El file system ha sido formateado.\n");
}

void eliminarArchivo(char *archivo) {

	void eliminarArch(t_archivo *unArchivo){
		eliminarArchivoYreferencias(unArchivo,listaArchivos,listaNodos);
	}

	validarArchivoYEjecutar(archivo,(void*) eliminarArch);
}

void renombrarArchivo(char *archivo, char *nuevoNombre) {
	renombrarArchivoPorNombre(archivo, nuevoNombre,archivosVisiblesDesdeActual());
}

void moverArchivo(char *archivo, char* padreString) {

	int padre = string_to_int(padreString);
	if (padre!=(-1)) {
		t_list *archivosVisibles = archivosVisiblesDesdeActual();
		moverArchivoPorNombreYPadre(archivo, archivosVisibles,	listaDirectorios, padre);
		free(archivosVisibles);
	}
}

void crearDirectorio(char *nomDirectorio) {

		crearDirectorioDadoPadreYNom(nomDirectorio, directorioActual->index, listaDirectorios);

}

void eliminarDirectorio(char *directorio) {

	void eliminarDir(t_directorio *unDirectorio){
		eliminarDirectorioYContenido(unDirectorio);
	}

	validarDirectorioYEjecutar(directorio,eliminarDir);

}

void renombrarDirectorio(char *directorio, char* nuevoNombre) {

	void renombrarDir(t_directorio *unDirectorio){
		renombrarDirectorioConNombre(nuevoNombre,unDirectorio);
	}

	validarDirectorioYEjecutar(directorio,(void*) renombrarDir);

}

void moverDirectorio(char *directorio, char* padreString) {

	int padre = string_to_int(padreString);
	if (padre!=(-1)){

		void moverDir(t_directorio *unDirectorio){
			if(unDirectorio->index==padre){
				printf("No se puede mover un Directorio adentro de si mismo\n");
			}
			else{
				moverDirectorioConPadre(padre,unDirectorio);
			}
		}
		validarDirectorioYEjecutar(directorio,(void*) moverDir);

	}
}
void copiarAMDFS(char *archivo) {
	printf("Copia el archivo %s al MDFS\n", archivo);
}

void copiarAFS(char *archivo) {

	obtenerArchivo(archivo, "sarasa", directorioActual->index);

	printf("Copia el archivo %s al FileSystem\n", archivo);
}

void solicitarMD5(char *archivo) {
	printf("Obtiene el solicitarMD5 de %s\n", archivo);
}

void verBloque(char *bloque) {
	printf("Vee el Bloque nro %s\n", bloque);
}

void eliminarBloque(char *bloque) {
	printf("Borra el Bloque nro %s\n", bloque);
}

void copiarBloque(char *bloque) {
	printf("Copia el Bloque nro %s\n", bloque);
}

void levantarNodo(char *nodo) {
	printf("Agrega el nodo %s\n", nodo);
}

void eliminarNodo(char *nodo) {
	printf("Borra el nodo %s\n", nodo);
}

void help() {
	printf("Comandos Validos\n");
	printf("formatearMDFS		Formatea el MDFS\n");
	printf("eliminarArchivo archivo		Borra el archivo 'archivo'\n");
	printf("renombarArchivo archivo		Renombra el archivo 'archivo'\n");
	printf("moverArchivo archivo		Mueve el archivo 'archivo'\n");
	printf(
			"crearDirectorio directorio		Crea un directorio llamado 'directorio'\n");
	printf("eliminarDirectorio directorio		Borra el directorio 'directorio'\n");
	printf(
			"renombrarDirectorio directorio		Renombra el directorio 'directorio'\n");
	printf("moverDirectorio directorio		Mueve el directorio 'directorio'\n");
	printf("copiarAMDFS archivo		Copia el archivo file al MDFS\n");
	printf("copiarAFS archivo		Copia el archivo file al File System\n");
	printf("solicitarMD5 archivo		Obtiene el MD5 de 'archivo'\n");
	printf("verBloque bloque		Muestra el bloque 'bloque'\n");
	printf("eliminarBloque bloque	Borra el bloque 'bloque'\n");
	printf("copiarBloque bloque		Copia el bloque 'bloque'\n");
	printf("levantarNodo nodo		Agrega el nodo 'nodo'\n");
	printf("eliminarNodo nodo		Borra el nodo 'nodo'\n");
	printf("salir		Cierra la consola del MDFS\n\n");
}

void ls(){
	t_list *directoriosVisibles = directoriosVisiblesDesdeActual();
	t_list *archivosVisibles  = archivosVisiblesDesdeActual();

	mostrarLista(archivosVisibles, (void*) mostrarNombreArchivo);
	mostrarLista(directoriosVisibles, (void*) mostrarNombreDirectorio);

	list_destroy(directoriosVisibles);
	list_destroy(archivosVisibles);
	if(dirVacio(directorioActual)){
		printf("Directorio vacio\n");
	}
}

void cd(char* nombreDirectorio){
	int nombreCoincide (t_directorio *unDirectorio){
		return !strcmp(unDirectorio->nombre,nombreDirectorio);
	}
	if(!strcmp(nombreDirectorio,"..")){
		if(directorioActual->index > 1) {
			t_directorio *directorioObjetivo = encontrarDirectorioHijo(listaDirectorios,directorioActual);
			if(directorioObjetivo!=NULL){
				directorioActual=directorioObjetivo;
				//Modifico la salida por pantalla (Socketes:~root/etc)
				int i=0;
				for(i=strlen(direccion)-1;direccion[i]!='/';i--);
				direccion[i]='\0';
				//
			}
		}

	}
	else{
		t_list *directoriosVisibles = directoriosVisiblesDesdeActual();
		if(directoriosVisibles!=NULL){
			t_directorio *directorioObjetivo = list_find(directoriosVisibles,(void *) nombreCoincide);
			if(directorioObjetivo!=NULL){
				directorioActual=directorioObjetivo;
				string_append_with_format(&direccion, "/%s", directorioActual->nombre);
			}
			list_destroy(directoriosVisibles);

	}
}
}
t_list *directoriosVisiblesDesdeActual(void){
	return list_filter(listaDirectorios,({ bool esHijoDeActual(t_directorio* unDir)
	{return esHijo(unDir,directorioActual);}esHijoDeActual;}));
}
t_list *archivosVisiblesDesdeActual(void){
	int esArchivoHijo(t_archivo* unArchivo){
		return unArchivo->padre==directorioActual->index;
	}
	return list_filter(listaArchivos,(void *) esArchivoHijo);
}
int comprobarParametros(int cantParametros, char** parametros){
	int cant;
	for(cant=0;parametros[cant]!=NULL;cant++);
	cant-=1;
	if(cantParametros<cant) {
		printf("Demasiados parametros.\n");
		return 2;
	}
	if(cantParametros==cant) {
		return 1;
	}
	else {
		printf("Faltan parametros.\n");
		return 0;
	}
}

void validarArchivoYEjecutar(char* unArch,void (*funcion)(void*)){
	t_list *archivosVisibles = archivosVisiblesDesdeActual();
	t_archivo *archivoObjetivo = buscarArchPorNombre(unArch,archivosVisibles);
	list_destroy(archivosVisibles);
	if(archivoObjetivo!=NULL) funcion(archivoObjetivo);
	else printf("Archivo no encontrado.\n");
}

void validarDirectorioYEjecutar(char* unDirectorio, void (*funcion)(void*)){
	int nombreCoincide (t_directorio *unDir){
			return !strcmp(unDir->nombre,unDirectorio);
		}
	t_list *directoriosVisibles = directoriosVisiblesDesdeActual();
	t_directorio *directorioObjetivo = list_find(directoriosVisibles,(void *) nombreCoincide);
	list_destroy(directoriosVisibles);
	if(directorioObjetivo!=NULL) funcion(directorioObjetivo);
	else printf("Directorio no encontrado.\n");
}

int string_to_int(char* string){
		int i; int j=0;
		for (i=0;i<(strlen(string));i++){
			if(string[i]>='0' && string[i]<='9') j=10*j+string[i]-'0';
			else {
				printf("Parametro invalido.\n");
				return -1;
			}
		}
		return j;
}
