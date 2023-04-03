# PDI_2023 - TP 1 - A
Protocolos de Internet - 2023
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


# PDI_2023 - TP 1 - B - Raw Sockets

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

> Utilizando el código raw.c como base escribir un "sniffer" que es un programa que muestra el contenido del tráfico que llega.
> Enviar tráfico al "sniffer" desde el cliente escrito en la parte A del TP1
> Mostrar resultados.
