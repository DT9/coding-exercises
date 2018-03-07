for i in range(1,13):
    row = [j*i for j in range(1,13)]
    print row

print [[j*i for j in range(1,13)] for i in range(1,13)]
