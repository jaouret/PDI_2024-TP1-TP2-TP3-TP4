#include<stdio.h>	
#include<stdlib.h>	//malloc
#include<string.h>	//memset
#include<netinet/ip_icmp.h>	//icmp encabezado
#include<netinet/udp.h>	//udp encabezado
#include<netinet/tcp.h>	//tcp encabezado
#include<netinet/ip.h>	//ip encabezado
#include<sys/socket.h>
#include<arpa/inet.h>

void ProcesarPaquete(unsigned char* , int);
void imprimir_ip_encabezado(unsigned char* , int);
void imprimir_tcp_paq(unsigned char* , int);
void imprimir_udp_paq(unsigned char * , int);
void imprimir_icmp_paq(unsigned char* , int);
void ImprimirDatos (unsigned char* , int);

int sock_raw;
FILE *archivo_log;
int tcp=0,udp=0,icmp=0,otros=0,igmp=0,total=0,i,j;
struct sockaddr_in origen,destino;

int main()
{
	int espacio_dir_orig , espacio_datos;
	struct sockaddr dir_orig;
	struct in_addr in;
	
	unsigned char *buffer = (unsigned char *)malloc(65536); //Reservo memoria para el buffer
	
	archivo_log=fopen("log.txt","w");
	if(archivo_log==NULL) printf("No se pudo crear archivo de log	");
	printf("Iniciando...\n");
	//Creo un socket raw para escuchar trafico.
	sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
	if(sock_raw < 0)
	{
		printf("Error al crear\n");
		return 1;
	}
	while(1)
	{
		espacio_dir_orig = sizeof dir_orig;
		//Recibo paquete
		espacio_datos = recvfrom(sock_raw , buffer , 65536 , 0 , &dir_orig , &espacio_dir_orig);
		if(espacio_datos <0 )
		{
			printf("Error en recvfrom, no pue recbir paqutes\n");
			return 1;
		}
		//Proceso el paquete
		ProcesarPaquete(buffer , espacio_datos);
	}
	close(sock_raw);
	printf("Fin");
	return 0;
}

void ProcesarPaquete(unsigned char* buffer, int Tam)
{
	//Traigo encabezado de paquete IP
	struct iphdr *iph = (struct iphdr*)buffer;
	++total;
	switch (iph->protocol) //Verifico el protocolo
	{
		case 1:  //ICMP
			++icmp;
			//imprimir_icmp_paq(buffer, Tam);
			break;
		
		case 2:  //IGMP 
			++igmp;
			break;
		
		case 6:  //TCP 
			++tcp;
			imprimir_tcp_paq(buffer , Tam);
			break;
		
		case 17: //UDP 
			++udp;
			imprimir_udp_paq(buffer , Tam);
			break;
		
		default: //Otros protocolos como ARP etc.
			++otros;
			break;
	}
	printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   otros : %d   Total : %d\r",tcp,udp,icmp,igmp,otros,total);
}

void imprimir_ip_encabezado(unsigned char* Buffer, int Tam)
{
	unsigned short iphdrlen;
		
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen =iph->ihl*4;
	
	memset(&origen, 0, sizeof(origen));
	origen.sin_addr.s_addr = iph->saddr;
	
	memset(&destino, 0, sizeof(destino));
	destino.sin_addr.s_addr = iph->daddr;
	
	fprintf(archivo_log,"\n");
	fprintf(archivo_log,"IP encabezado\n");
	fprintf(archivo_log,"   |-IP Version        : %d\n",(unsigned int)iph->version);
	fprintf(archivo_log,"   |-IP longitud encabezado : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
	fprintf(archivo_log,"   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
	fprintf(archivo_log,"   |-IP longitud total   : %d  Bytes(Tam. paquete)\n",ntohs(iph->tot_len));
	fprintf(archivo_log,"   |-Id  : %d\n",ntohs(iph->id));
	fprintf(archivo_log,"   |-TTL      : %d\n",(unsigned int)iph->ttl);
	fprintf(archivo_log,"   |-Protocolo : %d\n",(unsigned int)iph->protocol);
	fprintf(archivo_log,"   |-Checksum : %d\n",ntohs(iph->check));
	fprintf(archivo_log,"   |-Origen IP        : %s\n",inet_ntoa(origen.sin_addr));
	fprintf(archivo_log,"   |-Destino IP   : %s\n",inet_ntoa(destino.sin_addr));
}

void imprimir_tcp_paq(unsigned char* Buffer, int Tam)
{
	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen = iph->ihl*4;
	
	struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen);
			
	fprintf(archivo_log,"\n\n***********************TCP Packet*************************\n");	
		
	imprimir_ip_encabezado(Buffer,Tam);
		
	fprintf(archivo_log,"\n");
	fprintf(archivo_log,"TCP encabezado\n");
	fprintf(archivo_log,"   |-Origen Puerto     : %u\n",ntohs(tcph->source));
	fprintf(archivo_log,"   |-Destino Puerto: %u\n",ntohs(tcph->dest));
	fprintf(archivo_log,"   |-Numero Secuencia   : %u\n",ntohl(tcph->seq));
	fprintf(archivo_log,"   |-Numero Ack: %u\n",ntohl(tcph->ack_seq));
	fprintf(archivo_log,"   |-Encabezado Longitud      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
	fprintf(archivo_log,"   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
	fprintf(archivo_log,"   |-AckFlag : %d\n",(unsigned int)tcph->ack);
	fprintf(archivo_log,"   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
	fprintf(archivo_log,"   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
	fprintf(archivo_log,"   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
	fprintf(archivo_log,"   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
	fprintf(archivo_log,"   |-Window         : %d\n",ntohs(tcph->window));
	fprintf(archivo_log,"   |-Checksum       : %d\n",ntohs(tcph->check));
	fprintf(archivo_log,"   |-Puntero Urgente: %d\n",tcph->urg_ptr);
	fprintf(archivo_log,"\n");
	fprintf(archivo_log,"                        DATA Dump                         ");
	fprintf(archivo_log,"\n");
		
	fprintf(archivo_log,"IP encabezado\n");
	ImprimirDatos(Buffer,iphdrlen);
		
	fprintf(archivo_log,"TCP encabezado\n");
	ImprimirDatos(Buffer+iphdrlen,tcph->doff*4);
		
	fprintf(archivo_log,"Contenido de Datos en el Paquete\n");	
	ImprimirDatos(Buffer + iphdrlen + tcph->doff*4 , (Tam - tcph->doff*4-iph->ihl*4) );
						
	fprintf(archivo_log,"\n###########################################################");
}

void imprimir_udp_paq(unsigned char *Buffer , int Tam)
{
	
	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen = iph->ihl*4;
	
	struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen);
	
	fprintf(archivo_log,"\n\n***********************UDP Packet*************************\n");
	
	imprimir_ip_encabezado(Buffer,Tam);			
	
	fprintf(archivo_log,"\nUDP encabezado\n");
	fprintf(archivo_log,"   |-origen Puerto     : %d\n" , ntohs(udph->source));
	fprintf(archivo_log,"   |-Destino Puerto: %d\n" , ntohs(udph->dest));
	fprintf(archivo_log,"   |-UDP Longitud       : %d\n" , ntohs(udph->len));
	fprintf(archivo_log,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
	
	fprintf(archivo_log,"\n");
	fprintf(archivo_log,"IP encabezado\n");
	ImprimirDatos(Buffer , iphdrlen);
		
	fprintf(archivo_log,"UDP encabezado\n");
	ImprimirDatos(Buffer+iphdrlen , sizeof udph);
		
	fprintf(archivo_log,"Contenido de Datos en el Paquete\n");	
	ImprimirDatos(Buffer + iphdrlen + sizeof udph ,( Tam - sizeof udph - iph->ihl * 4 ));
	
	fprintf(archivo_log,"\n###########################################################");
}

void imprimir_icmp_paq(unsigned char* Buffer , int Tam)
{
	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen = iph->ihl*4;
	
	struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen);
			
	fprintf(archivo_log,"\n\n***********************ICMP Packet*************************\n");	
	
	imprimir_ip_encabezado(Buffer , Tam);
			
	fprintf(archivo_log,"\n");
		
	fprintf(archivo_log,"ICMP encabezado\n");
	fprintf(archivo_log,"   |-Tipo : %d",(unsigned int)(icmph->type));
			
	if((unsigned int)(icmph->type) == 11) 
		fprintf(archivo_log,"  (TTL Expirado)\n");
	else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY) 
		fprintf(archivo_log,"  (ICMP Echo Reply)\n");
	fprintf(archivo_log,"   |-Code : %d\n",(unsigned int)(icmph->code));
	fprintf(archivo_log,"   |-Checksum : %d\n",ntohs(icmph->checksum));
	fprintf(archivo_log,"\n");

	fprintf(archivo_log,"IP encabezado\n");
	ImprimirDatos(Buffer,iphdrlen);
		
	fprintf(archivo_log,"UDP encabezado\n");
	ImprimirDatos(Buffer + iphdrlen , sizeof icmph);
		
	fprintf(archivo_log,"Contenido de Datos en el Paquete\n");	
	ImprimirDatos(Buffer + iphdrlen + sizeof icmph , (Tam - sizeof icmph - iph->ihl * 4));
	
	fprintf(archivo_log,"\n###########################################################");
}

void ImprimirDatos (unsigned char* data , int Tam)
{
	
	for(i=0 ; i < Tam ; i++)
	{
		if( i!=0 && i%16==0)   //linea hexadcimal...
		{
			fprintf(archivo_log,"         ");
			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
					fprintf(archivo_log,"%c",(unsigned char)data[j]); //caracter numerico o alfabetico
				
				else fprintf(archivo_log,"."); //caso contrario imprimo un punto
			}
			fprintf(archivo_log,"\n");
		} 
		
		if(i%16==0) fprintf(archivo_log,"   ");
			fprintf(archivo_log," %02X",(unsigned int)data[i]);
				
		if( i==Tam-1)  //imprimo los ultimos espacios
		{
			for(j=0;j<15-i%16;j++) fprintf(archivo_log,"   "); //espacios extras
			
			fprintf(archivo_log,"         ");
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) fprintf(archivo_log,"%c",(unsigned char)data[j]);
				else fprintf(archivo_log,".");
			}
			fprintf(archivo_log,"\n");
		}
	}
}