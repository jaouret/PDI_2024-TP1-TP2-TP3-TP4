#include <stdio.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h> // Agregar esta línea

#define BUFFER_SIZE 65536

void procesar_paquete(unsigned char *buffer, int size) {
    struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    unsigned short longitud_encabezado_ip = encabezado_ip->ihl * 4;
    
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));
    source.sin_addr.s_addr = encabezado_ip->saddr;
    dest.sin_addr.s_addr = encabezado_ip->daddr;
    

    if (encabezado_ip->protocol == IPPROTO_TCP) {
        struct tcphdr *encabezado_tcp = (struct tcphdr *)(buffer + longitud_encabezado_ip);
        unsigned int puerto_origen = ntohs(encabezado_tcp->source);
        unsigned int puerto_destino = ntohs(encabezado_tcp->dest);
        
        printf("Paquete TCP - Dirección IP de origen: %s, Puerto de origen: %u, Dirección IP de destino: %s, Puerto de destino: %u\n",
            inet_ntoa(source.sin_addr), puerto_origen, inet_ntoa(dest.sin_addr), puerto_destino);
    } else if (encabezado_ip->protocol == IPPROTO_UDP) {
        struct udphdr *encabezado_udp = (struct udphdr *)(buffer + longitud_encabezado_ip);
        unsigned int puerto_origen = ntohs(encabezado_udp->source);
        unsigned int puerto_destino = ntohs(encabezado_udp->dest);
        
        printf("Paquete UDP - Dirección IP de origen: %s, Puerto de origen: %u, Dirección IP de destino: %s, Puerto de destino: %u\n",
            inet_ntoa(source.sin_addr), puerto_origen, inet_ntoa(dest.sin_addr), puerto_destino);
    } else if (encabezado_ip->protocol == IPPROTO_ICMP) {
        printf("Paquete ICMP - Dirección IP de origen: %s, Dirección IP de destino: %s\n",
            inet_ntoa(source.sin_addr), inet_ntoa(dest.sin_addr));
    } else {
        printf("Paquete de protocolo desconocido\n");
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

    // Configurar para recibir desde localhost (loopback)
    struct sockaddr_ll addr; // Corregir aquí
    socklen_t addr_len = sizeof(addr); // Corregir aquí
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = if_nametoindex("lo");

    // Enlazar el socket a la interfaz loopback
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
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