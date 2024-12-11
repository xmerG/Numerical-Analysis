import json
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
from mpl_toolkits.mplot3d import Axes3D
import os

# 读取JSON文件
def load_json(filename):
    with open(filename, 'r') as f:
        data = json.load(f)
    return np.array(data)

# 绘制球面
def plot_sphere(ax, radius=1):
    u = np.linspace(0, np.pi, 100)
    v = np.linspace(0, 2 * np.pi, 100)
    x = radius * np.outer(np.cos(u), np.sin(v))
    y = radius * np.outer(np.sin(u), np.sin(v))
    z = radius * np.outer(np.ones(np.size(u)), np.cos(v))
    ax.plot_surface(x, y, z, color='lightblue', alpha=0.3)

# 参数方程生成目标曲线
def target_curve():
    t = np.linspace(0, 2 * np.pi, 100)
    x = np.sin(np.cos(t)) * np.cos(np.sin(t))
    y = np.sin(np.cos(t)) * np.sin(np.sin(t))
    z = np.cos(np.cos(t))
    return x, y, z

# 平滑曲线插值
def smooth_curve(points, num_points=1000):
    # 假设点按索引顺序连接
    x, y, z = points[:, 0], points[:, 1], points[:, 2]
    # 使用1D插值函数进行平滑处理
    interp_x = interp1d(np.arange(len(x)), x, kind='cubic')
    interp_y = interp1d(np.arange(len(y)), y, kind='cubic')
    interp_z = interp1d(np.arange(len(z)), z, kind='cubic')
    
    # 插值生成新的平滑曲线
    smooth_x = interp_x(np.linspace(0, len(x) - 1, num_points))
    smooth_y = interp_y(np.linspace(0, len(y) - 1, num_points))
    smooth_z = interp_z(np.linspace(0, len(z) - 1, num_points))
    
    return smooth_x, smooth_y, smooth_z

# 主函数
def main():
    # 加载数据
    points = load_json("E_spherecurve.txt") 
    
    # 生成目标曲线
    target_x, target_y, target_z = target_curve()
    
    # 平滑曲线插值
    smooth_x, smooth_y, smooth_z = smooth_curve(points)
    
    # 创建3D图形
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # 绘制单位球面
    plot_sphere(ax)
    
    # 绘制目标曲线
    ax.plot(target_x, target_y, target_z, label="Target Curve", color='r', linewidth=0.5)

    # 绘制平滑曲线
    ax.plot(smooth_x, smooth_y, smooth_z, label="Smoothed Curve", color='g', linewidth=0.5)

    ax.legend()
    # 设置标签
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # 获取当前工作目录并构造输出路径
    current_dir = os.path.dirname(os.path.abspath(__file__))  # 获取当前脚本所在的目录

    
    output_dir = os.path.join(current_dir, "..", "figure")  # 目标文件夹路径: 上级目录/figure

    
    # 确保输出目录存在
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 保存图形到figure文件夹
    output_path = os.path.join(output_dir, "sphere_curve.png")
    fig.savefig(output_path, dpi=300)  # 保存为 PNG 格式，300 DPI，确保高清

    print(f"Figure saved to {output_path}")

    # 显示图形
    plt.close()

if __name__ == "__main__":
    main()
