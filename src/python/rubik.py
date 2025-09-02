class Rubik:
    """Rubik class"""

    def __init__(self):
        self.args = []
        self.mixed_cube = []

    def get_args(self):
        return self.args

    def get_mixed_cube(self):
        return self.mixed_cube

    def set_args(self, args):
        self.args = args

    def set_mixed_cube(self, mixed_cube):
        self.mixed_cube = mixed_cube
