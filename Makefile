CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

# Source files
THREAD_MANAGER_SRC = src/thread_manager.cpp
THREAD_MANAGER_TEST_SRC = tests/thread_manager_test.cpp
SYNC_TEST_SRC = tests/synchronization_test.cpp

# Object files
THREAD_MANAGER_OBJ = $(THREAD_MANAGER_SRC:.cpp=.o)
THREAD_MANAGER_TEST_OBJ = $(THREAD_MANAGER_TEST_SRC:.cpp=.o)
SYNC_TEST_OBJ = $(SYNC_TEST_SRC:.cpp=.o)

# Executables
THREAD_MANAGER_TEST_EXE = thread_manager_test
SYNC_TEST_EXE = synchronization_test

# Default target
all: $(THREAD_MANAGER_TEST_EXE) $(SYNC_TEST_EXE)

# Compile thread_manager_test
$(THREAD_MANAGER_TEST_EXE): $(THREAD_MANAGER_TEST_OBJ) $(THREAD_MANAGER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile synchronization_test
$(SYNC_TEST_EXE): $(SYNC_TEST_OBJ) $(THREAD_MANAGER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(THREAD_MANAGER_OBJ) $(THREAD_MANAGER_TEST_OBJ) $(SYNC_TEST_OBJ) $(THREAD_MANAGER_TEST_EXE) $(SYNC_TEST_EXE)

.PHONY: all clean 