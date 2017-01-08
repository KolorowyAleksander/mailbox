###MAILBOX
A simple message broker using BSD-style sockets and TCP goodness

####MAILBOX
is the broker - server which accepts and forwards messages to clients
(also called consumers/recievers/workers)

####POSTMAN
is the delivery guy - a simple client lib which allows to connect to the broker
via TCP and send or recieve messages

#####client-example
is the example client app

######libraries/external dependencies we're using:
-magical c++ date library: https://howardhinnant.github.io/date/date.html

#####caution:
logger will spit out gibberish on non-ansi terminal when printing errors
we may add a setting to configure it later
