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

# 绘制图形并保存为文件
def plot_data(data, filename, figure_dir='figures'):
    # 定位到项目根目录并确保 'figure' 文件夹存在
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
    figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

    # 使用 pandas 来处理数据
    dataframes = []
    for block in data:
        points = pd.DataFrame(block['points'], columns=['x', 'y'])
        boundary_type = boundaryType_dict.get(block['boundaryType'], 'unknown')
        knots_type = knotsType_dict.get(block['knotsType'], 'unknown')
        points['boundaryType'] = boundary_type
        points['knotsType'] = knots_type
        dataframes.append(points)

    # 每三个数据块为一组，生成一张图片
    for i in range(0, len(dataframes), 3):
        # 创建一个包含3个子图的图形
        fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # 每行3个子图

        # 获取当前三组数据块
        block1 = dataframes[i]
        block2 = dataframes[i + 1] if i + 1 < len(dataframes) else None  # 保护性代码，避免越界
        block3 = dataframes[i + 2] if i + 2 < len(dataframes) else None  # 保护性代码，避免越界

        # 将 boundaryType 和 knotsType 转换为对应的字符串
        boundary1 = boundaryType_dict.get(block1['boundaryType'].iloc[0], 'unknown')
        knots1 = knotsType_dict.get(block1['knotsType'].iloc[0], 'unknown')

        # 提取并绘制第一个数据块，绘制散点图，点大小为0.8
        axes[0].scatter(block1['x'], block1['y'], label=f"boundaryType: {boundary1}, knotsType: {knots1}", color='blue', s=0.8)

        # 如果有第二个数据块，将 boundaryType 和 knotsType 转换为字符串
        if block2:
            boundary2 = boundaryType_dict.get(block2['boundaryType'].iloc[0], 'unknown')
            knots2 = knotsType_dict.get(block2['knotsType'].iloc[0], 'unknown')

            # 提取并绘制第二个数据块，绘制散点图，点大小为0.8
            axes[1].scatter(block2['x'], block2['y'], label=f"boundaryType: {boundary2}, knotsType: {knots2}", color='green', s=0.8)

        # 如果有第三个数据块，将 boundaryType 和 knotsType 转换为字符串
        if block3:
            boundary3 = boundaryType_dict.get(block3['boundaryType'].iloc[0], 'unknown')
            knots3 = knotsType_dict.get(block3['knotsType'].iloc[0], 'unknown')

            # 提取并绘制第三个数据块，绘制散点图，点大小为0.8
            axes[2].scatter(block3['x'], block3['y'], label=f"boundaryType: {boundary3}, knotsType: {knots3}", color='orange', s=0.8)

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
