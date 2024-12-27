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

# 边界类型的映射
boundary_types = {
    0: "linear",  # no boundary
    1: "periodic",  # periodic boundary
    2: "complete",  # complete boundary
    3: "specified",  # specified boundary
    4: "natural",  # natural boundary
    5: "not_a_knot"  # not_a_knot boundary
}

# 边界条件对应的颜色映射
boundary_colors = {
    0: 'blue',     # linear
    1: 'green',    # periodic
    2: 'red',      # complete
    3: 'purple',   # specified
    4: 'orange',   # natural
    5: 'brown'     # not_a_knot
}

# 读取文件，解析 JSON 数据块
filename = 'output_C.json'
with open(filename, 'r') as file:
    # 读取 JSON 数据块
    json_data = json.load(file)

# 定位到项目根目录并确保 'figure' 文件夹存在
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

if not os.path.exists(figure_dir):
    os.makedirs(figure_dir)

# 逐6组数据绘制图像
for group_idx in range(0, len(json_data), 6):
    fig, ax = plt.subplots(figsize=(10, 8))  # 创建一个大的画布

    # 获取当前组的数据
    group_data = json_data[group_idx: group_idx + 6]

    # 绘制 1/(1 + x^2) 的曲线，使用黑色
    x_values = np.linspace(-5, 5, 1000)
    y_f_values = f(x_values)
    ax.plot(x_values, y_f_values, label=r"$f(x) = \frac{1}{1 + x^2}$", color='black', linestyle='--', linewidth=0.5)

    # 遍历当前组的每个数据块
    for idx, data in enumerate(group_data):
        # 提取 boundary_condition, knots（节点）和 polynomials（多项式系数）
        boundary_condition = data.get("boundary_condition", 0)  # 默认是 0，即 "non"
        knots = data["knots"]
        polynomials = data["polynomials"]

        # 根据 boundary_condition 获取边界类型名称
        boundary_type = boundary_types.get(boundary_condition, "non")

        # 根据 boundary_condition 获取边界条件对应的颜色
        color = boundary_colors.get(boundary_condition, 'red')  

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

            # 仅第一次绘制该边界条件时显示图例
            label = f"{boundary_type}" if i == 0 else None
            ax.plot(x_interval, y_poly_values, label=label, linestyle='-', color=color, linewidth=0.8)

    # 添加图例，网格和标签
    plt.legend(loc='best', fontsize=10)
    plt.grid(True)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(f"Bsplines (starting from {-6 + 0.5 * (group_idx // 6)})")

    # 保存当前图形
    output_filename = os.path.join(figure_dir, f"C_{group_idx // 6 + 1}.png")
    plt.savefig(output_filename)
    plt.close()  # 关闭当前图形

print(f"All plots saved in the 'figure' folder.")
