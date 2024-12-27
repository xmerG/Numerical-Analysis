# 定义变量
CPP_FILES = A.cpp C_and_D.cpp E.cpp F.cpp
PY_FILES = A.py C.py E.py sphere_curve.py F.py
OUT_FILES = $(CPP_FILES:.cpp=)  # 生成无扩展名的可执行文件

# 编译选项
CXX = g++
CXXFLAGS = -std=c++20 -O2  # 编译选项，包括优化，去掉警告相关选项
LDFLAGS = -llapacke -lblas  # 链接 LAPACK 和 BLAS 库
PYTHON = python3

# 默认目标: 编译并运行所有C++文件
all: $(OUT_FILES)
	@echo "All C++ programs compiled."
	@for prog in $(OUT_FILES); do \
		./$$prog; \
	done

# 编译 C++ 文件
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)  

# 目标: 运行所有 Python 文件
py:
	@echo "Running all Python files..."
	@for script in $(PY_FILES); do \
		$(PYTHON) $$script; \
	done

# 声明伪目标
.PHONY: all py clean cleanjson

# 清理目标: 删除所有编译生成的文件
clean:
	@echo "Cleaning up..."
	@rm -f $(OUT_FILES)

# 清理 JSON 文件
cleanjson:
	@echo "Cleaning up all .json files..."
	@rm -f *.json  # 删除所有 JSON 文件

# 清理 csv 文件
cleancsv:
	@echo "Cleaning up all .csv files..."
	@rm -f *.csv
