from ursina import *
from itertools import product
import tkinter

def display():
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    app = Ursina()
    center = Entity()
    root = tkinter.Tk()

    w, h = root.winfo_screenwidth(), root.winfo_screenheight()

    cubes = []

    window.borderless = False
    window.size = (w / 2, h / 2)
    window.position = (w / 4, h / 4)
    EditorCamera()

    for position in product((-1, 0, 1), repeat=3):
        cubes.append(Entity(model='textures/cube.obj', texture='textures/cube.png', position=position, scale=0.5))

    app.run()