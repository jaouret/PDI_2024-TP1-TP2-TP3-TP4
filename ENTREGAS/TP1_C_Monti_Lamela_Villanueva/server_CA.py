import select
import socket
import sys
import queue
import datetime

def main():
    # Creando un socket TCP/IP 
    servidor = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    servidor.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    servidor.setblocking(0)

    # Hago Bind del socket al puerto
    dir_servidor = ('localhost', 3535)
    #print('iniciando en {} port {}'.format(*dir_servidor), file=sys.stderr)
    servidor.bind(dir_servidor)

    # Escucho conexiones entrantes
    servidor.listen(5)
    print(f"Servidor escuchando en {dir_servidor[0]}:{dir_servidor[1]}")

    # Sockect que espero leer
    entradas = [servidor]

    # Sockets que espero enviar
    salidas = []

    # diccionario con conexiones y su respectivo datetime
    con_fechahora = {}

    try:
        while entradas:
            # Espero a que al menos uno de los sockets este listo para ser procesado
            print('Esperando el próximo evento', file=sys.stderr)
            readable, writable, exceptional = select.select(entradas, salidas, entradas)

            if not (readable or writable or exceptional):
                print('Tiempo excedido....', file=sys.stderr)
                continue

            # Manejo entradas
            for s in readable:
                if s is servidor:
                    # Un socket "leíble" está listo para aceptar conexiones
                    con, dir_cliente = s.accept()
                    print(f"[*] Conexión establecida desde {dir_cliente[0]}:{dir_cliente[1]}", file=sys.stderr)
                    con.setblocking(0)
                    entradas.append(con)

                    # Agrego la conexion a mi diccionario de conexion:datetime                 
                    fecha_hora_actual = datetime.datetime.now()
                    fecha_hora_formateada = fecha_hora_actual.strftime("%d-%m-%Y %H:%M:%S")
                    con_fechahora[con] = fecha_hora_actual;
                    con.sendall(fecha_hora_formateada.encode())
        
                else:
                    request = s.recv(1024).decode()
                    if(request and request == "quit"):
                        print('Se recibio "quit"')
                        duracion_conexion_seg = "{:.4f}".format((datetime.datetime.now() - con_fechahora.get(s)).total_seconds())
                        s.sendall(duracion_conexion_seg.encode())
                        print("Se envio duracion de conexion")
                        #s.close()
                    else:
                        # Si está vacío lo interpreto como una conexión a cerrar
                        print('Cerrando...', dir_cliente, file=sys.stderr)
                        # dejo de escuchar en la conexión
                        if s in salidas:
                            salidas.remove(s)
                        entradas.remove(s)
                        s.close()

            # Administro condiciones excepcionales"
            for s in exceptional:
                print('excepción en', s.getpeername(), file=sys.stderr)
                # Dejo de escuchar en las conexiones
                entradas.remove(s)
                if s in salidas:
                    salidas.remove(s)
                s.close()
    except KeyboardInterrupt:
        print("\nCerrando el servidor...")
        #cierro todos los sockets
        for s in entradas+salidas:
            s.close()

if __name__ == "__main__":
    main()