#!/usr/bin/python
import socket
import sys
import time
from datetime import date
from datetime import datetime

# Creando un socket TCP/IP
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conecta el socket en el puerto cuando el servidor esté escuchando
server_dir = ('localhost', 6667)
print ('conectando a %s puerto %s',server_dir)
sock.connect(server_dir)
print(f'Conexión confirmada: {datetime.now()}', end="\n\n")
empiezo = time.time()

contador = 0
while contador <= 5:
    # Enviando datos
    print("Paso:", contador)
    mensaje = input("Mensaje: ");
    # print('enviando "%rb"', mensaje)
    print(f"Enviado: {mensaje}")
    # sock.sendall(mensaje.encode('utf-8'))
    sock.sendall(mensaje.encode('utf-8'))
    

    # Buscando respuesta
    bytes_recibidos = 0
    bytes_esperados = len(mensaje)

    while bytes_recibidos < bytes_esperados:
        data = sock.recv(100)
        bytes_recibidos += len(data)
        print('recibiendo "%s"', data.decode('utf-8'))

    contador = contador + 1


salir = input(f"¿Cerrar conexión de {sock.getsockname()}? (Y/N): ")
if salir == "Y" or "y":
    print('Cerrando socket', sock.getsockname(),file=sys.stderr)
    sock.close()
    termino = time.time()
    print('Tiempo total de conexión: {:.2f} segundos'.format(termino-empiezo))
    exit
