# FT_IRC

This project is about creating your own IRC server. <br>
You can use an actual IRC client to connect to your server and test it. <br>

<br>
Here you can read more about the concept https://en.wikipedia.org/wiki/Internet_Relay_Chat <br>
<br>

<h2>USAGE</h2>
Some basic Git commands are:
```
[X] PASS
[X] USER
[X] NICK
[X] VERSION
[X] LUSERS
[X] INFO
[X] MODE  //progress need make test
[X] PRIVMSG
[X] NOTICE
[X] PING
[X] PONG
[X] JOIN //work in error
[X] QUIT
[X] OPER //need test
[X] PART
[X] TOPIC
[X] NAMES // in progress
[X] TIME
[X] INVITE
[X] KICK
[X] HELP
```
<h4>Server</h4>
```
git clone https://github.com/inkognida/ft_irc
cd ft_irc
make
./ircserv port password
```
* port: The port number on which your IRC server will be listening to for incoming
IRC connections. <br>
* password: The connection password. It will be needed by any IRC client that tries
to connect to your server <br>
```
./ircserv 6667 123
```

<h4>Client</h4>
```
nc -c localhost 6667
```

<h4>Bot</h4>
```
cd srcs/bot/
c++ -std=c++98 Bot.cpp && ./a.out | cat -e
```

<h4>Communications </h4>
```
CMD [args]
```
* Internet Relay Chat: Client Protocol: https://www.rfc-editor.org/rfc/rfc2811
* Internet Relay Chat: Channel Management: https://www.rfc-editor.org/rfc/rfc2811

<h3>Resources</h3>
* file.txt: File with intro msg for users after successful registration
* refs: Useful information about sockets and IRC. 
