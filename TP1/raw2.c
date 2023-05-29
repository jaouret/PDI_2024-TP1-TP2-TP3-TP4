#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

int main() {
    int fd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    // Los paquetes unitarios no suelen ser más grandes que 8192 bytes
    char buffer[8192]; 
    for (;;) {
        printf("Paso \n");
        while (read (fd, buffer, 8192) > 0) 
            printf ("Paquete TCP: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct tcphdr));
    }
}