CC=gcc

SRC_DIR?=src
BUILD_DIR?=build

.PHONY: all clean always client

all: client

#
# client
#
client: $(BUILD_DIR)/client
$(BUILD_DIR)/client: always $(SRC_DIR)/client/main.cpp
	mkdir -p $(BUILD_DIR)/client
	$(CC) -g -o $(BUILD_DIR)/client/main $(SRC_DIR)/*.cpp

#
# Always
#
always:
	mkdir -p $(BUILD_DIR)

#
# Clean
#
clean:
	rm -rf $(BUILD_DIR)/main
