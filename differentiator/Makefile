CC=g++
CFLAGS=-std=c++17 -g -Wall
LIBS=-lstdc++
SOURCES=node.cpp binarytree.cpp automat.cpp differentiator.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=differentiator

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

clear:
	rm -rf $(EXECUTABLE) *.o

exe: $(EXECUTABLE)
	@echo
	@./$(EXECUTABLE)
	@echo
	@echo



