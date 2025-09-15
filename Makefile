CXX=g++
CXXFLAGS = -Isrc -Wall -pthread
BIN = bin

logger: bin
	g++ $(CXXFLAGS) logX.cpp src/logger.cpp src/hooks/log_hook.h src/hooks/file_hook.h src/hooks/console_hook.h -o $(BIN)/$@

bin:
	@mkdir -p $(BIN)