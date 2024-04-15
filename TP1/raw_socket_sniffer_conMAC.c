#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/ethernet.h>

#define BUFFER_SIZE 65536

void procesar_paquete(unsigned char *buffer, int size) {
    struct ethhdr *encabezado_eth = (struct ethhdr *)buffer;

    // Verificar si el paquete es Ethernet
    if (ntohs(encabezado_eth->h_proto) == ETH_P_IP) {
        struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        unsigned short longitud_encabezado_ip = encabezado_ip->ihl * 4;

        printf("Paquete IP - Longitud: %d bytes\n", size);

        struct in_addr ip_origen, ip_destino;
        ip_origen.s_addr = encabezado_ip->saddr;
        ip_destino.s_addr = encabezado_ip->daddr;

        printf("  Dirección IP de origen: %s\n", inet_ntoa(ip_origen));
        printf("  Dirección IP de destino: %s\n", inet_ntoa(ip_destino));

        // Mostrar direcciones MAC de origen y destino
        printf("  Dirección MAC de origen: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
               encabezado_eth->h_source[0], encabezado_eth->h_source[1], encabezado_eth->h_source[2],
               encabezado_eth->h_source[3], encabezado_eth->h_source[4], encabezado_eth->h_source[5]);

        printf("  Dirección MAC de destino: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
               encabezado_eth->h_dest[0], encabezado_eth->h_dest[1], encabezado_eth->h_dest[2],
               encabezado_eth->h_dest[3], encabezado_eth->h_dest[4], encabezado_eth->h_dest[5]);

        // Desmenuzar el protocolo
        if (encabezado_ip->protocol == IPPROTO_TCP) {
            struct tcphdr *encabezado_tcp = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + longitud_encabezado_ip);
            printf("  Protocolo: TCP\n");
            printf("  Puerto de origen: %u\n", ntohs(encabezado_tcp->source));
            printf("  Puerto de destino: %u\n", ntohs(encabezado_tcp->dest));
        } else if (encabezado_ip->protocol == IPPROTO_UDP) {
            struct udphdr *encabezado_udp = (struct udphdr *)(buffer + sizeof(struct ethhdr) + longitud_encabezado_ip);
            printf("  Protocolo: UDP\n");
            printf("  Puerto de origen: %u\n", ntohs(encabezado_udp->source));
            printf("  Puerto de destino: %u\n", ntohs(encabezado_udp->dest));
        } else if (encabezado_ip->protocol == IPPROTO_ICMP) {
            printf("  Protocolo: ICMP\n");
        } else {
            printf("  Protocolo desconocido\n");
        }
    } else {
        printf("Paquete no Ethernet\n");
    }
}

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];
    
    // Crear un socket raw
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Recibir paquetes 
    while (1) {
        int bytes_recibidos = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_recibidos < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        
        procesar_paquete(buffer, bytes_recibidos);
    }

    return 0;
}

