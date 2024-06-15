import time
import tkinter

from itertools import product
from ursina import *

from Utils import generate_input

in_animation = False


def apply_movement(axis, layer):
    '''Apply movement function'''

    for cube in cubes:
        cube.position, cube.rotation = (
            round(cube.world_position, 1),
            round(cube.world_rotation, 1)
        )
        cube.parent = scene

    center.rotation = 0

    for cube in cubes:
        if getattr(cube.position, axis) == layer:
            cube.parent = center


def automatic_input(args):
    '''Pre Input keys method'''

    if args == None:
        return

    for i in args:
        if len(i) > 1 and i[1].isdigit():
            for j in range(2):
                axis, layer, angle = rot_dict[(i[0].lower())]
                apply_movement(axis, layer)
                shift = held_keys['shift']
                animate_rotation(center, axis, -angle if shift else angle, duration)
                invoke(end_animation, delay=duration)
        else:
            axis, layer, angle = rot_dict[i[0].lower()]
            apply_movement(axis, layer)
            shift = held_keys['shift']
            animate_rotation(center, axis, -angle if shift else angle, duration)
            invoke(end_animation, delay=duration)


def display(args):
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    global args_g

    args_g = args

    for position in product((-1, 0, 1), repeat=3):
        cubes.append(
            Entity(
                model='textures/cube.obj',
                texture='textures/cube.png',
                position=position,
                scale=0.5
            )
        )

    app.run()


def input(key):
    '''Input keys method'''

    global in_animation

    if held_keys['space']:
        automatic_input(args_g)
    elif held_keys['1']:
        automatic_input(generate_input(10))
    elif held_keys['escape']:
        exit()

    if key not in rot_dict or in_animation:
        return

    axis, layer, angle = rot_dict[key]
    in_animation = True
    apply_movement(axis, layer)
    shift = held_keys['shift']
    animate_rotation(center, axis, -angle if shift else angle, duration)
    invoke(end_animation, delay=duration)


def animate_rotation(entity, axis, angle, duration):
    if axis == 'x':
        entity.animate('rotation_x', angle, duration=duration)
    elif axis == 'y':
        entity.animate('rotation_y', angle, duration=duration)
    elif axis == 'z':
        entity.animate('rotation_z', angle, duration=duration)


def end_animation():
    global in_animation
    in_animation = False


app = Ursina()
center = Entity()
root = tkinter.Tk()

w, h = root.winfo_screenwidth(), root.winfo_screenheight()
duration = 0.5

cubes = []

window.borderless = True
window.size = (w / 2, h / 2)
window.position = (w / 4, h / 4)

b = Button(model='quad', scale=0.15, y=-.4, x=0, color=color.black, text='Scramble', text_size=.5, text_color=color.white)
b.text_size = 1
b.on_click = Sequence(Wait(.5), Func(automatic_input, generate_input(5)), )

EditorCamera()

rot_dict = {'u': ['y', 1, 90],    'e': ['y', 0, -90],    'd': ['y', -1, -90],
            'l': ['x', -1, -90],  'm': ['x', 0, -90],    'r': ['x', 1, 90],
            'f': ['z', -1, 90],   's': ['z', 0, 90],     'b': ['z', 1, -90]}
