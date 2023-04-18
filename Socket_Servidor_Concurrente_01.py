#servidor concurrente
import socket
import threading

host = "127.0.0.1"
port = 6667

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ("Socket Creado")
sock.bind((host, port))
print ("socket bind Completado")
sock.listen(1)
print ("socket en modo escucha - pasivo")

def proceso_hijo(*args): #*args valores de conexión y dirección de cliente devueltos por sock.accept()
    conn = args[0] #Conexión
    addr = args[1] #Dir Cliente
    try:
        print('conexion con {}.'.format(addr))
        conn.send("Servidor: Conectado con cliente".encode('UTF-8'))

        while True:
            data = conn.recv(10)
            print ('recibido "%s"',data)
            if data:
                print ('enviando mensaje de vuelta al cliente')
                conn.sendall(data)
            else:
                print ('no hay mas datos', addr)
                break

        # while True:
        #
        #     datos = conn.recv(4096)
        #     if datos:
        #         print('Recibido: {}'.format(datos.decode('utf-8')))
        #
        #     else:
        #         print("Prueba")
        #         break
    finally:
        conn.close()

while 1:
    conn, addr = sock.accept()
    threading.Thread(target=proceso_hijo, args=(conn, addr)).start()