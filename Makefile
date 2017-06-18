SRC = game_exceptions.cpp misc_sdl.cpp tower.cpp enemy.cpp engine.cpp main.cpp game_map.cpp
OBJ = misc_sdl.o game_map.o game_exceptions.o tower.o enemy.o engine.o main.o

CXX = g++
CXXFLAGS = -Wall -pedantic -std=c++11 -Wno-long-long -O0 -ggdb
LDFLAGS = -lSDL2
TESTING_FLAGS = -Wall -pedantic -std=c++11 -fsanitize=address -fno-omit-frame-pointer -Wno-long-long -O0 -ggdb
LINKER_FLAGS = -lSDL2 -fsanitize=address
LD_FLAGS = -lSDL2
OUT_NAME = rauchlad
TEST_NAME = a.out




DOX = Doxyfile
SRC_DIR = ./src
BIN_NAME = rauchlad


%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

compile: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(OUT_NAME)

test: $(SRC: %.cpp = $(SRC_DIR)/%.cpp)
	$(CXX) $(TESTING_FLAGS) $(LINKER_FLAGS) -o $(TEST_NAME)

run: ./$(OUT_NAME)

doc: $(DOX)
	doxygen $(DOX)

all: compile doc

run: $(OUT_NAME)
	./$(OUT_NAME)

clean: 
	rm -rf ./*.o ./doc ./$(OUT_NAME)
