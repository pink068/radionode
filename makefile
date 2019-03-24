PJBASE=/home/pinkdc/pj
include $(PJBASE)/build.mak

CC=g++
CFLAGS=-c -Wall
LDFLAGS= -Bstatic -lrt -lpthread -lcurl -luuid $(APP_LDLIBS)
SOURCES=main.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run
HEADERS= 

all: $(SOURCES) $(EXECUTABLE) $(HEADERS)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) $(LDFLAGS) -o $@
 
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm *.o
