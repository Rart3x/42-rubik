/*!
 *  @file cube.cpp
 *  @brief Implementation file for the struct representing a Rubik's Cube and its operations.
 *
 *  This file contains the implementation of the Cube struct, which includes methods for
 *  manipulating the state of a Rubik's Cube.
 */

#include "cube.hpp"
#include <stdexcept>

Cube get_solved_cube() {
    Cube c;
    std::iota(c.cp.begin(), c.cp.end(), 0);
    std::fill(c.co.begin(), c.co.end(), 0);
    std::iota(c.ep.begin(), c.ep.end(), 0);
    std::fill(c.eo.begin(), c.eo.end(), 0);
    return c;
}

bool isSolved(const Cube& c) {
    for (int i = 0; i < 8; i++)
        if (c.cp[i] != i || c.co[i] != 0)
            return false;
    for (int i = 0; i < 12; i++)
        if (c.ep[i] != i || c.eo[i] != 0)
            return false;
    return true;
}

///< @brief Helper function to cycle four elements clockwise (based on cube representation).
static void cycle4(uint8_t &a, uint8_t &b, uint8_t &c, uint8_t &d) {
    const uint8_t t = a;
    a = d;
    d = c;
    c = b;
    b = t; // (a,b,c,d) -> (d,a,b,c)
}

///< @brief Applies a quarter turn move to the Upper face of the cube.
static void Uq(Cube &c) {
    // corners 0,1,2,3 rotate, edges 0,1,2,3 rotate
    cycle4(c.cp[0], c.cp[1], c.cp[2], c.cp[3]);
    cycle4(c.co[0], c.co[1], c.co[2], c.co[3]); // co unchanged
    cycle4(c.ep[0], c.ep[1], c.ep[2], c.ep[3]);
    cycle4(c.eo[0], c.eo[1], c.eo[2], c.eo[3]); // eo unchanged
}

///< @brief Applies a quarter turn move to the Down face of the cube.
static void Dq(Cube &c) {
    // corners 4,5,6,7 rotate, edges 4,5,6,7 rotate
    cycle4(c.cp[7], c.cp[6], c.cp[5], c.cp[4]);
    cycle4(c.co[7], c.co[6], c.co[5], c.co[4]);
    cycle4(c.ep[7], c.ep[6], c.ep[5], c.ep[4]);
    cycle4(c.eo[7], c.eo[6], c.eo[5], c.eo[4]);
}

///< @brief Applies a quarter turn move to the Right face of the cube.
static void Rq(Cube &c) {
    // corners (7,4,0,3). Orientations URF(+1), UBR(+2), DRB(+1), DFR(+2) (mod 3);
    const uint8_t co7=c.co[7];
    const uint8_t co4=c.co[4];
    const uint8_t co0 = c.co[0];
    const uint8_t co3=c.co[3];

    cycle4(c.cp[7], c.cp[4], c.cp[0], c.cp[3]);
    c.co[7] = (co3 + 2) % 3; // new cp[7] came from old 3 (UBR)
    c.co[4] = (co7 + 1) % 3; // new cp[4] came from old 7 (DRB)
    c.co[0] = (co4 + 2) % 3; // new cp[0] came from old 4 (DFR)
    c.co[3] = (co0 + 1) % 3; // new cp[3] came from old 0 (URF)

    // edges (0,11,4,8) rotate; edge orientation unchanged on R
    cycle4(c.ep[11], c.ep[4], c.ep[8], c.ep[0]);
    cycle4(c.eo[11], c.eo[4], c.eo[8], c.eo[0]);
}

///< @brief Applies a quarter turn move to the Left face of the cube.
static void Lq(Cube &c) {
    // corners (5,6,2,1). For L: UFL(+2), ULB(+1), DBL(+2), DLF(+1)
    const uint8_t co5=c.co[5];
    const uint8_t co6=c.co[6];
    const uint8_t co2=c.co[2];
    const uint8_t co1=c.co[1];

    cycle4(c.cp[5], c.cp[6], c.cp[2], c.cp[1]);
    c.co[5] = (co1 + 2) % 3;
    c.co[6] = (co5 + 1) % 3;
    c.co[2] = (co6 + 2) % 3;
    c.co[1] = (co2 + 1) % 3;

    // edges (9,6,10,2) rotate; edge orientation unchanged on L
    cycle4(c.ep[9], c.ep[6], c.ep[10], c.ep[2]);
    cycle4(c.eo[9], c.eo[6], c.eo[10], c.eo[2]);
}

///< @brief Applies a quarter turn move to the Front face of the cube.
static void Fq(Cube &c) {
    // corners (4,5,1,0). For F: URF(+2), UFL(+1), DLF(+2), DFR(+1)
    const uint8_t co4=c.co[4];
    const uint8_t co5=c.co[5];
    const uint8_t co1=c.co[1];
    const uint8_t co0=c.co[0];

    cycle4(c.cp[4], c.cp[5], c.cp[1], c.cp[0]);
    c.co[4] = (co0 + 2) % 3;
    c.co[5] = (co4 + 1) % 3;
    c.co[1] = (co5 + 2) % 3;
    c.co[0] = (co1 + 1) % 3;

    // edges (8,5,9,1) rotate and flip (XOR 1 equivalent to "+ 1 mod 2")
    const uint8_t e8=c.eo[8];
    const uint8_t e5=c.eo[5];
    const uint8_t e9=c.eo[9];
    const uint8_t e1=c.eo[1];
    cycle4(c.ep[8], c.ep[5], c.ep[9], c.ep[1]);
    c.eo[8] = e1 ^ 1;
    c.eo[5] = e8 ^ 1;
    c.eo[9] = e5 ^ 1;
    c.eo[1] = e9 ^ 1;
}

///< @brief Applies a quarter turn move to the Back face of the cube.
static void Bq(Cube &c) {
    // corners (6,7,3,2)
    const uint8_t co6=c.co[6];
    const uint8_t co7=c.co[7];
    const uint8_t co3=c.co[3];
    const uint8_t co2=c.co[2];

    cycle4(c.cp[6], c.cp[7], c.cp[3], c.cp[2]);
    c.co[6] = (co2 + 2) % 3;
    c.co[7] = (co6 + 1) % 3;
    c.co[3] = (co7 + 2) % 3;
    c.co[2] = (co3 + 1) % 3;

    // edges (10,7,11,3) rotate and flip (XOR 1 equivalent to "+ 1 mod 2")
    const uint8_t e10=c.eo[10];
    const uint8_t e7=c.eo[7];
    const uint8_t e11=c.eo[11];
    const uint8_t e3=c.eo[3];
    cycle4(c.ep[10], c.ep[7], c.ep[11], c.ep[3]);
    c.eo[10] = e3 ^ 1;
    c.eo[7] = e10 ^ 1;
    c.eo[11] = e7 ^ 1;
    c.eo[3] = e11 ^ 1;
}

void apply_move(Cube &c, Move m) {
    switch (m) {
        case U:  Uq(c); break;  case U2: Uq(c); Uq(c); break;   case Up: Uq(c); Uq(c); Uq(c); break;
        case R:  Rq(c); break;  case R2: Rq(c); Rq(c); break;   case Rp: Rq(c); Rq(c); Rq(c); break;
        case F:  Fq(c); break;  case F2: Fq(c); Fq(c); break;   case Fp: Fq(c); Fq(c); Fq(c); break;
        case D:  Dq(c); break;  case D2: Dq(c); Dq(c); break;   case Dp: Dq(c); Dq(c); Dq(c); break;
        case L:  Lq(c); break;  case L2: Lq(c); Lq(c); break;   case Lp: Lq(c); Lq(c); Lq(c); break;
        case B:  Bq(c); break;  case B2: Bq(c); Bq(c); break;   case Bp: Bq(c); Bq(c); Bq(c); break;
        default: break;
    }
}

///< @brief Parses a string of moves into a vector of Move enums.
static std::vector<Move> parse_moves(const std::string& moves) {
    std::vector<Move> result;
    size_t i = 0;
    while (i < moves.size()) {
        if (moves[i] == ' ') {
            i++;
            continue;
        }
        std::string move_str;
        move_str += moves[i++];
        if (i < moves.size() && (moves[i] == '2' || moves[i] == '\'')) {
            move_str += moves[i++];
        }
        bool found = false;
        for (int m = 0; m < MOVE_N; m++) {
            if (move_str == MOVE_STR[m]) {
                result.push_back(static_cast<Move>(m));
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::invalid_argument("Invalid move: " + move_str);
        }
    }
    return result;
}

Cube get_mixed_cube(const std::string& moves) {
    const std::vector<Move> moves_list = parse_moves(moves);
    Cube c = get_solved_cube();
    for (const Move m : moves_list) {
        apply_move(c, m);
    }
    return c;
}