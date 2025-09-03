import os
import sys
import subprocess
from pathlib import Path
from shutil import which, rmtree

from rubik import Rubik
from utils import *
from Solver import Solver

from ursina import Ursina
from display import build_scene


def ensure_solver_built(exe="../cpp/rubik", make_dir="../cpp", target=None, timeout=300, quiet=False):
    """
    Ensure that the C++ solver executable is built using `make`.
    If the executable does not exist or is out of date, this function will
    invoke `make` in the specified directory to build it.

    @param exe: Path to the expected executable.
    @type exe: str
    @param make_dir: Directory containing the Makefile.
    @type make_dir: str
    @param target: Make target to build. If None, defaults to the executable name.
    @type target: str or None
    @param timeout: Maximum time to wait for the build process, in seconds.
    @type timeout: int
    @param quiet: If True, suppresses output from the build process unless there is an error.
    @type quiet: bool

    @return: None
    """
    exe = Path(exe)
    make_dir = Path(make_dir)
    target = target or exe.name

    make = which("make")
    if not make:
        raise RuntimeError("Could not find `make` on PATH.")

    needs_build = not exe.exists()
    if not needs_build:
        # -q runs make in "question" mode, returning 0 if target is up to date, 1 if it needs to be rebuilt
        rc = subprocess.run([make, "-q", target], cwd=make_dir).returncode
        if rc != 0:
            needs_build = True

    if needs_build:
        cmd = [make, target]
        proc = subprocess.run(
            cmd,
            cwd=make_dir,
            stdout=(subprocess.PIPE if quiet else None),
            stderr=(subprocess.STDOUT if quiet else None),
            timeout=timeout,
            check=False,
        )
        if proc.returncode != 0 or not exe.exists():
            msg = "" if not quiet else (proc.stdout.decode("utf-8", "ignore") if proc.stdout else "")
            raise RuntimeError(f"Build failed (exit {proc.returncode}). {msg}")


def main() -> int:
    """Main function"""

    # Ensure C++ solver is built
    file_path = Path(__file__).resolve().parent
    cpp_path = file_path / "../cpp"
    exe_path = cpp_path / "rubik"
    cpp_str = str(cpp_path.resolve())
    exe_str = str(exe_path.resolve())

    ensure_solver_built(exe=exe_str, make_dir=cpp_str)


    if os.path.exists("./models_compressed/"):
        rmtree("./models_compressed", ignore_errors=True)

    rubik = Rubik()

    try:

        moves = "U R B L D2 F U' R' B' L' D' F'"  # Placeholder for testing without GUI
        solver = Solver(exe_str)
        solution = solver.solve(moves)
        for m in solution:
            print(m, end=" | ")
        moves2 = "wefewfw"  # Placeholder for testing without GUI (ERROR testing)
        solution2 = solver.solve(moves2)
        for m in solution2:
            print(m, end=" | ")
        solver.close()


        rubik.set_args(check_args_validity(len(sys.argv), sys.argv))
        app = Ursina(development_mode=False, title="Rubik")
        build_scene(rubik)
        app.run()

    except Exception as e:
        print("\033[31m", e, "\033[30m")
        return 1

    return 0


if __name__ == "__main__":
    main()

