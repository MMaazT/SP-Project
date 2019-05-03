# SP-Project


Introduction
This project is an client-server application in which both the client, and the server are interactive, that is, take input from the user in real-time, without either of them going into block. 

Architecture
The architecture comprises a server, which acts as a connection handler; child processes of the server which act as client handlers; and the clients themselves. When a client asks the server to establish a connection and the server accepts, the server gets forked to produce a client handler which executes all the commands (add, sub, mul, div, run etc) entered by the user sung the client. The connection handler part can list connections(all and by ips). Both connection-handler and client-handler work inside threads.


Help
I have also created a help function in clients which lists down all the implemented commands.

Commands

The commands I have implemented are add, subtract, multipy, divide, run, kill, and help.



