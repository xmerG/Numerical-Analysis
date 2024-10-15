import re
import numpy as np
import matplotlib.pyplot as plt

def parse_polynomial(poly_str):
    """
    解析多项式字符串并返回系数列表。
    示例格式: "+2.5x^2 -3x^0"
    """
    # 匹配每个项
    pattern = re.compile(r'([+-]?\d*\.?\d*)x\^(\d+)')
    terms = pattern.findall(poly_str)
    
    # 解析项并存储到列表
    max_degree = max(int(term[1]) for term in terms)
    coefficients = [0.0] * (max_degree + 1)
    
    for coef, power in terms:
        coefficients[int(power)] = float(coef)
    
    return coefficients

def read_polynomials(filename):
    """
    从文件读取多项式，返回系数列表。
    """
    polynomials = []
    with open(filename, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # 查找多项式行
    for i, line in enumerate(lines):
        if "多项式P(x)" in line:
            poly_str = lines[i + 1].strip()
            coefficients = parse_polynomial(poly_str)
            polynomials.append(coefficients)
    
    return polynomials

def evaluate_polynomial(coefficients, x):
    """
    计算多项式的值。
    """
    return sum(coef * (x ** i) for i, coef in enumerate(coefficients))

def main():
    # 读取多项式
    filename = 'C_output.txt'
    polynomials = read_polynomials(filename)

    # 定义x的取值范围
    x_vals = np.linspace(-1, 1, 500)
    original_function = 1 / (1 + 25 * x_vals**2)

    # 绘制原始函数
    plt.plot(x_vals, original_function, label='1/(1+25x^2)', color='black')

    # 设置颜色
    colors = ['red', 'green', 'blue', 'purple']

    # 绘制插值多项式
    for idx, coeffs in enumerate(polynomials):
        y_poly = [evaluate_polynomial(coeffs, x) for x in x_vals]
        plt.plot(x_vals, y_poly, linestyle='--', color=colors[idx % len(colors)], 
                 label=f'n={5 * (idx + 1)}')

    plt.title('interpolation polynomials ')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()
