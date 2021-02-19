CC=g++
CXXFLAGS= -g -Wall -std=c++11 -I./

ifeq ($(PLATFORM),WINDOWS)
CXXFLAGS += -DWINDOWS
LDFLAGS += -lws2_32
endif

SRC=TCPServer.cpp TCPClient.cpp 
OUT=-o libSimpleNetwork.so -fPIC -shared

all:
	$(CC) $(CXXFLAGS) $(SRC) $(LDFLAGS) $(OUT)
	$(CC) -c $(CXXFLAGS) $(SRC) $(LDFLAGS)
	ar rcs libSimpleNetwork.a *.o

clean:
ifeq ($(PLATFORM),WINDOWS)
	del *.a *.so *.o
else
	rm *.a *.so *.o
endif