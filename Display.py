from ursina import *
from itertools import product
import tkinter


def parent_child_relationship(axis, layer):
    
    for cube in cubes:
        cube.position, cube.rotation = round(cube.world_position, 1), cube.world_rotation
        cube.parent = scene
    
    center.rotation = 0
    
    for cube in cubes:
        if getattr(cube.position, axis) == layer:
            cube.parent = center


def input(key):
    '''Input keys method'''

    if key not in rot_dict: return

    axis, layer, angle = rot_dict[key]

    parent_child_relationship(axis, layer)
    shift = held_keys['shift']
    eval(f'center.animate_rotation_{axis}({-angle if shift else angle}, duration = 0.3)')
    print("Turned")


def display(args):
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    for position in product((-1, 0, 1), repeat=3):
        cubes.append(Entity(model='textures/cube.obj', texture='textures/cube.png', position=position, scale=0.5))

    app.run()


app = Ursina()
center = Entity()
root = tkinter.Tk()

w, h = root.winfo_screenwidth(), root.winfo_screenheight()

cubes = []

window.borderless = False
window.size = (w / 2, h / 2)
window.position = (w / 4, h / 4)
EditorCamera()

rot_dict = {'u': ['y', 1, 90],    'e': ['y', 0, -90],    'd': ['y', -1, -90],
            'l': ['x', -1, -90],  'm': ['x', 0, -90],    'r': ['x', 1, 90],
            'f': ['z', -1, 90],   's': ['z', 0, 90],     'b': ['z', 1, -90]}