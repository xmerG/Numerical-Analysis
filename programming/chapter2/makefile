# 定义编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# 定义源文件和对应的可执行文件
SRCS = B.cpp C.cpp D.cpp E.cpp F.cpp
TARGETS = $(SRCS:.cpp=)

# 编译所有的可执行文件
all: $(TARGETS)

# 为每个源文件生成独立的可执行文件
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# 运行所有可执行文件
run: $(TARGETS)
	@for target in $(TARGETS); do \
		echo "Running $$target..."; \
		./$$target; \
	done

# 运行所有 Python 文件（作图）
OUTPUT_DIR = ./output
PY_FILES = $(wildcard $(OUTPUT_DIR)/*.py)

draw:
	@for py_file in $(PY_FILES); do \
		echo "Running $$py_file..."; \
		python3 $$py_file; \
	done

# 生成 PDF 报告
report: report.tex
	pdflatex report.tex

# 清理编译文件
clean:
	rm -f $(TARGETS) *.o *.aux *.log

# 清理所有生成文件（包括 PDF）
clean-all: clean
	rm -f report.pdf

