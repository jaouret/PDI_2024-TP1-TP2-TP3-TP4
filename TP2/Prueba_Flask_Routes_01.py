from flask import Flask,jsonify

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