CC=gcc
CFLAGS=-I. -Wimplicit-function-declaration
#INCLUDE_PATH = ../../inc

TARGET_OBJ = repro
TARGET_DIR = ./
TARGET_SRC = $(TARGET_DIR) 
OBJ_DIR = ../../obj
_OBJ = server.o json_handler.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

#DEPS = $(patsubst %,$(INCLUDE_PATH)/%,$(_DEPS))
DEPS := ../../../inc/serverClient_inc.h  \
         ../../../inc/json_inc.h 

SRC_FILE := ./server.c	\
			../../../JSON_HANDLER/src/json_handler.c\
			../../../JSON_HANDLER/src/json_main.c
	
all: $(OBJ)

$(OBJ): $(SRC_FILE) $(DEPS)
	$(CC) $(CFLAG) $(LIBS) -o $@ $^ 

clean:
	rm -f $(OBJ_DIR)/*.o