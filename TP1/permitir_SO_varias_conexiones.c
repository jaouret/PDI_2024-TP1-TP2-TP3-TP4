if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror(“Falla en setsockopt");
        exit(FALLA);
    }
