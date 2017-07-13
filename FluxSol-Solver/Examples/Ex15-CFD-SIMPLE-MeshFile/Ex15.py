#90 Degree and 45 degree Lid Driven Cavity

import pygmsh as pg
import numpy as np

geom = pg.Geometry()
poly = geom.add_polygon([
    [0.0,   0.5, 0.0],
    [-0.1,  0.1, 0.0],
    [-0.5,  0.0, 0.0],
    [-0.1, -0.1, 0.0],
    [0.0,  -0.5, 0.0],
    [0.1,  -0.1, 0.0],
    [0.5,   0.0, 0.0],
    [0.1,   0.1, 0.0]
    ],
    lcar=0.05
    )

axis = [0, 0, 1]

geom.extrude(
    poly,
    translation_axis=axis,
    rotation_axis=axis,
    point_on_axis=[0, 0, 0],
    angle=2.0 / 6.0 * np.pi
    )

points, cells, point_data, cell_data, field_data = pg.generate_mesh(geom)


import meshio
meshio.write('test.vtu', points, cells, cell_data=cell_data)

x = np.array( ((2,3), (3, 5)) )
y = np.array( ((1,2), (5, -1)) )
x*y

a = np.array([[3,1], [1,2]])
b = np.array([9,8])
x = np.linalg.solve(a, b)


np.set_printoptions(precision=3)
print(x)