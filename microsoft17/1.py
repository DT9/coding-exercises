import sys
import numpy as np
def check_symmetric(a, tol=1e-8):
    return np.allclose(a, a.T, atol=tol)

for line in sys.stdin:
    a = np.matrix(line)
    f = check_symmetric(a)
    if not f:
        print("Not symmetric")
    else:
        print("Symmetric")
