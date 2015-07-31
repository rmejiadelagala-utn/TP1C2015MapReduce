#include "servidorMultihilo.h"



int crearServerMultiHilo(uint16_t puerto, void* funcion_hilo)
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client; int yes=1;


    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    //puts("Socket created");

    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( puerto );


    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {

        perror("bind failed. Error");
        return 1;
    }
    //puts("bind done");

    listen(socket_desc , 100);


    //puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        //puts("Connection accepted");

        pthread_t hilo;
        new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        if( pthread_create( &hilo , NULL ,  funcion_hilo , (void*) new_sock) < 0)
                    {
                        perror("could not create thread");
                        return 1;
                    }

        //puts("Nodo conectado");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}



/*void *recibir_handler(void* sock_ptr)
{

	char buffer[BUFFERSIZE];
	char *p;
	int sock_desc = *(int*)sock_ptr;
    char  server_reply[BUFFERSIZE];
    int read_size;

        //Receive a reply from the server
        while( read_size=recv(sock_desc , server_reply , BUFFERSIZE , 0) > 0)
        {
        	if(server_reply=="Salir") close(sock_desc);
            printf("Otro: %s\n",server_reply);
            memset(server_reply,0,sizeof(server_reply));
        }
        if(read_size==0){
        	printf("Server down.");
        }
        if(read_size<0){
        	printf("Error.");
        }
        close(sock_desc);
        return 0;
}
void *enviar_handler(void* sock_ptr)
{
	char buffer[BUFFERSIZE];
	char *p;
	int sock_desc = *(int*)sock_ptr;
	char  server_reply[BUFFERSIZE];
	int send_size;
    system("stty -echo");
	while(fgets(buffer,BUFFERSIZE,stdin)!=NULL)
	        {

				if ((p = strchr(buffer, '\n')) != NULL)	*p = '\0';
			    send_size= send(sock_desc , buffer , BUFFERSIZE , 0);
	            printf("Vos: %s\n",buffer);
	            memset(buffer,0,sizeof(buffer));
	        }
	        if(send_size==0){
	        	printf("Server down.");
	        }
	        if(send_size<0){
	        	printf("Error.");
	        }
	        close(sock_desc);
	        return 0;
}*/


