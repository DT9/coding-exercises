import string

tc = int(input())
lel = [' ']+list(string.ascii_lowercase)


for _ in range(tc):
    temp = input()
    letter = temp[0]
    sentences = temp[2:]
    if letter == 'e':
        res = []
        vsi = [lel.index(s) for s in sentences]
        ui = [vsi[0]]
        for i in range(1,len(vsi)):
            ui.append(vsi[i]+ui[i-1])
        ui = [i % 27 for i in ui]
        ui = [lel[i] for i in ui]
        print (''.join(ui))
    else:
        vsi = [lel.index(s) for s in sentences]
        ui = [vsi[0]]
        for i in range(1,len(vsi)):
            ui.append((vsi[i]-vsi[i-1]))
        ui = [lel[i] for i in ui]
        print(''.join(ui))