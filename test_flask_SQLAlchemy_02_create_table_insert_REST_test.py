from datetime import datetime
import time

from sqlalchemy import create_engine, insert, select, MetaData, Table, Column, Integer, Float, String, DateTime
engine = create_engine('sqlite:///REST_test.db', echo = True)
connection=engine.connect()
meta = MetaData()

tabla_REST_test = Table(
   'tabla_REST_test', meta,
   Column('id', Integer, primary_key = True), 
   Column('co2', Float),
   Column('temp', Float),
   Column('hum', Float),
   Column('fecha', DateTime(), default=datetime.now)
)
meta.create_all(engine)
print("Columnas de la Tabla tabla_REST_test %s" %(tabla_REST_test.columns.keys()))
print(engine)

_cont=0

print("Paso 0")
s = select([tabla_REST_test])
rp = connection.execute("SELECT max(id) from tabla_REST_test")
una_fila=rp.fetchone()
una_fila=una_fila[0]
_id=una_fila
print("una fila")
print(una_fila)


while _cont < 2:
    # since the measurement interval is long (2+ seconds) we check for new data before reading
    # the values, to ensure current readings.
    if True:
        _id=_id+1
        _cont=_cont+1
        _co2=501
        _temp=18
        _hum=65
        
        print("Data Available!")
        print("CO2:", _co2, "PPM")
        print("Temperature:", _temp, "degrees C")
        print("Humidity:", _hum, "%%rH")
        print("")
        print("Waiting for new data...")
        print("")

        ins = insert(tabla_REST_test).values(
            id=_id,
            co2=_co2,
            temp=_temp,
            hum=_hum)
        _fecha=datetime.now
        _id=_id+1
        result2 = connection.execute("INSERT INTO tabla_REST_test (id,co2,temp,hum,fecha) \
                 VALUES (?,?,?,?,?);",(_id,_co2,_temp,_hum,datetime.now()))
        print("Paso 1")
        print("str(ins)")
        print(str(ins))
        print("params")
        print(ins.compile().params)
        
        result = connection.execute(ins)
        result.inserted_primary_key

    time.sleep(0.5)
    
print("Paso2")
s = select([tabla_REST_test])
rp = connection.execute(s)
results = rp.fetchall()
print (results)
print(rp.lastrowid)
