import socket
import threading
import time
import random

def client_process(n_process):
    # Creando un socket TCP/IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Conecta el socket en el puerto cuando el servidor esté escuchando
    server_dir = ('localhost', 3535)
    print(f'[{n_process}] Conectando a {server_dir[0]}:{server_dir[1]} ...')
    sock.connect(server_dir)

    print(f"[{n_process}] Dia y hora de conexion recibida del server: {sock.recv(1024).decode()}")

    random_time = random.uniform(0, 10)
    print(f"[{n_process}] Sleeping process for {random_time:.3f} seconds ...")
    time.sleep(random_time)
    print(f"[{n_process}] termino el sleep")
    sock.sendall("quit".encode())
    print(f"[{n_process}] envio quit")

    #print(f"[{n_process}] La conexion ha durado {recibirDatos(sock).decode()} segundos")
    print(f"[{n_process}] La conexion ha durado {sock.recv(1024).decode()} segundos")

    print(f'[{n_process}] Cerrando socket ...')
    sock.close()


def main():

    #print(f"Sleeping process [{1}] for {random.uniform(0, 10):.3f} seconds.")

    for i in range(5):
        client_thread = threading.Thread(target=client_process, args=(i+1,))
        client_thread.start()

if __name__ == "__main__":
    main()