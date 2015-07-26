#include "consolaFS.h"



char *direccion;
int consola(void* unListaNodo) {
	int esRoot(t_directorio* unDirectorio){
		return (unDirectorio->index==1 && !strcmp(unDirectorio->nombre,"root") && unDirectorio->padre==0);
	}
	if(!list_any_satisfy(listaDirectorios,esRoot)){
	directorioActual = nuevoDirectorio(1, "root", 0);
	list_add(listaDirectorios,directorioActual);
	direccion = string_duplicate(directorioActual->nombre);
	}
	else{
		directorioActual=list_find(listaDirectorios,esRoot);
	}
	// // lo defino como un char** porque necesito tener un "array"
	//con todas las cadenas, donde la primera es el comando y las demas los parametros
	char **comandoSeparado;
	char comando[100];
	char *comandosValidos[21];
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
	comandosValidos[20] = string_duplicate("verNodos");
	int i;

	printf("\nBienvenido a la consola: \n");

	do {

		mostrarDireccion();
		int entrada = COMANDO_INVALIDO; //Lo asigno por defecto en caso de que la entrada no coincida con ninguno de los comandos
		leerComando(comando, 100);


		//Cuando recibo el comando lo separo en comando-parametros (1 por ahora)
		comandoSeparado = string_split(comando," ");

		//Una vez que lo separe reviso si la primera parte de las separaciones comandoSeparado[0]
		//se corresponde con alguno de los comandos validos para asignar a entrada
		for (i = 0; i < 21; i++) {
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
			if(comprobarParametros(2,comandoSeparado)==1)
			copiarAMDFS(comandoSeparado[1],comandoSeparado[2]);
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
			if(comprobarParametros(2,comandoSeparado)==1)
			verBloque(comandoSeparado[1],comandoSeparado[2]);
			break;
		case ELIMINAR_BLOQUE:
			if(comprobarParametros(2,comandoSeparado)==1)
			eliminarBloque(comandoSeparado[1],comandoSeparado[2]);
			break;
		case COPIAR_BLOQUE:
			if(comprobarParametros(3,comandoSeparado)==1)
			copiarBloque(comandoSeparado[1],comandoSeparado[2],comandoSeparado[3]);
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
		case VER_NODOS:
			if(comprobarParametros(0,comandoSeparado)==1)
			verNodos();
			break;
		case COMANDO_INVALIDO:
			printf("Comando Invalido\n");
			break;

		}
			//para evitar que halla perdidas de memoria, esta funcion va liberando uno por uno los strings en comandoSeparado
		freeSeparaciones(comandoSeparado);


	} while (!exit);
	for (i = 0; i < 21; i++) free(comandosValidos[i]);
	free(direccion);
	raise(SIGINT);
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
	guardarPersistencia();
//	printf("El file system ha sido formateado.\n");
}

void eliminarArchivo(char *archivo) {

	void eliminarArch(t_archivo *unArchivo){
		eliminarArchivoYreferencias(unArchivo,listaArchivos,listaNodos);
	}

	validarYEjecutar(archivo,validarArchivo,(void*) eliminarArch);
}

void renombrarArchivo(char *archivo, char *nuevoNombre) {
	renombrarArchivoPorNombre(archivo, nuevoNombre,archivosVisiblesDesdeDirectorioDado(archivo));
}

void moverArchivo(char *archivo, char* padreString) {

	int padre = string_to_int(padreString);
	if (padre!=(-1)) {
		t_list *archivosVisibles = archivosVisiblesDesdeDirectorioDado(archivo);
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

	validarYEjecutar(directorio,validarDirectorio,(void*)eliminarDir);

}

void renombrarDirectorio(char *directorio, char* nuevoNombre) {

	void renombrarDir(t_directorio *unDirectorio){
		renombrarDirectorioConNombre(nuevoNombre,unDirectorio);
	}

	validarYEjecutar(directorio,validarDirectorio,(void*) renombrarDir);

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
		validarYEjecutar(directorio,validarDirectorio,(void*) moverDir);

	}
}
void copiarAMDFS(char *archivoDestino, char *archivoACopiar) {

	if(!strcmp(archivoACopiar,"*hourly.txt")){
		copiarAMDFS("201301hourly.txt","201301hourly.txt");
		copiarAMDFS("201302hourly.txt","201302hourly.txt");
		copiarAMDFS("201303hourly.txt","201303hourly.txt");
		copiarAMDFS("201304hourly.txt","201304hourly.txt");
	}

	t_list *archivosVisibles  = archivosVisiblesDesdeDirectorioDado("");

	int seLlamaIgual(t_archivo* unArchivo){
		return !strcmp(unArchivo->nombre,archivoDestino);
	}

	if(list_find(archivosVisibles,(void*)seLlamaIgual)){
		printf("Ya existe un archivo con ese nombre en el File System\n");
	}
	else{
		char* path = strdup("../Archivos/");
		string_append(&path,archivoACopiar);
		levantarArchivoAMemoriaYDistribuirANodos(path, archivoDestino, directorioActual->index);
	}
	list_destroy(archivosVisibles);
}

void copiarAFS(char *archivo) {
	void copiarArchAFS(t_archivo *unArchivo){
		int resultado = descargarArchivo(unArchivo);
		if(resultado>0) log_info(mdfs_logger,"El archivo fue copiado al FileSystem local.");
		else log_error(mdfs_logger,"Error al copiar el archivo al FileSystem local.");
}
	validarYEjecutar(archivo, validarArchivo, (void*)copiarArchAFS);

}

void solicitarMD5(char *archivo) {
	void pedirMD5(t_archivo *unArchivo){
		int resultado = descargarArchivo(unArchivo);
		if(resultado>0) {
			printf("El hash solicitado es:.\n");
			system("md5sum  ../archivoReconstruido");
			remove("../archivoReconstruido");
		}
		else	log_error(mdfs_logger,"Error al copiar el archivo al FileSystem local.");
}
	validarYEjecutar(archivo, validarArchivo, (void*)pedirMD5);
}

void verBloque(char *archivo, char *numeroBloque) {

	int nroBloque=atoi(numeroBloque);

	void verBloqueArch(t_archivo* unArchivo){

		verBloqueDeArchivo(unArchivo,nroBloque);

	}

	validarYEjecutar(archivo,validarArchivo, (void*)verBloqueArch);
}

void eliminarBloque(char *archivo, char* numeroBloque) {

	int nroBloque=atoi(numeroBloque);

	int eliminarBloqueDeArchivo(t_archivo *unArchivo){

			t_bloqueArch* bloqueDeArchivo = list_get(unArchivo->bloquesDeArch,nroBloque);

			if(detectarError(bloqueDeArchivo->copiasDeBloque,list_is_empty,"No existen mas copias de ese bloque para eliminar.\n")) return -1;

			t_bloqueEnNodo* copiaDelBloque = list_remove(bloqueDeArchivo->copiasDeBloque,0);

			disminuirNodo(copiaDelBloque);

			free(copiaDelBloque);

			return 1;

			}

	validarYEjecutar(archivo,validarArchivo, (void*)eliminarBloqueDeArchivo);
}

void copiarBloque(char *archivo, char* numeroBloque, char* idNodo) {

	int nroBloque=atoi(numeroBloque);

	int id=atoi(idNodo);

	void copiarBloque(t_archivo* unArchivo){

	funcionCopiarBloque(unArchivo,nroBloque,id);

	}

		validarYEjecutar(archivo,validarArchivo, (void*)copiarBloque);
}

void levantarNodo(char *nodo) {

	void activarUnNodo(t_nodo* unNodo){
		activarNodo(unNodo,socketDeMarta);
		log_info(mdfs_logger,"Se activo el nodo de id %d",unNodo->id);
	}

	char* nodoIDComoString(t_nodo* unNodo){
		return string_itoa(nodoID(unNodo));
	}

	if(string_equals_ignore_case(nodo,"todos")){
		t_list* idsDeNodos = list_map(listaNodos,nodoIDComoString);
		list_iterate(idsDeNodos,levantarNodo);
		list_destroy_and_destroy_elements(idsDeNodos,free);
	}

	else validarNodoYEjecutar(nodo,activarUnNodo);


}

void eliminarNodo(char *nodo) {
		void eliminarNod(t_nodo *unNodo){
			/*eliminarNodoYReferencias(unNodo,listaNodos,listaArchivos); XXX habria que eliminarlo o desactivarlo?
			printf("Se elimino nodo de id %d.\n",unNodo->id);*/
			desactivarNodo(unNodo,socketDeMarta);
			log_info(mdfs_logger,"Se desactivo nodo de id %d",unNodo->id);
		}
		validarNodoYEjecutar(nodo,(void*)eliminarNod);
}

void help() {
	//printf("Comandos Validos\n");
	//printf("formatearMDFS		Formatea el MDFS\n");
	//printf("eliminarArchivo archivo		Borra el archivo 'archivo'\n");
	//printf("renombarArchivo archivo		Renombra el archivo 'archivo'\n");
	//printf("moverArchivo archivo		Mueve el archivo 'archivo'\n");
	//printf(
			//"crearDirectorio directorio		Crea un directorio llamado 'directorio'\n");
	//printf("eliminarDirectorio directorio		Borra el directorio 'directorio'\n");
	//printf(
			//"renombrarDirectorio directorio		Renombra el directorio 'directorio'\n");
	//printf("moverDirectorio directorio		Mueve el directorio 'directorio'\n");
	//printf("copiarAMDFS archivo		Copia el archivo file al MDFS\n");
	//printf("copiarAFS archivo		Copia el archivo file al File System\n");
	//printf("solicitarMD5 archivo		Obtiene el MD5 de 'archivo'\n");
	//printf("verBloque bloque		Muestra el bloque 'bloque'\n");
	//printf("eliminarBloque bloque	Borra el bloque 'bloque'\n");
	//printf("copiarBloque bloque		Copia el bloque 'bloque'\n");
	//printf("levantarNodo nodo		Agrega el nodo 'nodo'\n");
	//printf("eliminarNodo nodo		Borra el nodo 'nodo'\n");
	//printf("salir		Cierra la consola del MDFS\n\n");
	
	 printf("===========================================================================\n");
	 printf("* Comandos Validos:                                                        \n");
	 printf("===========================================================================\n");
	 printf("* formatearMDFS                  |->(Formatea el MDFS)                     \n");
	 printf("* eliminarArchivo archivo        |->(Borra el archivo 'archivo')           \n");
	 printf("* renombarArchivo archivo        |->(Renombra el archivo 'archivo')        \n");
	 printf("* moverArchivo archivo           |->(Mueve el archivo 'archivo')           \n");
	 printf("* crearDirectorio directorio     |->(Crea un directorio 'directorio'       \n");
	 printf("* eliminarDirectorio directorio  |->(Borra el directorio 'directorio')     \n");
	 printf("* renombrarDirectorio directorio |->(Renombra el directorio 'directorio')	\n");
	 printf("* moverDirectorio directorio     |->(Mueve el directorio 'directorio')     \n");
	 printf("* copiarAMDFS archivo            |->(Copia el archivo file al MDFS)        \n");
	 printf("* copiarAFS archivo              |->(Copia el archivo file al File System) \n");
	 printf("* solicitarMD5 archivo           |->(Obtiene el MD5 de 'archivo')          \n");
	 printf("* verBloque bloque               |->(Muestra el bloque 'bloque')           \n");
	 printf("* eliminarBloque bloque          |->(Borra el bloque 'bloque')             \n");
	 printf("* copiarBloque bloque            |->(Copia el bloque 'bloque')             \n");
	 printf("* levantarNodo nodo              |->(Agrega el nodo 'nodo')                \n");
	 printf("* eliminarNodo nodo              |->(Borra el nodo 'nodo')                 \n");
	 printf("* Salir                          |->(Cierra la consola del MDFS)           \n");
	 printf("===========================================================================\n");
}

void ls(){
	t_list *directoriosVisibles = directoriosVisiblesDesdeDirectorioDado("");
	t_list *archivosVisibles  = archivosVisiblesDesdeDirectorioDado("");

	mostrarLista(archivosVisibles, (void*) mostrarNombreArchivo);
	mostrarLista(directoriosVisibles, (void*) mostrarNombreDirectorio);

	list_destroy(directoriosVisibles);
	list_destroy(archivosVisibles);
	if(dirVacio(directorioActual)){
		printf("Directorio vacio\n");
	}
}


void verNodos(){
	mostrarLista(listaNodos, (void*)mostrarNodo);
}


t_list *directoriosVisiblesDesdeDirectorioDado(char* unPath) {
	if (!strcmp(unPath, "")) {
		return hijosDelActual(directorioActual);
	} else {
		char** vectorPath = string_split(unPath,"/");
		t_directorio* directorioEnQueEstoyParado = ubicarseEnDirectorio(vectorPath);

		return hijosDelActual(directorioEnQueEstoyParado);

	}
}
t_list *archivosVisiblesDesdeDirectorioDado(char* unPath){
	if(!strcmp(unPath,"")){
		directoriosVisiblesDesdeDirectorioDado("");
	}
	else{
	char** vectorPath = string_split(unPath,"/");
	int i=0;
	for(i=0;vectorPath[i+1]!=NULL;i++);
	char* nombreArchivo = vectorPath[i-1];
	for(i=strlen(unPath)-1;unPath[i]!='/';i--);
	unPath[i]='\0';
	}
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

void validarArchivoYEjecutar(char* unPath,void (*funcion)(void*)){
	t_list *archivosVisibles;
	char* unArch;
	int tieneAlgunaBarra=0;
	int j;
	for(j=0;j<strlen(unPath);j++)	if(unPath[j]=='/') tieneAlgunaBarra=1;
	if(tieneAlgunaBarra){
	char** vectorPath = string_split(unPath,"/");
	int i=0;
	for(i=0;vectorPath[i+1]!=NULL;i++);
	unArch = vectorPath[i-1];
	for(i=strlen(unPath)-1;unPath[i]!='/';i--);
	unPath[i]='\0';
	archivosVisibles = archivosVisiblesDesdeDirectorioDado(unPath);
	}
	else{
		archivosVisibles = archivosVisiblesDesdeDirectorioDado("");
		unArch=unPath;
	}
	t_archivo *archivoObjetivo = buscarArchPorNombre(unArch,archivosVisibles);
	list_destroy(archivosVisibles);
	if(archivoObjetivo!=NULL) funcion(archivoObjetivo);
	else {
		printf("Archivo %s no encontrado.\nEl tamaño del archivo pedido es %d",unArch,strlen(unArch));
		fflush(stdout);
	}
}

void validarDirectorioYEjecutar(char* unPath, void (*funcion)(void*)) {

	t_list* directoriosVisibles;
	char* unArch;
	int tieneAlgunaBarra = 0;
	int j;
	char* unDirectorio;
	int nombreCoincide(t_directorio *unDir) {
		return !strcmp(unDir->nombre, unDirectorio);
	}
	for (j = 0; j < strlen(unPath); j++)
		if (unPath[j] == '/')
			tieneAlgunaBarra = 1;
	if (tieneAlgunaBarra) {
		char** vectorPath = string_split(unPath, "/");
		int i = 0;
		for (i = 0; vectorPath[i + 1] != NULL; i++)
			;
		unDirectorio = vectorPath[i - 1];
		for (i = strlen(unPath) - 1; unPath[i] != '/'; i--)
			;
		unPath[i] = '\0';

		directoriosVisibles = directoriosVisiblesDesdeDirectorioDado(unPath);
	} else {
		directoriosVisibles = directoriosVisiblesDesdeDirectorioDado("");
		unDirectorio = unPath;
	}
	t_directorio *directorioObjetivo = list_find(directoriosVisibles, (void *) nombreCoincide);
	list_destroy(directoriosVisibles);
	if (directorioObjetivo != NULL)
		funcion(directorioObjetivo);
	else
		printf("Directorio no encontrado.\n");
}

void validarNodoYEjecutar(char* unNodo, void (*funcion)(void*)) {
	int idNodo = atoi(unNodo);

	t_nodo *nodoObjetivo = buscarNodoPorId(idNodo, listaNodos);

	if ((nodoObjetivo != NULL)) {
		if (nodoEstas(nodoObjetivo->socket) && (nodoObjetivo->socket!=-1)) {
			funcion(nodoObjetivo);

		} else
			printf("El nodo no está conectado.\n");

	} else
		printf("El nodo no existe.\n");
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
int descargarArchivo(t_archivo *unArchivo){
		sem_init(&semaforo,0,0);
			archivoReconstruido = fopen("../archivoReconstruido","w");
			int resultado = obtenerArchivo(unArchivo);
			fclose(archivoReconstruido);
			if (resultado<=0) {
				remove("../archivoReconstruido");
				return 0;
		}
				else {

				return 1;
		}
}

int verBloqueDeArchivo(t_archivo *unArchivo, int nroBloque){

		archivoReconstruido = fopen("../Archivos/bloqueObtenido","w");

		t_bloqueArch* bloqueDeArchivo = list_get(unArchivo->bloquesDeArch,nroBloque);

		if(detectarError(bloqueDeArchivo->copiasDeBloque,list_is_empty,"No existen copias disponibles de ese bloque.\n")) return -1;

		t_bloqueEnNodo* bloqueEnNodo = list_get(bloqueDeArchivo->copiasDeBloque,0);

		t_nodo *nodoEncontrado = buscarNodoPorId(bloqueEnNodo->id, listaNodos);

		if (detectarError(nodoEncontrado,(void*)esNull,"Nodo no encontrado.\n")) return -1;

		pedirBloqueANodo(nodoEncontrado->socket,bloqueEnNodo->numeroDeBloqueEnNodo,VER_BLOQUE_NODO,bloqueEnNodo->tamanioBloque);

		sem_wait(&consola_sem);
		log_info(mdfs_sync_logger,"wait consola_sem");

		void* buffer;

		recibirBloqueDeNodo(nodoEncontrado->socket, (void*) &buffer);

		printf("\n\n%s\n\n",buffer);

		fflush(stdout);

		write(fileno(archivoReconstruido), buffer, strlen(buffer));

		free(buffer);

		sem_post(&escuchar_sem);
		log_info(mdfs_sync_logger,"post escuchar_sem");
		return 1;

	}

int funcionCopiarBloque(t_archivo *unArchivo, int nroBloque, int id){

	int esMayorACantDeBloques(int unNumero){
		return unNumero>=list_size(unArchivo->bloquesDeArch);
	}

	if (detectarError(nroBloque, esMayorACantDeBloques, "El numero de bloque no existe. Posiblemente estés tratando de acceder a un número mayor a la cantidad de bloques del archivo.\n")) return -1;

	t_bloqueArch* bloqueDeArchivo = list_get(unArchivo->bloquesDeArch,nroBloque);

	t_list* copiasDelBloque = bloqueDeArchivo->copiasDeBloque;

	if(detectarError(bloqueDeArchivo->copiasDeBloque,list_is_empty,"No existen copias disponibles de ese bloque.\n")) return -1;
	t_bloqueEnNodo* bloqueEnNodo = list_get(copiasDelBloque,0);

	t_nodo *nodoAlQuePidoBloque = buscarNodoPorId(bloqueEnNodo->id, listaNodos);

	if(detectarError(nodoAlQuePidoBloque,esNull,"Copia del bloque no disponible.\n")) return -1;

	t_nodo *nodoAlQueEnvioBloque = buscarNodoPorId(id, listaNodos);

	if(detectarError(nodoAlQueEnvioBloque,esNull,"Nodo no encontrado.\n")) return -1;

	pedirBloqueANodo(nodoAlQuePidoBloque->socket,bloqueEnNodo->numeroDeBloqueEnNodo,COPIAR_BLOQUE_NODO,bloqueEnNodo->tamanioBloque);

	sem_wait(&consola_sem);
	log_info(mdfs_sync_logger,"wait consola_sem");

	void* buffer;

	int tamanio = recibirBloqueDeNodo(nodoAlQuePidoBloque->socket, (void*) &buffer);

	setBloque(nodoAlQueEnvioBloque, buffer, tamanio, 0,copiasDelBloque);

	sem_post(&escuchar_sem);
	log_info(mdfs_sync_logger,"post escuchar_sem");

	return 1;

}

int detectarError(void* cosaAChequear,int (*condicion)(void*),char* mensaje){

	if(condicion(cosaAChequear)){
		if(mensaje!=NULL){
		printf(mensaje);
		}
		return 1;
	}
	return 0;
}

int esNull(void* algo){
	return algo==NULL;
}


t_list* hijosDelActual(t_directorio* unDirectorio) {
	return list_filter(listaDirectorios, ( {bool esHijoDeActual(t_directorio* unDir)
				{	return esHijo(unDir,unDirectorio);}esHijoDeActual;}));
}

int esInvalido(char* unPath){
	if(!strcmp(unPath,"/")) return 1;

	int i,contadorBarrasSeguidas; //Chequeo si tiene dos / seguidos
	for(i=0;unPath[i]!='\0';i++){
		if(unPath[i]=='/') contadorBarrasSeguidas++;
		else contadorBarrasSeguidas=0;
		if(contadorBarrasSeguidas>1) return 1;
	}

	return 0;
}


int validarYEjecutar (char* path, void* (*validador)(void*,void*), void (*funcion)(void*)){

	if(detectarError(path,esInvalido,"El path es invalido\n")) return -1;

	char** vectorPath = string_split(path,"/");

	int i;

	for(i=0;vectorPath[i+1]!=NULL;i++);

	t_directorio* directorioReferencia = ubicarseEnDirectorio(vectorPath);

	if(detectarError(directorioReferencia,esNull,"El path ingresado es invalido\n")) return -1;

	void* objeto = validador(vectorPath[i],directorioReferencia);

	if(detectarError(objeto,esNull,"El archivo pedido no existe\n")) return -1;

	funcion(objeto);

	for(i=0;vectorPath[i]!=NULL;i++) free(vectorPath[i]);

	free(vectorPath);

	return 1;

}

t_directorio* ubicarseEnDirectorio(char** vectorPath){
	t_directorio* directorioAuxiliar = directorioActual;
	t_list* hijosDelDirectorioAuxiliar;
	t_directorio* directorioSiguiente;
	int i;
	for(i=0;vectorPath[i+1]!=NULL;i++){
		if(!strcmp(vectorPath[i],"..")){
			if(directorioAuxiliar->index>1)
			directorioAuxiliar = directorioPadreDe(directorioAuxiliar);
		}
		else{
			int esHijo(t_directorio* unDir){
				return unDir->padre==directorioAuxiliar->index;
			}
			hijosDelDirectorioAuxiliar = list_filter(listaDirectorios,esHijo);
			directorioSiguiente = buscarDirPorNombre(vectorPath[i],hijosDelDirectorioAuxiliar);
			if(directorioSiguiente==NULL){
				list_destroy(hijosDelDirectorioAuxiliar);
				return NULL;
			}
			directorioAuxiliar = directorioSiguiente;
			list_destroy(hijosDelDirectorioAuxiliar);
		}
	}
	return directorioAuxiliar;
}

t_archivo* validarArchivo(char* nombreArchivo, t_directorio* directorioReferencia){
	int nombreCoincide(t_archivo* unArch){
		return !strcmp(unArch->nombre,nombreArchivo);
	}
	int estaEnDirectorio(t_archivo* unArch){
		return unArch->padre == directorioReferencia->index;
	}
	t_list* archivosVisibles = list_filter(listaArchivos,estaEnDirectorio);
	t_archivo* archivoBuscado = list_find(archivosVisibles,nombreCoincide);
	list_destroy(archivosVisibles);
	return archivoBuscado;
}

t_directorio* validarDirectorio(char* nombreDirectorio, t_directorio* directorioReferencia){
	if(!strcmp(nombreDirectorio,"..")) return directorioPadreDe(directorioReferencia);
	int nombreCoincide(t_directorio* unDir){
		return !strcmp(unDir->nombre,nombreDirectorio);
	}
	int estaEnDirectorio(t_directorio* unDir){
		return unDir->padre == directorioReferencia->index;
	}
	t_list* directoriosVisibles = list_filter(listaDirectorios,estaEnDirectorio);
	t_directorio* directorioBuscado = list_find(directoriosVisibles,nombreCoincide);
	list_destroy(directoriosVisibles);
	return directorioBuscado;
}

void posicionarseEnDirectorio(t_directorio* unDirectorio) {
	directorioActual = unDirectorio;
}

void cd(char* unPath) {

	if (!strcmp(unPath, "..")) {
		if (directorioActual->index > 1)
			directorioActual = directorioPadreDe(directorioActual);
	}

	else
		validarYEjecutar(unPath, validarDirectorio, posicionarseEnDirectorio);
}



t_directorio* directorioPadreDe(t_directorio* unDirectorio) { //PASAMANOS PORQUE ME GUSTA MAS ESTE NOMBRE
	return encontrarDirectorioPadre(listaDirectorios, unDirectorio);
}



void mostrarDireccion(){
	printf("Socketes:~root");
	t_list* direccionActual = list_create();
	t_directorio* dirAux;
	for(dirAux=directorioActual;dirAux->index!=1;dirAux=directorioPadreDe(dirAux)) list_add_in_index(direccionActual,0,dirAux);
	void imprimirDirectorio(t_directorio* unDir){
		printf("/%s",unDir->nombre);
	}
	list_iterate(direccionActual,imprimirDirectorio);
	printf(">");
	fflush(stdout);
	list_destroy(direccionActual);
}
