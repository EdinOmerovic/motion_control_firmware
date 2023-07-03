# Import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt

# Read CSV data using semicolon as delimiter
df = pd.read_csv('output.csv', delimiter=';', names=['A', 'B', 'C', 'D', 'E'])


# Assume 'data.csv' has five columns: 'A', 'B', 'C', 'D', 'E'
# Let's plot column 'A' against 'C' as an example.

# Plot data
plt.figure(figsize=(10, 6))
plt.plot(df['A'], df['B'])
plt.plot(df['A'], df['C'])
plt.plot(df['A'], df['D'])
plt.plot(df['A'], df['E'])

df['F'] = abs(df['B'] - df['C'])

# Compute the average of column 'F'
print(df['F'].mean())
# Customize plot
plt.title('My Plot')
plt.xlabel('Samples')
plt.ylabel('Values')

# Show plot
plt.show()
