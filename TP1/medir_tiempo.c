double tiempo_transcurrido(struct timespec *inicio, struct timespec *fin) {
	return (fin->tv_sec - inicio->tv_sec) + (fin->tv_nsec - inicio->tv_nsec) ; }

clock_gettime(CLOCK_MONOTONIC, &start);
// Leer mensaje del cliente
leovalor = read(nuevo_socket, buffer, BUFFER_SIZE);
printf("Mensaje del cliente: %s\n", buffer);
// Enviar respuesta al cliente
send(nuevo_socket, mensaje, strlen(mensaje), 0);
printf("Respuesta enviada.\n");
clock_gettime(CLOCK_MONOTONIC, &end);
tiempo_t = tiempo_transcurrido(&start, &end);
// Enviar tiempo de respuesta al cliente
send(nuevo_socket, (void*)&tiempo_t, sizeof(tiempo_t), 0);
close(nuevo_socket);

// Enviar mensaje al servidor
send(socket, mensaje, strlen(mensaje), 0);
printf("Mensaje enviado al servidor.\n");

// Recibir respuesta del servidor
leovalor = read(socket, buffer, BUFFER_SIZE);
printf("Respuesta del servidor: %s\n", buffer);

// Recibir tiempo de respuesta del servidor
leovalor = read(socket, (void*)&tiempo_t, sizeof(tiempo_t));
printf("Tiempo de respuesta del servidor: %f segundos\n", tiempo_t);
