import tkinter

from itertools import product
from ursina import *

from Utils import generate_input


app = Ursina(development_mode=False, title="Rubik")
center = Entity()
root = tkinter.Tk()

cubes = []
duration = 0.1
in_animation = False
w, h = root.winfo_screenwidth(), root.winfo_screenheight()

window.size = (w / 2, h / 2)
window.position = (w  / 4, h  / 4)

EditorCamera()

rot_dict = {
    'f': ['z', -1, 90],  'f\'': ['z', -1, -90],
    'r': ['x', 1, 90],    'r\'': ['x', 1, -90],
    'u': ['y', 1, 90],    'u\'': ['y', 1, -90],
    'b': ['z', 1, -90],   'b\'': ['z', 1, 90],
    'l': ['x', -1, -90],  'l\'': ['x', -1, 90],
    'd': ['y', -1, -90],  'd\'': ['y', -1, 90]
}


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

    if args is None:
        return

    def process_input(index):
        
        global in_animation

        if index >= len(args):
            return

        i = args[index]
        
        if len(i) > 1 and i[1].isdigit() and not in_animation:
            for _ in range(2):

                in_animation = True

                axis, layer, angle = rot_dict[i[0].lower()]
                apply_movement(axis, layer)
                
                shift = held_keys['shift']
                animate_rotation(center, axis, -angle if shift else angle, duration)
                
                invoke(lambda: process_next_input(index), delay=duration + duration / 2)

        elif len(i) > 1 and not i[1].isdigit() and not in_animation:
            
            in_animation = True

            axis, layer, angle = rot_dict[i[0].lower() + i[1]]
            apply_movement(axis, layer)
            
            shift = held_keys['shift']
            animate_rotation(center, axis, -angle if shift else angle, duration)
            
            invoke(lambda: process_next_input(index), delay=duration + duration / 2)
        
        elif not in_animation:
            
            in_animation = True
            
            axis, layer, angle = rot_dict[i[0].lower()]
            apply_movement(axis, layer)
            
            shift = held_keys['shift']
            animate_rotation(center, axis, -angle if shift else angle, duration)
            
            invoke(lambda: process_next_input(index), delay=duration + duration / 2)

    def process_next_input(prev_index):
        
        global in_animation
        in_animation = False
        
        process_input(prev_index + 1)

    process_input(0)


def displayR2(args):
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

    if in_animation:
        return

    if held_keys['1']:
        automatic_input(args_g)
    elif held_keys['space']:
        automatic_input(generate_input(20))
    elif held_keys['escape']:
        exit()

    if key not in rot_dict:
        return

    in_animation = True
    
    axis, layer, angle = rot_dict[key]
    apply_movement(axis, layer)
    
    shift = held_keys['shift']
    animate_rotation(center, axis, -angle if shift else angle, duration)
    
    invoke(end_animation, delay=duration + duration / 2)


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