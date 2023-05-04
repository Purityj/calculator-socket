# calculator-socket
A client server program which implements a simple calculator. The calculator can perform the following operations (+, -, *, /) chosen by the user on any two integers supplied by the user. The client server uses iterative connectionless and the iterative connection oriented models.

To run iterative connectionless calculators:

Client:

        gcc iter_connectionless_client.c -o icc
        ./icc
      
Server: 
       
       gcc iter_connectionless_server.c -o ics
       ./ics


To run iterative connection oriented calculators:

Client:

        gcc iter_connection-oriented_client.c -o icoc
        ./icoc
      
Server: 
       
       gcc iter_connection-oriented_server.c -o icos
       ./icos
