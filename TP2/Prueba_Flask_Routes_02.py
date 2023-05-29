from flask import Flask,jsonify
from flask import render_template
from flask import request
from flask import redirect, url_for

app = Flask(__name__)

sensors = [{'id':"0",
            'co2':"800",
            'temp':"23.0",
            'hum':"77.1",
            'fecha':"22/3/2021"},
           {'id': "1",
            'co2': "801",
            'temp': "24.0",
            'hum': "78.1",
            'fecha': "23/3/2021"}
           ]

@app.route('/')
def index():
    return "Hola"

@app.route('/login', methods=['POST'])
def login():
    nombre = request.form['nombre']
    clave = request.form['clave']
    if nombre == "javier" and clave == "abc":
        return "Hola %s con POST" %nombre

@app.route('/loging',methods = ['GET'])
def loging():
      nombre=request.args.get('nombre')
      clave=request.args.get('clave')
      if nombre=="javier" and clave=="abc":
          return "Hola %s con GET" %nombre

#  args es un objeto de diccionario que contiene la lista de pares de parámetros del formulario
#  y su valor correspondiente.
#  Podemos verificar la URL que también contiene los datos enviados con la solicitud al servidor.
#  Esta es una diferencia importante entre las solicitudes GET y las solicitudes POST,
#  ya que los datos enviados al servidor no se muestran en la URL del navegador en las solicitudes POST.

@app.route('/post_json', methods=['POST'])
def process_json():
    content_type = request.headers.get('Content-Type')
    if (content_type == 'application/json'):
        json = request.json
        return json
    else:
        return 'Content-Type no soportado!'

#curl -X POST -H "Content-type: application/json" -d "{\"Nombre\" : \"Juan\", \"Apellido\" : \"Sur\"}" "localhost:5000/post_json"

@app.route("/sensors", methods=['GET'])
def get():
    return jsonify ({'Sensors':sensors})

@app.route("/sensors/<int:id>", methods=['GET'])
def get_measure(id):
    return jsonify ({'Sensors':sensors[id]})

@app.route("/sensors", methods=['POST'])
def create():
    sensor = [{'id': "2",
                'co2': "800",
                'temp': "23.0",
                'hum': "77.1",
                'fecha': "22/3/2021"}
               ]
    sensors.append(sensor)
    return jsonify ({'Created':sensor})

@app.route("/sensors/<int:id>", methods=['PUT'])
def sensor_update(id):
    sensors[id]['fecha'] = "22/9/2021"
    return jsonify ({'Sensor':sensors[id]})

@app.route("/sensors/<int:id>", methods=['DELETE'])
def sensor_delete(id):
    sensors.remove(sensors[id])
    return jsonify ({'result':True})

if __name__  == "__main__":
    app.run(debug=True)