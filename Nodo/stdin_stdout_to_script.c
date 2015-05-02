/*
 * stdin_stdout_to_script.c
 *
 *  Created on: 2/5/2015
 *      Author: rmejia
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
int main(int argc, char* argv[])
    {
        int pfds[2];
        char buf[BUFSIZ];
        pipe(pfds);
       	int fd=open("salida.txt",O_WRONLY | O_CREAT | O_TRUNC,0777);
           // close(0);       /* cierra la stdin normal */


        if (!fork()) {
            close(1);       /* cerrar la stdout normal */
            dup(pfds[1]);   /* hace lo mismo que stdout con pfds[1] */
            close(pfds[0]); /* no necesitamos esto */
            execlp(argv[2], argv[2],argv[1] , NULL);
        } else {
            close(pfds[1]); /* no necesitamos esto */
            /* Esta parte de la lectura y escritura en disco
             * se puede mejorar con un while not EOF
             */
            read(pfds[0],buf,BUFSIZ);
            int len = strlen(buf);
            printf("tamaño de lo volcado %d",len);
            write(fd,buf,len);
        }
        return 0;
    }


