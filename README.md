# **Rubik**

42 school project: create a Rubik’s cube solver with time and move constraints.

---

## **Project Overview**

**Rubik** is a project developed as part of the 42 school curriculum. The goal
of this project is to create a program that can **solve a Rubik's cube** within
specific **time and move constraints**. The project involves implementing algorithms
to manipulate and solve the cube efficiently.  
A 3D visualization of the cube is also provided to enhance the user experience.

## **Objectives**

- Define a Rubik's cube representation.
- Implement algorithm to solve a Rubik's cube within defined time and move limits.
- Create a 3D visualization of the Rubik's cube.

---

## **Project Documentation**

[Auto-generated documentation from Docstrings](https://rart3x.github.io/42-rubik/)

---

## **Setup Project**

To set up the project, follow these steps:
1. **Clone the repository:**
   ```bash
    git clone git@github.com:Rart3x/42-rubik.git rubik
    cd rubik
    ```
2. **Compile:**

   To compile C++ rubik program, run:
   ```bash
    make
   ```
   Or directly run Python 3D visualizer (will compile C++ program if not done yet):
   ```bash
    python src/python/main.py
   ```
---

## **Usage**

### **C++ CLI Program**

To use the C++ command-line interface (CLI) program, run:
```bash
  ./rubik [mix_sequence] [options]
``` 
- `[mix_sequence]`: A string representing the sequence of moves to mix the cube (F R U B L D for Front / Right / Up / Back
/ Left / Down quarter clockwise turns, can add an apostrophe for counter-clockwise turns and a 2 for a half turn).  
If no mix sequence is provided, the program will wait for a user input.
- `[options]`: Optional flags to modify the program's behavior:
  - `-c | --continuous`: Enable continuous solving mode (the program will keep waiting for new mix sequences as user input until user send `QUIT`).
  - `-h | --help`: Display usage information.

**Example:**
```bash
  ./rubik " R2 D’ B’ D F2 R F2 R2 U L’ F2 U’ B’ L2 R" -c
```

### **Python 3D Visualizer**

To use the Python 3D visualizer, run:
```bash
  python src/python/main.py
```
If the C++ program is not compiled yet, it will be compiled automatically.

Then you can use the following commands in the visualizer:
- `drag right mouse button`: Rotate the cube.
- `scroll mouse wheel`: Zoom in/out.
- `R, L, U, D, F, B`: Rotate the corresponding face clockwise
- `Number in input box + click "mixing"`: Mix the cube with random moves for the specified number of times.
- `Click "solving"`: Solve the cube.
- `Click "reset"`: Reset the cube to its initial state.
- `ESC`: Exit the visualizer.

---

## **Implementation**

The project is implemented in **C++** for the core functionality and **Python** for the 3D visualization. 
The C++ program handles the **cube representation, mixing, and solving algorithms**, 
while the Python script provides an interactive **3D interface** using a graphics library.

### **Cube Representation**

The Rubik's cube is represented using 4 different arrays:
- `cp`: Corner Permutation - an array of 8 integers representing the **permutations of the corner pieces**.
- `co`: Corner Orientation - an array of 8 integers representing the **orientations of the corner pieces** (0, 1, or 2).
- `ep`: Edge Permutation - an array of 12 integers representing the **permutations of the edge pieces**.
- `eo`: Edge Orientation - an array of 12 integers representing the **orientations of the edge pieces** (0 or 1).

Corners index are as follows:
```
          U
       2-----3
      /|    /|
     1-----0 |  R
     | |   | |
  L  | 6---|-7   
     |/    |/ 
     5-----4
        D
        
0: URF, 1: UFL, 2: ULB, 3: UBR, 4: DFR, 5: DLF, 6: DBL, 7: DRB
(URF: Up-Right-Front, DBL: Down-Back-Left, etc.)
```

Up and Down edges index are as follows:
```
          U
       .--3--.
      2|    0|
     .--1--. |  R
     | |   | |
  L  | .--7|-.   
     |6    |4 
     .--5--.
        D
        
0: UR, 1: UF, 2: UL, 3: UB, 4: DR, 5: DF, 6: DL, 7: DB
```

E-slice edges index are as follows:
```
          U
       .-----.
      /|    /|
     .-----. 11  R
     | 10   ||
  L  9 .---8-.   
     |/    |/ 
     .-----.
        D
        
8: FR, 9: FL, 10: BL, 11: BR
```


### **Cube Moves**

The cube can be manipulated using the following moves:
- `U`: Up face clockwise
- `U'`: Up face counter-clockwise
- `U2`: Up face 180 degrees
- `D`: Down face clockwise
- `D'`: Down face counter-clockwise
- `D2`: Down face 180 degrees
- `L`: Left face clockwise
- `L'`: Left face counter-clockwise
- `L2`: Left face 180 degrees
- `R`: Right face clockwise
- `R'`: Right face counter-clockwise
- `R2`: Right face 180 degrees
- `F`: Front face clockwise
- `F'`: Front face counter-clockwise
- `F2`: Front face 180 degrees
- `B`: Back face clockwise
- `B'`: Back face counter-clockwise
- `B2`: Back face 180 degrees

Each move updates the `cp`, `co`, `ep`, and `eo` arrays accordingly.

---

### **Solving Algorithm**

The solving algorithm is based on a **two-phase** approach (**Kociemba**'s algorithm):
1. **Phase 1**: Reduce the cube to a state where **all edges and corners** pieces are **correctly oriented**
(but not necessarily in their correct positions) and all **edge pieces from E-slice are in it** (FR, FL, BL, BR. Not necessarily in their correct positions).
2. **Phase 2**: **Solve the cube** from the reduced state to the solved state. Can be done only using **moves that do not change the orientation of pieces
and do not move pieces out of E-slice** (U and its variations, D and its variations, R2, L2, F2, B2).

The algorithm uses **precomputed tables** to efficiently find the optimal solution within the defined constraints.  
We precompute:
- **Move tables** for corners and edges to quickly determine the result of applying a move to a given state.
- **Pruning tables** to estimate the minimum number of moves required to reach the end of phase state from any given state.


**IDA*** (Iterative Deepening A*) search is used to explore possible move sequences.

### **Move Tables**

Move tables are precomputed for both corners and edges to facilitate 
quick state transitions during the solving process.

For Phase 1, we use:
- **Corners Twists Table**: Maps each corners orientations sequence to its new orientations sequence after applying each of the 18 possible moves.
- **Edges Flips Table**: Maps each edges orientations sequence to its new orientations sequence after applying each of the 18 possible moves.
- **Slice Edges Permutations Table**: Maps each E-slice edges permutations sequence to its new permutations sequence after applying each of the 18 possible moves.

For Phase 2, we use:
- **Corners Permutations Table**: Maps each corners permutations sequence to its new permutations sequence after applying each of the 18 possible moves.
- **Edges from U and D Permutations Table**: Maps each edges from U and D permutations sequence to its new permutations sequence after applying each of the 18 possible moves.
- **E-slice Edges Permutations Table**: Maps each E-slice edges permutations sequence to its new permutations sequence after applying each of the 18 possible moves.

Each edge and corner state is mapped as an index in these tables, allowing for O(1) time complexity for state transitions.

### **Pruning Tables**

Pruning tables are used to estimate the minimum number of moves required to reach the goal state from any given state.
These tables help to prune the search space during the IDA* search (2 tables are used for each phase. We take the maximum of the two as heuristic in IDA*).

For Phase 1, we use:
- **Corners Twists/E-slice Pruning Table**: Provides the minimum number of moves required to solve the corner twists and E-slice edges being in E-slice from any given state.
- **Edges Flips/E-slice Pruning Table**: Provides the minimum number of moves required to solve the edge flips and E-slice edges being in E-slice from any given state.

For Phase 2, we use:
- **Corners Permutations/Edges from E Permutations Pruning Table**: Provides the minimum number of moves required to solve the corner permutations and E-slice edges permutations from any given state.
- **Edges from U and D Permutations/Edges from E Permutations Pruning Table**: Provides the minimum number of moves required to solve the edges from U and D permutations and edges from E permutations from any given state.

These tables are generated using a breadth-first search (BFS) approach, starting from the solved state and exploring all possible states up to a certain depth.

### **IDA_star Search**

The IDA* search algorithm is used to find the optimal solution to the Rubik's cube within the defined constraints. 
The algorithm iteratively deepens the search depth until a solution is found or the maximum depth is reached.

The search uses the precomputed move and pruning tables to efficiently explore the state space.
The algorithm maintains a path of moves taken and checks the heuristic value from the pruning tables to decide 
whether to continue exploring a path or prune it.

