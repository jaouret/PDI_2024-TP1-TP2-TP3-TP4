# Esta versión «lenta» del programa cliente se detiene después de enviar cada mensaje,
# para simular la latencia u otro retraso en la transmisión.

import socket
import sys
import time

# Crear un socket TCP/IP
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conectar el socket al puerto en el cual el servidor está escuchando
dir_servidor = ('localhost', 10000)
print('conectando a {} puerto {}'.format(*dir_servidor),
      file=sys.stderr)
sock.connect(dir_servidor)

time.sleep(1)

mensajes = [
    'Parte 1 del mensaje',
    'Parte 2 del mensaje',
]
datos_esperados = len(''.join(mensajes))

try:

    # enviando datos
    for mensaje in mensajes:
        data = mensaje.encode()
        print('enviando {!r}'.format(data), file=sys.stderr)
        sock.sendall(data)
        time.sleep(2.5)

    # Respuesta
    datos_recibidos = 0

    while datos_recibidos < datos_esperados:
        data = sock.recv(16)
        datos_recibidos += len(data)
        print('recibidos {!r}'.format(data), file=sys.stderr)

finally:
    print('cerrando socket', file=sys.stderr)
    sock.close()