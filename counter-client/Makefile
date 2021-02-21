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
CXXFLAGS += -I ../src/

LDFLAGS += -pthread ../src/libSimpleNetwork.a

ifeq ($(PLATFORM),WINDOWS)
CXXFLAGS += -DWINDOWS
LDFLAGS += -lws2_32
endif

ifeq ($(PLATFORM),LINUX)
UNIT_TEST = unit_test
endif

EXES = client

.PHONY: build clean

build : clean $(UNIT_TEST) $(EXES)

$(EXES): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@  $(LDFLAGS)

$(UNIT_TEST): ut_main.o $(OBJS) $(UT_OBJS)
	$(CXX) $(CXXFLAGS) $(UT_CFLAGS) $^ -o $@  $(LDFLAGS) $(UT_LDFLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) $^ -o $@	

clean :
	$(RM) *.o $(EXES)