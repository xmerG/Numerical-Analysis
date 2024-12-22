import json
import matplotlib.pyplot as plt
import os
import numpy as np

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

# 绘制图形并保存为文件
def plot_data(data, filename, figure_dir='figures'):
# 定位到项目根目录并确保 'figure' 文件夹存在
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
    figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

    # 根据文件名判断绘制哪种额外的曲线
    if 'E_curve2.json' in filename:
        # 对应的公式 x(t) = sin(t) + t * cos(t), y(t) = cos(t) - t * sin(t)
        t_values = np.linspace(0, 6 * np.pi, 100)
        x_t = np.sin(t_values) + t_values * np.cos(t_values)
        y_t = np.cos(t_values) - t_values * np.sin(t_values)
    elif 'E_curve1.json' in filename:
        # 对应的公式 x(t) = sqrt(3) * cos(t), y(t) = (sqrt(3) * sin(t) + sqrt(abs(x(t)))) * 2 / 3
        t_values = np.linspace(0, 2 * np.pi, 100)
        x_t = np.sqrt(3) * np.cos(t_values)
        y_t = (np.sqrt(3) * np.sin(t_values) + np.sqrt(np.abs(x_t))) * 2 / 3
    else:
        raise ValueError("Unknown JSON file format!")

    # 每三个数据块为一组，生成一张图片
    for i in range(0, len(data), 3):
        # 创建一个包含3个子图的图形
        fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # 每行3个子图

        # 获取当前三组数据块
        block1 = data[i]
        block2 = data[i + 1] if i + 1 < len(data) else None  # 保护性代码，避免越界
        block3 = data[i + 2] if i + 2 < len(data) else None  # 保护性代码，避免越界

        # 将 boundaryType 和 knotsType 转换为对应的字符串
        boundary1 = boundaryType_dict.get(block1['boundaryType'], 'unknown')
        knots1 = knotsType_dict.get(block1['knotsType'], 'unknown')

        # 提取并绘制第一个数据块
        x_values1 = [point[0] for point in block1['points']]
        y_values1 = [point[1] for point in block1['points']]
        axes[0].plot(x_values1, y_values1, label=f"boundaryType: {boundary1}, knotsType: {knots1}", color='blue')

        # 如果有第二个数据块，将 boundaryType 和 knotsType 转换为字符串
        if block2:
            boundary2 = boundaryType_dict.get(block2['boundaryType'], 'unknown')
            knots2 = knotsType_dict.get(block2['knotsType'], 'unknown')

            # 提取并绘制第二个数据块
            x_values2 = [point[0] for point in block2['points']]
            y_values2 = [point[1] for point in block2['points']]
            axes[1].plot(x_values2, y_values2, label=f"boundaryType: {boundary2}, knotsType: {knots2}", color='green')

        # 如果有第三个数据块，将 boundaryType 和 knotsType 转换为字符串
        if block3:
            boundary3 = boundaryType_dict.get(block3['boundaryType'], 'unknown')
            knots3 = knotsType_dict.get(block3['knotsType'], 'unknown')

            # 提取并绘制第三个数据块
            x_values3 = [point[0] for point in block3['points']]
            y_values3 = [point[1] for point in block3['points']]
            axes[2].plot(x_values3, y_values3, label=f"boundaryType: {boundary3}, knotsType: {knots3}", color='orange')

        # 在每个子图上绘制额外的曲线
        axes[0].plot(x_t, y_t, label="x(t), y(t) curve", color='red', linestyle='--')

        if block2:
            axes[1].plot(x_t, y_t, label="x(t), y(t) curve", color='red', linestyle='--')

        if block3:
            axes[2].plot(x_t, y_t, label="x(t), y(t) curve", color='red', linestyle='--')

        # 设置每个子图的标题和标签
        axes[0].set_title(f"compare 3 types of splines({10*4**((i+1)//12)}) - Bspline")
        axes[0].set_xlabel('X')
        axes[0].set_ylabel('Y')
        axes[0].legend(loc='best')

        if block2:
            axes[1].set_title(f"compare 3 types of splines({10*4**((i+1)//12)}) - ppForm")
            axes[1].set_xlabel('X')
            axes[1].set_ylabel('Y')
            axes[1].legend(loc='best')

        if block3:
            axes[2].set_title(f"compare 3 types of splines({10*4**((i+1)//12)}) - ppForm")
            axes[2].set_xlabel('X')
            axes[2].set_ylabel('Y')
            axes[2].legend(loc='best')

        # 自动调整布局避免重叠
        plt.tight_layout()

        # 保存当前图形为 PNG 文件
        output_filename = os.path.join(figure_dir, f"{filename}_{(i+1)//3+1}.png")
        plt.savefig(output_filename)
        plt.close()  # 关闭当前图形

        print(f"Saved: {output_filename}")

# 主函数
def main():
    filename = 'E_curve2.json'  # 你的 JSON 文件路径
    data = load_json(filename)
    plot_data(data, filename)

    filename1 = 'E_curve1.json'  # 你的 JSON 文件路径
    data = load_json(filename1)
    plot_data(data, filename1)

if __name__ == '__main__':
    main()
