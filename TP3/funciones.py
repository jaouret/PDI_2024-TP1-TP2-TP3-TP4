def geo_latlon():
    import geocoder
    import requests, json

    g = geocoder.ip('me')
    print(g.latlng)
    lat, lon = g.latlng
    print("Lat =", lat, "Lon = ", lon)

    api_key = "2f66bd561ebc7e4bde0d2a8951df0098"

    # base_url variable to store url
    base_url = "http://api.openweathermap.org/data/2.5/weather?"

    # Give city name
    # city_name = input("Ciudad: ")
    print("Ciudad o Geo ? = ")
    opcion = ['ciudad', 'geo']
    opcion_elegida = ''
    mensaje = "Elegir opción:\n"
    for index, item in enumerate(opcion):
        mensaje += f'{index + 1}) {item}\n'
    mensaje += 'Opcíón:'

    while opcion_elegida.lower() not in opcion:
        opcion_elegida = input(mensaje)

    print('Seleccionado: ' + opcion_elegida)

    # complete_url variable to store
    # complete url address
    if opcion_elegida == 'ciudad':
        city_name = input("Ciudad: ")
        complete_url = base_url + "appid=" + api_key + "&q=" + city_name + "&units=metric" + "&lang=es"
    else:
        complete_url = base_url + "lat=" + str(lat) + "&lon=" + str(
            lon) + "&appid=" + api_key + "&units=metric" + "&lang=es"
    print(complete_url)
    # get method of requests module
    # return response object
    response = requests.get(complete_url)

    # json method of response object
    # convert json format data into
    # python format data
    x = response.json()
    print(x)
    print(x["cod"])

    # Now x contains list of nested dictionaries
    # Check the value of "cod" key is equal to
    # "404", means city is found otherwise,
    # city is not found
    if x["cod"] != "404" and x["cod"] != "400":

        # store the value of "main"
        # key in variable y
        y = x["main"]

        # store the value corresponding
        # to the "temp" key of yRio
        temp_ext = y["temp"]

        # store the value corresponding
        # to the "pressure" key of y
        presion = y["pressure"]

        # store the value corresponding
        # to the "humidity" key of y
        humedad_ext = y["humidity"]

        # store the value of "weather"
        # key in variable z
        z = x["weather"]

        # store the value corresponding
        # to the "description" key at
        # the 0th index of z
        descripcion_clima = z[0]["description"]

        # print following values
        print(" Temperatura = " +
              str(temp_ext)[0:5] + " C" +
              "\n Presión Atmosférica = " +
              str(presion) + " hPa" +
              "\n Humedad  = " +
              str(humedad_ext) + " %" +
              "\n Cielo = " +
              str(descripcion_clima))

    else:
        print(" Ciudad no encontrada ")
    return temp_ext, presion, humedad_ext, descripcion_clima

