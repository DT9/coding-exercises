import sys
import argparse
import random
import re
from HTMLParser import HTMLParser
debug = False
class ColorParser(HTMLParser):
    def __init__(self, data):
        HTMLParser.__init__(self)
        self.data = data
        self.exclusion = ['br']
        self.stack = ['default']
        self.prefix = '\color'
        self.html = []
        self.colors = {'default':'FFFFFF'}
        self.line = None
        self.lineTag = []
        self.front = False
    def parse(self):
        for line in self.data:
            self.line = line.strip()
            self.feed(self.line)
            print self.html
            self.html = []
            self.lineTag = []
            # self.stack = []
    def handle_starttag(self, tag, attr):
        if tag not in self.colors:
            self.colors[tag] = self.color()
        if tag not in self.exclusion:
            self.stack.append(tag)
        temp = self.prefix + self.colors[tag] + self.createOpen(tag,attr)
        self.html.append(temp)
        self.lineTag.append(temp)

        self.front = True
        if debug: print 'start', tag, self.front        
    def handle_endtag(self, tag):
        top = self.stack[-1]
        if self.front == True:
            temp = self.createClose(tag)
        else:
            temp = self.prefix + self.colors[tag] + self.createClose(tag)
        self.html.append(temp)
        self.stack.pop()
        
        self.front = False
        if debug: print 'end', tag, self.front
    def handle_data(self, data):
        top = self.stack[-1]
        if debug: print 'data', top, self.front
        if self.front == True and self.line != data:
            temp = data
        else:
            temp = self.prefix + self.colors[top] + data
            self.front = True
        self.html.append(temp)
    def color(self):
        return '[' + ''.join([random.choice('0123456789ABCDEF') for _ in range(6)]) + ']'
    def printColor(self):
        print ' '
    def createOpen(self,tag,attr):
        attrs = ''.join([' %s="%s"'%(i,j) for i,j in attr])
        return '<'+tag+attrs+'>'
    def createClose(self,tag):
        return '</%s>' % tag        
    def checkGroup(self,tag):
        if self.stack[-1] != tag or tag in self.lineTag:
            return True
        return False
cp = ColorParser(sys.stdin)
cp.parse()
cp.printColor()