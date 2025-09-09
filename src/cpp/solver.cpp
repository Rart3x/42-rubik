/*!
 *  @file solver.cpp
 *  @brief Source file for the solver used in Rubik's Cube algorithms.
 *
 *  This file contains the implementation of the solver that utilizes
 *  precomputed pruning tables and coordinate tables to solve the Rubik's Cube.
 */

#include "include/solver.hpp"
#include <chrono>
#include <algorithm>
#include <functional>

/**
 * @struct Timer
 * @brief A simple timer class to track elapsed time and check for time limits.
 *
 * This struct uses the steady clock to measure elapsed time since its creation.
 * It provides methods to get the elapsed time in seconds and to check if a specified
 * time limit has been reached.
 */
struct Timer {
    using clock = std::chrono::steady_clock;    ///< Type alias for steady clock
    clock::time_point t0;                       ///< Start time point
    double limit_s;                             ///< Time limit in seconds

    ///< @brief Constructor initializing the timer with a specified time limit in seconds.
    explicit Timer(const double seconds) : t0(clock::now()), limit_s(seconds) {}

    ///< @brief Returns the elapsed time in seconds since the timer was started.
    double elapsed() const { return std::chrono::duration<double>(clock::now() - t0).count(); }

    ///< @brief Checks if the elapsed time has reached or exceeded the specified time limit.
    bool time_up() const { return elapsed() >= limit_s; }
};

/**
 * @brief Heuristic function for phase 1 of the Rubik's Cube solving algorithm.
 *
 * This function computes the heuristic value based on the slice, twist, and flip coordinates
 * of the cube state. It returns the maximum depth from the precomputed pruning tables
 * for the given coordinates.
 *
 * @param P1 The phase 1 pruning tables.
 * @param sliceIdx The slice index of the cube state.
 * @param twistIdx The twist index of the cube state.
 * @param flipIdx The flip index of the cube state.
 * @return The heuristic value (maximum depth) for the given coordinates.
 */
static int h1(const PrunP1& P1, const int sliceIdx, const int twistIdx, const int flipIdx) {
    const int d1 = P1.slice_twist.get(sliceIdx * TWIST_N + twistIdx);
    const int d2 = P1.slice_flip .get(sliceIdx * FLIP_N + flipIdx);
    return std::max(d1, d2);
}

/**
 * @brief Heuristic function for phase 2 of the Rubik's Cube solving algorithm.
 *
 * This function computes the heuristic value based on the edge permutation of E-slice edges,
 * corner permutation, and edge permutation of UD-slice edges of the cube state.
 * It returns the maximum depth from the precomputed pruning tables for the given coordinates.
 *
 * @param P2 The phase 2 pruning tables.
 * @param eE The edge permutation of E-slice edges index of the cube state.
 * @param cperm The corner permutation index of the cube state.
 * @param eUD The edge permutation of UD-slice edges index of the cube state.
 * @return The heuristic value (maximum depth) for the given coordinates.
 */
static int h2(const PrunP2& P2, const int eE, const int cperm, const int eUD) {
    const int d1 = P2.eE_c .get(eE*FACT8 + cperm);
    const int d2 = P2.eE_eU.get(eE*FACT8 + eUD);
    return std::max(d1, d2);
}

/**
 * @brief Iterative Deepening A* (IDA*) search for phase 1 of the Rubik's Cube solving algorithm.
 *
 * This function performs an IDA* search to find all shortest solutions for phase 1 of the
 * Rubik's Cube solving algorithm. It explores the state space using depth-first search
 * with iterative deepening, guided by the heuristic function h1.
 *
 * @param scrambled The scrambled cube state to be solved.
 * @param maxBound The maximum depth bound for the search.
 * @param cur A vector to store the current path of moves being explored.
 * @param outStates A vector to store the resulting cube states that reach phase 2.
 * @param outPaths A vector to store the corresponding paths of moves for each resulting state.
 * @param coord_tables Precomputed coordinate tables for the cube.
 * @param P1 Precomputed phase 1 pruning tables.
 * @param tm Timer object to track elapsed time and enforce time limits.
 */
static void ida_p1(
    const Cube &scrambled,
    const int maxBound,
    std::vector<Move>& cur,
    std::vector<Cube>& outStates,
    std::vector<std::vector<Move>>& outPaths,
    const CoordTables& coord_tables,
    const PrunP1& P1,
    const Timer &tm
    ) {
    const int slice0 = slice_index(scrambled);
    const int twist0 = twist_index(scrambled);
    const int flip0 = flip_index(scrambled);
    const int lower_bound = h1(P1, slice0, twist0, flip0);
    for(int bound = lower_bound; bound <= maxBound; bound++){
        auto go = [&](auto&& self, const Cube& c, const int depth, const int lastFace, const int slice, const int twist, const int flip){
            if (tm.time_up())
                return true;
            const int h = h1(P1, slice, twist, flip);
            if (depth + h > bound)
                return false;
            if (h == 0) {
                outStates.push_back(c);
                outPaths.push_back(cur);
                return false;
            }
            for(int m = 0; m < MOVE_N; m++){
                const int face = m / 3;
                if (lastFace == face)
                    continue;
                Cube d = c;
                apply_move(d, static_cast<Move>(m));
                const int slice2 = coord_tables.sliceMove[slice][m];
                const int twist2 = coord_tables.twistMove[twist][m];
                const int flip2 = coord_tables.flipMove[flip][m];
                cur.push_back(static_cast<Move>(m));
                const bool abort = self(self, d, depth + 1, face, slice2, twist2, flip2);
                cur.pop_back();
                if (abort)
                    return true;
            }
            return false;
        };
        cur.clear();
        go(go, scrambled, 0, -1, slice0, twist0, flip0);
        if (!outStates.empty())
            return; // return shortest P1 solutions only
    }
}

/**
 * @brief Depth-First Search (DFS) for phase 2 of the Rubik's Cube solving algorithm.
 *
 * This function performs a DFS to find a solution for phase 2 of the Rubik's Cube
 * solving algorithm. It explores the state space using depth-first search, guided
 * by the heuristic function h2.
 *
 * @param c The current cube state.
 * @param depth The current depth in the search tree.
 * @param bound The maximum depth bound for the search.
 * @param lastMove The last move made to reach the current state (to avoid redundant moves).
 * @param eE The edge permutation of E-slice edges index of the current cube state.
 * @param cperm The corner permutation index of the current cube state.
 * @param eUD The edge permutation of UD-slice edges index of the current cube state.
 * @param cur A vector to store the current path of moves being explored.
 * @param out A vector to store the resulting path of moves if a solution is found.
 * @param P2 Precomputed phase 2 pruning tables.
 * @param tm Timer object to track elapsed time and enforce time limits.
 * @return True if the search was aborted due to time limit, false otherwise.
 */
static bool dfs_p2(
    const Cube &c, const int depth,
    const int bound, const int lastMove,
    const int eE, const int cperm, const int eUD,
    std::vector<Move>& cur, std::vector<Move>& out,
    const PrunP2& P2, const Timer &tm
    ) {
    if (tm.time_up())
        return true; // abort signal
    const int h = h2(P2, eE, cperm, eUD);
    if (depth + h > bound)
        return false;
    if (isSolved(c)) {
        out = cur;
        return false;
    }
    auto legal = [](const int m) {
        return m==U || m==U2 || m==Up
            || m==D || m==D2 || m==Dp
            || m==R2 || m==L2 || m==F2 || m==B2;
    };
    for(int m = 0; m < MOVE_N; m++) {
        if (legal(m)) {
            if (lastMove == m)
                continue;
            Cube d = c;
            apply_move(d, static_cast<Move>(m));
            const int eE2 = epermE_index(d);
            const int cperm2 = cperm_index(d);
            const int eUD2 = epermUD_index(d);
            cur.push_back(static_cast<Move>(m));
            const bool abort = dfs_p2(d, depth + 1, bound, m, eE2, cperm2, eUD2, cur, out, P2, tm);
            cur.pop_back();
            if (abort)
                return true;
            if (!out.empty())
                return false; // found
        }
    }
    return false;
}


std::vector<Move> solve(const Cube &scrambled, const CoordTables& coord_tables, const PrunP1& P1, const PrunP2& P2) {
    const Timer tm(2.8);
    std::vector<Move> cur;
    std::vector<Cube> p1_states;
    std::vector<std::vector<Move>> p1_paths;
    ida_p1(scrambled, 12, cur, p1_states, p1_paths, coord_tables, P1, tm); // typical P1 <= 8–12
    std::vector<Move> bestAns;
    int bestLen = INT32_MAX;
    for(size_t i = 0; i < p1_states.size(); i++){
        if (tm.time_up())
            break;
        Cube c = p1_states[i];
        const int eE = epermE_index(c);
        const int cperm = cperm_index(c);
        const int eUD = epermUD_index(c);
        // IDA* on Phase 2
        const int lower_bound = h2(P2, eE, cperm, eUD);
        for(int bound = lower_bound; bound <= 18; bound++){
            cur = p1_paths[i]; // carry P1 path prefix
            std::vector<Move> out;
            Cube d = c;
            const bool abort = dfs_p2(d, 0, bound, -1, eE, cperm, eUD, cur, out, P2, tm);
            if (abort)
                break; // time-up
            if (!out.empty()){
                if (out.size() < bestLen) {
                    bestLen = out.size();
                    bestAns = out;
                }
                break;
            }
        }
    }
    return bestAns;
}
