CC = gcc
TARGET = bmp-editor 
LIB_TARGET = libbmp.a
FLAGS = -fno-common -std=c11 -g -Wall -Werror -lm -Iinclude
AR = ar
ARFLAGS = rcs

SRC_DIR = src
OBJ_DIR = obj

ALL_SOURCES = $(wildcard $(SRC_DIR)/*.c)

CLI_SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/parse.c

LIB_SOURCES = $(filter-out $(CLI_SOURCES), $(ALL_SOURCES))

LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CLI_OBJECTS = $(CLI_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ALL_OBJECTS = $(ALL_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ifeq ($(sanitize), 1)
    FLAGS += -fsanitize=address
    TARGET = bmp-editor_s 
endif

all: $(LIB_TARGET) $(TARGET)

$(LIB_TARGET): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $(LIB_TARGET) $(LIB_OBJECTS)

$(TARGET): $(CLI_OBJECTS) $(LIB_TARGET)
	$(CC) $(CLI_OBJECTS) -L. -lbmp -o $(TARGET) $(FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(FLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) 
	rm -f *.bmp $(TARGET) s_cw $(LIB_TARGET)