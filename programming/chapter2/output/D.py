import numpy as np
import matplotlib.pyplot as plt

# Define the polynomial function
def polynomial(x):
    return (75 + 14.3238*x - 30.2859*x**2 + 22.0325*x**3 - 7.69148*x**4 +
            1.45825*x**5 - 0.15313*x**6 + 0.00832472*x**7 - 0.000182013*x**8)

# Create an array of x values from 0 to 13
x_vals = np.linspace(0, 13, 500)

# Calculate the corresponding y values using the polynomial
y_vals = polynomial(x_vals)

# Plot the polynomial curve
plt.plot(x_vals, y_vals, label='Polynomial', color='blue')

# Plot the horizontal line at y = 81
plt.axhline(y=81, color='green', linestyle='-', label='y = 81')

# Label the graph
plt.title('velocity between 0 to 13 seconds')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()

# Show grid
plt.grid(True)

# Display the plot
plt.show()