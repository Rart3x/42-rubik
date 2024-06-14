from ursina import *
from itertools import product


def display():
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    app = Ursina()
    center = Entity()

    cubes = []

    window.borderless = False
    window.size = (800, 800)
    window.position = (2000, 200)
    EditorCamera()

    for position in product((-1, 0, 1), repeat=3):
        cubes.append(Entity(model='textures/cube.obj', texture='textures/cube.png', position=position, scale=0.5))

    app.run()