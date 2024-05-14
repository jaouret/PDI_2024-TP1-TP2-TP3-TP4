import socket
import sys

def main():
    # Creando un socket TCP/IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Conecta el socket en el puerto cuando el servidor esté escuchando
    server_dir = ('localhost', 3535)
    print(f'Conectando a {server_dir[0]}:{server_dir[1]} ...')
    sock.connect(server_dir)

    print(f"Hora de conexion recibida del server: {sock.recv(1024).decode()}")

    print('Cuando quiera cerrar la conexion escriba "quit": ');
    while((salida := input()) != "quit"):
        pass

    sock.sendall(salida.encode())

    print(f"La conexion ha durado {sock.recv(1024).decode()} segundos")

    print('Cerrando socket ...')
    sock.close()

if __name__ == "__main__":
    main()
