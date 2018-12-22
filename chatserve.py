############################################
# Name: Deirdre Moran			   #
# Program: chatserve.py			   #
# Description:  Server side of server/     #
#		client socket chat  	   #
############################################
import socket
import sys

#set first argument as port number 
portNum = int(sys.argv[1])
# create socket stream
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#get hostname and port to create server address
server_address = (socket.gethostbyname(socket.gethostname()), portNum)
#signal for comparing user quit string
quitSig = '\quit'
#set socket options
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# bind the server to the socket
sock.bind(server_address)
# listen for client connection
sock.listen(1)
#while true
while(1):
	#accept connections from client
	connection, client_address = sock.accept()
	#get data sent by client and print
	data = connection.recv(1024)
	print '%s' % data
	# get server response from stdin
	response = raw_input()
	# if server wants to quit, close connection and break
	if response == quitSig:
		connection.sendall(response)
		connection.close()
		break
	#else send response and temporarily close connections
	else:
		connection.sendall(response)	
		connection.close()	
#close connection	
connection.close()
