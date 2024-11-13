import json
import numpy as np
import matplotlib.pyplot as plt

# 读取JSON文件
with open('output.txt', 'r') as f:
    data = json.load(f)

# 提取knots（节点）和polynomials（多项式系数）
knots = data["knots"]
polynomials = data["polynomials"]

# 定义函数f(x) = 1 / (1 + 25 * x^2)
def f(x):
    return 1 / (1 + 25 * x**2)

# 生成x值用于绘图
x_values = np.linspace(-1, 1, 1000)  # 从 -1 到 1，生成1000个点
y_f_values = f(x_values)

# 创建一个图形
plt.figure(figsize=(8, 6))

# 绘制1/(1 + 25x^2)的曲线
plt.plot(x_values, y_f_values, label=r"$f(x) = \frac{1}{1 + 25x^2}$", color='blue', linestyle='--')

# 绘制每个多项式对应的曲线
for i, polynomial in enumerate(polynomials):
    # 获取当前多项式的系数
    coefficients = polynomial["coefficients"]
    
    # 获取当前区间的起始和结束节点
    x_start = knots[i]
    x_end = knots[i + 1]
    
    # 生成当前区间内的x值
    x_interval = np.linspace(x_start, x_end, 100)  # 当前区间生成100个点
    
    # 定义当前多项式的函数
    def poly(x, coeffs):
        return sum(c * x**idx for idx, c in enumerate(coeffs))
    
    # 计算当前多项式的y值
    y_poly_values = np.array([poly(x, coefficients) for x in x_interval])
    
    # 绘制当前多项式曲线
    plt.plot(x_interval, y_poly_values, label=f"Polynomial {i+1}", linestyle='-', linewidth=1)

# 设置图形标题和标签
plt.title("Polynomials and f(x) = 1 / (1 + 25 * x^2)")
plt.xlabel("x")
plt.ylabel("y")


# 显示图形
plt.grid(True)
plt.show()
