
BUILD_DIR=./build/
SRC_DIR=./src/
CXX_SRC=$(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS=$(patsubst $(SRC_DIR)%.cpp, $(BUILD_DIR)/%.o,$(CXX_SRC))
TARGET_FILE=dragonparse
CC=g++
CC_FLAGS=-g -std=c++17
INCLUDES=-I./include/





# Rule to compile each source file into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) $(INCLUDES) -c $< -o $@


# Rule to link object files into executable
$(BUILD_DIR)/$(TARGET_FILE): $(OBJS)
	$(CC) $^ -o $@ $(CC_FLAGS)
	@chmod +x $@



clean:
	rm -rf ./build


run: clean dev
	./$(BUILD_DIR)/$(TARGET_FILE) ./input.grammar ./test.tokens
	dot -Tpng parser.dot -o graph.png
	open graph.png


dev: $(BUILD_DIR)/$(TARGET_FILE)
	chmod +x $^
