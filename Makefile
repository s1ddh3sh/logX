CXX=g++
CXXFLAGS = -Isrc -Wall -pthread
BIN = bin

logger: bin
	g++ $(CXXFLAGS) logX.cpp src/logger.cpp -o $(BIN)/$@

bin:
	@mkdir -p $(BIN)