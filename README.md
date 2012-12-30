armingDB
========

This is an open souce key-value db based on Tokyocabinet. It implements some simple operations for the moment,
there will be more soon.
These operations are as followd:

1. open filename - EX:open db.hdb
2. set key value - EX:set 100 helloworld
3. get key - EX:get 100
4. delete key - EX:delete 100
5. close - leave xm.db
6. help - list cmds info 
7. exit - exit from the program

----------------------------------------------------------------
## add C/S structure.

The server and the client can be on different machines, they can communicate with each other through TCP connection.
It supports the same operations as the above.

***************************************
## Support multithread and I/O multiplexing

Multi-clients can operate on the same Database file, and different clients can open different db files.

**************************************
## Support distribute db server

You can start more than one db servers, and the client saves or gets data from different db servers.
However you must let the clients know all the db server nodes, then you can connect with them.
Our next goal is to support auto-adapt, in that clients can get the server nodes when connecting.
