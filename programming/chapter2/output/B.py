import re
import numpy as np
import matplotlib.pyplot as plt

class PolynomialPlotter:
    def __init__(self, original_function, filenames, x_range=(-5, 5), num_points=500):
        """
        初始化插值绘制器类。

        参数:
        - original_function: 原始函数（传递为一个函数对象，如 lambda x: 1 / (1 + x**2)）
        - filenames: 要读取的插值多项式文件名列表
        - x_range: x 的取值范围，默认为 (-5, 5)
        - num_points: x 轴取样点的数量，默认为 500
        """
        self.original_function = original_function
        self.filenames = filenames
        self.x_vals = np.linspace(x_range[0], x_range[1], num_points)
        self.colors = ['red', 'green', 'blue', 'purple', 'orange']  

    def parse_polynomial(self, poly_str):
        """
        解析多项式字符串并返回系数列表。
        示例格式: "+2.5x^2 -3x^1 +1x^0"
        """
        pattern = re.compile(r'([+-]?\d*\.?\d*)x\^(\d+)')
        terms = pattern.findall(poly_str)

        # 获取最大幂次
        max_degree = max(int(term[1]) for term in terms)
        coefficients = [0.0] * (max_degree + 1)

        # 填充系数
        for coef, power in terms:
            coefficients[int(power)] = float(coef)

        return coefficients

    def read_polynomials(self, filename):
        """
        从文件读取多项式，返回系数列表。
        """
        polynomials = []
        with open(filename, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        # 查找多项式行
        for i, line in enumerate(lines):
            if "插值多项式" in line:
                poly_str = lines[i + 1].strip()
                coefficients = self.parse_polynomial(poly_str)
                polynomials.append(coefficients)

        return polynomials

    def evaluate_polynomial(self, coefficients, x):
        """
        根据系数列表计算给定 x 处的多项式值。
        """
        return sum(coef * (x ** i) for i, coef in enumerate(coefficients))

    def plot_function_and_polynomials(self, filename):
        """
        绘制原函数和指定文件的插值多项式。
        
        参数:
        - filename: 文件路径
        """
        # 绘制原函数
        plt.plot(self.x_vals, self.original_function(self.x_vals), label='origional', color='black')

        # 读取并绘制插值多项式
        polynomials = self.read_polynomials(filename)
        for idx, coeffs in enumerate(polynomials):
            y_poly = [self.evaluate_polynomial(coeffs, x) for x in self.x_vals]
            plt.plot(self.x_vals, y_poly, linestyle='-', color=self.colors[idx % len(self.colors)], 
                     label=f' n={2 * (idx + 1)}')

        # 图形细节
        plt.title(f'Interpolation Polynomial ({filename})')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.legend()
        plt.grid(True)
        plt.show()

    def plot_all(self):
        """
        对所有文件进行绘图，每个文件生成一个独立的图。
        """
        for filename in self.filenames:
            self.plot_function_and_polynomials(filename)

#只需在这里传入原始函数和文件名列表
def main():
    # 定义原始函数
    original_function = lambda x: 1 / (1 + x**2)

    # 文件名列表
    filenames = ['Boutput.txt']

    # 创建绘图器对象并执行绘图
    plotter = PolynomialPlotter(original_function, filenames)
    plotter.plot_all()

if __name__ == "__main__":
    main()
