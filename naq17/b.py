def floydwarshall(graph):
    # Initialize dist and pred:
    # copy graph into dist, but add infinite where there is
    # no edge, and 0 in the diagonal
    dist = {}
    pred = {}
    for u in graph:
        dist[u] = {}
        pred[u] = {}
        for v in graph:
            dist[u][v] = 1000
            pred[u][v] = -1
        dist[u][u] = 0
        for neighbor in graph[u]:
            dist[u][neighbor] = graph[u][neighbor]
            pred[u][neighbor] = u
 
    for t in graph:
        # given dist u to v, check if path u - t - v is shorter
        for u in graph:
            for v in graph:
                newdist = dist[u][t] + dist[t][v]
                if newdist < dist[u][v]:
                    dist[u][v] = newdist
                    pred[u][v] = pred[t][v] # route new path through t
    return dist, pred


G = {}
n,m,f,s,t = list(map(int,raw_input().split()))
for _ in range(m):
    i,j,c = list(map(int,raw_input().split()))
    G[i] = {j:c}
    G[j] = {i:c}
for _ in range(f):
    u,v = list(map(int,raw_input().split()))
    G[u] = {v:1}


dist, pred = floydwarshall(G)
#print Predecesors in shortest path:
for v in pred: 
    print "%s: %s" % (v, pred[v])
#print Shortest distance from each vertex:
for v in dist: 
    print "%s: %s" % (v, dist[v])
print dist[s][t]
 