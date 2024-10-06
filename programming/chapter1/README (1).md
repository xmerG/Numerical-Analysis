
# 项目概述

本项目包含多个 C++ 源文件和一个 LaTeX 报告。以下是如何编译并运行所有 C++ 程序，以及生成 LaTeX 报告的说明。

## 使用方法

### 1. 编译并运行所有 C++ 文件
要编译并运行所有 C++ 源文件，请使用以下命令：

```bash
make run
```

该命令将会：
- 编译 C++ 源文件（如 `B.cpp`, `C.cpp`, `D.cpp`, `E.cpp`, `F.cpp`）为可执行文件。
- 顺序运行每个已编译的程序。

### 2. 生成 LaTeX PDF 报告
要将 LaTeX 报告文件（`report.tex`）编译为 PDF，请使用以下命令：

```bash
make report
```

该命令将会：
- 使用 `pdflatex`编译 `report.tex` 并生成 PDF 文件。
