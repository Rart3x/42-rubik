import tkinter

from email.policy import default
from itertools import product
from ursina import *

from rubik import Rubik
from utils import decompose_arr_args, expand_double_inputs, generate_input, insert_and_shift, insert_and_shift_arr, reverse_seq


cubes = []
duration = 0.3
in_animation = False

idx, r_seq_len_a, r_seq_len_b = 0, 0, 0
cubes, r_seq, seq = [], [], []


rot_dict = { 'f': ['z', -1, 90],  'r': ['x', 1, 90],     'u': ['y', 1, 90],
             'b': ['z', 1, -90],  'l': ['x', -1, -90],   'd': ['y', -1, -90],
             'e': ['y', 0, -90],  'm': ['x', 0, -90],    's': ['z', 0, 90],

            'f\'': ['z', -1, -90],'r\'': ['x', 1, -90],  'u\'': ['y', 1, -90],
            'b\'': ['z', 1, 90],  'l\'': ['x', -1, 90],  'd\'': ['y', -1, 90],
            'e\'': ['y', 0, 90],  'm\'': ['x', 0, 90],   's\'': ['z', 0, -90]
}


def apply_movement(axis, layer):
    """
    Apply movement function
    Move the cubes to the center entity if they are in the layer to be rotated
    and set their parent to the center entity

    @param axis: axis of rotation
    @param layer: layer to be rotated

    @return: None
    """

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


def animation_sequence(key):
    """Animation sequence function"""

    global in_animation

    in_animation = True

    # Get the axis, layer and angle of the movement from the dictionary
    axis, layer, angle = rot_dict[key]
    
    apply_movement(axis, layer)
    animate_rotation(center, axis, angle, duration)
    invoke(end_animation, delay=duration + duration / 2)


def automatic_input(args):
    '''Pre Input keys method'''

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


def display(rubik: Rubik):
    '''Display 3D Rubik's Cube, manipulating Blender Object'''

    global args_g, app

    rubik.set_mixed_cube(init())
    args_g = rubik.get_args()

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


def init():
    """Init function"""

    global app, center, nbr_field

    app = Ursina(development_mode=False, title="Rubik")
    center = Entity()
    inputs = generate_input(10)
    nbr_field = InputField(y=-.35, limit_content_to='0123456789', active=True)
    root = tkinter.Tk()

    # Create a button to submit the number of movements to mix the cube
    Button(text='Mixing', scale=.1, color=color.cyan.tint(-.4), x=0.30, y=-.35, on_click=submit).fit_to_text()

    w, h = root.winfo_screenwidth(), root.winfo_screenheight()

    window.size = (w / 2, h / 2)
    window.position = (w / 4, h / 4)

    EditorCamera()

    return inputs


def input(key):
    '''Input keys method'''

    global in_animation, idx, seq, r_seq, r_seq_len_a, r_seq_len_b

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
            idx += 1
            animation_sequence(seq[idx])

   # Generate 25 random inputs
    if held_keys['space']:
        inputs = generate_input(25)
        inputs = expand_double_inputs(inputs)
        
        if idx < len(seq) - 1:
            seq = insert_and_shift_arr(seq, idx, inputs)
        
        decompose_arr_args(inputs, seq)
        reverse_seq_update()
        automatic_input(inputs)
        
        idx = len(seq) - 1

    # Use user inputs
    if held_keys['tab']:
        if args_g is not None:
            if idx < len(seq) - 1:
                seq = insert_and_shift_arr(seq, idx, args_g)
            
            decompose_arr_args(args_g, seq)
            reverse_seq_update()
            automatic_input(args_g)
            
            idx = len(seq) - 1

    if key not in rot_dict:
        return

    # Check if the key is in the dictionary
    if key not in rot_dict:
        return

    # Register keyboard inputs
    if idx < len(seq) - 1:
        seq = insert_and_shift(seq, idx, key)
        idx = len(seq) - 1
    else:
        seq.append(key)

    reverse_seq_update()

    if r_seq_len_b < r_seq_len_a:
        idx = r_seq_len_a

    animation_sequence(key)


def animate_rotation(center, axis, angle, duration):
    """Animate rotation function"""

    if axis == 'x':
        center.animate('rotation_x', angle, duration=duration)
    elif axis == 'y':
        center.animate('rotation_y', angle, duration=duration)
    elif axis == 'z':
        center.animate('rotation_z', angle, duration=duration)


def end_animation():
    """End animation function"""

    global in_animation
    in_animation = False


def reverse_seq_update():
    """Reverse sequence update function"""

    global r_seq, r_seq_len_a, r_seq_len_b, idx

    r_seq_len_b = len(r_seq) - 1
    r_seq = reverse_seq(seq)
    r_seq_len_a = len(r_seq) - 1


def submit():
    """Submit method for mixing generator in frontend"""

    global idx, seq

    input_text = nbr_field.text
    
    if not input_text.isdigit():
        return
    
    input_integer = int(input_text)
    nbr_field.text = ""

    if input_integer > 1000:
        return
    
    inputs = generate_input(input_integer)
    inputs = expand_double_inputs(inputs)
    
    if idx < len(seq) - 1:
        seq = insert_and_shift_arr(seq, idx, args_g)
    
    decompose_arr_args(inputs, seq)
    reverse_seq_update()
    automatic_input(inputs)
    
    idx = len(seq) - 1
