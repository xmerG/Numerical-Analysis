# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -g

# Executables to be generated
EXECUTABLES = program_B program_C program_D program_E program_F

# Source files
SOURCES_B = B.cpp
SOURCES_C = C.cpp
SOURCES_D = D.cpp
SOURCES_E = E.cpp
SOURCES_F = F.cpp

# Object files
OBJECTS_B = $(SOURCES_B:.cpp=.o)
OBJECTS_C = $(SOURCES_C:.cpp=.o)
OBJECTS_D = $(SOURCES_D:.cpp=.o)
OBJECTS_E = $(SOURCES_E:.cpp=.o)
OBJECTS_F = $(SOURCES_F:.cpp=.o)

# All target: builds and runs all programs
all: $(EXECUTABLES) run

# Build each program individually
program_B: $(OBJECTS_B)
	$(CXX) $(CXXFLAGS) -o program_B $(OBJECTS_B)

program_C: $(OBJECTS_C)
	$(CXX) $(CXXFLAGS) -o program_C $(OBJECTS_C)

program_D: $(OBJECTS_D)
	$(CXX) $(CXXFLAGS) -o program_D $(OBJECTS_D)

program_E: $(OBJECTS_E)
	$(CXX) $(CXXFLAGS) -o program_E $(OBJECTS_E)

program_F: $(OBJECTS_F)
	$(CXX) $(CXXFLAGS) -o program_F $(OBJECTS_F)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run all programs after compilation
run: $(EXECUTABLES)
	@echo "Running program_B..."
	./program_B
	@echo "Running program_C..."
	./program_C
	@echo "Running program_D..."
	./program_D
	@echo "Running program_E..."
	./program_E
	@echo "Running program_F..."
	./program_F

# Clean target
clean:
	rm -f *.o $(EXECUTABLES)
