# PDI - TP 1 - A (Entrega 22/04/2024)

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


# PDI - TP 1 - B - Raw Sockets (Entrega 29/04/2024)

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

# PDI - TP 1 - C - Cliente Servidor utilizando Sockets en Python. (Entrega 13/05/2024)

Para realizar programas Cliente Servidor con Python utlizamos la librería o paquete socket.py ( https://github.com/python/cpython/blob/3.10/Lib/socket.py )
). Esta librería es una transcripción sencilla de la llamada al sistema sockets de BSD Unixal estilo orientado a objetos de Python:.
La función socket() devuelve a socket object métodos que implementan las diversas llamadas al sistema de socket. Los tipos de parámetros tienen un nivel algo más alto que en la interfaz C, como con read() y write() en el uso de los archivos Python, la asignación del buffer es automática y la longitud del buffer está implícita en las operaciones de envío.

Para detalles de la implementación de socket.py ver;

Para la parte C del trabajo práctico usar como ejemplos de base los siguientes códigos (la explicación de la implementación está detallada dentro del mismo código)

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

# PDI - TP 2 - A - SNMP (Entrega 13/05/2024)

Descargar de internet un gestor de SNMP
Instalar un agente SNMP en Windows

* En Windows instalar SNMP y activarlo con PowerShell como administrador

Get-WindowsCapability  -Online -Name SNMP*

Get-WindowsCapability -name SNMP* -online | Add-WindowsCapability -Online

Configuración básica:

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\services\SNMP\Parameters\RFC1156Agent" -Name "sysContact" -Value "Nombre" -type String

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\services\SNMP\Parameters\RFC1156Agent" -Name "sysLocation" -Value "UCA" -type String

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\SNMP\Parameters\ValidCommunities" -Name "COMUNIDAD_PDI" -Value 8 -type DWord

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\SNMP\Parameters\PermittedManagers" -Name "1" -Value "localhost" -type String

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\SNMP\Parameters\PermittedManagers" -Name "2" -Value "192.168.1.78" -type String

Restart-Service -Name SNMP

Mofidicar la configuración del MIB Browser

En address agregar "localhost"

![image](https://github.com/jaouret/PDI_2024-TP1-TP2-TP3-TP4/assets/111520053/ad5e4c2c-04a2-4dc2-830c-57262cdfb967)


En advanced en los campos read community y write community agregar COMUNIDAD_PDI. Y la version 2
![image](https://github.com/jaouret/PDI_2024-TP1-TP2-TP3-TP4/assets/111520053/36b36bb8-9f46-438b-87fd-944f64ab80c7)



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

# PDI - TP 3 - A - RestAPI - Servidores sin control de estado - Gestión de la Base de Datos (Entrega 10/06/2024)
Instalar SQLite.

Instalar Heidi SQL en Windows.

Para Linux y Windows se puede usar DBeaver Community Edition.

También se puede instalar DB Browser for SQLite.

Desde una terminal crear la base de datos, si no existe SQLite la crea automáticamente.

La idea es crear un repositorio de datos para recibir datos de sensores.

Conectarse a la base de datos desde el gestor elegido.

Crear una tabla para recibir los datos de los sensores.

Verificar con el gestor o desde el SQLite desde una terminal que esté todo bien.

Verificar que Python y pip esté instalado.

Acceder a la base de datos de Python para ingresar nuevos valores.

Desde el programa en Python importo las dependencias.

Adaptar el código en github de acuerdo al criterio de diseño elegido.

Notas:
* Hubo varias consultas referidas a que no se desplegaban correctamente los datos.
* La idea de la versión r1 era que para el TP se adapte el código resolviendo las inconsistencias.
* Ahora podán encontrar las versiones r2 con algunas modificaciones
* * sensores_r1 genera una db llamada datos_sendores.db
* * sensor_editar_tabla_r2 agrega nuevas rutas y mensajes.    

@app.route('/api/prueba')

@app.route('/')

@app.route('/api/todos-los-datos')

@app.route('/api/primer-registro')

@app.route('/api/directorio-db')

@app.route('/api/insertar-dato')

Verificar y mejorar.

# PDI - TP 4 - A - RestAPI - IoT - MQTT (Entrega 24/06/2024 o en Primera Fecha de Final de Julio)

Instalar MQTT en Windows o Linux.

Usaremos Mosquitto con Broker MQTT

* Instalar
En Linux (Ubuntu)

sudo apt-get install mosquitto mosquitto-clients -y

* Configurar
El archivo de configurar se encuentra en: /etc/mosquitto/mosquitto.conf

Editarlo 

Mosquitto como servidor escucha en el puerto 1883. Se puede cambiar en caso de ser necesario.

* Arrancar y verificar el Servidor Mosquitto
  
sudo systemctl start mosquitto

sudo systemctl enable mosquitto

sudo systemctl status mosquitto

* Suscribirse a un tema o tópico desde la terminal 1.
* * El cliente se suscribe al tema tempratura en el sitio1 y queda a la espera de recibir mensajes publicados en ese tema.

mosquitto_sub -h localhost -t sitio1/temperatura

* Publicar datos desde la terminal 2
  
mosquitto_pub -h localhost -t sitio1/tempratura -m "Sitio1 Temp. = 22 C"

mosquitto_pub -h localhost -t sitio1/tempratura -m "Sitio1 Temp. = 23 C"

El broker (Mosquitto) escucha en el puerto 1883 para conexiones MQTT sin encriptación.
Gestiona las suscripciones y publica mensajes a los clientes suscritos a los temas o tópicos específicos.

El cliente suscriptor (sería un servidor en este caso) se conecta al broker y se suscribe a uno o varios temas o tópicos.
Queda en modo escuchs de mensajes a que el broker envíe a esos temas o tópicos.

El cliente publicador se conecta al broker y publica mensajes en uno o varios temas tópicos.El broker recibe el mensaje y lo reenvía a todos los clientes suscritos a esos tópicos.

* Para hacer todo lo anterior en Windows:

* Descargar e instalar Mosquitto desde 

](https://mosquitto.org/)

* Ejecutar el instalador
  
Asegúrarse de seleccionar la opción para instalar el servicio Mosquitto para que se inicie automáticamente.

* Configuración del Path

Agregar la ruta de Mosquitto a las variables de entorno del sistema:

C:\Program Files\mosquitto).

* Ejecutar Mosquitto Broker

Abrir una ventana de terminal (símbolo del sistema) como administrador y ejecutar:

sh

net start mosquitto

Para detener el broker:

sh

net stop mosquitto

* Abrir dos ventanas de terminal (símbolo del sistema).

* Seguir los mismos pasos explicados para Linux.

# PDI - TP 4 - B - IoT MQTT (Entrega 24/06/2024 o en Primera Fecha de Final de Julio)

Una vez que tengan funcionando lo anterior deberán simular la publicación de datos de sensores tomando lo que está ingresado en la base datos.
Por practicidad y seguridad se puede hacer una aplicación cliente servidor entre los sensores y una base de datos, para que luego un publicador mqtt tome los datos desde ahí.

Luego un suscriptor toma los datos desde el publicador y los pasa a otra base datos local y propia del publicador para su análisis.

Esto permite que varios suscritores usen los mismos datos.

Realizar la modificación de las rutas RestAPI para mostrar los datos de los sensores suscriptos de esta forma.

Como base pueden usar los siguientes scripts:

* mqtt_pub_r1.py

* mqtt_sub_r1.py

En la clase, de ser posible tomaremos datos reales y serán cargado en la BD a las que debe conectarse el publicador.


