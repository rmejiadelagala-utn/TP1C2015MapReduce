/*
 * funcionesNodo.c
 *
 *  Created on: 18/5/2015
 *      Author: utnso
 */
#include "funcionesNodo.h"

uint32_t TAMANIODISCO, TAMANIOARCHIVO;
int fd = -1, fd_a;

t_config_nodo* leerArchivoConfig(char *path_config){

	t_config *config;
	t_config_nodo* configNodo = malloc(sizeof(t_config_nodo));

	config = config_create(path_config);

	if (config->properties->elements_amount == 0) {
		printf("\nERROR AL LEER ARCHIVO DE CONFIGURACION %s \n", path_config);
		config_destroy(config);
		exit(-1);
	}

	configNodo->IP_FS     = config_get_string_value(config, "IP_FS");
	configNodo->PUERTO_FS = config_get_int_value(config, "PUERTO_FS");
	configNodo->ARCH_BIN  = config_get_string_value(config, "ARCHIVO_BIN");
	configNodo->DIR_TEMP  = config_get_string_value(config, "DIR_TEMP");
	configNodo->NODO_NEW  = config_get_string_value(config, "NODO_NUEVO");
	configNodo->IP_NODO   = config_get_string_value(config, "IP_NODO");
	configNodo->PUERTO_NODO = config_get_int_value(config, "PUERTO_NODO");

/*	printf("Conectando a IP: %s\n", config_get_string_value(config, "IP_FS"));
	printf("Puerto: %d\n", config_get_int_value(config, "PUERTO_FS"));
	printf("Archivo_bin: %s\n", config_get_string_value(config, "ARCHIVO_BIN"));
	printf("Directorio Temporal: %s\n", config_get_string_value(config, "DIR_TEMP"));
	printf("Nodo Nuevo: %s\n", config_get_string_value(config, "NODO_NUEVO"));
	printf("IP Nodo: %s\n", config_get_string_value(config, "IP_NODO"));
	printf("Puerto Nodo: %d\n", config_get_int_value(config, "PUERTO_NODO"));*/

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


