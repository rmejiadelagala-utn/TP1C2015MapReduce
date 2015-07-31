#!  /bin/bash

function crearConfigNodo {
	echo #ConfigNodo$1 > Nodo/Archivos/ConfigNodo$1.txt
	echo "PUERTO_FS="$3 >> Nodo/Archivos/ConfigNodo$1.txt
	echo "IP_FS="$2 >> Nodo/Archivos/ConfigNodo$1.txt
	echo "ARCHIVO_BIN=data"$1".bin" >> Nodo/Archivos/ConfigNodo$1.txt
	echo "DIR_TEMP=/TMP" >> Nodo/Archivos/ConfigNodo$1.txt
	echo "NODO_NUEVO="$6 >> Nodo/Archivos/ConfigNodo$1.txt
	echo "IP_NODO="$4 >> Nodo/Archivos/ConfigNodo$1.txt
	echo "PUERTO_NODO="$5 >> Nodo/Archivos/ConfigNodo$1.txt
	echo "ID="$1 >> Nodo/Archivos/ConfigNodo$1.txt
	truncate -s $7M Nodo/Archivos/Data$1.bin
}

function ejecutarNodo {
	cd Nodo/Debug
	./Nodo $1
}



echo "Ingrese id del nodo"
read idNodo
echo "Ingrese ip del FileSystem"
read ipFS
echo "Ingrese puerto del FileSystem"
read puertoFS
echo "Ingrese ip del nodo"
read ipNodo
echo "Ingrese puerto del Nodo"
read puertoNodo
echo "Ingrese si el nodo es nuevo"
read nuevo
echo "Ingrese el tamaño del data.bin en MB"
read tamanioDataBin

crearConfigNodo $idNodo $ipFS $puertoFS $ipNodo $puertoNodo $nuevo $tamanioDataBin

ejecutarNodo $idNodo