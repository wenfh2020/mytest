CXX = g++
CPPFLAGS = -g -O0 -Wall  
CXXFLAG = -O0 -Wall -ggdb -m64 -Wl,--export-dynamic

ARCH:=$(shell uname -m)        
ARCH32:=i686                   
ARCH64:=x86_64                 
                               
ifeq ($(ARCH),$(ARCH64))
SYSTEM_LIB_PATH:=/usr/lib64    
else                           
SYSTEM_LIB_PATH:=/usr/lib      
endif                          

INC := -I .

CPP_SRCS = $(foreach dir, ., $(wildcard $(dir)/*.cpp))
OBJS = $(patsubst %.cpp, %.o, $(CPP_SRCS))

TARGET = main
all: $(TARGET)

#	@echo $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(INC) $(CXXFLAG) -o $@ $(OBJS)

%.o:%.cpp
	$(CXX) $(INC) $(CPPFLAGS) $< -c -o $@

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
