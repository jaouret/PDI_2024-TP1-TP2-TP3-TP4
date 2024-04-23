#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <arpa/inet.h>
#include <time.h>
#define MAX 80
#define PORT 8080
#define FALLA 0
#define EPROCESO_EXITOSO 1
#define SA struct sockaddr
   
//calculo el factorial
int fact(int a){
    int res = 1;
    while(a>=2){
        res *= a;
        a--;
    }
    return res;
}

// Función para el chat entre el cliente y el servidor
void func(int connfd,int num)
{
    char buff[MAX];
    int n;
    // Loop para el chat
    for (;;) {
        bzero(buff, MAX);
   
        // Leo el mesaje del cliente y lo copio en un buffer
        read(connfd, buff, sizeof(buff));
        // Muestro el buffer con los datos del cliente
        printf("Del cliente[%d]: %s\t: ",num, buff);
        bzero(buff, MAX);
        n = 0;
        // Copio el mensaje del servidor en el buffer
        //while ((buff[n++] = getchar()) != '\n');

        // y envío el buffer al cliente
        //write(connfd, buff, sizeof(buff));
   
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
    int server_sd, nuevo_socket, longdir,leovalor;
    struct sockaddr_in servaddr, direccion;
    char buffer[MAX] = {0},buffer1[MAX],mensaje[MAX] = {0};
    pid_t pid_hijo;
    clock_t start, end;
    double cpu_time_used;
   
    // socket create and verification
    server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sd == -1) {
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
    if ((bind(server_sd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Falla socket bind ...\n");
        exit(0);
    }
    else
        printf("Se hace el socket bind ..\n");
   
    // El servidor está en modo escucha (pasivo) y lo verifico
    if ((listen(server_sd, 5)) != 0) {
        printf("Falla el listen ...\n");
        exit(0);
    }
    else
        printf("Servidor en modo escucha ...\n");
    longdir = sizeof(direccion);
    int num = 0;

    // Bucle principal
    while(1) {
  // Aceptar una nueva conexión de la cola del listen
        num++;
        if ((nuevo_socket = accept(server_sd, (struct sockaddr *)&direccion,(socklen_t*)&longdir))<0) {
            perror("Falla accept");
            exit(FALLA);
        }
        else{
            printf("El servidor acepta al cliente[%d] ...\n",num);
        }
        // Crear un nuevo proceso hijo para manejar la conexión
        int pid = fork();
        if (pid < 0) {
            perror("Falla fork");
            exit(FALLA);
        }
        if (pid == 0) { // Identifico al Proceso hijo
            start = clock();
            close(server_sd); // Cerrar el descriptor de archivo del servidor en el proceso hijo
            leovalor = read(nuevo_socket, buffer, MAX);
            printf("Mensaje del cliente: %s\n", buffer);
            int i,j=0;
            sscanf(buffer, "%d", &i);
            int resultado = fact(i);
            j = sprintf(buffer1, "Factorial: %d\n", resultado);
            j += sprintf(buffer1+j,"Pid hijo: %d\n",pid);
            j += sprintf(buffer1+j,"Pid padre: %d\n",getpid());
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            j += sprintf(buffer1+j,"Tiempo: %f\n",cpu_time_used);
            strcpy(mensaje,buffer1);
            send(nuevo_socket, mensaje, strlen(mensaje), 0);
            printf("Respuesta enviada.\n");
            close(nuevo_socket); // Cerrar el socket en el proceso hijo
            exit(EPROCESO_EXITOSO);
        }else{ // Proceso padre
            close(nuevo_socket);
        } // Cerrar el socket en el proceso padre 
           
    }  // Cierro bucle principal
    // Cierro el socket al terminar el chat
    close(server_sd);
}