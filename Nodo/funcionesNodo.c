/*
 * funcionesNodo.c
 *
 *  Created on: 18/5/2015
 *      Author: utnso
 */
#include "funcionesNodo.h"

uint32_t TAMANIODISCO, TAMANIOARCHIVO;
int fd = -1, fd_a;

void guardarEnDisco(t_archivoTemporal* unArchivo) {
	FILE *archivoEnDisco = fopen(unArchivo->pathDestino, "w");
	write(fileno(archivoEnDisco), unArchivo->data, unArchivo->tamanio);
	free(archivoEnDisco);
	liberarArchivoTemporal(unArchivo);
}

void liberarArchivoTemporal(t_archivoTemporal* unArchivo) {
	free(unArchivo->data);
	free(unArchivo->pathDestino);
	free(unArchivo);
}

void recibirArchivo(int socket) {
	t_archivoTemporal *unArchivo = malloc(sizeof(t_archivoTemporal));
	unArchivo->tamanio = recibirInt(socket);
	unArchivo->pathDestino = strdup(recibirString(socket));
	unArchivo->data = strdup(recibirString(socket));
}

t_config_nodo* leerArchivoConfig(char *path_config) {

	t_config *config;
	t_config_nodo* configNodo = malloc(sizeof(t_config_nodo));

	config = config_create(path_config);

	if (config->properties->elements_amount == 0) {
		log_error(nodo_logger, "ERROR AL LEER ARCHIVO DE CONFIGURACION %s ",
				path_config);
		config_destroy(config);
		exit(-1);
	}

	configNodo->IP_FS = strdup(config_get_string_value(config, "IP_FS"));
	configNodo->PUERTO_FS = config_get_int_value(config, "PUERTO_FS");
	char* pathDataBin = strdup("../Archivos/");
	string_append(&pathDataBin, config_get_string_value(config, "ARCHIVO_BIN"));
	configNodo->ARCH_BIN = pathDataBin;
	configNodo->DIR_TEMP = strdup(config_get_string_value(config, "DIR_TEMP"));
	configNodo->NODO_NEW = (config_get_string_value(config, "NODO_NUEVO"))[0];
	configNodo->IP_NODO = strdup(config_get_string_value(config, "IP_NODO"));
	configNodo->PUERTO_NODO = config_get_int_value(config, "PUERTO_NODO");
	configNodo->ID = config_get_int_value(config, "ID");
	log_info(nodo_logger,
			"Nodo en la espera de conexiones de JOBs y otros Nodos.");
	log_info(nodo_logger, "Conectando a IP: %s",
			config_get_string_value(config, "IP_FS"));
	log_info(nodo_logger, "Puerto: %d",
			config_get_int_value(config, "PUERTO_FS"));
	log_info(nodo_logger, "Archivo_bin: %s",
			config_get_string_value(config, "ARCHIVO_BIN"));
	log_info(nodo_logger, "Directorio Temporal: %s",
			config_get_string_value(config, "DIR_TEMP"));
	log_info(nodo_logger, "Nodo Nuevo: %c",
			(config_get_string_value(config, "NODO_NUEVO"))[0]);
	log_info(nodo_logger, "IP Nodo: %s",
			config_get_string_value(config, "IP_NODO"));
	log_info(nodo_logger, "Puerto Nodo: %d",
			config_get_int_value(config, "PUERTO_NODO"));
	log_info(nodo_logger, "Hola, soy el nodo %d",
			config_get_int_value(config, "ID"));

	/* Una vez que se levantaron los datos del archivo de configuracion
	 puedo destruir la estructura config. */
	config_destroy(config);

	return configNodo;
}

char* mapeo_archivo(char* path) {

	char* data_archivo;
//	uint32_t TAMANIOARCHIVO;

	if ((fd_a = open(path, O_RDONLY)) == -1)
		err(1, "Nodo: Error al abrir archivo (open)");

	struct stat bufa;

	stat(path, &bufa);
	TAMANIOARCHIVO = bufa.st_size;

	if ((data_archivo = mmap(0, TAMANIOARCHIVO, PROT_READ, MAP_SHARED, fd_a, 0))
			== MAP_FAILED) {
		log_error(nodo_logger, "Error al iniciar el mapeo de disco %s. '%s' ",
				obtenerNombreArchivo(path), strerror(errno));
		close(fd_a);
		exit(1);
	}
	close(fd_a);
	return data_archivo;
}

char* mapeo_disco(char* path) {

	char* data_disco;
	//uint32_t TAMANIODISCO;

	TAMANIODISCO = obtener_tamanio_disco(path);

	if ((data_disco = mmap(NULL, TAMANIODISCO, PROT_READ | PROT_WRITE,
			MAP_FILE | MAP_SHARED, fd, 0)) == MAP_FAILED) {
		;
		log_error(nodo_logger, "Error al iniciar el mapeo de disco %s. '%s' ",
				obtenerNombreArchivo(path), strerror(errno));
		close(fd);
		exit(1);
	}
	return data_disco;
}

uint32_t obtenerDirBloque(uint32_t nroBloque) {
	nroBloque = nroBloque * BLKSIZE;
	return nroBloque;
}

uint32_t obtener_tamanio_disco(char* path) {

	uint32_t tamanio_disco;

	if ((fd = open(path, O_RDWR)) == -1)
		err(1, "Nodo: Error al abrir midata1.bin (open)");

	struct stat buf;
	stat(path, &buf);
	tamanio_disco = buf.st_size;

	return tamanio_disco;
}

int obtener_cant_elem_array(char **array) {

	int elem = 0;

	while (array[elem] != NULL) {
		elem++;
	}

	return elem;
}

char* obtenerNombreArchivo(char* ruta) {

	char* file_name;
	char** array_path;
	int cant_elem;

	array_path = string_split(ruta, "/");
	cant_elem = obtener_cant_elem_array(array_path);
	file_name = array_path[cant_elem - 1];

	return file_name;
}

char* getFileContent(char* nombreFile, char * ruta_archivo) {

	char* path;
	char* fileMaped;
	int fd_a;

	struct stat buff;
	path = strdup("");

	strcpy(path, ruta_archivo);
	strcat(path, "/");
	strcat(path, nombreFile);

	fd_a = open(path, O_RDWR);
	stat(path, &buff);

	fileMaped = mmap(NULL, buff.st_size, PROT_READ | PROT_WRITE,
			MAP_FILE | MAP_SHARED, fd_a, 0);

	if (fileMaped == MAP_FAILED) {
		perror("mmap");
		log_error(nodo_logger, "Falló el mapeo para el archivo solicitado: %s",
				nombreFile);
		exit(-1);
	}
	close(fd_a); //Cierro el archivo

	return fileMaped;
}

void crearScriptMapper(const char* codigo_script, char* nombre) {

	fflush(stdout);

	FILE* scriptMapper;

	if ((scriptMapper = fopen(nombre, "w+")) == NULL) {
		perror("Error al crear el script del mapper");
		exit(1);
	}

	fputs(codigo_script, scriptMapper);

	int fileDes = fileno(scriptMapper);

	if (fchmod(fileDes, 0755)) {
		fclose(scriptMapper);
		printf("Hubo algun error en el fchmod\n");
		fflush(stdout);
		return;
	}

	fflush(stdout);
	int resultado = fclose(scriptMapper);
	if (resultado < 0) {
		log_error(nodo_logger, "Hubo un error al cerrar el archivo");
	}

	return;
}

void crearScriptReduce(const char* codigo_script, char* nombre) {

//	FILE* fd;
	FILE* scriptReduce;
//	char texto[100];

//	if((fd =fopen("/home/utnso/Escritorio/TPSO-2015/reduce.pl","r"))==NULL){
//		perror("Error al abrir el script del reduce");
//		exit(1);
//	}

//	while (feof(fd) == 0) {
//		fgets(texto,100,fd);
//		printf( "%s",texto );
//	}

	if ((scriptReduce = fopen(nombre, "w+")) == NULL) {
		perror("Error al crear el script del Reduce");
		exit(1);
	}

//	while (feof(fd) == 0) {
//		fgets(texto,100,fd);
//		fputs(texto,scriptReduce);
//	}

	fputs(codigo_script, scriptReduce);

	char *permisosCommand = string_new();

	string_append(&permisosCommand, "chmod u+x ");
	string_append(&permisosCommand, nombre);

	system(permisosCommand);
	fclose(scriptReduce);
	free(permisosCommand);

	fflush(stdout);

	return;
}

int redireccionar_stdin_stdout_mapper(char *pathPrograma,
		char *pathArchivoSalida, char* data_bloque) {

	int mapFallido = 0;

	FILE *pipeMapper = NULL;

	size_t tamanioComando = strlen(pathPrograma) + 11
			+ strlen(pathArchivoSalida);

	char *comando = malloc(tamanioComando);

	fflush(stdout);
	snprintf(comando, tamanioComando, "%s | sort > %s", pathPrograma,
			pathArchivoSalida);

	void signal_callback_handler(int signal) {
		mapFallido = 1;
	}

	signal(SIGPIPE, signal_callback_handler);

	pipeMapper = popen(comando, "w");

	if (!pipeMapper) {
		printf("Error al popenear\n");
		fflush(stdout);
		return -1;
	}
	fflush(stdout);

	fputs(data_bloque, pipeMapper);
	int closear = pclose(pipeMapper);
	if (closear < 0) {
		printf("No se pudo peclosear bien\n");
		fflush(stdout);
		return -1;
	}

	if (mapFallido)
		return -1;
	else
		return 1;
}

//TODO hacer esto de forma correcta, obteniendo los path de los de reducir y apareando
int redireccionar_stdin_stdout_reduce(char *pathPrograma,
		char *pathArchivoSalida, t_list* archivosAReducir, int idReduce) {
	FILE *stdin = NULL;

	char *comando = malloc(
			strlen(pathPrograma) + 12 + strlen(pathArchivoSalida));

	sprintf(comando, "%s | sort >> %s", pathPrograma, pathArchivoSalida);

	stdin = popen(comando, "w");

	log_info(nodo_logger, "Voy a vaciar el archivo truncado");
	char* archivoATruncar = string_new();
	fflush(stdout);
	string_append(&archivoATruncar, pathArchivoSalida);
	log_info(nodo_logger, "Arme el nombre del archivo");
	char* comandoParaTruncar = string_new();
	string_append(&comandoParaTruncar, "truncate -s 0 ");
	string_append(&comandoParaTruncar, archivoATruncar);
	system(comandoParaTruncar);
	fflush(stdout);

	if (stdin != NULL) {
		aparear(archivosAReducir,idReduce);
		char* nombreArchAReducir = strdup("/tmp/archivoApareado");
		string_append_with_format(&nombreArchAReducir,"_%i_%i",arch_config->ID,idReduce);
		FILE* archivoAReducir = fopen(nombreArchAReducir, "r");
		struct stat datosArch;
		stat(nombreArchAReducir, &datosArch);
		log_info(nodo_logger, "El tamaño del archivo es %d", datosArch.st_size);
		char* dataArchivoAReducir = mmap((caddr_t) 0, datosArch.st_size,
				PROT_READ, MAP_SHARED, fileno(archivoAReducir), 0);
		if (fprintf(stdin, "%s", dataArchivoAReducir) < 0) {
			log_error(nodo_logger, "Error al enviar archivo al reduce");
		}
		if (pclose(stdin) < 0) {
			printf("Error al cerrar con pclose\n");
			fflush(stdout);
			return -1;
		}
		free(comando);
		free(nombreArchAReducir);
	} else {

		log_error(nodo_logger, "No se pudo ejecutar el programa!");
		return -1;
	}

	return 1;
}

int ejecutarMapper(char * path_s, char* path_tmp, char* datos_bloque) {

	if ((redireccionar_stdin_stdout_mapper(path_s, path_tmp, datos_bloque))
			< 1) {

		log_info(nodo_logger, "Error al ejecutar Mapper");
		return -1;
	}
	return 1;

}

int ejecutarReduce(char * path_s, char* path_tmp, t_list* archivosAReducir, int idReduce) {

	if ((redireccionar_stdin_stdout_reduce(path_s, path_tmp, archivosAReducir, idReduce))
			< 1) {
		log_info(nodo_logger, "Error al ejecutar Reduce");
		return -1;
	}
	return 1;

}

//Intentando entender que hace esta función para luego ejecutarla en el redirec.
//Toma una lista de elementos t_RegistroArch
//Devulve un elemento t_RegistroArch
//Para cada elemento de la lista Lista_reg, ejecuta realizoApareo
//Dentro de la misma, se compara cada uno de esos registros con otro copiar_datos
//Si al compararlo es menor, asigna a datosReg el valor del registro y a su vez lo
//copia a copiar_datos.
//luego de todas las pasadas, se queda con el registro que salió de comparar con los
//demás y dar <=0 Sea lo que sea esto.

//Se me ocurre a mi, que esto debe hacerse para todos los registros de todos los
//archivos a reducir. A medida que te vas quedando con esos datosReg, vas agarrando
//el siguiente registro del archivo que tenía ese registro seleccionado.
//Todos esos registros seleccionados irían concatenados y formarían parte del archivo
//total a reducir.

void aparear(t_list* archivosAReducir, int idReduce) {
	char* comando = strdup("cat ");
	//printf("Mi ip es %d, la ip de este archivo es %d",unArchivo->ipNodo,inet_addr(arch_config->IP_NODO));
	int noLoTengo(t_archivoAReducir* unArchivo) {
		return (unArchivo->ipNodo != inet_addr(arch_config->IP_NODO))
				|| (unArchivo->puertoNodo != arch_config->PUERTO_NODO);
	}
	t_list* archivosQueMeFaltan = list_filter(archivosAReducir,
			(void*) noLoTengo);
	void conseguirArchivo(t_archivoAReducir* unArchivo) {
		//string_append_with_format(&(unArchivo->nombreArch), "/tmp/%s ", unArchivo->nombreArch);
		char* nombreDeArchivoAPedir = strdup("/tmp/");
		string_append(&nombreDeArchivoAPedir, unArchivo->nombreArch);
		struct in_addr ipaddr;
		ipaddr.s_addr = unArchivo->ipNodo;
		int socketDeNodo = crearCliente(inet_ntoa(ipaddr),
				unArchivo->puertoNodo);
		void* mensaje = crearBufferConProtocolo(ENVIO_ARCHIVOS_NODO_NODO);
		bufferAgregarString(mensaje, nombreDeArchivoAPedir,
				strlen(nombreDeArchivoAPedir) + 1);
		enviarBuffer(mensaje, socketDeNodo);
		char* archivoData;
		if ((archivoData = recibirString(socketDeNodo)) < 0)
			return;
		fflush(stdout);
		FILE* fileArchivo = fopen(nombreDeArchivoAPedir, "w");
		fputs(archivoData, fileArchivo);
		fclose(fileArchivo);
		close(socketDeNodo);
	}

	list_iterate(archivosQueMeFaltan, conseguirArchivo);

	void concatenar(t_archivoAReducir* unArchivo) {
		fflush(stdout);
		string_append_with_format(&comando, "/tmp/%s ", unArchivo->nombreArch);
	}
	list_iterate(archivosAReducir, (void*) concatenar);
	string_append_with_format(&comando, "|sort > /tmp/archivoApareado_%i_%i", arch_config->ID, idReduce);
	system(comando);
	log_info(nodo_logger, "Concatenacion correcta");
}

t_RegistroArch* apareoDeRegistros(t_list* Lista_reg) {

	t_RegistroArch* datosReg = malloc(sizeof(t_RegistroArch));
	t_RegistroArch* infoRegArch;
	char* copiar_datos;

	copiar_datos = malloc(BLKSIZE);

	infoRegArch = list_get(Lista_reg, 0);

	strcpy(copiar_datos, infoRegArch->registro);
	printf("La copia es %s\n", infoRegArch->registro);

	void realizoApareo(t_RegistroArch* unRegistro) {

		if ((strcmp(unRegistro->registro, copiar_datos)) <= 0) {
			copiar_datos = malloc(BLKSIZE);

			strcpy(copiar_datos, unRegistro->registro);
			datosReg->registro = strdup(copiar_datos);
			strcat(datosReg->registro, "\n");
			datosReg->socket = unRegistro->socket;
		}

	}

	list_iterate(Lista_reg, (void*) realizoApareo);
	free(copiar_datos);

	return datosReg;
}

/*char* aparear_registros(char* datos_archivo){

 char** reg_archivo;
 char* copiar_datos;
 int i;

 reg_archivo = string_split(datos_archivo, "\n");
 copiar_datos = malloc (BLKSIZE);

 strcpy(copiar_datos, reg_archivo[0]);

 for(i = 0; reg_archivo[i]!= NULL; i++){

 if ((strcmp(reg_archivo[i],copiar_datos)) <= 0){
 copiar_datos = malloc (BLKSIZE);
 strcpy(copiar_datos, reg_archivo[i]);
 }

 }
 strcat(copiar_datos, "\n");
 return copiar_datos;

 }*/
