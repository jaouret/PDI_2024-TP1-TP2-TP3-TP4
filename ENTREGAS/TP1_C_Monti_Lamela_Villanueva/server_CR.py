import socket
import threading
import datetime

def handle_client(client_socket):
    fecha_hora_actual = datetime.datetime.now()
    fecha_hora_formateada = fecha_hora_actual.strftime("%d-%m-%Y %H:%M:%S")

    client_socket.sendall(fecha_hora_formateada.encode())

    request = client_socket.recv(1024).decode() #recibo solicitud para desconectar
    if(request == "quit"):
        print('Se recibio "quit"')
        duracion_conexion_seg = "{:.4f}".format((datetime.datetime.now() - fecha_hora_actual).total_seconds())
        client_socket.sendall(duracion_conexion_seg.encode())
        print("Se envio duracion de conexion")

    client_socket.close()

def main():
    # Configuración del servidor
    server_host = '127.0.0.1'
    server_port = 3535

    # Crear un socket TCP/IP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # Asociar el socket al host y puerto especificados
    server_socket.bind((server_host, server_port))
    # Escuchar conexiones entrantes
    server_socket.listen(5)
    
    print(f"Servidor escuchando en {server_host}:{server_port}")
    
    try:
        while True:
            # Aceptar la conexión del cliente
            client_socket, client_address = server_socket.accept()
            print(f"[*] Conexión establecida desde {client_address[0]}:{client_address[1]}")
            # Iniciar un hilo para manejar al cliente
            client_thread = threading.Thread(target=handle_client, args=(client_socket,))
            client_thread.start()
    except KeyboardInterrupt:
        print("\nCerrando el servidor...")
        # Cerrar el socket del servidor
        server_socket.close()

if __name__ == "__main__":
    main()