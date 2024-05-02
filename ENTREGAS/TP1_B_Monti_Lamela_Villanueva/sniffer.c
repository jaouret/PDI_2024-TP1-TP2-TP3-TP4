#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#define BUFFER_SIZE 65536
#define PORT 3535

int verificar_paquete(unsigned char *buffer){
    //funcion que verifica si el paquete es del cliente o servidor mirando el puerto de origen/destino o si es ICMP
    unsigned int puerto_origen = 0;
    unsigned int puerto_destino = 0;
    struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    if(encabezado_ip->protocol == IPPROTO_TCP){
        struct tcphdr *encabezado_tcp = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + encabezado_ip->ihl * 4);
        puerto_origen = ntohs(encabezado_tcp->source);
        puerto_destino = ntohs(encabezado_tcp->dest);
    }
    if(encabezado_ip->protocol == IPPROTO_UDP){
        struct udphdr *encabezado_udp = (struct udphdr *)(buffer + sizeof(struct ethhdr) + encabezado_ip->ihl * 4);
        puerto_origen = ntohs(encabezado_udp->source);
        puerto_destino = ntohs(encabezado_udp->dest);
    }
    
    if(puerto_origen == PORT || puerto_destino == PORT || encabezado_ip->protocol == IPPROTO_ICMP) return 1;
    
    return 0;
}

void mostrar_ethhdr(unsigned char *buffer){
    struct ethhdr *encabezado_eth = (struct ethhdr *)buffer;
    printf("|*| Encabezado Ethernet\n");
    printf("    - Dirección MAC de origen: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
           encabezado_eth->h_source[0], encabezado_eth->h_source[1], encabezado_eth->h_source[2],
           encabezado_eth->h_source[3], encabezado_eth->h_source[4], encabezado_eth->h_source[5]);
    printf("    - Dirección MAC de destino: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
           encabezado_eth->h_dest[0], encabezado_eth->h_dest[1], encabezado_eth->h_dest[2],
           encabezado_eth->h_dest[3], encabezado_eth->h_dest[4], encabezado_eth->h_dest[5]);
    printf("    - Tipo de protocolo: 0x%.4X\n", ntohs(encabezado_eth->h_proto));
}

void mostrar_iphdr(unsigned char *buffer){
    struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    printf("|*| Encabezado IP\n");
    printf("    - Versión IP: %d\n", encabezado_ip->version);
    printf("    - Longitud del encabezado IP: %d bytes\n", encabezado_ip->ihl * 4);
    printf("    - Tipo de servicio: %d\n", encabezado_ip->tos);
    printf("    - Longitud total: %d bytes\n", ntohs(encabezado_ip->tot_len));
    printf("    - Identificación: %d\n", ntohs(encabezado_ip->id));
    printf("    - Fragmentación: Flags: %d, Offset: %d\n", (encabezado_ip->frag_off & 0x1FFF), (encabezado_ip->frag_off & 0xE000) >> 13);
    printf("    - Tiempo de vida: %d\n", encabezado_ip->ttl);
    printf("    - Protocolo: %d\n", (unsigned int)encabezado_ip->protocol);
    printf("    - Suma de control: 0x%.4X\n", ntohs(encabezado_ip->check));
    printf("    - Dirección IP de origen: %s\n", inet_ntoa(*(struct in_addr *)&encabezado_ip->saddr));
    printf("    - Dirección IP de destino: %s\n", inet_ntoa(*(struct in_addr *)&encabezado_ip->daddr));
}

void verificar_transporthdr(unsigned char *buffer){
    // funcion para verificar si el segmento es TCP o UDP y en caso de serlo mostrar el hdr
    struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    // Encabezado TCP
    if(encabezado_ip->protocol == IPPROTO_TCP) {
        struct tcphdr *encabezado_tcp = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + encabezado_ip->ihl * 4);
        printf("|*| Encabezado TCP\n");
        printf("    - Puerto de origen: %d\n", ntohs(encabezado_tcp->source));
        printf("    - Puerto de destino: %d\n", ntohs(encabezado_tcp->dest));
        printf("    - Número de secuencia: %u\n", ntohl(encabezado_tcp->seq));
        printf("    - Número de acuse de recibo: %u\n", ntohl(encabezado_tcp->ack_seq));
        printf("    - Longitud de encabezado TCP: %d bytes\n", encabezado_tcp->doff * 4);
        printf("    - Flags: ");
        if (encabezado_tcp->syn) printf("SYN");
        if (encabezado_tcp->ack) printf("ACK");
        if (encabezado_tcp->fin) printf("FIN");
        if (encabezado_tcp->rst) printf("RST");
        if (encabezado_tcp->psh) printf("PSH");
        if (encabezado_tcp->urg) printf("URG");
        printf("\n");
        printf("    Tamaño de ventana: %d\n", ntohs(encabezado_tcp->window));
        printf("    Suma de control: 0x%.4X\n", ntohs(encabezado_tcp->check));
        printf("    Puntero de urgencia: %d\n", ntohs(encabezado_tcp->urg_ptr));
    }

    // Encabezado UDP
    if(encabezado_ip->protocol == IPPROTO_UDP) {
        struct udphdr *encabezado_udp = (struct udphdr *)(buffer + sizeof(struct ethhdr) + encabezado_ip->ihl * 4);
        printf("|*| Encabezado UDP\n");
        printf("    - Puerto de origen: %d\n", ntohs(encabezado_udp->source));
        printf("    - Puerto de destino: %d\n", ntohs(encabezado_udp->dest));
        printf("    - Longitud total: %d bytes\n", ntohs(encabezado_udp->len));
        printf("    - Suma de control: 0x%.4X\n", ntohs(encabezado_udp->check));
    }
}

void mostrar_datos(unsigned char *buffer, int size){
    // funcion que muestra el payload de un segmento/paquete
    struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    printf("|*| Datos:\n");
    int i;
    //printf("\t")
    for(i = sizeof(struct ethhdr) + encabezado_ip->ihl * 4; i < size; ++i) {
        printf("%.2X ", buffer[i]);
        if((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

void verificar_icmp(unsigned char *buffer, int size){
    // funcion para verificar si el paquete es ICMP y en caso de serlo mostrar el hdr
    struct iphdr *encabezado_ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    if(encabezado_ip->protocol == IPPROTO_ICMP){
        struct icmphdr *icmph = (struct icmphdr *)(buffer + sizeof(struct ethhdr) + encabezado_ip->ihl * 4);
        printf("|*| Encabezado ICMP\n");
        printf("    - Tipo: %d\n",(unsigned int)(icmph->type));
        printf("    - Code: %d\n",(unsigned int)(icmph->code));
        printf("    - Checksum: %d\n",ntohs(icmph->checksum));
    }
}

void procesar_paquete(unsigned char *buffer, int size) {
    printf("****************************************************************\n");
    printf("Paquete recibido - Longitud: %d bytes\n", size);
    mostrar_ethhdr(buffer);
    mostrar_iphdr(buffer);
    verificar_transporthdr(buffer);
    verificar_icmp(buffer, size);
    mostrar_datos(buffer, size);
}

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];
    
    // Crear un socket raw
    //htons(ETH_P_ALL)
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("{*} Socket creado...\n");
    
    // Recibir paquetes 
    while (1) {
        int bytes_recibidos = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytes_recibidos < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        
        if(verificar_paquete(buffer)) procesar_paquete(buffer, bytes_recibidos);
    }

    return 0;
}