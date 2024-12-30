# 项目说明

本项目使用 `Makefile` 自动化管理 C++ 程序的编译、Python 脚本的执行、TeX 文档的编译，并提供了清理编译生成文件的功能。通过简单的 `make` 命令，可以完成多个常见任务。

## 目录结构
- `src/` 包含所有C++源代码文件和python文件
- `doc/` 包含所有Latex文件和编译生成的pdf文件
- `figure/` 包含所有生成的图片

## 使用方法

### 1. 编译并运行所有 C++ 文件
要编译并运行所有 C++ 源文件，请使用以下命令：

```bash
make run
```

该命令将会：
- 编译 C++ 源文件（`A.cpp`, `C_and_D.cpp`, `E.cpp`, `F.cpp`）为可执行文件。
- 顺序运行每个已编译的程序。

### 2.运行所有 python 文件
要运行所有python文件，请使用以下命令：

```bash
make py
```
生成的图片会保存在`figure`文件夹中`
### 3. 生成 LaTeX PDF 报告
要将 LaTeX 报告文件（`report.tex`, `document.tex`）编译为 PDF，请使用以下命令：

```bash
make report
```

该命令将会：
- 使用 `xelatex`编译 `.tex` 文件并生成 PDF 文件。

### 4. 清除文件

```bash
make cleanflie
```

清除所有生成的`.json`文件和`.csv`文件，由于文件是用追加模式写入的，所以重新编译C++源文件之前最好删除掉`.json`文件和`.csv`文件


```bash
make clean
```
删除所有运行`.cpp`文件生成的可执行文件和`.tex`文件生成的中间过程文件