inp = input()[1:-1].split(', ')
data = [i.replace('(','').replace(')','') for i in inp]
strikes = 0
hits = 0
outs = 0
runs = 0

for i,k in zip(data[0::2], data[1::2]):
    #fastball fb
    #changeup c
    #fast or slow
    if i == 'FB' and k == 'F':
        runs += hits + 1
        hits = 0
        strikes = 0
    elif i == 'C' and k == 'S':
        strikes = 0
        hits += 1
    elif (i == 'FB' and k == 'S'):
        strikes += 1
    elif (i == 'C' and k == 'F'):
        strikes += 1
    if hits == 4:
        runs += 1
        hits -= 1
    if strikes == 3:
        outs += 1
    if outs == 3:
        break
print(runs)