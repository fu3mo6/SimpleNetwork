# SimpleNetwork

## Get started:
### Simply type "make" to build all needed binary files

```shell
make
```

## Get started: Counter example

1. Start counter-server by giving a free port number.

```shell
cd counter-server
./server $(PORT_NUMBER)
```

2. Open other terminals to start several counter-client to connect with counter-server.

```shell
cd counter-client
./client 127.0.0.1 $(PORT_NUMBER)
```

3. Check the count status from count-server
