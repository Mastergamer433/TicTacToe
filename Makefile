CC=gcc

SRC_DIR=src
BUILD_DIR=build

.PHONY: all clean always client

all: client

#
# client
#
client: $(BUILD_DIR)/client
$(BUILD_DIR)/client: always $(SRC_DIR)/client/main.cpp
	mkdir -p $(BUILD_DIR)/client
	$(MAKE) -C $(SRC_DIR)/Client BUILD_DIR=$(abspath $(BUILD_DIR))

#
# Always
#
always:
	mkdir -p $(BUILD_DIR)

#
# Clean
#
clean:
	$(MAKE) -C $(SRC_DIR)/client BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*
