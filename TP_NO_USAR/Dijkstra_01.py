# Algoritmo de Dijkstra

# Para ordenar y realizar un seguimiento de los nodos que aún no hemos visitado, usaremos PriorityQueue
from queue import PriorityQueue

# implementamos un constructor para clase Grafo:
# En este constructor parametrizado indicamos el número de vértices en el gráfico como argumento 
# e inicializamos tres campos:
#   v: Representa el número de nodos en el grafo.
#   enlaces: representa la lista de enlaces en forma de matriz. 
#   para los nodos u y v, self.edges[u][v] = costo del enlace.
#   recorridos: Un conjunto que contendrá los nodos recorridos.
# Definimos una función que agregará un enlace al grafo: agregar_enlace()

class Grafo:
    def __init__(self, num_nodos):
        self.v = num_nodos
        self.enlaces = [[-1 for i in range(num_nodos)] for j in range(num_nodos)]
        self.recorridos = []

    def agregar_enlace(self, u, v, costo):
        self.enlaces[u][v] = costo
        self.enlaces[v][u] = costo

    # algortimo principal
    # En este código, primero creamos una lista D del tamaño v.
    # La lista completa se inicializa hasta el infinito.
    # Esta será una lista en la que mantendremos las rutas más cortas desde nodo_origen a todos los demás nodos.
    # Establecemos el valor del nodo de inicio en 0, ya que esa es su distancia a sí mismo.
    # Luego, inicializamos una cola de prioridad, que usaremos para ordenar los nodos de menor a mayor distancia.
    # Ponemos el nodo de inicio en la cola de prioridad.
    # Ahora, para cada nodo en la cola de prioridad, primero los marcaremos como recorridas y
    # luego iteraremos a través de sus vecinos.
    # Si el vecino no es recorrido, compararemos su costo antiguo y su costo nuevo.
    # El costo antiguo es el valor actual del camino más corto desde el ndod inicial al vecino,
    # mientras que el costo nuevo es el valor de la suma del camino más corto desde el nodo inicial al
    # nodo actual y la distancia entre el nodo actual y el vecino.
    # Si el costo nuevo es más bajo que el costo anterior, colocamos al vecino y su costo en la cola de prioridad
    # y actualizamos la lista donde mantenemos las rutas más cortas.
    # Después de visitar todos los nodos y la cola de prioridad está vacía, devolvemos la lista D.
    # Inicializamos un gráfo probar el algoritmo:

    def dijkstra(self, nodo_inicio):
        D = {v: float('inf') for v in range(self.v)}
        D[nodo_inicio] = 0

        pq = PriorityQueue()
        pq.put((0, nodo_inicio))

        while not pq.empty():
            (dist, nodo_actual) = pq.get()
            self.recorridos.append(nodo_actual)

            for vecino in range(self.v):
                if self.enlaces[nodo_actual][vecino] != -1:
                    distancia = self.enlaces[nodo_actual][vecino]
                    if vecino not in self.recorridos:
                        costo_anterior = D[vecino]
                        nuevo_costo = D[nodo_actual] + distancia
                        if nuevo_costo < costo_anterior:
                            pq.put((nuevo_costo, vecino))
                            D[vecino] = nuevo_costo
        return D

g = Grafo(9)
g.agregar_enlace(0, 1, 4)
g.agregar_enlace(0, 6, 7)
g.agregar_enlace(1, 6, 11)
g.agregar_enlace(1, 7, 20)
g.agregar_enlace(1, 2, 9)
g.agregar_enlace(2, 3, 6)
g.agregar_enlace(2, 4, 2)
g.agregar_enlace(3, 4, 10)
g.agregar_enlace(3, 5, 5)
g.agregar_enlace(4, 5, 15)
g.agregar_enlace(4, 7, 1)
g.agregar_enlace(4, 8, 5)
g.agregar_enlace(5, 8, 12)
g.agregar_enlace(6, 7, 1)
g.agregar_enlace(7, 8, 3)

D = g.dijkstra(0)
print(D)

for nodo in range(len(D)):
    print("distancia del nodo 0 al nodo", nodo, "is", D[nodo])
