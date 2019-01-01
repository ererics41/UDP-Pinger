# UDP-Pinger
UDP is an unreiable transport service meaning UDP does not ensure packets are reliably sent. Packets may get lost due to a variety of
reasons such as router queue overflows and faulty hardware. However, packet loss is still relatively rare so packet loss is simulated on the server side for a ping application. This client-server application mimics the ping application to detect packet loss. A client will send labelled packets to a UDP server written in python that will randomly drop around 30% of packets recieved from the client. The server will then send the packets that are not dropped back to the client and the client will print a report on the packet loss. 
The client will output a message in the form: PING received from machine_name: seq#=X time=Y ms whenever is recieves a packet from the server.
After the client either recieves all 10 ping messages from he server or a 1 second timeout occurs a summary message in the form:
--- ping statistics --- X packets transmitted, Y received, Z% packet loss rtt min/avg/max = MIN AVG
MAX ms 
will be output.

