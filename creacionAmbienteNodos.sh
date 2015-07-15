#!  /bin/bash

echo "Debo estar parado en Mi repo: "
echo "			tp-2015-1c-socketes-planificados"

#Crear directorios Nodo
mkdir Nodo1
mkdir Nodo2
mkdir Nodo3
mkdir Nodo4
mkdir Nodo5
mkdir Nodo6
mkdir Nodo7

# Crear directorios Debug
mkdir Nodo1/Debug
mkdir Nodo2/Debug
mkdir Nodo3/Debug
mkdir Nodo4/Debug
mkdir Nodo5/Debug
mkdir Nodo6/Debug
mkdir Nodo7/Debug

# Crear directorios Debug
mkdir Nodo1/Debug/tmp
mkdir Nodo2/Debug/tmp
mkdir Nodo3/Debug/tmp
mkdir Nodo4/Debug/tmp
mkdir Nodo5/Debug/tmp
mkdir Nodo6/Debug/tmp
mkdir Nodo7/Debug/tmp

# Crear directorios ArchivosVarios
mkdir Nodo1/ArchivosVarios
mkdir Nodo2/ArchivosVarios
mkdir Nodo3/ArchivosVarios
mkdir Nodo4/ArchivosVarios
mkdir Nodo5/ArchivosVarios
mkdir Nodo6/ArchivosVarios
mkdir Nodo7/ArchivosVarios

#Copiar el ConfigNodo.txt desde el Nodo a los otros nodos NodoX
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo1/ArchivosVarios
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo2/ArchivosVarios
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo3/ArchivosVarios
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo4/ArchivosVarios
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo5/ArchivosVarios
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo6/ArchivosVarios
cp  Nodo/ArchivosVarios/ConfigNodo.txt  Nodo7/ArchivosVarios

#Copiar el databin desde el Nodo a los otros nodos NodoX
cp  Nodo/ArchivosVarios/data.bin  Nodo1/Debug/
cp  Nodo/ArchivosVarios/data.bin  Nodo2/Debug/
cp  Nodo/ArchivosVarios/data.bin  Nodo3/Debug/
cp  Nodo/ArchivosVarios/data.bin  Nodo4/Debug/
cp  Nodo/ArchivosVarios/data.bin  Nodo5/Debug/
cp  Nodo/ArchivosVarios/data.bin  Nodo6/Debug/
cp  Nodo/ArchivosVarios/data.bin  Nodo7/Debug/

#Creacion de los enlances simbolicos:
ln -s ../../Nodo/Debug/Nodo Nodo1/Debug/Nodo
ln -s ../../Nodo/Debug/Nodo Nodo2/Debug/Nodo
ln -s ../../Nodo/Debug/Nodo Nodo3/Debug/Nodo
ln -s ../../Nodo/Debug/Nodo Nodo4/Debug/Nodo
ln -s ../../Nodo/Debug/Nodo Nodo5/Debug/Nodo
ln -s ../../Nodo/Debug/Nodo Nodo6/Debug/Nodo
ln -s ../../Nodo/Debug/Nodo Nodo7/Debug/Nodo

exit 0;
