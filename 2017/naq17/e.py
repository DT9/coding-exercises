#!/usr/bin/env python
import numpy as np
from graphviz import Graph

#==== Genreate data ====
# nodes = map(str, [2, 4, 5, 6, 7, 8])
n = 20
nodes = np.random.randint(1, 100, n)
nodes = map(str, sorted(set(nodes)))
nInf = np.inf

#==== build cost matrix ====
n = len(nodes)
cMtx = [nInf] * (n + 1) # dimension 1x(n+1)
cMtx = [cMtx] * (n + 1 + 1)   # dimension (n+2)x(n+1)

#==== build root matrix ====
cMtx = np.matrix(cMtx)
rMtx = np.matrix(cMtx)

#==== initial boundary ====
for i in xrange(1, n + 2): # 1-6
    cMtx[i, i-1] = 0

#==== filling table (diagonally) ====
for l in xrange(1, n + 1): # 1-5
    for i in xrange(1, n - l + 1 + 1):
        j = i + l - 1
        # cMtx[i, j] = nInf
        for r in xrange(i, j + 1):
            tmp = cMtx[i, r-1] + cMtx[r+1, j] + (j - i + 1)
            if tmp < cMtx[i, j]:
                cMtx[i, j] = tmp
                rMtx[i, j] = r

#==== See ====                
print cMtx
print rMtx

#==== Trace root and build optimal tree ====
g = Graph(comment = 'G', filename = '/Users/yunyan/test.gv', engine='sfdp')
def renderGraph (root, nodes, g):
    if (len(root) != len(nodes) + 2):
        print "Root position matrix is not compatible with nodes"
        return 1
    n = len(nodes)
    r = int(root[1, n])
    print str(nodes[r-1]) + "node is root"
    g.node(str(nodes[r-1]), color = 'red')
    preorderRend(1, r-1, r, root, nodes, 'left', g) #left
    preorderRend(r+1, n, r, root, nodes, 'right', g) #right
    
def preorderRend(i, j, r, root, nodes, dire, g):
    if i <= j:
        t = int(root[i, j])
        print str(nodes[t-1]) + "node is " + dire +" child of " + str(nodes[r-1]) + "node"
        g.edge(nodes[r-1], nodes[t-1]) # r-->t #notice 0-based
        preorderRend(i, t-1, t, root, nodes, 'left', g) #left
        preorderRend(t+1, j, t, root, nodes, 'right', g) #right
    else:
        print str(nodes[r-1]) + "node's" + dire + " is empty"
renderGraph(root = rMtx, nodes = nodes, g = g)
g.render()
g.view()