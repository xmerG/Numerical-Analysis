import json
import numpy as np
import matplotlib.pyplot as plt
import os


# 定义多项式的计算函数（按照升序系数）
def poly(x, coeffs):
    return sum(c * x**idx for idx, c in enumerate(coeffs))

# 定义 x(s) 和 y(s) 函数
def x_func(s):
    return np.sin(s) + s * np.cos(s)

def y_func(s):
    return np.cos(s) - s * np.sin(s)

# 读取文件，解析每个 JSON 数据块
filename = 'output_E.txt'
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

    # 创建一个图形
    plt.figure(figsize=(8, 6))

    # 生成全局 s 值并计算 x(s) 和 y(s) 的值
    s_values = np.linspace(0, 6 * np.pi, 1000)
    x_values_func = x_func(s_values)
    y_values_func = y_func(s_values)

    # 绘制真实的 x(s) 和 y(s) 函数曲线
    plt.plot(x_values_func, y_values_func, label="True Curve (x(s), y(s))", color='black', linewidth=0.5)

    # 逐个多项式，计算并绘制每个区间的 (x(t), y(t)) 曲线
    for i in range(0, len(polynomials), 2):  # 每两个多项式为一组
        # 获取当前组的两个多项式系数
        x_coeffs = polynomials[i]["coefficients"]
        y_coeffs = polynomials[i+1]["coefficients"]

        # 获取当前区间的起始和结束节点
        x_start = knots[i//2]
        x_end = knots[i//2 + 1]

        # 生成当前区间内的 t 值
        t_values = np.linspace(x_start, x_end, 100)

        # 计算当前区间内的 x(t) 和 y(t) 值
        x_values_poly = np.array([poly(t, x_coeffs) for t in t_values])
        y_values_poly = np.array([poly(t, y_coeffs) for t in t_values])

        # 绘制当前区间的 (x(t), y(t)) 曲线
        plt.plot(x_values_poly, y_values_poly, label=f"Poly Group {i//2 + 1} (x(t), y(t))", linestyle='-', linewidth=1)

    # 设置图形标题和标签
    plt.title(f"Polynomials and True Curve (Data Block {idx+1})")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)
    # 保存当前图形到 'figure' 文件夹
    output_filename = os.path.join(figure_dir, f"E_{idx+1}.png")
    plt.savefig(output_filename)
    plt.close()  # 关闭当前图形，避免重叠

    print(f"Plot for data block {idx+1} saved as {output_filename}")
