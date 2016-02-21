PROJECT=test

CC=gcc
CPP=g++
MKDIR=mkdir -p
DEL=rm -rf

CFLAGS=-c -g -W -Wall
LINK_FLAGS=-g

OBJ_DIR=obj
INCLUDE_DIR=-Isrc
OUTPUT=-o

OBJECTS=\
		$(OBJ_DIR)/main.o       \
		$(OBJ_DIR)/simple_c.o   \
		$(OBJ_DIR)/simple_cpp.o


.PHONY: dir_structure

.PHONY: all

all: dir_structure $(PROJECT)

dir_structure: $(OBJ_DIR)

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

$(PROJECT): $(OBJECTS)
	$(CC) $(LINK_FLAGS) $(OBJECTS) $(OUTPUT) $(PROJECT)

$(OBJ_DIR)/main.o: src/main.cpp
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) src/main.cpp $(OUTPUT) $(OBJ_DIR)/main.o

$(OBJ_DIR)/simple_c.o: src/simple_c.c src/simple_c.h
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) src/simple_c.c $(OUTPUT) $(OBJ_DIR)/simple_c.o

$(OBJ_DIR)/simple_cpp.o: src/simple_cpp.cpp src/simple_cpp.h
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) src/simple_cpp.cpp $(OUTPUT) $(OBJ_DIR)/simple_cpp.o

run: $(PROJECT)
	./$(PROJECT)

clean:
	$(DEL) $(OBJ_DIR)
	$(DEL) $(PROJECT)

