OBJS_DIR = obj
BUILD_DIR = build
CLIENT    = src/Client
SERVER    = src/Server
all       = $(CLIENT) $(SERVER)

.PHONY: all $(all)
all: $(all)

$(all):
	$(MAKE) --directory=$@ BUILD_DIR=$(abspath $(BUILD_DIR)) OBJS_DIR=$(abspath $(OBJS_DIR))

