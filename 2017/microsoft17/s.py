import numpy as np
t = int(input())
for _ in range(t):
    n,s = list(map(int,input().split()))
    gates = [list(map(int,input().split())) for _ in range(n)]
    flags = []
    for i in range(len(gates)):
        a = np.array([i,gates[i][0]])
        b = np.array([i,gates[i][1]])
        flags.append(a)
        flags.append(b)
    total = 0
    flags.insert(0,np.array([0,0]))
    nodes = [for i in range(len(flags))]
    distances = {}
    for i in range(len(flags)):
        distances[i] = {}
    for j,k in range(len(flags)):
        distances[i][]
    unvisited = {node: None for node in nodes} #using None as +inf
    visited = {}
    current = 'B'
    currentDistance = 0
    unvisited[current] = currentDistance

    while True:
        for neighbour, distance in distances[current].items():
            if neighbour not in unvisited: continue
            newDistance = currentDistance + distance
            if unvisited[neighbour] is None or unvisited[neighbour] > newDistance:
                unvisited[neighbour] = newDistance
        visited[current] = currentDistance
        del unvisited[current]
        if not unvisited: break
        candidates = [node for node in unvisited.items() if node[1]]
        current, currentDistance = sorted(candidates, key = lambda x: x[1])[0]


    print(total)