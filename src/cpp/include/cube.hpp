/*!
 *  @file cube.hpp
 *  @brief Header file for the struct representing a Rubik's Cube and its operations.
 *
 *  This file contains the declaration of the Cube struct and methods for
 *  manipulating the state of a Rubik's Cube.
 */

#ifndef CPP_CUBE_HPP
#define CPP_CUBE_HPP

#include <array>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>


/**
 *  @struct Cube
 *  @brief Represents the state of a Rubik's Cube.
 *
 *  Corner indices (0..7):
 *  0 URF, 1 UFL, 2 ULB, 3 UBR, 4 DFR, 5 DLF, 6 DBL, 7 DRB
 *  Edge indices (0..11):
 *  0 UR, 1 UF, 2 UL, 3 UB, 4 DR, 5 DF, 6 DL, 7 DB, 8 FR, 9 FL, 10 BL, 11 BR
 *  E-slice edges are 8..11: FR, FL, BL, BR
 */
struct Cube {
    std::array<uint8_t, 8> cp{};     ///< Corner permutation array.
    std::array<uint8_t, 8> co{};     ///< Corner orientation array (0..2).
    std::array<uint8_t, 12> ep{};    ///< Edge permutation array.
    std::array<uint8_t, 12> eo{};    ///< Edge orientation array (0..1).
};


enum Move : uint8_t {
    U, U2, Up,
    R, R2, Rp,
    F, F2, Fp,
    D, D2, Dp,
    L, L2, Lp,
    B, B2, Bp,
    MOVE_N = 18
}; ///< @brief Enumeration of possible moves on a Rubik's Cube.


static const char* MOVE_STR[MOVE_N] = {
    "U", "U2", "U'",
    "R", "R2", "R'",
    "F", "F2", "F'",
    "D", "D2", "D'",
    "L", "L2", "L'",
    "B", "B2", "B'"
}; ///< @brief String representations of the moves.


Cube    get_solved_cube();              ///< @brief Returns a solved Cube.
bool    isSolved(const Cube& c);        ///< @brief Checks if the Cube is in a solved state.
void    apply_move(Cube &c, Move m);    ///< @brief Applies a single move to the Cube.

Cube    get_mixed_cube(const std::string& moves);   ///< @brief Returns a Cube mixed according to the given validated string of moves.

#endif