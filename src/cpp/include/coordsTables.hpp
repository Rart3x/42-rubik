/*!
 *  @file coordsTables.hpp
 *  @brief Header file for coordinate tables used in Rubik's Cube algorithms.
 *
 *  This file contains declarations for precomputed tables that assist in
 *  Rubik's Cube solving algorithms.
 */


#ifndef CPP_COORDSTABLES_HPP
#define CPP_COORDSTABLES_HPP

#include <array>
#include <cstdint>

static const int FACT8 = 40320;     ///< 8!
static const int FACT4 = 24;        ///< 4!
static const int TWIST_N = 2187;    ///< 3^7
static const int FLIP_N  = 2048;    ///< 2^11
static const int SLICE_N = 495;     ///< C(12,4)
static const int MOVE_N = 18;       ///< Number of possible moves


/**
 * @struct CoordTables
 * @brief Precomputed tables for Rubik's Cube coordinates and moves.
 *
 * This struct contains arrays that represent the results of applying each possible move
 * to various cube states, both for phase 1 and phase 2 of the solving algorithm
 *
 * @param twistMove Twist state after each move from any twist state (0..2186).
 * @param flipMove Flip state after each move from any flip state (0..2047).
 * @param sliceMove Slice state after each move from any slice state (0..494).
 * @param cpermMove Corner permutation after each move from any corner permutation (0..40319).
 * @param epermUDMove Edge permutation of UD-slice edges after each move from any edge permutation (0..40319).
 * @param epermEMove Edge permutation of E-slice edges after each move from any edge permutation (0..23).
 */
struct CoordTables {
    // Phase 1
    std::array<std::array<uint16_t, MOVE_N>, TWIST_N> twistMove{};
    std::array<std::array<uint16_t, MOVE_N>, FLIP_N>  flipMove{};
    std::array<std::array<uint16_t, MOVE_N>, SLICE_N> sliceMove{};

    // Phase 2
    std::array<std::array<uint16_t, MOVE_N>, FACT8> cpermMove{};
    std::array<std::array<uint16_t, MOVE_N>, FACT8> epermUDMove{};
    std::array<std::array<uint8_t , MOVE_N>, FACT4> epermEMove{};
};


#endif