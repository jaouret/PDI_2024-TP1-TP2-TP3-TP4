# El programa cliente de ejemplo utiliza dos sockets para demostrar cómo el servidor con select() administra múltiples conexiones al mismo tiempo.
# El cliente comienza conectando cada socket TCP/IP al servidor.

import socket
import sys
import time
from datetime import date
from datetime import datetime

mensajes = [
    'Este mensaje ',
    'es enviado ',
    'en partes.',
]
dir_servidor = ('localhost', 10000)

# Creo socket
socks = [
    socket.socket(socket.AF_INET, socket.SOCK_STREAM),
    socket.socket(socket.AF_INET, socket.SOCK_STREAM),
]

# Conectar el socket al puerto en el cual el servidor está escuchando
print('Conectando a {} puerto {}'.format(*dir_servidor), file=sys.stderr)

# Recorre la lista de sockets
times = []
for s in socks:
    s.connect(dir_servidor)
    empiezo = time.time()
    times.append(empiezo)
    print(f'Conexión confirmada: {datetime.now()}', end="\n\n")
   
for mensaje in mensajes:
    datos_salientes = mensaje.encode()
    # envío mensajes en ambos sockets
    for s in socks:
        print('{}: enviando {!r}'.format(s.getsockname(), datos_salientes), file=sys.stderr)
        s.send(datos_salientes)

    # leo respuestas en ambos sockets
    for s in socks:
        data = s.recv(1024)
        print('{}: recibido {!r}'.format(s.getsockname(), data), file=sys.stderr)

i = 0
for s in socks:
    salir = input(f"¿Cerrar conexión de {s.getsockname()}? (Y/N): ")
    if salir == "Y" or "y":
        print('Cerrando socket', s.getsockname(),file=sys.stderr)
        s.close()
        termino = time.time()
        print('Tiempo de conexión: {:.2f} segundos'.format(termino-times[i]))
        i += 1

