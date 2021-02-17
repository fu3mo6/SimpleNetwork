CC=g++
FLAGS= -Wall -std=c++11 -I./ 
SRC=TCPServer.cpp TCPClient.cpp 
OUT=-o libSimpleNetwork.so -fPIC -shared

all:
	$(CC) $(FLAGS) $(SRC) $(OUT)
	$(CC) -c $(FLAGS) $(SRC)
	ar rcs libSimpleNetwork.a *.o

clean:
	rm *.a *.so *.o