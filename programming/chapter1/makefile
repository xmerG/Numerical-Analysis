# 定义编译器
CXX = g++
# 定义编译标志
CXXFLAGS = -Wall -std=c++11

# 定义源文件和目标文件
SRCS = B.cpp C.cpp D.cpp E.cpp F.cpp
OBJS = $(SRCS:.cpp=.o)

# 生成的可执行文件名
TARGET = program

# 规则：生成最终的可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 规则：编译各个.cpp文件
%.o: %.cpp EquationSolver.hpp Function.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理命令
clean:
	rm -f $(OBJS) $(TARGET)
