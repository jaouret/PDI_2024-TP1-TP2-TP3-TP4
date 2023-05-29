# Bellman Ford Moore - Ejemplo 01

# número de nodos en el gráfico
print('Ingresar número de nodos:- ')
n = int(input())

# n
print('Ingresar número de enlaces:- ')
m = int(input())

# gráfico que contiene los enlaces con costo
grafo = []

print('Ingresar los costos de los enlaces en la forma nodo i nodo j costo:')
for i in range(m):
    u, v, w = list(map(int, input().split()))
    grafo.append([u, v, w])


def BellmanFord(origen):
    # Initializando la distancia del origen a todos los destinos como infinito y distancia 0 al origen
    dist = [float("inf") for i in range(n)]
    dist[origen] = 0

    # Recorro todos los enlaces | V | - 1 vez. 
    # Una ruta simple más corta desde origen a cualquier otro vértice puede tener como máximo |V| - 1 bordes
    for i in range(n-1):
        # Actualice el valor de dist y el índice principal de los nodos adyacentes del nodo seleccionado.
        # Considero solo aquellos nodos que todavía están en cola.
        for u, v, w in grafo:
            if dist[u] != float("inf") and dist[u]+w < dist[v]:
                dist[v] = dist[u]+w

    # Verifico los ciclos de peso negativo.
    # El paso anterior garantiza distancias más cortas si el gráfico no contiene un ciclo de peso negativo.
    # Si obtenemos un camino más corto, entonces hay un ciclo.
    ciclo = 0
    for u, v, w in grafo:
        if dist[u] != float("Inf") and dist[u] + w < dist[v]:
            print("gráfico contiene costos negativos")
            ciclo = 1
            break
    if ciclo == 0:
        print('Distancia del nodo origen',origen)
        print('Enlace Distancia desde Origen')
        for i in range(len(dist)):
            print(i,'\t\t\t',dist[i])


BellmanFord(0)