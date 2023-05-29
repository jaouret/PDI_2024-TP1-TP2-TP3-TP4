#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
   
// Función para el chat entre el cliente y el servidor
void func(int connfd)
{
    char buff[MAX];
    int n;
    // Loop para el chat
    for (;;) {
        bzero(buff, MAX);
   
        // Leo el mesaje del cliente y lo copio en un buffer
        read(connfd, buff, sizeof(buff));
        // Muestro el buffer con los datos del cliente
        printf("Del cliente: %s\t: ", buff);
        bzero(buff, MAX);
        n = 0;
        // Copio el mensaje del servidor en el buffer
        while ((buff[n++] = getchar()) != '\n')
            ;
   
        // y envío el buffer al cliente
        write(connfd, buff, sizeof(buff));
   
        // si el mensaje dice "SALIR" salgo y cierro conexión
        if (strncmp("SALIR", buff, 4) == 0) {
            printf("Salgo del servidor...\n");
            break;
        }
    }
}
   
// Función principal
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Falla la creación del socket...\n");
        exit(0);
    }
    else
        printf("Socket creado...\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // asigno IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // Bind del nuevo socket a la dirección IP y lo verifico
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Falla socket bind ...\n");
        exit(0);
    }
    else
        printf("Se hace el socket bind ..\n");
   
    // El servidor está en modo escucha (pasivo) y lo verifico
    if ((listen(sockfd, 5)) != 0) {
        printf("Falla el listen ...\n");
        exit(0);
    }
    else
        printf("Servidor en modo escucha ...\n");
    len = sizeof(cli);
   
    // Acepto los paquetes del cliente y verifico
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Falla al aceptar datos en el servidor...\n");
        exit(0);
    }
    else
        printf("El servidor acepta al cliente ...\n");
   
    // Funcion para el chat entre el cliente y el servidor
  
    func(connfd);
   
    // Cierro el socket al terminar el chat
    close(sockfd);
}