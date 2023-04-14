/*
Protocolos de Internet- Javier Ouret 
RAW SOCKETS 
*/

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<signal.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<sys/types.h>
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

FILE* log_txt;
int total,tcp,udp,icmp,igmp,otros,iphdrlen;

struct sockaddr saddr;
struct sockaddr_in origen,destino;

/* Encabezado etehernet */
void encabezado_ethernet(unsigned char* buffer,int buflen)
{
	struct ethhdr *eth = (struct ethhdr *)(buffer);
	fprintf(log_txt,"\n Encabezado Ethernet\n");
	fprintf(log_txt,"\t|-Dirección origen	: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
	fprintf(log_txt,"\t|-Dirección destino	: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
	fprintf(log_txt,"\t|-Protocolo		: %d\n",eth->h_proto);

}

// Función para construir un encabezado (header) IP
void encabezado_ip(unsigned char* buffer,int buflen)
{
	struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));

	iphdrlen =ip->ihl*4;

	memset(&origen, 0, sizeof(origen));
	origen.sin_addr.s_addr = ip->saddr;     
	memset(&destino, 0, sizeof(destino));
	destino.sin_addr.s_addr = ip->daddr;     

	fprintf(log_txt , "\nIP Encabezado\n");

	fprintf(log_txt , "\t|-Version              : %d\n",(unsigned int)ip->version);
	fprintf(log_txt , "\t|-Encabezado Internet  Longitud  : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
	fprintf(log_txt , "\t|-ToS   : %d\n",(unsigned int)ip->tos);
	fprintf(log_txt , "\t|-Longitud total      : %d  Bytes\n",ntohs(ip->tot_len));
	fprintf(log_txt , "\t|-Id    : %d\n",ntohs(ip->id));
	fprintf(log_txt , "\t|-Time To Live	    : %d\n",(unsigned int)ip->ttl);
	fprintf(log_txt , "\t|-Protocolo 	    : %d\n",(unsigned int)ip->protocol);
	fprintf(log_txt , "\t|-Encabezado Checksum   : %d\n",ntohs(ip->check));
	fprintf(log_txt , "\t|-Origen IP         : %s\n", inet_ntoa(origen.sin_addr));
	fprintf(log_txt , "\t|-Destino IP    : %s\n",inet_ntoa(destino.sin_addr));
}

// Función para mostrar datos (datos_transportados)
void datos_transportados(unsigned char* buffer,int buflen)
{
	int i=0;
	unsigned char * data = (buffer + iphdrlen  + sizeof(struct ethhdr) + sizeof(struct udphdr));
	fprintf(log_txt,"\nDatos\n");
	int remaining_data = buflen - (iphdrlen  + sizeof(struct ethhdr) + sizeof(struct udphdr));
	for(i=0;i<remaining_data;i++)
	{
		if(i!=0 && i%16==0)
			fprintf(log_txt,"\n");
		fprintf(log_txt," %.2X ",data[i]);
	}

	fprintf(log_txt,"\n");
}

// Función para construir un encabezado (header) TCP
void encabezado_tcp(unsigned char* buffer,int buflen)
{
	fprintf(log_txt,"\n*************************Paquete TCP ******************************");
   	encabezado_ethernet(buffer,buflen);
  	encabezado_ip(buffer,buflen);

   	struct tcphdr *tcp = (struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
   	fprintf(log_txt , "\nTCP Encabezado\n");
   	fprintf(log_txt , "\t|-origen Puerto          : %u\n",ntohs(tcp->source));
   	fprintf(log_txt , "\t|-Destino Puerto     : %u\n",ntohs(tcp->dest));
   	fprintf(log_txt , "\t|-Número Secuencia     : %u\n",ntohl(tcp->seq));
   	fprintf(log_txt , "\t|-Ack   : %u\n",ntohl(tcp->ack_seq));
   	fprintf(log_txt , "\t|-Longitud Encabezado        : %d DWORDS or %d BYTES\n" ,(unsigned int)tcp->doff,(unsigned int)tcp->doff*4);
	fprintf(log_txt , "\t|----------Flags-----------\n");
	fprintf(log_txt , "\t\t|-Urgent Flag          : %d\n",(unsigned int)tcp->urg);
	fprintf(log_txt , "\t\t|-Acknowledgement Flag : %d\n",(unsigned int)tcp->ack);
	fprintf(log_txt , "\t\t|-Push Flag            : %d\n",(unsigned int)tcp->psh);
	fprintf(log_txt , "\t\t|-Reset Flag           : %d\n",(unsigned int)tcp->rst);
	fprintf(log_txt , "\t\t|-Synchronise Flag     : %d\n",(unsigned int)tcp->syn);
	fprintf(log_txt , "\t\t|-Finish Flag          : %d\n",(unsigned int)tcp->fin);
	fprintf(log_txt , "\t|-Tamaño Ventana          : %d\n",ntohs(tcp->window));
	fprintf(log_txt , "\t|-Checksum             : %d\n",ntohs(tcp->check));
	fprintf(log_txt , "\t|-Puntero urgente       : %d\n",tcp->urg_ptr);

	datos_transportados(buffer,buflen);

fprintf(log_txt,"*****************************************************************\n\n\n");
}

// Función para construir un encabezado (header) UDP
void encabezado_udp(unsigned char* buffer, int buflen)
{
	fprintf(log_txt,"\n*************************UDP Packet******************************");
	encabezado_ethernet(buffer,buflen);
	encabezado_ip(buffer,buflen);
	fprintf(log_txt,"\nUDP Encabezado\n");

	struct udphdr *udp = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	fprintf(log_txt , "\t|-Puerto origen     	: %d\n" , ntohs(udp->source));
	fprintf(log_txt , "\t|-Puerto Destino	: %d\n" , ntohs(udp->dest));
	fprintf(log_txt , "\t|-Longitud UDP    	: %d\n" , ntohs(udp->len));
	fprintf(log_txt , "\t|-UDP Checksum   	: %d\n" , ntohs(udp->check));

	datos_transportados(buffer,buflen);

	fprintf(log_txt,"*****************************************************************\n\n\n");
}

void proceso_datos(unsigned char* buffer,int buflen)
{
	struct iphdr *ip = (struct iphdr*)(buffer + sizeof (struct ethhdr));
	++total;
	// Protocolo UDP
	// Ver /etc/protocols
	switch (ip->protocol)   
	{

		case 6:
			++tcp;
			encabezado_tcp(buffer,buflen);
			break;

		case 17:
			++udp;
			encabezado_udp(buffer,buflen);
			break;

		default:
			++otros;

	}
	printf("TCP: %d  UDP: %d  Otros: %d  Total: %d  \r",tcp,udp,otros,total);

}


int main()
{

	int sock_r,saddr_len,buflen;

	unsigned char* buffer = (unsigned char *)malloc(65536); 
	memset(buffer,0,65536);

	log_txt=fopen("log.txt","w");
	if(!log_txt)
	{
		printf("no se pudo abrir log.txt\n");
		return -1;

	}

	printf("Iniciando .... \n");

	sock_r=socket(AF_INET,SOCK_RAW,IPPROTO_TCP); 
	if(sock_r<0)
	{
		printf("error en socket %d\n", sock_r);
		return -1;
	}

	while(1)
	{
		saddr_len=sizeof saddr;
		buflen=recvfrom(sock_r,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);

		if(buflen<0)
		{
			printf("error leyendo recvfrom \n");
			return -1;
		}
		fflush(log_txt);
		proceso_datos(buffer,buflen);

	}

	// usar signal para cerrar socket
	close(sock_r);

	printf("FIN...\n");

}
