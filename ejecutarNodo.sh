#! /bin/bash

DIRLOG=$HOME
export DIR_LOG
LOG=$DIRLOG/ejecutarNodo_`date '+%Y%m%d'`.log

OK=0

if test $1
then

echo "=====================================================================" >> $LOG
echo "Proceso que ejecuta un Nodo recibiendo el nro de nodo como parametro " >> $LOG
echo "`date '+Dia %d/%m/%EY Hora %H:%M:%S'`                                " >> $LOG
echo "Parametro recibido: $1                                               " >> $LOG
echo "---------------------------------------------------------------------" >> $LOG

cd Nodo$1/Debug    >> $LOG
./Nodo   ../ArchivosVarios/ConfigNodo.txt 
Res=$?

  if ($Res -ne $OK)
    then 
        echo "Fallo ejecucion de Nodo"
  fi

Resultado=$Res     >>$LOG

else
     echo "se esperaba paremetro nro de Nodo"
     Res=1
     Resultado=$Res   >>$LOG
fi

exit $Res
