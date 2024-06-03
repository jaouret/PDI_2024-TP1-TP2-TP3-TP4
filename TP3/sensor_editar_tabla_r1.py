from flask import Flask, render_template, request, abort, jsonify
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.types import DECIMAL

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///datos_sensores.db"
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
app.app_context().push()

class lectura_sensores(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    co2 = db.Column(db.Float(10, 2))
    co2_corregido = db.Column(db.Numeric(precision=10, scale=2))
    temp = db.Column(db.Numeric(precision=10, scale=2))
    hum = db.Column(db.Numeric(precision=10, scale=2))
    fecha = db.Column(db.Text)
    lugar = db.Column(db.Text)
    altura = db.Column(db.Numeric(precision=8, scale=0))
    presion = db.Column(db.Numeric(precision=8, scale=2))
    presion_nm = db.Column(db.Numeric(precision=8, scale=2))
    temp_ext = db.Column(db.Numeric(precision=8, scale=2))
    temp_ref = db.Column(db.Numeric(precision=8, scale=2))

    def __repr__(self):
        return '<Id %d>' % self.id

    def to_dict(self):
        return {
            'id': self.id,
            'co2': self.co2,
            'co2_corregido': self.co2_corregido,
            'temp': self.temp,
            'hum': self.hum,
            'fecha': self.fecha,
            'lugar': self.lugar,
            'altura': self.altura,
            'presion': self.presion,
            'presion_nm': self.presion_nm,
            'temp_ext': self.temp_ext,
            'temp_ref': self.temp_ref
        }

# Crear las tablas en la base de datos si no existen
db.create_all()

@app.route('/')
def index():
    return render_template('tabla_sensores_para_editar.html')

@app.route('/api/datos')
def datos():
    query = lectura_sensores.query

    # Filtro de búsqueda
    search = request.args.get('search')
    if search:
        query = query.filter(lectura_sensores.lugar.like(f'%{search}%'))
    total = query.count()

    # Ordenamiento
    sort = request.args.get('sort')
    if sort:
        order = []
        for s in sort.split(','):
            column_name = s.strip()
            if column_name not in ['lugar']:
                column_name = 'lugar'
            col = getattr(lectura_sensores, column_name)
            if s[0] == ' ':
                col = col.desc()
            order.append(col)
        if order:
            query = query.order_by(*order)

    # Paginación
    start = request.args.get('start', type=int, default=-1)
    length = request.args.get('length', type=int, default=-1)
    if start != -1 and length != -1:
        query = query.offset(start).limit(length)

    # Respuesta
    return jsonify({
        'data': [lectura_sensores.to_dict() for lectura_sensores in query],
        'total': total,
    })

@app.route('/api/data', methods=['POST'])
def update():
    data = request.get_json()
    if 'id' not in data:
        abort(400)
    lectura_sensores = lectura_sensores.query.get(data['id'])
    if not lectura_sensores:
        abort(404)
    for field in ['lugar']:
        if field in data:
            setattr(lectura_sensores, field, data[field])
    db.session.commit()
    return '', 204

if __name__ == '__main__':
    app.run(debug=True)
