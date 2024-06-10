import os
import json
from flask import Flask, render_template, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_socketio import SocketIO, emit

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
db_path = os.path.join(os.getcwd(), 'datos_sensores.db')
app.config['SQLALCHEMY_DATABASE_URI'] = f"sqlite:///{db_path}"
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
socketio = SocketIO(app)

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

    def to_dict(self):
        return {
            'id': self.id,
            'co2': self.co2,
            'temp': self.temp,
            'hum': self.hum,
            'fecha': self.fecha,
            'lugar': self.lugar,
            'altura': self.altura,
            'presion': self.presion,
            'presion_nm': self.presion_nm,
            'temp_ext': self.temp_ext
        }

# Crear las tablas en la base de datos si no existen
with app.app_context():
    db.create_all()

@app.route('/')
def index():
    return render_template('websockets_sensores_r1.html')

@app.route('/api/todos-los-datos')
def mostrar_todos_los_datos():
    try:
        records = LecturaSensores.query.all()
        if not records:
            return jsonify({'mensaje': 'No se encontraron datos'}), 404
        return jsonify([record.to_dict() for record in records])
    except Exception as e:
        return jsonify({'error': 'Error al buscar datos', 'detalle': str(e)}), 500

# SocketIO event for sending sensor data
@socketio.on('connect')
def handle_connect():
    print('Cliente conectado')
    emit('mensaje', {'data': 'Conexión establecida'})

@socketio.on('request_data')
def handle_request_data():
    try:
        records = LecturaSensores.query.all()
        data = [record.to_dict() for record in records]
        emit('sensor_data', data)
    except Exception as e:
        emit('error', {'error': str(e)})

if __name__ == '__main__':
    socketio.run(app, debug=True, port=5000)
