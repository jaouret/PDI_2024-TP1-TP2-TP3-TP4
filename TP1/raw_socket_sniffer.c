/*
Protocolos de Internet- Javier Ouret 
RAW SOCKETS VERSION SIMPLIFICADA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 65536

void procesar_paquete(unsigned char *buffer, int size) {
    struct iphdr *encabezado_ip = (struct iphdr *)buffer;
    unsigned short longitud_encabezado_ip = encabezado_ip->ihl * 4;
    
    if (encabezado_ip->protocol == IPPROTO_TCP) {
        struct tcphdr *encabezado_tcp = (struct tcphdr *)(buffer + longitud_encabezado_ip);
        unsigned int puerto_origen = ntohs(encabezado_tcp->source);
        unsigned int puerto_destino = ntohs(encabezado_tcp->dest);
        
        printf("Paquete TCP - Puerto de origen: %u, Puerto de destino: %u\n", puerto_origen, puerto_destino);
    } else if (encabezado_ip->protocol == IPPROTO_UDP) {
        struct udphdr *encabezado_udp = (struct udphdr *)(buffer + longitud_encabezado_ip);
        unsigned int puerto_origen = ntohs(encabezado_udp->source);
        unsigned int puerto_destino = ntohs(encabezado_udp->dest);
        
        printf("Paquete UDP - Puerto de origen: %u, Puerto de destino: %u\n", puerto_origen, puerto_destino);
    } else if (encabezado_ip->protocol == IPPROTO_ICMP) {
        printf("Paquete ICMP\n");
    } else {
        printf("Paquete de protocolo desconocido\n");
    }
}

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];
    
    // Crear socket raw
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Recibir paquetes 
    while (1) {
        int bytes_recibidos = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytes_recibidos < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        
        procesar_paquete(buffer, bytes_recibidos);
    }

    return 0;
}
