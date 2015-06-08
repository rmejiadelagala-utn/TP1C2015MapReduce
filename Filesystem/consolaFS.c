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
		comandoSeparado = string_split(comando, " ");

		//Una vez que lo separe reviso si la primera parte de las separaciones comandoSeparado[0]
		//se corresponde con alguno de los comandos validos para asignar a entrada
		for (i = 0; i < 20; i++) {
			if (string_equals_ignore_case(comandoSeparado[0],
					comandosValidos[i])) {
				entrada = i;
			}
		}
		//Si el comando es uno de aquellos que necesitan parametros, valida el parametro antes de ejecutarlo
		if ((entrada >= ELIMINAR_ARCHIVO) && (entrada <= ELIMINAR_NODO)
				&& (comandoSeparado[1] == NULL))
			entrada = FALTA_PARAMETRO;
		if(entrada==CD && (comandoSeparado[1] == NULL)) entrada = FALTA_PARAMETRO;
		switch (entrada) {
		case FORMATEAR:
			formatearMDFS();
			break;
		case ELIMINAR_ARCHIVO:
			eliminarArchivo(comandoSeparado[1]);
			break;
		case RENOMBRAR_ARCHIVO:
			renombrarArchivo(comandoSeparado[1]);
			break;
		case MOVER_ARCHIVO:
			moverArchivo(comandoSeparado[1]);
			break;
		case CREAR_DIRECTORIO:
			crearDirectorio(comandoSeparado[1]);
			break;
		case ELIMINAR_DIRECTORIO:
			eliminarDirectorio(comandoSeparado[1]);
			break;
		case RENOMBRAR_DIRECTORIO:
			renombrarDirectorio(comandoSeparado[1]);
			break;
		case MOVER_DIRECTORIO:
			moverDirectorio(comandoSeparado[1]);
			break;
		case COPIAR_A_MDFS:
			copiarAMDFS(comandoSeparado[1]);
			break;
		case COPIAR_A_FS:
			copiarAFS(comandoSeparado[1],listaNodo);
			break;
		case SOLICITAR_MD5:
			solicitarMD5(comandoSeparado[1]);
			break;
		case VER_BLOQUE:
			verBloque(comandoSeparado[1]);
			break;
		case ELIMINAR_BLOQUE:
			eliminarBloque(comandoSeparado[1]);
			break;
		case COPIAR_BLOQUE:
			copiarBloque(comandoSeparado[1]);
			break;
		case LEVANTAR_NODO:
			levantarNodo(comandoSeparado[1]);
			break;
		case ELIMINAR_NODO:
			eliminarNodo(comandoSeparado[1]);
			break;
		case HELP:
			help();
			break;
		case SALIR:
			exit = 1;
			break;
		case CD:
			cd(comandoSeparado[1]);
			break;
		case LS:
			ls();
			break;
		case COMANDO_INVALIDO:
			printf("Comando Invalido\n");
			break;
		case FALTA_PARAMETRO:
			printf("Parametro invalido\n");
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
	printf("Formatea el MDFS\n");
}

void eliminarArchivo(char *archivo) {
	printf("Borra el archivo %s\n", archivo);
}

void renombrarArchivo(char *archivo) {
	printf("Renombra el archivo %s\n", archivo);
}

void moverArchivo(char *archivo) {
	printf("Mueve el archivo %s\n", archivo);
}

void crearDirectorio(char *directorio) {
	printf("Crea el directorio %s\n", directorio);
}

void eliminarDirectorio(char *directorio) {
	printf("Borra el directorio %s\n", directorio);
}

void renombrarDirectorio(char *directorio) {
	printf("Renombra el directorio %s\n", directorio);
}

void moverDirectorio(char *directorio) {
	printf("Mueve el directorio %s\n", directorio);
}

void copiarAMDFS(char *archivo) {
	printf("Copia el archivo %s al MDFS\n", archivo);
}

void copiarAFS(char *archivo, t_list *unaListaNodo) {

	//obtenerArchivo(archivo, "path");
	int i=0;
	int pedirBloques (){
		send(list_get(unaListaNodo,i),"Nodo, dame tu bloque\n",strlen("Nodo, dame tu bloque\n"),0);
		i++;
		return i;
	}
	list_iterate(unaListaNodo,pedirBloques);

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
	int indexActual = directorioActual->index;
	int esDirectorioHijo(t_directorio* unDirectorio){
		return unDirectorio->padre==indexActual;
	}
	int esArchivoHijo(t_archivo* unArchivo){
		return unArchivo->padre==indexActual;
	}
	t_list *directoriosVisibles = list_filter(listaDirectorios,(void *) esDirectorioHijo);
	t_list *archivosVisibles  = list_filter(listaArchivos,(void *) esArchivoHijo);

	void listarArchivo(t_archivo* unArchivo){
		printf("%s \n",unArchivo->nombre);
	}

	void listarDirectorio(t_directorio* unDirectorio){
		printf("%s \n",unDirectorio->nombre);
	}
	if (archivosVisibles!=NULL){
	mostrarLista(archivosVisibles, (void*) listarArchivo);


	}
	if(directoriosVisibles!=NULL){
		mostrarLista(directoriosVisibles, (void*) listarDirectorio);

	}
	list_destroy(directoriosVisibles);
	list_destroy(archivosVisibles);

}

void cd(char* nombreDirectorio){
	int esDirectorioPadre(t_directorio *unDirectorio){
		return directorioActual->padre==unDirectorio->index;
	}
	int esDirectorioHijo(t_directorio *unDirectorio){
		return unDirectorio->padre==directorioActual->index;
	}
	int nombreCoincide (t_directorio *unDirectorio){
		return !strcmp(unDirectorio->nombre,nombreDirectorio);
	}
	if(!strcmp(nombreDirectorio,"..")){
		if(directorioActual->index > 1) {
			t_directorio *directorioObjetivo;
			directorioObjetivo = list_find(listaDirectorios,(void *) esDirectorioPadre);
			if(directorioObjetivo!=NULL){
				directorioActual=directorioObjetivo;
				int i=0;
				for(i=strlen(direccion)-1;direccion[i]!='/';i--);
				direccion[i]='\0';
			}
		}

	}
	else{
		t_list *directoriosVisibles = list_filter(listaDirectorios,(void *) esDirectorioHijo);
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
