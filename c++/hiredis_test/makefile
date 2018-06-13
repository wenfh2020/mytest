CC = gcc 
CXX = g++
CFLAGS = -g -O0 #-Wall
CPPFLAGS = -fPIC  
CXXFLAG = -g -O0 -Wall -ggdb -m64 -Wl,--export-dynamic

ARCH:=$(shell uname -m)        
ARCH32:=i686                   
ARCH64:=x86_64                 
                               
ifeq ($(ARCH),$(ARCH64))
SYSTEM_LIB_PATH:=/usr/lib64    
else                           
SYSTEM_LIB_PATH:=/usr/lib      
endif                          

LIB3RD_PATH = ./l3oss

INC := -I $(LIB3RD_PATH)/include

LDFLAGS := -L$(LIB3RD_PATH)/lib -lhiredis \
           -L$(SYSTEM_LIB_PATH) -lz -ldl -lrt

CPP_SRCS = $(foreach dir, ., $(wildcard $(dir)/*.cpp))
OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS))

TARGET = main
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(INC) $(CXXFLAG) -o $@ $(OBJS) $(LDFLAGS)

%.o:%.cpp
	$(CXX) $(INC) $(CPPFLAGS) $(CFLAGS) $< -c -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
