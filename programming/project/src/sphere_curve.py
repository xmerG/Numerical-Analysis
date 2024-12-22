import json
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os  # 确保导入了 os 模块

# boundaryType 枚举映射
boundaryType_dict = {
    0: 'non',
    1: 'periodic',
    2: 'complete',
    3: 'specified',
    4: 'natural',
    5: 'not_a_knot'
}

# knotsType 枚举映射
knotsType_dict = {
    0: 'uniform',
    1: 'cumulate_chordal'
}

# 读取 JSON 数据
def load_json(filename):
    with open(filename, 'r') as file:
        return json.load(file)

# 绘制三维单位球面
def plot_unit_sphere(ax):
    # 使用球坐标公式生成球面点
    u = np.linspace(0, np.pi, 100)
    v = np.linspace(0, 2 * np.pi, 100)
    x = np.outer(np.sin(u), np.cos(v))
    y = np.outer(np.sin(u), np.sin(v))
    z = np.outer(np.cos(u), np.ones_like(v))
    
    # 绘制球面
    ax.plot_surface(x, y, z, color='lightgray', alpha=0.3)

# 绘制三维曲线 x(t), y(t), z(t)
def plot_curve(ax, t_values):
    # 定义曲线
    x_t = np.sin(np.cos(t_values)) * np.cos(np.sin(t_values))
    y_t = np.sin(np.cos(t_values)) * np.sin(np.sin(t_values))
    z_t = np.cos(np.cos(t_values))
    
    # 绘制曲线并设置线条宽度为0.5，使曲线变细
    ax.plot(x_t, y_t, z_t, label="x(t), y(t), z(t) curve", color='red', linestyle='--', linewidth=0.5)

# 绘制图形并保存为文件
def plot_data(data, filename, figure_dir='figures'):
    # 定位到项目根目录并确保 'figure' 文件夹存在
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
    figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

    # 使用 pandas 来处理数据
    dataframes = []
    for block in data:
        points = pd.DataFrame(block['points'], columns=['x', 'y', 'z'])
        boundary_type = boundaryType_dict.get(block['boundaryType'], 'unknown')
        knots_type = knotsType_dict.get(block['knotsType'], 'unknown')
        points['boundaryType'] = boundary_type
        points['knotsType'] = knots_type
        dataframes.append(points)

    # 每两组数据为一组，生成一张图片
    for i in range(0, len(dataframes), 2):
        # 创建一个包含2个子图的图形
        fig = plt.figure(figsize=(12, 8))

        # 获取当前两组数据块
        ax1 = fig.add_subplot(121, projection='3d')
        ax2 = fig.add_subplot(122, projection='3d')

        # 提取并绘制第一个数据块
        block1 = dataframes[i]
        # 使用 plot 连接点，并设置 linewidth 变细
        ax1.plot(block1['x'], block1['y'], block1['z'], label=f"boundaryType: {block1['boundaryType'].iloc[0]}, knotsType: {block1['knotsType'].iloc[0]}", color='blue', linewidth=0.5)

        # 提取并绘制第二个数据块（如果有）
        if i + 1 < len(dataframes):
            block2 = dataframes[i + 1]
            # 使用 plot 连接点，并设置 linewidth 变细
            ax2.plot(block2['x'], block2['y'], block2['z'], label=f"boundaryType: {block2['boundaryType'].iloc[0]}, knotsType: {block2['knotsType'].iloc[0]}", color='green', linewidth=0.5)

        # 绘制三维单位球面
        plot_unit_sphere(ax1)
        if i + 1 < len(dataframes):
            plot_unit_sphere(ax2)

        # 绘制额外的三维曲线
        t_values = np.linspace(0, 2 * np.pi, 100)  # t 范围从 0 到 2*pi
        plot_curve(ax1, t_values)
        if i + 1 < len(dataframes):
            plot_curve(ax2, t_values)

        # 设置每个子图的标题和标签
        ax1.set_title(f"2 types of knots({10*2**(i//8)} knots) - cumulate_chordal")
        ax1.set_xlabel('X')
        ax1.set_ylabel('Y')
        ax1.set_zlabel('Z')
        ax1.legend(loc='best')

        if i + 1 < len(dataframes):
            ax2.set_title(f"2 types of knots({10*4**(i//8)} knots) - uniform knots")
            ax2.set_xlabel('X')
            ax2.set_ylabel('Y')
            ax2.set_zlabel('Z')
            ax2.legend(loc='best')

        # 自动调整布局避免重叠
        plt.tight_layout()

        # 保存当前图形为 PNG 文件
        output_filename = os.path.join(figure_dir, f"plot_{i // 2 + 1}.png")
        plt.savefig(output_filename)
        plt.close()  # 关闭当前图形

        print(f"Saved: {output_filename}")

# 主函数
def main():
    filename = 'sphere_curve.json'  # 你的 JSON 文件路径
    data = load_json(filename)
    plot_data(data, filename)

if __name__ == '__main__':
    main()
