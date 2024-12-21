import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 读取 CSV 文件
filename = 'sphere_curve.csv'
data = pd.read_csv(filename)

# 绘制图形（包括单位球面和曲线）
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# 提取 x, y, z 列（曲线数据）
x = data['x']
y = data['y']
z = data['z']

# 绘制 3D 曲线（将曲线绘制得更细）
ax.plot(x, y, z, c='r', linewidth=1)  # 曲线更细，linewidth 设置为 1

# 绘制单位球面
# 生成球面上的点
u = np.linspace(0, 2 * np.pi, 100)  # u 为经度
v = np.linspace(0, np.pi, 50)       # v 为纬度
x_sphere = np.outer(np.cos(u), np.sin(v))  # 球面上的 X 坐标
y_sphere = np.outer(np.sin(u), np.sin(v))  # 球面上的 Y 坐标
z_sphere = np.outer(np.ones(np.size(u)), np.cos(v))  # 球面上的 Z 坐标

# 绘制球面
ax.plot_surface(x_sphere, y_sphere, z_sphere, color='b', alpha=0.3, rstride=5, cstride=5)  # 设置透明度 alpha=0.3，使球面不遮挡曲线

# 设置标签
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

# 保存图像到文件
output_filename = 'scatter_with_sphere_plot.png'  # 输出文件名
plt.savefig(output_filename)

# 显示图像
plt.show()

print(f"图像已保存为 {output_filename}")
