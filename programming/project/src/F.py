import os
import pandas as pd
import matplotlib.pyplot as plt

# 定位到项目根目录并确保 'figure' 文件夹存在
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))  # 定位到项目根目录
figure_dir = os.path.join(project_root, "figure")  # figure 文件夹路径

# 确保 figure 文件夹存在
if not os.path.exists(figure_dir):
    os.makedirs(figure_dir)

# 读取 CSV 文件并生成单独的图形
def plot_from_csv(filename):
    # 读取数据
    data = pd.read_csv(filename, header=None)

    # 第一列作为横坐标
    x = data.iloc[:, 0]

    # 为每一列（从第二列开始）生成单独的图形
    for i in range(1, data.shape[1]):
        plt.figure(figsize=(8, 6))

        # 绘制当前列数据
        plt.plot(x, data.iloc[:, i], label=f"Column {i + 1}")

        # 设置图形标题和标签
        plt.title(f"Data from {filename} - Column {i + 1}")
        plt.xlabel("X (First Column)")
        plt.ylabel(f"Y (Column {i + 1})")
        plt.legend(loc='best')

        # 保存图形到 'figure' 文件夹中
        output_filename = os.path.join(figure_dir, f"{os.path.basename(filename)}_Column_{i + 1}.png")
        plt.savefig(output_filename)
        plt.close()


# 绘制 F_n=1.csv 数据
plot_from_csv("F_n=1.csv")

# 绘制 F_n=2.csv 数据
plot_from_csv("F_n=2.csv")
