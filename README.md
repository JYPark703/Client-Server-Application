# Client-Server-Application

Name: Jin Young Park

I have implemented a client server application that can chat using Windows Sockets programming.

==Description==
Client-Server Application is used by Windows socket programming. It use this programming to create a socket and a structure and assigm a value to hold the components of the socket on each of the server and client. The server then makes a name connection to the socket(bind) and waits for a client connection(listen). On the other hand, the client tries to connect to the server(connect). And, the server accepts the client. After connection, the client sends the information "Client send" to the server. And the server then creates a buffer to receive the information, and prints the information with the time when the information was received. In this way, the server sends information to the client, and the client creates a buffer to receive the information and outputs it with time.
Then the chat between the server and the client takes place. In the case of char, both the server and the client read a stream and send the stream to the other side. It receives the information sent by the other party and outputs it with time. And this loop continues until "/quit"is read. Close the socket after the chat ends.
I defined the server's port number as 4578 and also specified the server IP address on the client side. To use this code elsewhere, you need to change the server IP address.
