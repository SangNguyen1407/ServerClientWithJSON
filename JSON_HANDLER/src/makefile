CC = gcc
INCLUDE_PATH = ../inc
CFLAGS = -I$(INCLUDE_PATH)
LIBS = -lm

TARGET_DIR = $(CURDIR)
OBJ_DIR = ../obj

############
_DEPS = json_inc.h
DEPS = $(patsubst %,$(INCLUDE_PATH)/%,$(_DEPS))

_OBJ = json_handler.o json_main.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

SRC_FILE := ./json_handler.c\
			./json_main.c
	
############
all: $(OBJ)

$(OBJ): $(SRC_FILE) $(DEPS)
	$(CC) $(CFLAG) $(LIBS) -o $@ $^ 

	
clean:
	rm -f $(OBJ_DIR)/*.o