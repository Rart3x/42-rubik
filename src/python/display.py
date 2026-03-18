from itertools import product
import tkinter
import time

from ursina import (Entity,
                    Button,
                    InputField,
                    EditorCamera,
                    color,
                    scene,
                    window,
                    held_keys,
                    invoke,
                    Text)

from Solver import Solver
from rubik import Rubik
from utils import (decompose_arr_args,
                   expand_double_inputs,
                   generate_input,
                   insert_and_shift,
                   insert_and_shift_arr,
                   reverse_seq)

center = None
nbr_field = None
args_g = None

solver_timer_text = None
solver_moves_text = None

duration = 0.1
in_animation = False

idx, r_seq_len_a, r_seq_len_b = 0, 0, 0
cubes, r_seq, seq = [], [], []

solver_obj = None

rot_dict = {'f': ['z', -1, 90], 'r': ['x', 1, 90], 'u': ['y', 1, 90],
            'b': ['z', 1, -90], 'l': ['x', -1, -90], 'd': ['y', -1, -90],

            'f\'': ['z', -1, -90], 'r\'': ['x', 1, -90], 'u\'': ['y', 1, -90],
            'b\'': ['z', 1, 90], 'l\'': ['x', -1, 90], 'd\'': ['y', -1, 90],
            }


def build_scene(rubik: Rubik, solver: Solver):
    """
    Build the 3D cube scene and initialize UI elements.

    :param rubik: Rubik cube object
    :param solver: Solver object
    """
    _setup_ui()

    # prime rubik with initial randomized state
    inputs = generate_input(10)
    rubik.set_mixed_cube(inputs)
    global args_g, solver_obj
    args_g = rubik.get_args()
    solver_obj = solver

    # build the 3x3x3 cubelets
    for position in product((-1, 0, 1), repeat=3):
        cubes.append(
            Entity(
                model='textures/cube.obj',
                texture='textures/cube.png',
                position=position,
                scale=0.5
            )
        )


def _setup_ui():
    """
    Setup the user interface: input fields, buttons, and solver stats.
    """
    global center, nbr_field
    global solver_timer_text, solver_moves_text

    center = Entity()
    nbr_field = InputField(y=-.35, limit_content_to='0123456789', active=True)

    # Bouton Mixing
    Button(
        text='Mixing', scale=.1, color=color.cyan.tint(-.4),
        x=0.30, y=-.35, on_click=submit
    ).fit_to_text()

    # Bouton Solve
    Button(
        text='Solve', scale=.1, color=color.azure,
        x=0.45, y=-.35, on_click=solve
    ).fit_to_text()

    solver_timer_text = Text(
        text="Solver time: 0.000000s",
        position=(-0.75, 0.45),
        origin=(0, 0),
        scale=1.2,
        color=color.yellow
    )

    solver_moves_text = Text(
        text="Solver moves: 0",
        position=(-0.80, 0.40),
        origin=(0, 0),
        scale=1.2,
        color=color.yellow
    )

    root = tkinter.Tk()
    root.withdraw()
    w, h = root.winfo_screenwidth(), root.winfo_screenheight()
    root.destroy()

    window.size = (w / 2, h / 2)
    window.position = (w / 4, h / 4)

    EditorCamera()


def solve():
    """
    Solve the current cube sequence, measure solver execution time,
    and update UI with the number of moves and elapsed time.
    """
    global seq, solver_obj, idx
    global solver_timer_text, solver_moves_text
    global in_animation

    if in_animation:
        return

    if solver_obj is None:
        return

    # Only keep moves up to the current index
    seq = seq[:idx + 1]

    if not seq:
        return

    moves_str = " ".join(m.upper() for m in seq)

    try:
        start_time = time.perf_counter()
        solution = solver_obj.solve(moves_str)
        end_time = time.perf_counter()
        elapsed_time = end_time - start_time
    except Exception as e:
        print("Solver error:", e)
        return

    if not solution:
        return

    if isinstance(solution, str):
        solution = solution.split()

    solution = expand_double_inputs(solution)
    solution = [move.lower() for move in solution]

    # Update UI
    solver_timer_text.text = f"Solver time: {elapsed_time:.6f}s"
    solver_moves_text.text = f"Solver moves: {len(solution)}"

    # Append solution properly
    if idx < len(seq) - 1:
        seq = insert_and_shift_arr(seq, idx, solution)
    else:
        seq.extend(solution)

    reverse_seq_update()
    idx = len(seq) - 1

    automatic_input(solution)


def apply_movement(axis, layer):
    """
    Apply a rotation movement on a specific layer of the cube.

    :param axis: Axis of rotation ('x', 'y', or 'z')
    :param layer: Layer index to rotate
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
    """
    Animate a cube movement based on the given key.

    :param key: Rotation key (e.g., 'f', 'r', 'u', etc.)
    """
    global in_animation

    in_animation = True

    axis, layer, angle = rot_dict[key]

    apply_movement(axis, layer)
    animate_rotation(center, axis, angle, duration)
    invoke(end_animation, delay=duration + duration / 2)


def automatic_input(args):
    """
    Automatically process a list of cube moves.

    :param args: List of moves
    """
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
            invoke(lambda: process_next_input(index),
                   delay=duration + duration / 2)

        elif not in_animation:
            in_animation = True
            axis, layer, angle = rot_dict[i[0].lower()]
            apply_movement(axis, layer)
            animate_rotation(center, axis, angle, duration)
            invoke(lambda: process_next_input(index),
                   delay=duration + duration / 2)

    def process_next_input(prev_index):
        global in_animation
        in_animation = False
        process_input(prev_index + 1)

    process_input(0)


def input(key):
    """
    Handle user keyboard input for cube navigation and rotations.

    :param key: Pressed key
    """
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
            new_args_g = [arg.lower() for arg in args_g]

            if idx < len(seq) - 1:
                seq = insert_and_shift_arr(seq, idx, new_args_g)

            decompose_arr_args(new_args_g, seq)
            reverse_seq_update()
            automatic_input(new_args_g)

            idx = len(seq) - 1

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
    """
    Animate rotation of a cube center entity along a specific axis.

    :param center: Center entity to rotate
    :param axis: Axis of rotation ('x', 'y', 'z')
    :param angle: Rotation angle in degrees
    :param duration: Duration of animation
    """
    if axis == 'x':
        center.animate('rotation_x', angle, duration=duration)
    elif axis == 'y':
        center.animate('rotation_y', angle, duration=duration)
    elif axis == 'z':
        center.animate('rotation_z', angle, duration=duration)


def end_animation():
    """Mark the end of a rotation animation"""
    global in_animation
    in_animation = False


def reverse_seq_update():
    """Update the reverse sequence and its lengths"""
    global r_seq, r_seq_len_a, r_seq_len_b, idx

    r_seq_len_b = len(r_seq) - 1
    r_seq = reverse_seq(seq)
    r_seq_len_a = len(r_seq) - 1


def submit():
    """Handle the user mixing input from the interface"""
    global idx, seq

    input_text = nbr_field.text

    if not input_text.isdigit():
        return

    input_integer = int(input_text)
    nbr_field.text = ""

    if input_integer > 1000:
        return

    seq = seq[:idx + 1]

    inputs = generate_input(input_integer)
    inputs = expand_double_inputs(inputs)

    decompose_arr_args(inputs, seq)
    reverse_seq_update()
    automatic_input(inputs)

    idx = len(seq) - 1
