CC=gcc
CFLAGS:=-Iinc -Wall $(shell python3-config --includes) -g
LDFLAGS:=-lm $(shell python3-config --embed --ldflags)

BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
SRC_DIR=src
SRC:=$(wildcard $(SRC_DIR)/*.c)
OBJ:=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


TARGET=$(BUILD_DIR)/path


default: $(TARGET)


$(TARGET): $(OBJ)  
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS) 


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c  
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@	


clean:
	rm -f $(TARGET) ; rm -rf $(OBJ_DIR)
