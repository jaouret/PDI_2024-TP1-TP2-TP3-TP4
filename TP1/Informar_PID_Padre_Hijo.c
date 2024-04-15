// Bucle principal
 while(1) {
  // Aceptar una nueva conexión de la cola del listen
  if ((nuevo_socket = accept(server_sd, (struct sockaddr *)&direccion,(socklen_t*)&longdir))<0) {
            perror(“Falla accept");
            exit(FALLA);
        }
        // Crear un nuevo proceso hijo para manejar la conexión
        int pid = fork();
        if (pid < 0) {
                    perror(“Falla fork");
                    exit(FALLA);
        }
        if (pid == 0) { // Identifico al Proceso hijo
                    close(server_sd); // Cerrar el descriptor de archivo del servidor en el proceso hijo
                    leovalor = read(nuevo_socket, buffer, BUFFER_SIZE);
                    printf("Mensaje del cliente: %s\n", buffer);
                    send(nuevo_socket, mensaje, strlen(mensaje), 0);
                    printf("Respuesta enviada.\n");
                    close(nuevo_socket); // Cerrar el socket en el proceso hijo
                    exit(EPROCESO_EXITOSO);
        } else { // Proceso padre
                    close(nuevo_socket);} // Cerrar el socket en el proceso padre 
           
    }
  }  // Cierro bucle principal
        
      
