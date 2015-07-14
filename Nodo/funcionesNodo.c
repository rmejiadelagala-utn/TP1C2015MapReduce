/*
 * funcionesNodo.c
 *
 *  Created on: 18/5/2015
 *      Author: utnso
 */
#include "funcionesNodo.h"

uint32_t TAMANIODISCO, TAMANIOARCHIVO;
int fd = -1, fd_a;

void guardarEnDisco(t_archivoTemporal* unArchivo){
	FILE *archivoEnDisco = fopen(unArchivo->pathDestino,"w");
	write(fileno(archivoEnDisco), unArchivo->data, unArchivo->tamanio);
	free(archivoEnDisco);
	liberarArchivoTemporal(unArchivo);
}

void liberarArchivoTemporal(t_archivoTemporal* unArchivo){
	free(unArchivo->data);
	free(unArchivo->pathDestino);
	free(unArchivo);
}

void recibirArchivo (int socket){
	t_archivoTemporal *unArchivo= malloc(sizeof(t_archivoTemporal));
	unArchivo->tamanio=recibirInt(socket);
	unArchivo->pathDestino=strdup(recibirString(socket));
	unArchivo->data=strdup(recibirString(socket));
}

t_config_nodo* leerArchivoConfig(char *path_config){

	t_config *config;
	t_config_nodo* configNodo = malloc(sizeof(t_config_nodo));

	config = config_create(path_config);

	if (config->properties->elements_amount == 0) {
		printf("\nERROR AL LEER ARCHIVO DE CONFIGURACION %s \n", path_config);
		config_destroy(config);
		exit(-1);
	}

	configNodo->IP_FS     = strdup(config_get_string_value(config, "IP_FS"));
	configNodo->PUERTO_FS = config_get_int_value(config, "PUERTO_FS");
	configNodo->ARCH_BIN  = strdup(config_get_string_value(config, "ARCHIVO_BIN"));
	configNodo->DIR_TEMP  = strdup(config_get_string_value(config, "DIR_TEMP"));
	configNodo->NODO_NEW  = (config_get_string_value(config, "NODO_NUEVO"))[0];
	configNodo->IP_NODO   = strdup(config_get_string_value(config, "IP_NODO"));
	configNodo->PUERTO_NODO = config_get_int_value(config, "PUERTO_NODO");
	configNodo->ID     = config_get_int_value(config, "ID");

	printf("Conectando a IP: %s\n", config_get_string_value(config, "IP_FS"));
	printf("Puerto: %d\n", config_get_int_value(config, "PUERTO_FS"));
	printf("Archivo_bin: %s\n", config_get_string_value(config, "ARCHIVO_BIN"));
	printf("Directorio Temporal: %s\n", config_get_string_value(config, "DIR_TEMP"));
	printf("Nodo Nuevo: %c\n", (config_get_string_value(config, "NODO_NUEVO"))[0]);
	printf("IP Nodo: %s\n", config_get_string_value(config, "IP_NODO"));
	printf("Puerto Nodo: %d\n", config_get_int_value(config, "PUERTO_NODO"));
	printf("Hola, soy el nodo %d\n", config_get_int_value(config, "ID"));

	/* Una vez que se levantaron los datos del archivo de configuracion
	   puedo destruir la estructura config. */
	config_destroy(config);

	return configNodo;
}


char* mapeo_archivo(char* path){

	char* data_archivo;
//	uint32_t TAMANIOARCHIVO;

	if ((fd_a = open(path, O_RDONLY)) == -1)
		err(1, "Nodo: Error al abrir archivo (open)");

	struct stat bufa;

	stat(path, &bufa);
	TAMANIOARCHIVO = bufa.st_size;

	if ((data_archivo = mmap(0, TAMANIOARCHIVO, PROT_READ, MAP_SHARED, fd_a, 0)) == MAP_FAILED){
		printf("Error al iniciar el mapeo de disco %s. '%s' ", obtenerNombreArchivo(path), strerror(errno));
		close(fd_a);
		exit(1);
	}

	return data_archivo;
}

char* mapeo_disco(char* path){

	char* data_disco;
	//uint32_t TAMANIODISCO;

	TAMANIODISCO = obtener_tamanio_disco(path);

	if ((data_disco = mmap(NULL, TAMANIODISCO, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0))== MAP_FAILED){;
		printf("Error al iniciar el mapeo de disco %s. '%s' ", obtenerNombreArchivo(path), strerror(errno));
		close(fd);
	    exit(1);
	}
	 return data_disco;
}


uint32_t obtenerDirBloque(uint32_t nroBloque){
	nroBloque = nroBloque * BLKSIZE;
	return nroBloque;
}

uint32_t obtener_tamanio_disco(char* path){

	uint32_t tamanio_disco;

	if ((fd = open(path, O_RDWR)) == -1)
		err(1, "Nodo: Error al abrir midata1.bin (open)");

		struct stat buf;
		stat(path, &buf);
		tamanio_disco = buf.st_size;

	return tamanio_disco;
}

int obtener_cant_elem_array(char **array){

	int elem = 0;

	while(array[elem]!=NULL){
		elem++;
	}

  return elem;
}

char* obtenerNombreArchivo(char* ruta){

	char* file_name;
	char** array_path;
	int cant_elem;

	array_path = string_split(ruta, "/");
	cant_elem = obtener_cant_elem_array(array_path);
    file_name = array_path[cant_elem-1];

	return file_name;
}

char* getFileContent(char* nombreFile, char * ruta_archivo){

	char* path;
	char* fileMaped;
	int fd_a;

	struct stat buff;
	path=strdup("");

	strcpy(path,ruta_archivo);
	strcat(path,"/");
	strcat(path,nombreFile);

	fd_a = open(path,O_RDWR);
	stat(path, &buff);

	fileMaped=mmap(NULL,buff.st_size,PROT_READ|PROT_WRITE,MAP_FILE|MAP_SHARED,fd_a,0);

		if (fileMaped==MAP_FAILED){
			perror("mmap");
			printf("Falló el mapeo para el archivo solicitado\n");
			exit(-1);
		}
	close(fd_a); //Cierro el archivo

	return fileMaped;
}

void crearScriptMapper(const char* codigo_script, char* nombre){

	FILE* scriptMapper;



	if((scriptMapper=fopen(nombre,"w+"))==NULL){
		perror("Error al crear el script del mapper");
		exit(1);
	}

	fputs(codigo_script,scriptMapper);
	
	char *permisosCommand = string_new();

	string_append(&permisosCommand, "chmod a+x ");
	string_append(&permisosCommand,nombre);

	system(permisosCommand);
	fclose(scriptMapper);
	free(permisosCommand);

	return;
}

void crearScriptReduce(const char* codigo_script, char* nombre){

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


	if((scriptReduce=fopen(nombre,"w+"))==NULL){
		perror("Error al crear el script del Reduce");
		exit(1);
	}

//	while (feof(fd) == 0) {
//		fgets(texto,100,fd);
//		fputs(texto,scriptReduce);
//	}

	fputs(codigo_script,scriptReduce);
	
	char *permisosCommand = string_new();

	string_append(&permisosCommand, "chmod u+x ");
	string_append(&permisosCommand,nombre);

	system(permisosCommand);
	fclose(scriptReduce);
	free(permisosCommand);

	return;
}

int redireccionar_stdin_stdout_mapper(char *pathPrograma,char *pathArchivoSalida,char* data_bloque)
{
	FILE *stdin = NULL;

	char *comando = malloc(strlen(pathPrograma)+11+strlen(pathArchivoSalida));

	sprintf(comando,"%s | sort > %s",pathPrograma, pathArchivoSalida);	

	stdin = popen (comando,"w");

	if (stdin != NULL){

		fprintf(stdin, "%s\n",data_bloque);

		pclose (stdin);
		free(comando);
	}
	else{

		printf("No se pudo ejecutar el programa!");
		return -1;
	}

	return 0;
}

int redireccionar_stdin_stdout_reduce(char *pathPrograma,char *pathArchivoSalida,char* data_bloque)
{
	FILE *stdin = NULL;

	char *comando = malloc(strlen(pathPrograma)+12+strlen(pathArchivoSalida));

	sprintf(comando,"%s | sort >> %s",pathPrograma, pathArchivoSalida);	

	stdin = popen (comando,"w");

	if (stdin != NULL){

		fprintf(stdin, "%s\n",data_bloque);

		pclose (stdin);
		free(comando);
	}
	else{

		printf("No se pudo ejecutar el programa!");
		return -1;
	}

	return 0;
}

void ejecutarMapper(char * path_s, char* path_tmp, char* datos_bloque){

	if ((redireccionar_stdin_stdout_mapper(path_s, path_tmp, datos_bloque)) < 0)
		printf("Error al ejecutar Mapper");

}

void ejecutarReduce(char * path_s, char* path_tmp, char* datos_bloque){

	if ((redireccionar_stdin_stdout_reduce(path_s, path_tmp, datos_bloque)) < 0)
		printf("Error al ejecutar Reduce");

}


t_RegistroArch* apareoDeRegistros(t_list* Lista_reg){

	t_RegistroArch* datosReg = malloc(sizeof(t_RegistroArch));
	t_RegistroArch* infoRegArch;
	char* copiar_datos;

	copiar_datos = malloc(BLKSIZE);

	infoRegArch = list_get(Lista_reg, 0);

	strcpy(copiar_datos, infoRegArch->registro);
	printf("La copia es %s\n", infoRegArch->registro);

	void realizoApareo(t_RegistroArch* unRegistro) {

		if ((strcmp(unRegistro->registro,copiar_datos)) <= 0){
			copiar_datos = malloc (BLKSIZE);

			strcpy(copiar_datos, unRegistro->registro);
			datosReg->registro = strdup(copiar_datos);
			strcat(datosReg->registro, "\n");
			datosReg->socket = unRegistro->socket;
		}

	}

	list_iterate(Lista_reg, (void*)realizoApareo);

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
