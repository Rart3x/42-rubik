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
#include "cube.hpp"

static const int FACT8 = 40320;     ///< 8!
static const int FACT4 = 24;        ///< 4!
static const int TWIST_N = 2187;    ///< 3^7
static const int FLIP_N  = 2048;    ///< 2^11
static const int SLICE_N = 495;     ///< C(12,4)


/**
 * @struct CoordTables
 * @brief Precomputed tables for Rubik's Cube coordinates and moves.
 *
 * This struct contains arrays that represent the results of applying each possible move
 * to various cube states, both for phase 1 and phase 2 of the solving algorithm
 */
struct CoordTables {
    // Phase 1
    std::array<std::array<uint16_t, MOVE_N>, TWIST_N>   twistMove{};  ///< Twist state after each move from any twist state (0..2186).
    std::array<std::array<uint16_t, MOVE_N>, FLIP_N>    flipMove{};   ///< Flip state after each move from any flip state (0..2047).
    std::array<std::array<uint16_t, MOVE_N>, SLICE_N>   sliceMove{};  ///< Slice state after each move from any slice state (0..494).

    // Phase 2
    std::array<std::array<uint16_t, MOVE_N>, FACT8>     cpermMove{};    ///< Corner permutation after each move from any corner permutation (0..40319).
    std::array<std::array<uint16_t, MOVE_N>, FACT8>     epermUDMove{};  ///< Edge permutation of UD-slice edges after each move from any edge permutation (0..40319).
    std::array<std::array<uint8_t, MOVE_N>, FACT4>      epermEMove{};   ///< Edge permutation of E-slice edges after each move from any edge permutation (0..23).
};


/**
 * @brief Builds all coordinate move tables.
 *
 * This function constructs the complete set of coordinate move tables for
 * both phase 1 and phase 2 of the Rubik's Cube solving algorithm. It calls
 * individual functions to build each specific table and aggregates them
 * into a single CoordTables struct.
 *
 * @return A CoordTables struct containing all precomputed move tables.
 */
CoordTables build_coords_tables();


int twist_index(const Cube& c);
int flip_index(const Cube& c);
int slice_index(const Cube& c);
int cperm_index(const Cube& c);
int epermUD_index(const Cube& c);
int epermE_index(const Cube& c);


#endif