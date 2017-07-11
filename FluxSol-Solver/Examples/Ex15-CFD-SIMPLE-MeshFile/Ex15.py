#90 Degree and 45 degree Lid Driven Cavity

import scipy
import numpy as np

x = np.array( ((2,3), (3, 5)) )
y = np.array( ((1,2), (5, -1)) )
x*y

a = np.array([[3,1], [1,2]])
b = np.array([9,8])
x = np.linalg.solve(a, b)


np.set_printoptions(precision=3)
print(x)