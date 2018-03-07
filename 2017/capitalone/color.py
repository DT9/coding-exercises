#!/usr/bin/env python
import argparse
import random
import re
from HTMLParser import HTMLParser

class ColorParser(HTMLParser):
    def __init__(self, data):
        HTMLParser.__init__(self)
        self.data = data  # raw html
        self.exclusion = ['br', 'area', 'embed', 'hr', 'meta', 'param', 'source','track', 'wbr', 'img', 'input', 'col', 'base']  # unary operators
        self.stack = ['default']  # tags stack
        self.prefix = '\color'  # color prefix
        self.html = []  # colored html
        self.colors = {'default': '[FFFFFF]'}  # tag colors
        self.line = None  # current line
        self.group = False  # current tag grouped?

    def parse(self):
        for line in self.data:
            self.line = line
            self.feed(self.line)
        # print self.html

    def printColor(self):
        print ''.join(self.html)

    def output(self):
        return self.html
    # start tag hook

    def handle_starttag(self, tag, attr):
        # add unique tag color
        if tag not in self.colors:
            self.colors[tag] = self._color()
        # exclude unary operators
        if tag not in self.exclusion:
            self.stack.append(tag)
            self.group = True  # beginning of a grouping
        # create colored string
        temp = self.prefix + self.colors[tag] + self._createOpen(tag, attr)
        self.html.append(temp)
    # end tag hook

    def handle_endtag(self, tag):
        top = self.stack[-1]        
        # non-colored when in a group
        if self.group == True:
            temp = self._createClose(tag)
        else:
            temp = self.prefix + self.colors[tag] + self._createClose(tag)
        self.html.append(temp)
        if top == tag:
            self.stack.pop()
        self.group = False  # no longer in a group
    # data hook

    def handle_data(self, data):
        top = self.stack[-1]
        # in a group or on separate new line
        if data == '\n':
            self.html.append(data)
            return
        if self.group == True and self.line != data:
            temp = data
        else:
            temp = self.prefix + self.colors[top] + data
            self.group = True
        self.html.append(temp)

    # creates random 6 hex chars
    def _color(self):
        return '[' + ''.join([random.choice('0123456789ABCDEF') for _ in range(6)]) + ']'

    def _createOpen(self, tag, attr):
        attrs = ''.join([' %s="%s"' % (i, j) for i, j in attr])
        return '<' + tag + attrs + '>'

    def _createClose(self, tag):
        return '</%s>' % tag


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='This script highlights html elements by their tag.')
    parser.add_argument('-i', '--input', help='HTML file', required=False)
    parser.add_argument('-o', '--output', help='Colored file', required=False)
    args = vars(parser.parse_args())
    if args['input']:
        inFile = open(args['input'], 'r')
    else:
        inFile = open('input.html', 'r')        
    cp = ColorParser(inFile.readlines())
    cp.parse()
    if args['output']:
        outFile = open(args['output'], 'w')
        outFile.write(''.join(cp.output()))
    else:
        outFile = open('output.html', 'w')
        outFile.write(''.join(cp.output()))        
        # cp.printColor()
