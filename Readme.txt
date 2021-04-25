In this zip/folder you should see 3 files:
-Readme.txt
-Server.c
-Client.c

The server and client work off of a TCP/IP protocol for a simple file transferring service.
The server and the client should both work on separate machines, however, on the same local network.

To compile the server first you will need to compile the program.  
On a Mac machine, put the files in 2 separate folders or directories.
Next, you will want too cd into those directors and run 2 commands, one for the server and one for the client
Server: gcc -o server server.c
Client: gcc -o client client.c

Next in the terminal, once both have been compiled and there are no errors you will want to run the server in the same directories, using the following command 
Server:./server
CLient:./client
MAKE SURE TO RUN THE SERVER FIRST AND THAT NO OTHER SERVERS ARE RUNNING

After running the server and client, you will want to make your commands through the client so then your commands can be sent to the server.  

There are 4 commands in this project:
1)ls --> This will list all of the files in the clients directory

2)Upload--> This will upload a file to the server directory.  Enter the name of the file you would like to upload to the server

3)Download--> This will download a file from the server into the clients directory. Enter the name of the file located within the server directory

4)bye--> This will close both server and client

The commands are case sensitive please follow them verbatim as listed in the Readme.txt

HAPPY TRANSFERRING! 