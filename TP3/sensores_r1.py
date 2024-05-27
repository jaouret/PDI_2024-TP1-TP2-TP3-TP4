import mysql.connector
import time
import random
import sys
import logging
from flask import Flask, render_template
from flask_sqlalchemy import SQLAlchemy
from scp import SCPClient
from datetime import datetime
from funciones import geo_latlon

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///db.sqlite'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

app.app_context().push()

class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), index=True)
    age = db.Column(db.Integer, index=True)
    address = db.Column(db.String(256))
    phone = db.Column(db.String(20))
    email = db.Column(db.String(120), index=True)

db.create_all()

@app.route('/')
def index():
    users = User.query
    return render_template('basic_table.html', users=users)

if __name__ == '__main__':
    app.run()


class crear_dict(dict):
    # __init__ function
    def __init__(self):
        self = dict()
    # Function to add key:value
    def add(self, key, value):
        self[key] = value

print('Defino credenciales de la base de datos ..')
user = 'usuario'
password = 'password'
host = '127.0.0.1'
port = 3306
database = 'base_de_datos'

print("Conecto con la base de datos utilizando mysql.connector...")
cnx = mysql.connector.connect(user='',
                              password='',
                              host='127.0.0.1',
                              port=puerto_local,
                              database='')
if cnx:
    print ("Connectado")
else:
    print ("No conectado")

cursor = cnx.cursor()
query = "SELECT * FROM [table]"
cursor.execute(query)
records = cursor.fetchall()
temp_ext, presion, humedad_ext, descripcion_clima = geo_latlon()
print("Resultados= ",temp_ext, presion, humedad_ext, descripcion_clima)

#datos del lugar
while True:
    try:
        lugar=input("Lugar de la captura de los datos: ")
        tipo_lugar=input("Tipo de lugar [au=abierto urbano] [an=abierto no urbano] [c=cerrado] ")
        superficie=int(input("Superficie aproximada del lugar [m2]: "))
        altura=int(input("Altura aproximada del lugar [m]: "))
        presion_nm=presion
        cant_capturas=int(input("Cantidad de capturas: "))
        delta_t_capturas=int(input("Tiempo entre capturas (segs) : "))
    except ValueError:
        print("Error al ingresar datos...")
        continue
    else:
        break
cont=0    
while cont<cant_capturas:
    cont=cont+1
    verdadero = 1
    if verdadero == 1:
        print("Datos Disponibles!")
        CO2_medido = random.uniform(250, 1100)
        temp_sensor = random.uniform(temp_ext, temp_ext+10)
        humedad_relativa = random.uniform(40, 80)
        print("CO2: %d PPM" % CO2_medido)
        print("Temperatura: %0.2f degrees C" % temp_sensor)
        print("Humedad: %0.2f %% rH" % humedad_relativa)

        d=datetime.now()
        print("Fecha",d)
        timestampStr = d.strftime("%d-%b-%Y (%H:%M:%S.%f)")
        cursor = cnx.cursor()
        query ="INSERT INTO data3(co2, temp, hum, fecha, lugar,altura,presion,presion_nm,temp_ext)\
                VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        record =(CO2_medido,temp_sensor,humedad_relativa,timestampStr,lugar,altura,presion,presion_nm,temp_ext)
        cursor.execute(query,record)
        cnx.commit()
        print(cursor.rowcount, "Registro insertado..., acumulados:",cont,"\n")
        time.sleep(delta_t_capturas)
        print("\nEsperando nuevo registro de datos ...\n")

cnx.close()
print("Cierro conexión ...")

