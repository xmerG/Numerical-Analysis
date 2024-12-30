# 定义变量
CPP_FILES = src/A.cpp src/C_and_D.cpp src/E.cpp src/F.cpp
PY_FILES = src/A.py src/C.py src/E.py src/sphere_curve.py src/F.py
TEX_FILES = doc/document.tex doc/report.tex  # TeX 文件

# 生成无扩展名的可执行文件
OUT_FILES = $(CPP_FILES:src/%.cpp=%)

# 编译选项
CXX = g++
CXXFLAGS = -std=c++20 -O2  # 编译选项，包括优化，去掉警告相关选项
LDFLAGS = -llapacke -lblas  # 链接 LAPACK 和 BLAS 库
PYTHON = python3
XELATEX = xelatex  # 使用 xelatex 编译 TeX 文件

# 默认目标: 编译并运行所有C++文件
all: $(OUT_FILES)
	@echo "All C++ programs compiled."
	@for prog in $(OUT_FILES); do \
		./$$prog; \
	done

# 编译 C++ 文件
%: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 目标: 运行所有 Python 文件
py:
	@echo "Running all Python files..."
	@for script in $(PY_FILES); do \
		$(PYTHON) $$script; \
	done

# 目标: 编译所有 TeX 文件
report: $(TEX_FILES)
	@echo "Compiling TeX files with xelatex..."
	@for texfile in $(TEX_FILES); do \
		$(XELATEX) $$texfile; \
		$(XELATEX) $$texfile;  # 运行两遍以处理引用等
	done

# 声明伪目标
.PHONY: all py clean cleanfile report

# 清理目标: 删除所有可执行文件和中间文件
clean:
	@echo "Cleaning up all executables and intermediate files..."
	@rm -f $(OUT_FILES)  # 删除所有可执行文件
	@rm -f doc/*.aux doc/*.log doc/*.out doc/*.toc doc/*.lof doc/*.lot doc/*.bbl doc/*.blg doc/*.xml doc/*.fdb_latexmk doc/*.fls  # 删除所有 LaTeX 中间文件

# 清理 .json 和 .csv 文件
cleanfile:
	@echo "Cleaning up all files..."
	@rm -f *.json *.csv  # 删除所有 JSON 和 CSV 文件

