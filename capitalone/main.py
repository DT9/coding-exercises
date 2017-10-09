import sys, argparse, random, re
from HTMLParser import HTMLParser
result = []

prefix = "\color"
Tags = {}
stack = []
exclude = ['<br>']


class ParseHTML(HTMLParser):
    def __init__(self):
        self.tags = []
    def grabTag(self,tag):
        temp = tag.split(' ')[0]
        return temp.lower()
        
    def parseLine(self,line):
        front = []
        AllTags = re.findall('<(.*?)>',line)
        for tg in AllTags:
            t = self.grabTag(tg)
            front.append(t)
            if t[1] != '/' and t not in Tags:
                tEnd = '</'+t[1:]
                col = color()
                for tag in [t,tEnd]:
                    obj = {
                        'col' : col,
                        'idx' : line.index(tg),
                        'type': 'open'
                    }
                    Tags[tag] = obj
                stack.append(t)
        return tags

def color():
    return '['+''.join([random.choice('0123456789ABCDEF') for _ in range(6)])+']'

for line in sys.stdin:
    res = []

# print Tags, hello