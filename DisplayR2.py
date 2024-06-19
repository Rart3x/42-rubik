import tkinter

from itertools import product
from ursina import *

from Utils import expand_double_inputs, generate_input


cubes = []
duration = 0.1
in_animation = False

rot_dict = { 'f': ['z', -1, 90],  'r': ['x', 1, 90],     'u': ['y', 1, 90],
            'b': ['z', 1, -90],  'l': ['x', -1, -90],   'd': ['y', -1, -90],
            'e': ['y', 0, -90],  'm': ['x', 0, -90],    's': ['z', 0, 90],

            'f\'': ['z', -1, -90],'r\'': ['x', 1, -90],  'u\'': ['y', 1, -90],
            'b\'': ['z', 1, 90],  'l\'': ['x', -1, 90],  'd\'': ['y', -1, 90],
            'e\'': ['y', 0, 90],  'm\'': ['x', 0, 90],   's\'': ['z', 0, -90]
}


def apply_movement(axis, layer):
    '''Apply movement function'''

    global cubes

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
    '''Automatic/Pre Input keys method'''

    if args is None:
        return

    modified_args = expand_double_inputs(args)

    def process_input(index):
        
        global in_animation

        if index >= len(modified_args):
            return

        i = modified_args[index]

        if len(i) > 1 and not i[1].isdigit() and not in_animation:
            
            in_animation = True

            axis, layer, angle = rot_dict[i[0].lower() + i[1]]
            apply_movement(axis, layer)
            
            animate_rotation(center, axis, angle, duration)
            
            invoke(lambda: process_next_input(index), delay=duration + duration / 2)
        
        elif not in_animation:
            
            in_animation = True
            
            axis, layer, angle = rot_dict[i[0].lower()]
            apply_movement(axis, layer)
            
            animate_rotation(center, axis, angle, duration)
            
            invoke(lambda: process_next_input(index), delay=duration + duration / 2)

    def process_next_input(prev_index):
        
        global in_animation
        in_animation = False
        
        process_input(prev_index + 1)

    process_input(0)


def displayR2(args):
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    init()

    global args_g, app, cube

    args_g = args

    for position in product((0, 0, 1), repeat=3):
        cubes.append(
            Entity(
                model='textures/cube.obj',
                texture='textures/cube.png',
                position=position,
                scale=0.5
            )
        )

    automatic_input(generate_input(100))
    app.run()


def init():
    '''Init function'''

    global app, center

    app = Ursina(development_mode=False, title="Rubik")
    center = Entity()
    root = tkinter.Tk()

    w, h = root.winfo_screenwidth(), root.winfo_screenheight()

    window.size = (w / 2, h / 2)
    window.position = (w  / 4, h  / 4)

    EditorCamera()


def input(key):
    '''Input keys function'''

    global in_animation

    if held_keys['escape']:
        exit()

    if in_animation:
        return

    if held_keys['1']:
        automatic_input(args_g)
    if held_keys['space']:
        automatic_input(generate_input(100))

    if key not in rot_dict:
        return

    in_animation = True
    
    axis, layer, angle = rot_dict[key]
    apply_movement(axis, layer)
    
    animate_rotation(center, axis, angle, duration)
    
    invoke(end_animation, delay=duration + duration / 2)


def animate_rotation(center, axis, angle, duration):
    '''Animation rotation function, apply good animation'''

    if axis == 'x':
        center.animate('rotation_x', angle, duration=duration)
    elif axis == 'y':
        center.animate('rotation_y', angle, duration=duration)
    elif axis == 'z':
        center.animate('rotation_z', angle, duration=duration)


def end_animation():
    '''End animation function, pass in_animation to False after duration time'''

    global in_animation
    in_animation = False
