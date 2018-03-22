import sys
import socket
import struct
import os
import binascii

def tcp_tervehdys (address, port):

	enclist = [None] * 20
	i = 0

	tcpsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	tcpsocket.connect((address, port))
	tcpsocket.sendall('HELLO ENC\r\n')

	while(i<20):
		enclist[i] = str(binascii.b2a_hex(os.urandom(32))) + "\r\n"
		tcpsocket.sendall(enclist[i])
		i = i + 1

	tcpsocket.sendall('.\r\n')
	
	data = tcpsocket.recv(4096)
	templist = data.split("\r\n")
	message,token,udp_port_s = templist[0].split( )
	serverenc = templist[1:21]

	udp_port = int(udp_port_s)
	
	tcpsocket.close()

	udp_yhteys(address, token, udp_port, enclist, serverenc)

	return

def udp_yhteys(address, token, port, clientenc, serverenc):
	
	message = "Hello from"

	encmessage = xorcryption(message,clientenc[0])

	encmessage = encmessage + " " + token + "\r\n"

	ack = True
	eom = False
	length = len(message)
	remaining = 0
	udpsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	y = 0
	x = 1

	while (eom == False):
		sdata = struct.pack('!8s??HH64s', token, ack, eom, remaining, length, encmessage)
		udpsocket.sendto(sdata, (address, port))
		rdata = udpsocket.recv(1024)
		token, ack, eom, remaining, length, received_message = struct.unpack('!8s??HH64s', rdata)
		
		if (eom == False):
			loppu = received_message.count('\x00')
			msg = received_message.strip('\x00')
			encmessage = xorcryption(msg,serverenc[y])
			print encmessage
			y = y + 1
			rlist = encmessage.split( )
			reversed_list = rlist[::-1]
			reversed_message = " ".join(reversed_list)
			fixed_message = reversed_message.ljust(loppu, '\0')
			print fixed_message + "\n"
			encmessage = xorcryption(fixed_message,clientenc[x])
			x = x + 1
			encmessage + "\r\n"
			length = len(encmessage)
		else:
			print(received_message)
		

	udpsocket.close()
	
	return

def xorcryption(message, key):

	return "".join([chr(ord(xor1) ^ ord(xor2)) for (xor1,xor2) in zip(message,key)])



def main():
	USAGE = 'usage: %s <address> <port>' % sys.argv[0]

	try:
		server_address = str(sys.argv[1])
		server_port = int(sys.argv[2])

	except (IndexError, ValueError):
		sys.exit(USAGE)

	tcp_tervehdys(server_address, server_port)



if __name__ == '__main__':
	main()