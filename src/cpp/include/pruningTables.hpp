/*!
 *  @file pruningTables.hpp
 *  @brief Header file for pruning tables used in Rubik's Cube algorithms.
 *
 *  This file contains declarations for precomputed pruning tables that assist in
 *  Rubik's Cube solving algorithms.
 */

#ifndef CPP_PRUNINGTABLES_HPP
#define CPP_PRUNINGTABLES_HPP

#include <vector>
#include <cstdint>
#include "coordsTables.hpp"
#include "cube.hpp"


/**
 * @struct DepthTable
 * @brief A simple depth table for storing depth information for pruning.
 *
 * This struct represents a depth table used in pruning algorithms for Rubik's Cube solving.
 * It contains a vector of 8-bit unsigned integers to store depth values and provides methods
 * to get and set these values. The table is initialized with a specified size, and unvisited
 * entries are marked with 0xFF.
 */
struct DepthTable { // 0xFF = unvisited
    std::vector<uint8_t> a;
    int N;
    explicit DepthTable(const int n): a(n, 0xFF), N(n) {}
    uint8_t get(const int i) const { return a[i]; }
    void set(const int i, const uint8_t v){ a[i] = v; }
};

/**
 * @struct PrunP1
 * @brief Phase 1 pruning tables for Rubik's Cube solving.
 *
 * This struct contains two 2D pruning tables used in phase 1 of the solving algorithm:
 * one for the combination of slice and twist coordinates, and another for the combination
 * of slice and flip coordinates.
 */
struct PrunP1 {
    DepthTable slice_twist; ///< Pruning table for (slice, twist) coordinates.
    DepthTable slice_flip;  ///< Pruning table for (slice, flip) coordinates.
    PrunP1(): slice_twist(SLICE_N * TWIST_N), slice_flip(SLICE_N * FLIP_N) {}   ///< @brief Constructor initializing the pruning tables for P1.
};


/**
 * @struct PrunP2
 * @brief Phase 2 pruning tables for Rubik's Cube solving.
 *
 * This struct contains two 2D pruning tables used in phase 2 of the solving algorithm:
 * one for the combination of edge permutation of E-slice edges and corner permutation,
 * and another for the combination of edge permutation of E-slice edges and edge permutation
 * of UD-slice edges.
 */
struct PrunP2 {
    DepthTable eE_c;    ///< Pruning table for (eE, c) coordinates.
    DepthTable eE_eU;   ///< Pruning table for (eE, eU) coordinates.
    PrunP2(): eE_c(FACT4 * FACT8), eE_eU(FACT4 * FACT8){}   ///< @brief Constructor initializing the pruning tables for P2.
};

/**
 * @brief Builds all phase 1 pruning tables.
 *
 * This function constructs the complete set of phase 1 pruning tables for
 * the Rubik's Cube solving algorithm. It calls individual functions to build
 * each specific table and aggregates them into a single PrunP1 struct.
 *
 * @param T The coordinate tables used to determine the resulting states after moves.
 * @return A PrunP1 struct containing all precomputed pruning tables for phase 1.
 */
PrunP1 build_P1(const CoordTables &T);

/**
 * @brief Builds all phase 2 pruning tables.
 *
 * This function constructs the complete set of phase 2 pruning tables for
 * the Rubik's Cube solving algorithm. It calls individual functions to build
 * each specific table and aggregates them into a single PrunP2 struct.
 *
 * @param T The coordinate tables used to determine the resulting states after moves.
 * @return A PrunP2 struct containing all precomputed pruning tables for phase 2.
 */
PrunP2 build_P2(const CoordTables &T);


#endif