// EXIT_FAILURE = 1
// EXIT_SUCCESS = 0
// (valores standard según man de gcc)
// Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Crear múltiples conexiones utilizando fork()
    for (int i = 0; i < NUM_CONNECTIONS; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Proceso hijo
            printf("Proceso hijo %d creando conexión...\n", getpid());

            // Enviar mensaje al servidor
            sprintf(buffer, "Mensaje desde el cliente %d\n", getpid());
            send(sockfd, buffer, strlen(buffer), 0);
            printf("Proceso hijo %d: Mensaje enviado al servidor\n", getpid());

            // Recibir respuesta del servidor
            int bytes_recibidos = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (bytes_recibidos > 0) {
                buffer[bytes_recibidos] = '\0';
                printf("Proceso hijo %d: Respuesta del servidor: %s\n", getpid(), buffer);
            } else if (bytes_recibidos == 0) {
                printf("Proceso hijo %d: Conexión cerrada por el servidor\n", getpid());
            } else {
                perror("recv");
            }

            // Cerrar el socket y terminar el proceso hijo
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < NUM_CONNECTIONS; ++i) {
        wait(NULL);
    }

    // Cerrar el socket del padre
    close(sockfd);
    return 0;


