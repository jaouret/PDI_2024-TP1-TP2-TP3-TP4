#!/usr/bin/python
import socket
import sys

# Creando un socket TCP/IP
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#addr = ('0.0.0.0', 0)
#sock.bind(addr) si fuese en servidor

# Muestro tupla antes de la asignación de IP y puerto
print('Datos de la conexión antes de completar datos en estructura:', sock)


# Conecta el socket en el puerto cuando el servidor esté escuchando
server_dir = ('localhost', 6667)
print ('conectando a %s puerto %s',server_dir)
sock.connect(server_dir)
# Muestro tupla luego de la asignación de IP y puerto
print('Datos de la conexión luego de completar datos en estructura', sock)
# Datos de la tupla

hostname = socket.gethostname()
ip_address = socket.gethostbyname(hostname)
print(f"Host: {hostname}")
print(f"IP: {ip_address}")

contador = 0
while contador <= 5:
    # Enviando datos
    print("Paso:", contador)
    datos = sock.recv(10)
    print(datos.decode('utf-8'))
    mensaje = b'123456789012345678901234567890'
    print('enviando "%rb"', mensaje)
    sock.sendall(mensaje)

    # Buscando respuesta
    bytes_recibidos = 0
    bytes_esperados = len(mensaje)

    while bytes_recibidos < bytes_esperados:
        data = sock.recv(10)
        bytes_recibidos += len(data)
        print('recibiendo "%s"', data)

    contador = contador + 1
    print ("Paso:", contador)

print ('cerrando socket')
sock.close()
