import tkinter
import os

from itertools import product
from ursina import *

from Rubik import Rubik
from Utils import decompose_arr_args, expand_double_inputs, generate_input, reverse_seq

idx = 0
r_seq_len_a = 0
r_seq_len_b = 0

seq = []
r_seq = []

cubes = []

duration = 0.1

in_animation = False

rot_dict = {'f': ['z', -1, 90], 'r': ['x', 1, 90], 'u': ['y', 1, 90],
            'b': ['z', 1, -90], 'l': ['x', -1, -90], 'd': ['y', -1, -90],
            'e': ['y', 0, -90], 'm': ['x', 0, -90], 's': ['z', 0, 90],

            'f\'': ['z', -1, -90], 'r\'': ['x', 1, -90], 'u\'': ['y', 1, -90],
            'b\'': ['z', 1, 90], 'l\'': ['x', -1, 90], 'd\'': ['y', -1, 90],
            'e\'': ['y', 0, 90], 'm\'': ['x', 0, 90], 's\'': ['z', 0, -90]
            }


def apply_movement(axis, layer):
    """Apply movement function"""

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
    """Automatic/Pre Input keys method"""

    if args is None:
        return

    modified_args = expand_double_inputs(args)

    def process_input(index):

        global in_animation

        if index >= len(modified_args):
            return

        i = modified_args[index]

        if len(i) > 1 and not i[1].isdigit() and not in_animation:
            animation_sequence(i[0].lower() + i[1])
            invoke(lambda: process_next_input(index), delay=duration + duration / 2)

        elif not in_animation:
            animation_sequence(i[0].lower())
            invoke(lambda: process_next_input(index), delay=duration + duration / 2)

    def process_next_input(prev_index):

        global in_animation
        in_animation = False

        process_input(prev_index + 1)

    process_input(0)


def display(rubik: Rubik):
    """Display 3D Rubik's Cube, manipulating Blender Object"""

    rubik.set_mixed_cube(init())

    global args_g, app

    args_g = rubik.get_args()

    #Create each Rubik's cube, each cube have every color on each faces, the Rubik's Cube is completed when created
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
    """Input keys function"""

    global in_animation, idx, r_seq, r_seq_len_a, r_seq_len_b

    if held_keys['escape']:
        exit()

    if in_animation:
        return

    # Navigate on normal/reverse movements with keyboard arrows
    if held_keys["left arrow"]:
        if len(r_seq) > 0 and idx - 1 >= -1:
            if len(r_seq) > idx >= 0:
                animation_sequence(r_seq[idx])
                idx -= 1

    if held_keys["right arrow"] and len(seq) > 0:
        if idx < len(seq) - 1:
            animation_sequence(seq[idx])
            idx += 1

    #Use user inputs
    if held_keys['tab']:
        decompose_arr_args(args_g, seq)
        r_seq_len_b = len(r_seq) - 1
        r_seq = reverse_seq(seq)
        r_seq_len_a = len(r_seq) - 1
        automatic_input(args_g)

    #Generate 100 random inputs
    if held_keys['space']:
        inputs = generate_input(100)
        decompose_arr_args(inputs, seq)
        r_seq_len_b = len(r_seq) - 1
        r_seq = reverse_seq(seq)
        r_seq_len_a = len(r_seq) - 1
        automatic_input(inputs)

    if key not in rot_dict:
        return

    #Register keyboard inputs
    seq.append(key)

    r_seq_len_b = len(r_seq) - 1
    r_seq = reverse_seq(seq)
    r_seq_len_a = len(r_seq) - 1

    if r_seq_len_b < r_seq_len_a:
        idx = r_seq_len_a

    animation_sequence(key)


def animation_sequence(key):
    """Animation sequence function"""

    global in_animation

    in_animation = True

    axis, layer, angle = rot_dict[key]
    apply_movement(axis, layer)

    animate_rotation(center, axis, angle, duration)

    invoke(end_animation, delay=duration + duration / 2)


def animate_rotation(center, axis, angle, duration):
    """Animation rotation function, apply good animation"""

    if axis == 'x':
        center.animate('rotation_x', angle, duration=duration)
    elif axis == 'y':
        center.animate('rotation_y', angle, duration=duration)
    elif axis == 'z':
        center.animate('rotation_z', angle, duration=duration)


def end_animation():
    """End animation function, pass in_animation to False after duration time"""

    global in_animation
    in_animation = False


def init():
    """Init function"""

    global app, center, nbr_field

    app = Ursina(development_mode=False, title="Rubik")
    center = Entity()
    root = tkinter.Tk()

    nbr_field = InputField(y=-.35, limit_content_to='0123456789', active=True)

    Button(text='Mixing', scale=.1, color=color.cyan.tint(-.4), x=0.30, y=-.35, on_click=submit).fit_to_text()

    w, h = root.winfo_screenwidth(), root.winfo_screenheight()

    window.size = (w / 2, h / 2)
    window.position = (w / 4, h / 4)

    EditorCamera()

    inputs = generate_input(50)
    # decompose_arr_args(inputs, seq)
    # automatic_input(inputs)


def submit():
    """Submit method for mixing generator in frontend"""

    input_text = nbr_field.text
    input_integer = int(input_text)

    nbr_field.text = ""

    if input_integer > 1000:
        return

    inputs = generate_input(input_integer)
    decompose_arr_args(inputs, seq)
    automatic_input(inputs)
