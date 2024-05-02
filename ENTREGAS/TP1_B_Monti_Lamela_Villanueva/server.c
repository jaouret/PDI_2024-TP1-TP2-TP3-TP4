#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MY_PORT 3535
#define BACKLOG 10
#define MAX_DATA_SIZE 1024

long long int factorial(long long int n);
double medir_tiempo_de_factorial(long long int num, long long int *result);

int main(){

    int sockfd, new_fd, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    int sin_size;
    char buff[MAX_DATA_SIZE] = {0};
    int yes = 1;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MY_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size))==-1){
            perror("accept");
            continue;
        }
        printf("PID: %d | Server: got connection from |%s|\n", getpid(), inet_ntoa(client_addr.sin_addr));

        if(!fork()){
            if((numbytes = recv(new_fd, buff, MAX_DATA_SIZE, 0)) == -1){
                perror("recv");
                exit(1);
            }

            buff[numbytes] = '\0';
            
            //recibo del client el numero del cual debo calcular su factorial
            printf("PID: %d | Received: %s\n", getpid(), buff);
            long long int result;
            int num = atoi(buff); 
            double tiempo = medir_tiempo_de_factorial(num, &result);
            printf("%d! = %lld\n", num, result);
            printf("PID: %d | Tiempo de ejecucion: %.6f milisegundos\n", getpid(),tiempo);
            
            //envio el resultado del factorial al cliente
            bzero(buff, MAX_DATA_SIZE);
            sprintf(buff, "%lld", result);
            send(new_fd, buff, sizeof(buff), 0);

            //envio cuanto tiempo me tomo calcularlo
            bzero(buff, MAX_DATA_SIZE);
            sprintf(buff, "%f", tiempo);
            send(new_fd, buff, sizeof(buff), 0);

            close(new_fd);
            exit(0);
        }
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }
    close(sockfd);
    return 0;
}

long long int factorial(long long int n){
    if (n<=1) return 1;
    return n * factorial(n-1);
}

double medir_tiempo_de_factorial(long long int num, long long int *result) {
    clock_t inicio, fin;
    double tiempo;

    inicio = clock(); // Marca de tiempo inicial

    *result = factorial(num); // Llamada a la función cuyo tiempo de ejecución queremos medir

    fin = clock(); // Marca de tiempo final

    // Cálculo del tiempo transcurrido en milisegundos
    tiempo = (((double)(fin - inicio)) / CLOCKS_PER_SEC) * 1000; //1000ms = 1seg

    return tiempo;
}

