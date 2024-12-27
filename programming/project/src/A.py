import json
import numpy as np
import matplotlib.pyplot as plt
import os

# 定义函数 f(x) = 1 / (1 + 25 * x^2)
def f(x):
    return 1 / (1 + 25 * x**2)

# 定义多项式的计算函数（按照升序系数）
def poly(x, coeffs):
    return sum(c * x**idx for idx, c in enumerate(coeffs))

# 定义边界类型映射
boundary_type_mapping = {
    0: "linear",
    1: "cubic Periodic spline",
    2: "cubic Complete spline",
    3: "cubic Specified spline",
    4: "cubic Natural spline",
    5: "cubic Not a Knot spline"
}

# 读取文件，解析 JSON 数据块
filename = 'output_A.json'
with open(filename, 'r') as file:  
    data = json.load(file)  # 文件中包含一个 JSON 数据块，包含多个数据组

# 定位到项目根目录并确保 'figure' 文件夹存在
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

if not os.path.exists(figure_dir):
    os.makedirs(figure_dir)

# 定义颜色列表
colors = ['red', 'green', 'blue', 'yellow', 'orange', 'pink', 'cyan', 'brown']

# 按每6组数据画一张图
for idx in range(0, len(data), 6):
    # 创建一个图形
    plt.figure(figsize=(8, 6))

    # 生成全局 x 值和 f(x) 的值
    x_values = np.linspace(-1, 1, 1000)
    y_f_values = f(x_values)

    # 绘制 1/(1 + 25x^2) 的曲线
    plt.plot(x_values, y_f_values, label=r"$f(x) = \frac{1}{1 + 25x^2}$", color='black', linestyle='--')

    # 处理当前组的6个数据块
    for i, data_group in enumerate(data[idx:idx+6]):
        # 提取 knots（节点）和 polynomials（多项式系数）
        knots = data_group["knots"]
        polynomials = data_group["polynomials"]
        
        # 提取边界条件类型
        boundary_conditions = data_group.get("boundary_conditions", None)
        boundary_type = boundary_conditions if boundary_conditions is not None else 0  # 默认类型为 "No Boundary"
        
        # 获取实际边界条件名称
        boundary_condition_name = boundary_type_mapping.get(boundary_type, "Unknown Boundary")
        
        # 获取当前的颜色
        color = colors[i % len(colors)]
        
        # 绘制每个多项式的曲线，仅在对应区间内
        for j, polynomial in enumerate(polynomials):
            # 获取当前多项式的系数
            coefficients = polynomial["coefficients"]

            # 获取当前区间的起始和结束节点
            x_start = knots[j]
            x_end = knots[j + 1]

            # 生成当前区间内的 x 值
            x_interval = np.linspace(x_start, x_end, 100)

            # 计算当前多项式的 y 值
            y_poly_values = np.array([poly(x, coefficients) for x in x_interval])

            label = f"{boundary_condition_name}" if j == 0 else None

            # 绘制当前多项式曲线
            plt.plot(x_interval, y_poly_values, label=label, color=color, linewidth=0.5)

    # 设置图形标题和标签
    plt.title(f"pp-Form - Knots {5*2**(idx//6 +1)}")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)

    # 设置图例并调整字体大小
    plt.legend(loc='best', fontsize=10)  # 修改字体大小

    # 保存当前图形到 'figure' 文件夹，文件名根据起始索引命名
    output_filename = os.path.join(figure_dir, f"A_{idx//6 + 1}.png")
    plt.savefig(output_filename)
    plt.close()  # 关闭当前图形，避免重叠

    print(f"A_{(idx+1)//6+1} saved as {output_filename}")

# 绘制错误图形
filename = 'error_A.json'  # 假设你的文件名是 'error_A.json'
with open(filename, 'r') as file:
    data = json.load(file)  # 读取整个 JSON 数据

# 定位到项目根目录并确保 'figure' 文件夹存在
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

if not os.path.exists(figure_dir):
    os.makedirs(figure_dir)

# 定义颜色列表
colors = ['blue', 'green', 'red', 'purple', 'orange', 'brown', 'pink', 'cyan', 'yellow', 'black']

# 逐6组数据绘制图像
for group_idx in range(0, len(data), 6):
    # 创建一个新的图形
    plt.figure(figsize=(10, 8))
    
    # 获取当前组的6个数据
    group_data = data[group_idx: group_idx + 6]

    # 绘制每组数据，使用不同的颜色
    for idx, item in enumerate(group_data):
        mid = item["mid"]
        error = item["error"]
        
        # 选择当前组的颜色
        color = colors[idx % len(colors)]  # 如果组的数量超过颜色数量，则循环使用颜色
        
        boundary_label = boundary_type_mapping.get(idx, "unknown")  # 使用映射来获取边界名称

        # 绘制折线图，mid 作为横坐标，error 作为纵坐标
        plt.plot(mid, error, label=f"{boundary_label}", marker='o', linestyle='-', color=color)

    # 添加标题和标签
    plt.title(f"Error on Midpoints of subintervals for knots {10*2**(group_idx//6)}")
    plt.xlabel("Mid")
    plt.ylabel("Error")
    plt.grid(True)

    # 显示图例
    plt.legend()

    # 保存当前图形
    output_filename = os.path.join(figure_dir, f"A_error_{group_idx // 6 + 1}.png")
    plt.savefig(output_filename)
    plt.close()  # 关闭当前图形

print("All plots have been saved in the 'figure' folder.")
