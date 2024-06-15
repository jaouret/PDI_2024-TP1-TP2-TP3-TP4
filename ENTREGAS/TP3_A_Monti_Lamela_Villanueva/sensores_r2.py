import time
import random
import sqlite3
from flask import Flask, render_template, jsonify, request, redirect, url_for
from datetime import datetime
from funciones import geo_latlon
import os

app = Flask(__name__)

def create_table():
    conn = sqlite3.connect('datos_sensores.db')
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS lectura_sensores (
        id INTEGER PRIMARY KEY,
        co2 REAL,
        temp REAL,
        hum REAL,
        fecha TEXT,
        lugar TEXT,
        altura REAL,
        presion REAL,
        presion_nm REAL,
        temp_ext REAL
    )''')
    conn.commit()
    conn.close()

@app.route('/')
def index():
    return render_template('tabla_sensores_para_editar.html')

@app.route('/agregar-registro')
def form_agregar_registro():
    return render_template("form_agregar_registro.html")

@app.route('/api/todos-los-datos')
def datos():
    conn = sqlite3.connect('datos_sensores.db')
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM lectura_sensores')
    records = cursor.fetchall()
    conn.close()
    return jsonify([{
        'id': record[0],
        'co2': record[1],
        'temp': record[2],
        'hum': record[3],
        'fecha': record[4],
        'lugar': record[5],
        'altura': record[6],
        'presion': record[7],
        'presion_nm': record[8],
        'temp_ext': record[9]
    } for record in records])

@app.route('/api/primer-registro')
def primer_registro():
    conn = sqlite3.connect('datos_sensores.db')
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM lectura_sensores LIMIT 1')
    record = cursor.fetchone()
    conn.close()

    if record is None:
        return jsonify({'error': 'No records found'}), 404

    return jsonify([{
        'id': record[0],
        'co2': record[1],
        'temp': record[2],
        'hum': record[3],
        'fecha': record[4],
        'lugar': record[5],
        'altura': record[6],
        'presion': record[7],
        'presion_nm': record[8],
        'temp_ext': record[9]
    }])
    

@app.route('/api/directorio-db')
def directorio_db():
    return jsonify({"directorio":os.getcwd()})

@app.route('/api/insertar-dato', methods=["POST"])
def insertar_dato():
    data = request.form
    co2 = data.get('co2')
    temp = data.get('temp')
    hum = data.get('hum')
    fecha = data.get('fecha')
    lugar = data.get('lugar')
    altura = data.get('altura')
    presion = data.get('presion')
    presion_nm = data.get('presion_nm')
    temp_ext = data.get('temp_ext')

    conn = sqlite3.connect('datos_sensores.db')
    cursor = conn.cursor()
    cursor.execute('''
        INSERT INTO lectura_sensores (co2, temp, hum, fecha, lugar, altura, presion, presion_nm, temp_ext)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    ''', (co2, temp, hum, fecha, lugar, altura, presion, presion_nm, temp_ext))
    conn.commit()
    conn.close()

    return redirect(url_for('index'))
    
if __name__ == '__main__':

    app.run(debug=True)
    create_table()

    temp_ext, presion, humedad_ext, descripcion_clima = geo_latlon()
    print("Resultados= ", temp_ext, presion, humedad_ext, descripcion_clima)

    while True:
        try:
            lugar = input("Lugar de la captura de los datos: ")
            tipo_lugar = input("Tipo de lugar [au=abierto urbano] [an=abierto no urbano] [c=cerrado] ")
            superficie = int(input("Superficie aproximada del lugar [m2]: "))
            altura = int(input("Altura aproximada del lugar [m]: "))
            presion_nm = presion
            cant_capturas = int(input("Cantidad de capturas: "))
            delta_t_capturas = int(input("Tiempo entre capturas (segs) : "))
        except ValueError:
            print("Error al ingresar datos...")
            continue
        else:
            break

    cont = 0
    while cont < cant_capturas:
        cont += 1
        verdadero = 1
        if verdadero == 1:
            print("Datos Disponibles!")
            CO2_medido = random.uniform(250, 1100)
            temp_sensor = random.uniform(temp_ext, temp_ext + 10)
            humedad_relativa = random.uniform(40, 80)
            print("CO2: %d PPM" % CO2_medido)
            print("Temperatura: %0.2f degrees C" % temp_sensor)
            print("Humedad: %0.2f %% rH" % humedad_relativa)

            d = datetime.now()
            print("Fecha", d)
            timestampStr = d.strftime("%d-%b-%Y (%H:%M:%S.%f)")

            conn = sqlite3.connect('datos_sensores.db')
            cursor = conn.cursor()
            cursor.execute('''INSERT INTO lectura_sensores (co2, temp, hum, fecha, lugar, altura, presion, presion_nm, temp_ext)
                              VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)''',
                           (CO2_medido, temp_sensor, humedad_relativa, timestampStr, lugar, altura, presion, presion_nm, temp_ext))
            conn.commit()
            conn.close()

            print("Registro insertado..., acumulados:", cont, "\n")
            time.sleep(delta_t_capturas)
            print("\nEsperando nuevo registro de datos ...\n")

    print("Cierro conexión ...")