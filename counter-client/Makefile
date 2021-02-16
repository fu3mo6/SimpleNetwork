CONFIG_DEBUG=y

CXX ?= g++
CXXFLAGS ?= -g -Wall -std=c++11

UT_CFLAGS = -I /usr/local/include
UT_LDFLAGS = -l gtest -l gmock

########################################
###  Add objects according to option ###
########################################

# Main Objects
OBJS = CountClient.o
UT_OBJS = CountClient_ut.o

ifeq ($(CONFIG_DEBUG),y)
CXXFLAGS += -DCONFIG_DEBUG
endif

# Compile option
CXXFLAGS += -Wall -std=c++11 -I ../src/

LDFLAGS += -pthread ../src/libSimpleNetwork.so

EXES = client
UNIT_TEST = unit_test

.PHONY: build clean

build : clean $(EXES) $(UNIT_TEST)

$(EXES): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@  $(LDFLAGS)

$(UNIT_TEST): ut_main.o $(OBJS) $(UT_OBJS)
	$(CXX) $(CXXFLAGS) $(UT_CFLAGS) $^ -o $@  $(LDFLAGS) $(UT_LDFLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) $^ -o $@	

clean :
	rm -f *.o $(OBJS) $(EXES)

#all: 
#	g++ -Wall -o server server.cpp -I../src/ ../src/TCPServer.cpp ../src/TCPClient.cpp -std=c++11 -lpthread
