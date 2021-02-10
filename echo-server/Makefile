CONFIG_DEBUG=y

CXX ?= g++
CXXFLAGS ?= -g -Wall -std=c++11

########################################
###  Add objects according to option ###
########################################

# Main Objects
OBJS = EchoServer.o

ifeq ($(CONFIG_DEBUG),y)
CXXFLAGS += -DCONFIG_DEBUG
endif

# Compile option
CXXFLAGS += -Wall -std=c++11 -I../src/

LDFLAGS += -pthread ../src/libSimpleNetwork.so

EXES = server

.PHONY: build clean

build : clean $(EXES)

$(EXES): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@  $(LDFLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) $^ -o $@	

clean :
	rm -f *.o $(OBJS) $(EXES)

#all: 
#	g++ -Wall -o server server.cpp -I../src/ ../src/TCPServer.cpp ../src/TCPClient.cpp -std=c++11 -lpthread
