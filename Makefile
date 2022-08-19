CC=gcc

SRC_DIR=src
BUILD_DIR=build

.PHONY: all clean always online local

all: online local

#
# online
#
online: $(BUILD_DIR)/online
$(BUILD_DIR)/tools/fat: always $(TOOLS_DIR)/fat/fat.c
	mkdir -p $(BUILD_DIR)/online
	$(CC) -g -o $(BUILD_DIR)/online/client.cpp $(SRC_DIR)/online/client.cpp

#
# local
#
local: $(BUILD_DIR)/online
$(BUILD_DIR)/tools/fat: always $(BUILD_DIR)/local/main
	mkdir -p $(BUILD_DIR)/local
	$(CC) -g -o $(BUILD_DIR)/local/main $(SRC_DIR)/local/main.cpp
#
# Always
#
always:
	mkdir -p $(BUILD_DIR)

#
# Clean
#
clean:
	$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*
