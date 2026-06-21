CC = gcc
TARGET = bmp-editor 
FLAGS = -fno-common -std=c11 -g -Wall -Werror -lpng -lm -Iinclude
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ifeq ($(sanitize), 1)
	FLAGS += -fsanitize=address
	TARGET = s_cw
endif

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(FLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) 
	rm -f *.bmp $(TARGET) s_cw