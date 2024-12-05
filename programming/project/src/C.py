import json
import numpy as np
import matplotlib.pyplot as plt
import os

# 定义函数 f(x) = 1 / (1 + x^2)
def f(x):
    return 1 / (1 + x**2)

# 定义多项式的计算函数（按照升序系数）
def poly(x, coeffs):
    return sum(c * x**idx for idx, c in enumerate(coeffs))

# 读取文件，解析每个 JSON 数据块
filename = 'output_C.txt'
with open(filename, 'r') as file:  
    # 文件中多个 JSON 数据块用换行分隔
    blocks = file.read().strip().split("\n}\n{")
    json_blocks = []
    for block in blocks:
        # 修复分隔导致的 JSON 格式问题
        if not block.startswith("{"):
            block = "{" + block
        if not block.endswith("}"):
            block = block + "}"
        json_blocks.append(json.loads(block))

# 定位到项目根目录并确保 'figure' 文件夹存在
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

if not os.path.exists(figure_dir):
    os.makedirs(figure_dir)

# 逐个解析 JSON 数据块并绘制图像
for idx, data in enumerate(json_blocks):
    # 提取 knots（节点）和 polynomials（多项式系数）
    knots = data["knots"]
    polynomials = data["polynomials"]

    # 生成全局 x 值和 f(x) 的值
    x_values = np.linspace(-5, 5, 1000)
    y_f_values = f(x_values)

    # 创建一个图形
    plt.figure(figsize=(8, 6))

    # 绘制 1/(1 +x^2) 的曲线
    plt.plot(x_values, y_f_values, label=r"$f(x) = \frac{1}{1 + x^2}$", color='blue', linestyle='--')

    # 绘制每个多项式的曲线，仅在对应区间内
    for i, polynomial in enumerate(polynomials):
        # 获取当前多项式的系数
        coefficients = polynomial["coefficients"]

        # 获取当前区间的起始和结束节点
        x_start = knots[i]
        x_end = knots[i + 1]

        # 生成当前区间内的 x 值
        x_interval = np.linspace(x_start, x_end, 100)

        # 计算当前多项式的 y 值
        y_poly_values = np.array([poly(x, coefficients) for x in x_interval])

        # 绘制当前多项式曲线
        plt.plot(x_interval, y_poly_values, label=f"Polynomial {i+1}", linestyle='-', linewidth=1)

    # 设置图形标题和标签
    plt.title(f"Polynomials and f(x) (Data Block {idx+1})")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)

    # 保存当前图形到 'figure' 文件夹
    output_filename = os.path.join(figure_dir, f"C_{idx+1}.png")
    plt.savefig(output_filename)
    plt.close()  # 关闭当前图形，避免重叠

    print(f"Plot for data {idx+1} saved as {output_filename}")
