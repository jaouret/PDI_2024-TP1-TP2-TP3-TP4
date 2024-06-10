import os
import json
import time
import logging
import paho.mqtt.client as mqtt
from flask import Flask
from flask_sqlalchemy import SQLAlchemy

# Configuración del logging para depuración
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s: %(message)s')

# Configuración de la aplicación Flask y la base de datos
app = Flask(__name__)
db_path = os.path.join(os.getcwd(), 'datos_sensores.db')
app.config['SQLALCHEMY_DATABASE_URI'] = f"sqlite:///{db_path}"
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
app.app_context().push()

class LecturaSensores(db.Model):
    __tablename__ = 'lectura_sensores'
    id = db.Column(db.Integer, primary_key=True)
    co2 = db.Column(db.Numeric(precision=10, scale=2))
    temp = db.Column(db.Numeric(precision=10, scale=2))
    hum = db.Column(db.Numeric(precision=10, scale=2))
    fecha = db.Column(db.Text)
    lugar = db.Column(db.Text)
    altura = db.Column(db.Numeric(precision=8, scale=2))
    presion = db.Column(db.Numeric(precision=8, scale=2))
    presion_nm = db.Column(db.Numeric(precision=8, scale=2))
    temp_ext = db.Column(db.Numeric(precision=8, scale=2))

# Configuración de MQTT
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "sensores/datos"

client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT, 60)

def publish_data():
    while True:
        try:
            records = LecturaSensores.query.all()
            for record in records:
                data = {
                    'co2': float(record.co2),
                    'temp': float(record.temp),
                    'hum': float(record.hum),
                    'fecha': record.fecha,
                    'lugar': record.lugar,
                    'altura': float(record.altura),
                    'presion': float(record.presion),
                    'presion_nm': float(record.presion_nm),
                    'temp_ext': float(record.temp_ext)
                }
                client.publish(MQTT_TOPIC, json.dumps(data))
                logging.debug(f"Datos publicados: {data}")
                time.sleep(5)  # Publicar cada 5 segundos
        except Exception as e:
            logging.error(f"Error al publicar datos: {e}")
            time.sleep(5)

if __name__ == '__main__':
    logging.info("Iniciando publicador de MQTT")
    publish_data()
