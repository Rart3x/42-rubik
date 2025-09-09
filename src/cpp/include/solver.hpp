/*!
 *  @file solver.hpp
 *  @brief Header file for the solver used in Rubik's Cube algorithms.
 *
 *  This file contains declarations for the solver that utilizes
 *  precomputed pruning tables and coordinate tables to solve the Rubik's Cube.
 */

#ifndef CPP_SOLVER_HPP
#define CPP_SOLVER_HPP

#include <vector>
#include "cube.hpp"
#include "coordsTables.hpp"
#include "pruningTables.hpp"


/**
 * @brief Solves a scrambled Rubik's Cube using precomputed tables.
 *
 * This function takes a scrambled cube state and utilizes the provided
 * coordinate tables and pruning tables to find a solution sequence of moves.
 * It returns a vector of moves that solve the cube.
 *
 * @param scrambled The scrambled cube state to be solved.
 * @param coord_tables Precomputed coordinate tables for the cube.
 * @param P1 Precomputed phase 1 pruning tables.
 * @param P2 Precomputed phase 2 pruning tables.
 * @return A vector of moves that solve the cube.
 */
std::vector<Move> solve(const Cube &scrambled, const CoordTables& coord_tables, const PrunP1& P1, const PrunP2& P2);


#endif