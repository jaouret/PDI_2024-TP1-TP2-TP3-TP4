#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <sys/wait.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define BUFFER_SIZE 1024
#define NUM_CONNECTIONS 5
void func(int sockfd,struct sockaddr_in cli)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Ingrese texto : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Servidor : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Cliente cierra conexión...\n");
            break;
        }
    }
}
 
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char buffer[MAX] = {0};
 
    // socket: creo socket y lo verifico
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Falla la creación del socket...\n");
        exit(0);
    }
    else
        printf("Socket creado ..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // asigno IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // Conecto los sockets entre cliente y servidor
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("Falla de conexión con servidor...\n");
        exit(0);
    }
    else{
        printf("Conectado al servidor..\n");
        printf("Cliente conectado desde: %s:%d\n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    }
    // Crear múltiples conexiones utilizando fork()
    for (int i = 0; i < NUM_CONNECTIONS; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Proceso hijo
            printf("Proceso hijo %d creando conexión...\n", getpid());

            // Enviar mensaje al servidor
            sprintf(buffer, "Mensaje desde el cliente %d\n", getpid());
            send(sockfd, buffer, strlen(buffer), 0);
            printf("Proceso hijo %d: Mensaje enviado al servidor\n", getpid());

            // Recibir respuesta del servidor
            int bytes_recibidos = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (bytes_recibidos > 0) {
                buffer[bytes_recibidos] = '\0';
                printf("Proceso hijo %d: Respuesta del servidor: %s\n", getpid(), buffer);
            } else if (bytes_recibidos == 0) {
                printf("Proceso hijo %d: Conexión cerrada por el servidor\n", getpid());
            } else {
                perror("recv");
            }

            // Cerrar el socket y terminar el proceso hijo
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < NUM_CONNECTIONS; ++i) {
        wait(NULL);
    }
    // Función para el chat
    /*for(int i = 0;i < 10;i++){
        func(sockfd,cli);
    }*/
 
    //Cierro el socket
    close(sockfd);
}