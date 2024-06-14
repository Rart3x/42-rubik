from ursina import *
from itertools import product

import time
import tkinter


def apply_movement(axis, layer):
    '''Apply movement function'''

    for cube in cubes:
        cube.position, cube.rotation = round(cube.world_position, 1), cube.world_rotation
        cube.parent = scene
    
    center.rotation = 0
    
    for cube in cubes:
        if getattr(cube.position, axis) == layer:
            cube.parent = center


def display(args):
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    for position in product((-1, 0, 1), repeat=3):
        cubes.append(Entity(model='textures/cube.obj', texture='textures/cube.png', position=position, scale=0.5))

    # pre_input(args)
    app.run()


import time

def input(key):
    '''Input keys method'''
    
    if held_keys['escape']:
        os.remove("./models_compressed/cube.bam")
        os.rmdir("./models_compressed")
        exit()

    if key not in rot_dict: 
        return

    axis, layer, angle = rot_dict[key]
    
    apply_movement(axis, layer)

    shift = held_keys['shift']
    eval(f'center.animate_rotation_{axis}({-angle if shift else angle}, duration = {duration})')



def pre_input(args):
    '''Pre Input keys method'''

    for i in args:
        if len(i) > 1 and i[1].isdigit():
            for j in range(2):
                axis, layer, angle = rot_dict[(i[0].lower())]
                apply_movement(axis, layer)
                shift = held_keys['shift']
                eval(f'center.animate_rotation_{axis}({-angle if shift else angle}, duration = {duration})')
        else:
            axis, layer, angle = rot_dict[i[0].lower()]
            apply_movement(axis, layer)
            shift = held_keys['shift']
            eval(f'center.animate_rotation_{axis}({-angle if shift else angle}, duration = {duration})')


app = Ursina()
center = Entity()
root = tkinter.Tk()

w, h = root.winfo_screenwidth(), root.winfo_screenheight()
duration = 0.08

cubes = []

window.borderless = True
window.size = (w / 2, h / 2)
window.position = (w / 4, h / 4)

EditorCamera()

rot_dict = {'u': ['y', 1, 90],    'e': ['y', 0, -90],    'd': ['y', -1, -90],
            'l': ['x', -1, -90],  'm': ['x', 0, -90],    'r': ['x', 1, 90],
            'f': ['z', -1, 90],   's': ['z', 0, 90],     'b': ['z', 1, -90]}