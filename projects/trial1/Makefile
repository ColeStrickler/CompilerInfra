
BUILD_DIR=./build/
SRC_DIR=./src/
CXX_SRC=$(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS=$(patsubst $(SRC_DIR)%.cpp, $(BUILD_DIR)/%.o,$(CXX_SRC))
TARGET_FILE=dragonlex
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


dev: $(BUILD_DIR)/$(TARGET_FILE)
	chmod +x $^




build:
	


run: dev
	./$(BUILD_DIR)/$(TARGET_FILE) ./spec.l

loc:
	python3 ./lines_of_code.py .

dragonlex.hpp : run
	

dragonlex: dragonlex.hpp test_harness.cpp
	g++ -std=c++17 -o dragonlextest test_harness.cpp -I./

test:  dragonlex
	chmod +x dragonlextest
	./dragonlextest input.test


clean:
	rm -rf ./build/* ./dragonlextest ./dragonlex.hpp