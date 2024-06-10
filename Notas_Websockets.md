# WebSockets 

* Permite la comunicación bidireccional en tiempo real entre un cliente (como un navegador web) y un servidor. 

* A diferencia de HTTP, que siguen un patrón de solicitud-respuesta, los WebSockets permiten que ambos lados de la comunicación envíen datos en cualquier momento sin la necesidad de realizar solicitudes adicionales.

* Se pueden usar en aplicaciones que requieren actualizaciones en tiempo real, como chats, juegos en línea, aplicaciones de finanzas, etc.


* * Pasos para la comunicación:

- Inicio de la conexión: La comunicación comienza con una solicitud de actualización del protocolo HTTP del cliente al servidor. Esta solicitud incluye una cabecera especial que indica que el cliente desea establecer una conexión WebSocket.

- Respuesta del servidor: Si el servidor soporta WebSockets, responde con una cabecera de actualización, y la conexión se establece. A partir de ese momento, la conexión permanece abierta, permitiendo el intercambio de datos en tiempo real.

- La conexión permite que tanto el cliente como el servidor envíen y reciban mensajes en cualquier momento. Esto se logra a través de un canal de comunicación persistente que no requiere solicitudes adicionales.

- Cierre de la conexión: La conexión WebSocket puede ser cerrada por cualquiera de las partes en cualquier momento.

![alt tag](https://github.com/jaouret/PDI_2024-TP1-TP2-TP3-TP4/blob/main/protocols.png)

Ver ejemplo con flask en el TP4
