import os
from flask import Flask, render_template, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.exc import OperationalError

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
try:
    db.create_all()
    print("Base de datos abierta correctamente")
    print(f"Directorio de la base de datos: {db_path}")
except OperationalError as e:
    print(f"Error al abrir la base de datos: {e}")

@app.route('/api/prueba')
def index2():
    return "Prueba de ruta API..."

@app.route('/')
def index():
    return render_template('tabla_sensores_para_editar.html')

@app.route('/api/todos-los-datos')
def mostrar_todos_los_datos():
    try:
        records = LecturaSensores.query.all()
        if not records:
            return jsonify({'mensaje': 'No se encontraron datos'}), 404
        return jsonify([record.to_dict() for record in records])
    except Exception as e:
        return jsonify({'error': 'Error al buscar datos', 'detalle': str(e)}), 500

@app.route('/api/primer-registro')
def mostrar_primer_registro():
    try:
        primer_registro = LecturaSensores.query.first()
        if not primer_registro:
            return jsonify({'mensaje': 'No se encontraron datos'}), 404
        return jsonify(primer_registro.to_dict())
    except Exception as e:
        return jsonify({'error': 'Error al buscar datos', 'detalle': str(e)}), 500

@app.route('/api/directorio-db')
def directorio_db():
    return jsonify({'directorio': os.getcwd(), 'path': db_path})

@app.route('/api/insertar-dato')
def insertar_dato_prueba():
    try:
        datos_prueba = LecturaSensores(
            co2=400.00,
            temp=22.50,
            hum=45.00,
            fecha="2023-06-08",
            lugar="Oficina",
            altura=100.00,
            presion=1013.25,
            presion_nm=1010.00,
            temp_ext=18.00
        )
        db.session.add(datos_prueba)
        db.session.commit()
        return jsonify({'mensaje': 'Datos insertados correctamente'})
    except Exception as e:
        db.session.rollback()
        return jsonify({'error': 'Error al insertar datos', 'detalle': str(e)}), 500

if __name__ == '__main__':
    print("Directorio:", os.getcwd())
    app.run(debug=True, port=5010)
