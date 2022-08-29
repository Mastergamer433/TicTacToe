.PHONY: clean run
CXX := g++
CXXFLAGS :=
LINKFLAGS := 
OBJS_DIR? = obj/
BUILD_DIR? = build/
SOURCE_DIR := .
EXEC := server
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
SOURCE_HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
OBJS := $(patsubst $(SOURCE_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SOURCE_FILES))

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LINKFLAGS) -o $(BUILD_DIR)/$(EXEC)

$(OBJS_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Files to be compiled
$(OBJS_DIR)/main.o: $(SOURCE_FILES) $(SOURCE_HEADERS)

run:
	./build/main
clean:
	rm $(OBJS_DIR)/*.o 
	rm $(OUT_DIR)/$(EXEC)
