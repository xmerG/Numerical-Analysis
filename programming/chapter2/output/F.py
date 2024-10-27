import numpy as np
import matplotlib.pyplot as plt

# 读取输出的多项式系数
def read_polynomials(filename):
    polynomials = []
    with open(filename, 'r') as file:
        lines = file.readlines()
        for i in range(0, len(lines), 2):  # 每两行一组
            x_coefficients = list(map(float, lines[i].strip().split()))
            y_coefficients = list(map(float, lines[i + 1].strip().split()))
            polynomials.append((x_coefficients, y_coefficients))
    return polynomials

# 计算多项式值
def evaluate_polynomial(coefficients, t_values):
    result = np.zeros_like(t_values)
    for i, coeff in enumerate(coefficients):
        result += coeff * (t_values ** i)
    return result

# 绘制曲线
def plot_curves_in_subplot(ax, polynomials, title):
    t_values = np.linspace(0, 1, 100)
    
    for idx, (x_coeff, y_coeff) in enumerate(polynomials):
        x_values = evaluate_polynomial(x_coeff, t_values)
        y_values = evaluate_polynomial(y_coeff, t_values)
        
        ax.plot(x_values, y_values, label=f'Curve {idx+1}')
    
    ax.set_xlabel("x(t)")
    ax.set_ylabel("y(t)")
    ax.set_title(title)
    ax.grid(True)

# 主程序
filenames = ['F_for_n=10.txt', 'F_for_n=30.txt', 'F_for_n=40.txt', 'F_for_n=160.txt']
titles = ['n = 10', 'n=30', 'n = 40', 'n = 160']

fig, axes = plt.subplots(4, 1, figsize=(10, 15))

for i, filename in enumerate(filenames):
    polynomials = read_polynomials(filename)
    plot_curves_in_subplot(axes[i], polynomials, titles[i])

plt.tight_layout()
plt.show()
