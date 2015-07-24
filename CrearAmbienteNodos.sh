#!  /bin/bash
source CrearAmbienteNodos.cfg
puerto_fs_default=$puerto_fs_cfg
ip_fs_default=$ip_fs_cfg
nuevo_default=$nuevo_cfg
ip_default=$ip_cfg
puerto_default=$puerto_cfg
tam_databin_default=$tam_databin_cfg

mkdir Nodo/Archivos 2>log

function crearDataBin {
	truncate -s $2M Nodo/Archivos/data$1.bin
}

function crearConfig {
	echo "#ConfigNodo"$1 > Nodo/Archivos/ConfigNodo$1.txt
	mostrarResultado $1 $2 $3 $4 $5 $6 $7 $8 >> Nodo/Archivos/ConfigNodo$1.txt
}

function mostrarResultado {
	echo "PUERTO_FS="$2
	echo "IP_FS="$3
	echo "ARCHIVO_BIN=data"$1".bin"
	echo "DIR_TEMP=/TMP"
	echo "NODO_NUEVO="$5
	echo "IP_NODO="$6
	echo "PUERTO_NODO="$7
	echo "ID="$8
}

function generarNodo {
	echo ""
	echo "Creando data.bin..."
	crearDataBin $1 $4
	crearConfig $1 $2 $3 $4 $5 $6 $7 $8
	mostrarResultado $1 $2 $3 $4 $5 $6 $7 $8
}

function CrearNodos {
	clear
	echo "¿Cual es la ip del FileSystem?"
	read ip_fs
	echo "¿Cual es el puerto del FileSystem?"
	read puerto_fs
cantidadDeNodos=$1
for ((i=$2;i<=$cantidadDeNodos;i++))
do 
	clear
	echo "Vas a crear el nodo numero "$i
	echo "¿Cual es el id de este nodo?"
	read id
	echo "¿Cual es la ip de este nodo? Escribí 'local' para usar 127.0.0.1"
	read ip
	echo "¿Cual es el puerto de este nodo?"
	read puerto
	echo "¿De qué tamaño es el data.bin de este nodo? (En megas)"
	read databin
	echo "¿Es el nodo nuevo?"
	read nuevo	
	generarNodo $i $puerto_fs $ip_fs $databin $nuevo $ip $puerto $id 
	echo "Presione enter para continuar..."
	read enter
done
	echo "Los nodos fueron creados correctamente."
	
}

function CrearAmbienteDeCero {
	echo "Vas a crear un ambiente de cero"
	echo "¿Cuantos nodos vas a querer?"
	read cantidadDeNodos
	CrearNodos $cantidadDeNodos 1
}

function AgregarUnNodo {
	echo "Vas a agregar un nodo"
	cantNodosActuales=`ls -l Nodo/Archivos/ConfigNodo* | wc -l `
	numeroNodoProximo=$cantNodosActuales+1
	read sarasa
	CrearNodos $numeroNodoProximo $numeroNodoProximo
}

function CrearAmbientePorDefecto {

date > log
for ((i=1;i<=7;i++))
do 
	generarNodo $i $puerto_fs_default $ip_fs_default $tam_databin_default $nuevo_default $ip_default $puerto_default$i  $i >> log
done
}

function SettearNuevoEnNodos {
cantNodosActuales=`ls -l Nodo/Archivos/ConfigNodo* | wc -l `
echo "Si querés settear todos los valores de una vez, escribi TODOS, si queres settearlos uno por uno, escribí cualquier otra cosa."
read respuesta
if [ "$respuesta" == "TODOS" ];then
	echo "Ingrese valor para todos los nodos"
	read nuevo
	for ((i=1;i<=$cantNodosActuales;i++))
	do
	sed -i 6s/.*/NODO_NUEVO=$nuevo/ Nodo/Archivos/ConfigNodo$i.txt
	done
else
	for ((i=1;i<=$cantNodosActuales;i++))
	do
	echo "Ingrese valor para el nodo"$i
	read nuevo
	sed -i 6s/.*/NODO_NUEVO=$nuevo/ Nodo/Archivos/ConfigNodo$i.txt
	done
fi
echo "Nodos setteados."
}


clear
echo "Para que este script funcione correctamente debo estar parado en el repo:"
echo "tp-2015-1c-socketes-planificados"

echo "Presioná la tecla correspondiente a la acción que quieras realizar:"
echo "a - Crear ambiente de cero"
echo "b - Agregar un nodo"
echo "c - Crear ambiente por defecto (para testear el TP)."
echo "d - Settear valor NUEVO de los nodos."

read opcion

if [ "$opcion" = "a" ];  then
	CrearAmbienteDeCero
elif [ "$opcion" = "a" ];  then
	CrearAmbienteDeCero
elif [ "$opcion" = "b" ]; then
	AgregarUnNodo
elif [ "$opcion" = "c" ]; then
	CrearAmbientePorDefecto
elif [ "$opcion" = "d" ]; then
	SettearNuevoEnNodos
else
 	echo "Opción invalida"
fi

exit 0;