CXX=g++
CXXFLAGS=-std=c++17 -Wall -g -I -O3.

INC_DIR = ./headers
SRC_DIR = ./source
OBJ_DIR = ./object

DEPS := $(addprefix $(INC_DIR)/, File.h Util.h Dir.h memory_map.h FileSystem.h Connection.h)
OBJ := $(addprefix $(OBJ_DIR)/, File.o Util.o Dir.o memory_map.o FileSystem.o Connection.o)
SRC := $(addprefix $(SRC_DIR)/, File.cpp Util.cpp Dir.cpp memory_map.cpp FileSystem.cpp Connection.cpp)

LIBS=-lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

default:
	mkdir -p $(OBJ_DIR)

filesystem: $(OBJ) $(OBJ_DIR)/main.o
	$(CXX) -o $@ $^ $(LIBS)

server: $(OBJ) $(OBJ_DIR)/main_server.o
	$(CXX) -o $@ $^ $(LIBS)

client: $(OBJ_DIR)/client.o
	$(CXX) -o $@ $^ $(LIBS)

clean:
	rm -f $(OBJ_DIR)/*.o filesystem *.dat *_out.txt */*_out.txt server client
