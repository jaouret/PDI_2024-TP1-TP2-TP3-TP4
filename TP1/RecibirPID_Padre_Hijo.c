// API Necesarias (antes les decíamos preprocesamiento con includes)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Elegimos nro de Puerto y tamaño del bloque de información que vamos a mover entre el S y el C.
#define PUERTO // #de puerto del servidor
#define BUFFER_SIZE 1024

// Estructuras y variables
int main(int argc, char const *argv[]) {
  struct sockaddr_in direccion;
  int cliente_sd = 0, leovalor;
  struct sockaddr_in serv_dir;
  char *mensaje = “Mensaje desde el cliente";
  char buffer[BUFFER_SIZE] = {0};
  
  // Creo el socket de flujo
  if ((cliente_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Error : No se pudo crear el socket \n");
    return -1;
  }
  memset(&serv_dir, '0', sizeof(serv_dir));
  serv_dir.sin_family = AF_INET;
  serv_dir.sin_port = htons(PUERTO);
  // Conversión de la dirección IP de IPv4 en formato de cadena a binario
  if (inet_pton(AF_INET, "127.0.0.1", &serv_dir.sin_addr) <= 0) {
      printf("\nDirección Inválidad\n");
      return -1;
  }
  // Conectarse al servidor
  if (connect(cliente_sd, (struct sockaddr *)&serv_dir, sizeof(serv_dir)) < 0) {
     printf("\nFalla en la conexión \n");
     return -1;
  }
  // Enviar mensaje al servidor
  send(cliente_sd, mensaje, strlen(mensaje), 0);
  printf("Mensaje enviado al servidor.\n");
  leovalor = read(cliente_sd, buffer, BUFFER_SIZE);
  printf("Respuesta del servidor: %s\n", buffer);
  return 0;
}
