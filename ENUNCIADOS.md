# PDI - TP 1 - A

Explicar qué es un socket y los diferentes tipos de sockets.

Cuáles son las estructuras necesarias para operar con sockets en el modelo C-S y cómo se hace para ingresar los datos requeridos. Explicar con un ejemplo.

Explicar modo bloqueante y no bloqueante en sockets y cuáles son los “sockets calls” a los cuales se pueden aplicar estos modos. Explicar las funciones necesarias.

Describir el modelo C-S aplicado a un servidor con Concurrencia Real. Escribir un ejemplo en lenguaje C.

Cómo se cierra una conexión C-S ?. Métodos que eviten la pérdida de informacion.

Probar el código del chat entre cliente y servidor. Cambiar tipo de socket y volver a probar.

Presentar las respuestas en archivos ,pdf identificando alumno y grupo.

C-S = Cliente - Servidor

Bibliografía a consultar:

Clases Teóricas.

Brian "Beej Jorgensen" Hall. Beej's Guide to Network Programming: Using Internet Sockets. 2019.

Douglas E. Comer, David L. Stevens. Internetworking With Tcp/Ip: Client-Server Programming and Applications. Prentice Hall. 2011.


# PDI - TP 1 - B - Raw Sockets

El concepto básico de sockets a bajo nivel es enviar un solo paquete por vez con todos los encabezados de los protocolos
completados dentro del programa en lugar de usar el kernel.

Unix provee dos tipo de sockets para el acceso directo a la red:

SOCK_PACKET, recibe y envía datos al dispositivo ubicado en la capa de enlace. Esto significa que el "header" de la placa de red
incluido en los datos puede ser escrito o leído. En general es el header de Ethernet. Todos los encabezados de los 
protocolos subsecuentes también deben ser incluidos en los datos.

SOCK_RAW, es el que usaremos por ahora, que incluye los encabezados IP, protocolos y datos subsecuentes.

Desde el momento que lo creamos se puede enviar cualquier tipo de paquetes IP por este socket.
También se pueden recibir cualquier tipo de paquetes que lleguen al host, después que el socket fue creado, si se
hace un "read()" desde él.
Se puede observar que aunque el socket es una interfaz al header IP, es tambiés específico para una capa de transporte.
Esto significa que para escuchar tráfico TCP, UDP, ICMP hay que crear 3 raw sockets por separado, usando
IPPROTO_TCP, IPPROTO_UDP y IPPROTO_ICMP (números de protocolo son 0 ó 6 para tcp, 17 para udp y 1 para icmp).

Con esta información es posible crear un simple "sniffer", que muestre todo el contenido de los paquetes TCP
que se reciban. ( En este ejemplo se evitan los headers IP y TCP, y se imprime solamente el "payload" con encabezados IP y
TCP contenidos en el paquete ).

* Utilizando el código raw.c como base escribir un "sniffer" que es un programa que muestra el contenido del tráfico que llega.
* Enviar tráfico al "sniffer" desde el cliente escrito en la parte A del TP1
* Enviar tráfico ICMP al "sniffer" y mostrar los resultados del LOG con comentarios.
* Mostrar resultados.

# PDI - TP 1 - C - Cliente Servidor utilizando Sockets en Python.

Para realizar programas Cliente Servidor con Python utlizamos la librería o paquete socket.py ( https://github.com/python/cpython/blob/3.10/Lib/socket.py )
). Esta librería es una transcripción sencilla de la llamada al sistema sockets de BSD Unixal estilo orientado a objetos de Python:.
La función socket() devuelve a socket object métodos que implementan las diversas llamadas al sistema de socket. Los tipos de parámetros tienen un nivel algo más alto que en la interfaz C, como con read() y write() en el uso de los archivos Python, la asignación del buffer es automática y la longitud del buffer está implícita en las operaciones de envío.

Para detalles de la implementación de socket.py ver;

Para la parte C del trabajo práctico usar como ejemplos de base los siguientes códigos (la explicación de la implementación está detallada dentro del mismo código)

https://trello.com/1/cards/64184f125aa3494c371fb5f6/attachments/6445b7a56208701889f75156/download/image.png

y escribir una aplicación cliente servidor que muestre las direcciones y puertos de todos los clientes conectados del lado del servidor y devuelva a cada cliente el día y hora de conexión, y el tiempo que estuvo (o está conectado).

Realizar la misma aplicación tanto para C-S con Concurrencia Aparente (Select) como C-S Concurrente.

De ser necesario agregar tiempo de espera, loop, sleep, con contadores para demorar los procesos.

Implementar una limpieza de recuersos al salir del los programas (agregar opción de pregunta al usuario para cerrar los clientes).

Para crear un socket (stream) en Python:
socket.socket(family=AF_INET, type=SOCK_STREAM, proto=0, fileno=None)

Los parámetros son los mismos que se usan en C
```
import socket
Creo socket IPv4
sock_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
if sock_fd == -1:

Administro el error

Ejemplo de cliente sencillo

from socket import socket as Socket
from socket import AF_INET, SOCK_STREAM
SERVIDOR = 'a.b.c.d'  # IP 
NROPUERTO = 41267          # puerto
BUFFER = 80                 # tamaño del buffer

DIRECCION_SERVIDOR = (SERVIDOR, NROPUERTO)
CLIENTE = Socket(AF_INET, SOCK_STREAM)
try:
    CLIENTE.connect(SERVER_ADDRESS)
    print('cliente conectado')
    DATOS = input('Mensaje : ')
    CLIENTE.send(DATOS.encode())
except OSError:
    print('connection failed')
CLIENT.close()
```

# PDI - TP 2 - A - SNMP

Descargar de internet un gestor de SNMP
Instalar un agente SNMP en Windows

* En Windows instalar SNMP y activarlo con PowerShell como administrador

Get-WindowsCapability  -Online -Name SNMP*
Get-WindowsCapability -name SNMP* -online | Add-WindowsCapability -Online

Configuración básica:

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\services\SNMP\Parameters\RFC1156Agent" -Name "sysContact" -Value "Nombre" -type String

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\services\SNMP\Parameters\RFC1156Agent" -Name "sysLocation" -Value "UCA" -type String

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\SNMP\Parameters\ValidCommunities" -Name "COMUNIDAD_PDI" -Value 8 -type DWord

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\SNMP\Parameters\PermittedManagers" -Name "1" -Value "localhost" -type StringSet-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\SNMP\Parameters\PermittedManagers" -Name "2" -Value "192.168.1.78" -type String

Restart-Service -Name SNMP

Mofidcar la configuración del MIB Browser

* Si se instala en Linux

sudo apt-get install snmpd.

Seguramente requiera net-tools previamente instalado

sudo nano /etc/snmp/snmpd.conf

Configurar agentAddress para escuchar en IPv4 e IPv6: agentAddress udp:161,udp6:[::1]:161agentAddress udp:192.168.1.5:161

rocommunity public

sudo service snmpd restart

sudo service snmpd status

Ejecutar el MIB Browser sobre Windows o Linux.

Describir lo que se observa en un informe.

# PDI - TP 2 - B - RestAPI - Servidores sin control de estado
https://trello.com/c/18DDc2Ll/2-parte-b-ingresar-a-la-tarjeta-para-leer-el-contenido

# PDI - TP 3 - A - Algoritmo BFM y Dijkstra - Ruteo
https://trello.com/c/CgJGp5Fz/1-parte-a-ingresar-a-la-tarjeta-para-leer-el-contenido
https://trello.com/c/GmBZh9LJ/3-parte-b-ingresar-a-la-tarjeta-para-leer-el-contenido


