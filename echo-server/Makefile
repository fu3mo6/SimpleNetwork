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
CXXFLAGS += -I../src/

LDFLAGS += -pthread ../src/libSimpleNetwork.a

ifeq ($(PLATFORM),WINDOWS)
CXXFLAGS += -DWINDOWS
LDFLAGS += -lws2_32
endif

EXES = server

.PHONY: build clean

build : clean $(EXES)

$(EXES): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@  $(LDFLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) $^ -o $@	

clean :
	$(RM) *.o $(EXES)


#all: 
#	g++ -Wall -o server server.cpp -I../src/ ../src/TCPServer.cpp ../src/TCPClient.cpp -std=c++11 -lpthread
