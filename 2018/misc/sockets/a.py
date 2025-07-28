import SocketServer
import socket
import sys
import select 

class hwRequestHandler( SocketServer.StreamRequestHandler ):
  def handle( self ):
    self.wfile.write("Hello World!\n")
 
 
server = SocketServer.TCPServer( ("", 2525), hwRequestHandler )
server.serve_forever()

########### socket

streamSock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

dgramSock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )

streamSock.close()

del streamSock

url1 = ( '192.168.1.1', 80 )
url2 = ( 'www.ibm.com', 25 )

######### listen srv

sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
sock.bind( ('', 2525) )
sock.listen( 5 )
newsock, (remhost, remport) = sock.accept()

######### client srv

sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
sock.connect( ('192.168.1.1', 2525) )

############# server udp

sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
sock.bind( ('', 2525) )

################# server tcp

import socket

srvsock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
srvsock.bind( ('', 23000) )
srvsock.listen( 5 )

while 1:
  clisock, (remhost, remport) = srvsock.accept()
  str = clisock.recv(100)
  clisock.send( str )
  clisock.close()

########## connecting 
sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
sock.connect( ('192.168.1.1', 2525) )

sock = socket.socket( socket.AF_INET, sock.sock_DGRAM )
sock.connect( ('192.168.1.1', 2525) )

########## echo stream server
import socket

srvsock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
srvsock.bind( ('', 23000) )
srvsock.listen( 5 )

while 1:

  clisock, (remhost, remport) = srvsock.accept()
  str = clisock.recv(100)
  clisock.send( str )
  clisock.close()

######### client response
import socket

clisock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

clisock.connect( ('', 23000) )

clisock.send("Hello World\n")
print clisock.recv(100)

clisock.close()
############## dgram srv
import socket

dgramSock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
dgramSock.bind( ('', 23000) )

while 1:
#no accept
  msg, (addr, port) = dgramSock.recvfrom( 100 )
  dgramSock.sendto( msg, (addr, port) )
############ client udp
import socket

dgramSock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )

dgramSock.sendto( "Hello World\n", ('', 23000) )
print dgramSock.recv( 100 )
dgramSock.close()
########## options
sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

# Get the size of the socket's send buffer
bufsize = sock.getsockopt( socket.SOL_SOCKET, socket.SO_SNDBUF )

# Get the state of the SO_REUSEADDR option
state = sock.getsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR )

# Enable the SO_REUSEADDR option, up buffer perf
sock.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
########### async io basic fd
rlist, wlist, elist = select.select( [sys.stdin], [], [] )

print sys.stdin.read()
######### socket select
sock1 = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
sock2 = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

sock1.connect( ('192.168.1.1', 25) )
sock2.connect( ('192.168.1.1', 25) )

while 1:

  # Await a read event
  rlist, wlist, elist = select.select( [sock1, sock2], [], [], 5 )

  # Test for timeout
  if [rlist, wlist, elist] == [ [], [], [] ]:

    print "Five seconds elapsed.\n"

  else:

    # Loop through each socket in rlist, read and print the available data
    for sock in rlist:

      print sock.recv( 100 )
############ http lib
import httplib

httpconn = httplib.HTTPConnection("www.ibm.com")

httpconn.request("GET", "/developerworks/index.html")

resp = httpconn.getresponse()

if resp.reason == "OK":

  resp_data = resp.read()

  print resp_data

###### smtp client
import smtplib

fromAdrs = 'mtj@mtjones.com'
toAdrs = 'you@mail.com'
msg = 'From: me@mail.com\r\nTo: you@mail.com\r\nSubject:Hello\r\nHi!\r\n'

mailClient = smtplib.SMTP('192.168.1.1')
mailClient.sendmail( fromAdrs, toAdrs, msg )
mailClient.quit

######### pop3 

import poplib
import re

popClient = poplib.POP3('192.168.1.1')

popClient.user('user')
popClient.pass_('password')

numMsgs, mboxSize = popClient.stat()

print "Number of messages ", numMsgs
print "Mailbox size", mboxSize
print

for id in range (numMsgs):
  for mail in popClient.retr(id+1)[1]:
    if re.search( 'Subject:', mail ):
      print mail

  print


popClient.quit()

###### 