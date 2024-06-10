import logging
import paho.mqtt.client as mqtt

# Configuración del logging para depuración
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s: %(message)s')

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "sensores/datos"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logging.info("Conectado al broker MQTT")
        client.subscribe(MQTT_TOPIC)
    else:
        logging.error(f"Conexión fallida con código de resultado: {rc}")

def on_message(client, userdata, msg):
    logging.debug(f"Mensaje recibido en tópico {msg.topic}: {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    logging.info("Conectando al broker MQTT")
    client.loop_forever()
except Exception as e:
    logging.error(f"Error al conectar al broker MQTT: {e}")
