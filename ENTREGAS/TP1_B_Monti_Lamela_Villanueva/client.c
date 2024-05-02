#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3535
#define MAX_DATA_SIZE 1024

//netstat -ano | findstr :3535
//netsh interface portproxy add v4tov4 listenport=3535 listenaddress=0.0.0.0 connectport=3535 connectaddress= 192.168.160.103
//netsh interface portproxy delete v4tov4 listenport=3535 listenaddress=0.0.0.0
//netsh interface portproxy show all

void recibir_bytes(int sockfd, char *buff);

int main(int argc, char *argv[]){
    int sockfd;
    char buff[MAX_DATA_SIZE];
    struct sockaddr_in server_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.0");
    bzero(&(server_addr.sin_zero), 8);

    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
        perror("connect");
        exit(1);
    }

    int n=0;
    char c;
    printf("Ingrese el numero del cual quiere calcular su factorial (como maximo 20): ");
    while((c = getchar())!='\n'){
        buff[n] = c;
        n++;
    }
    buff[n] = '\0';

    //envio al server el numero del cual quiero calcular su factorial
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));

    //recibo el calculo del factorial
    recibir_bytes(sockfd, buff);
    printf("El resultado es: %s\n", buff);
    bzero(buff, sizeof(buff));

    //recibo cuanto tiempo le tomo al server hacer el calculo
    recibir_bytes(sockfd, buff);
    printf("El tiempo transcurrido es %s milisegundos\n", buff);
    bzero(buff, sizeof(buff));

    close(sockfd);

    return 0;
}

void recibir_bytes(int sockfd, char *buff){
    int numbytes;
    if((numbytes = recv(sockfd, buff, MAX_DATA_SIZE, 0)) == -1){
        perror("recv");
        exit(1);
    }
    buff[numbytes] = '\0';
}