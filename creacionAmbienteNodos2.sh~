#!  /bin/bash

echo "Debo estar parado en Mi repo: "
echo "			tp-2015-1c-socketes-planificados"

# Crear directorios Debug
mkdir Nodo1/Debug
mkdir Nodo2/Debug
mkdir Nodo3/Debug
mkdir Nodo4/Debug
mkdir Nodo5/Debug
mkdir Nodo6/Debug
mkdir Nodo7/Debug

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
