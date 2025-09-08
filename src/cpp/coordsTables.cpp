/*!
 *  @file coordsTables.cpp
 *  @brief Implementation file for coordinate tables used in Rubik's Cube algorithms.
 *
 *  This file contains definitions for precomputed tables that assist in
 *  Rubik's Cube solving algorithms.
 */

#include "coordsTables.hpp"
#include <algorithm>


// ****************************************************************************
// *                             Twist table                                  *
// ****************************************************************************

/**
 * @brief Computes the twist index of a given cube state.
 *
 * The twist index is a unique identifier for the corner orientations of the cube.
 * It is calculated by treating the corner orientations as a base-3 number.
 *
 * @param c The cube state.
 * @return The twist index (0 to 2186).
 */
static int twist_index(const Cube& c) {
    int idx = 0;
    int sum = 0;
    for(int i = 0; i < 7; i++) {
        idx = 3 * idx + c.co[i];
        sum += c.co[i];
    }
    const int last = (3 - (sum % 3)) % 3; // implied, written for comprehension
    (void) last;
    return idx;
}

/**
 * @brief Sets the corner orientations of a cube based on a given twist index.
 *
 * This function decodes the twist index back into the corner orientation array.
 * The last corner's orientation is determined by the first seven to ensure a valid cube state.
 *
 * @param c The cube state to modify.
 * @param idx The twist index (0 to 2186).
 */
static void twist_set(Cube& c, int idx) {
    int sum = 0;
    for(int i = 6; i >= 0; i--) {
        c.co[i] = idx % 3;
        sum += c.co[i];
        idx /= 3;
    }
    c.co[7] = (3 - (sum % 3)) % 3;
}

/**
 * @brief Creates a cube state from a given twist index.
 *
 * This function initializes a solved cube and then sets its corner orientations
 * based on the provided twist index.
 *
 * @param idx The twist index (0 to 2186).
 * @return The cube state corresponding to the twist index.
 */
static Cube cube_from_twist(const int idx) {
    Cube c = get_solved_cube();
    twist_set(c, idx);
    return c;
}

/**
 * @brief Builds the twist move table.
 *
 * This function precomputes the results of applying each possible move to every
 * twist state and stores them in a 2D array. Each entry in the array represents
 * the new twist index after applying a specific move to a specific twist state.
 *
 * @return A 2D array where each row corresponds to a twist state and each column
 *         corresponds to a move, containing the resulting twist index.
 */
static std::array<std::array<uint16_t, MOVE_N>, TWIST_N> build_twist_moves(){
    std::array<std::array<uint16_t, MOVE_N>, TWIST_N> twistMove{};
    for(int idx = 0; idx < TWIST_N; idx++) {
        const Cube c = cube_from_twist(idx);
        for(int m = 0; m < MOVE_N; m++) {
            Cube d = c;
            apply_move(d, static_cast<Move>(m));
            twistMove[idx][m] = twist_index(d);
        }
    }
    return twistMove;
}


// ****************************************************************************
// *                              Flip table                                  *
// ****************************************************************************

/**
 * @brief Computes the flip index of a given cube state.
 *
 * The flip index is a unique identifier for the edge orientations of the cube.
 * It is calculated by treating the edge orientations as a base-2 number.
 *
 * @param c The cube state.
 * @return The flip index (0 to 2047).
 */
static int flip_index(const Cube& c) {
    int idx = 0;
    int sum = 0;
    for(int i = 0; i < 11; i++) {
        idx = (idx << 1) | (c.eo[i] & 1);
        sum += c.eo[i] & 1;
    }
    const int last = (2 - (sum % 2)) % 2; // implied, written for comprehension
    (void) last;
    return idx;
}

/**
 * @brief Sets the edge orientations of a cube based on a given flip index.
 *
 * This function decodes the flip index back into the edge orientation array.
 * The last edge's orientation is determined by the first eleven to ensure a valid cube state.
 *
 * @param c The cube state to modify.
 * @param idx The flip index (0 to 2047).
 */
static void flip_set(Cube& c, int idx) {
    int sum = 0;
    for(int i = 10; i >= 0; i--) {
        c.eo[i] = idx & 1;
        sum += c.eo[i];
        idx >>= 1;
    }
    c.eo[11] = (2 - (sum % 2)) % 2;
}

/**
 * @brief Creates a cube state from a given flip index.
 *
 * This function initializes a solved cube and then sets its edge orientations
 * based on the provided flip index.
 *
 * @param idx The flip index (0 to 2047).
 * @return The cube state corresponding to the flip index.
 */
static Cube cube_from_flip(const int idx) {
    Cube c = get_solved_cube();
    flip_set(c, idx);
    return c;
}

/**
 * @brief Builds the flip move table.
 *
 * This function precomputes the results of applying each possible move to every
 * flip state and stores them in a 2D array. Each entry in the array represents
 * the new flip index after applying a specific move to a specific flip state.
 *
 * @return A 2D array where each row corresponds to a flip state and each column
 *         corresponds to a move, containing the resulting flip index.
 */
static std::array<std::array<uint16_t, MOVE_N>, FLIP_N> build_flip_moves(){
    std::array<std::array<uint16_t, MOVE_N>, FLIP_N> flipMove{};
    for(int idx = 0; idx < FLIP_N; idx++) {
        const Cube c = cube_from_flip(idx);
        for(int m = 0; m < MOVE_N; m++) {
            Cube d = c;
            apply_move(d, static_cast<Move>(m));
            flipMove[idx][m] = flip_index(d);
        }
    }
    return flipMove;
}


// ****************************************************************************
// *                             Slice table                                  *
// ****************************************************************************

/**
 * @brief Computes the binomial coefficient C(n, k).
 *
 * This function calculates the binomial coefficient, which represents the number
 * of ways to choose k elements from a set of n elements without regard to the order
 * of selection.
 *
 * @param n The total number of elements.
 * @param k The number of elements to choose.
 * @return The binomial coefficient C(n, k).
 */
static int C(const int n, const int k) {
    if(k < 0 || k > n) return 0;
    if(k == 0 || k == n) return 1;
    long long r = 1;
    for(int i = 1; i <= k; i++) {
        r = r * (n - k + i) / i;
    }
    return static_cast<int>(r);
}

/**
 * @brief Ranks a subset of 4 positions from a total of 12.
 *
 * This function computes the rank of a given sorted subset of 4 positions
 * chosen from a total of 12 positions. The rank is calculated using combinatorial
 * mathematics, specifically binomial coefficients.
 * Meaning a 1–1 mapping between a sorted 4-subset of {0..11} and an integer in [0, C(12,4)−1] = [0,494]
 *
 * @param sorted_pos An array of 4 sorted positions (0 to 11).
 * @return The rank of the subset (0 to 494).
 */
static int subset_rank_12_4(const std::array<uint8_t,4>& sorted_pos) {
    int r = 0;
    int last = -1;
    const int k = 4;
    const int n = 12;
    for(int i = 0; i < 4; i++) {
        for(int v = last + 1; v < sorted_pos[i]; v++) {
            r += C(n - 1 - v, k - 1 - i);
        }
        last = sorted_pos[i];
    }
    return r;
}

/**
 * @brief Unranks a subset of 4 positions from a total of 12.
 *
 * This function computes the subset of 4 positions corresponding to a given rank.
 * The subset is determined using combinatorial mathematics, specifically binomial
 * coefficients, and is returned in sorted order.
 *
 * @param r The rank of the subset (0 to 494).
 * @param pos An array to store the resulting sorted positions (0 to 11).
 */
static void subset_unrank_12_4(int r, std::array<uint8_t,4>& pos) {
    const int n = 12;
    const int k = 4;
    int x = 0;
    for(int i = 0; i < k; i++) {
        for(int v = x; v < n; v++) {
            const int c = C(n - 1 - v, k - 1 - i);
            if (r < c) {
                pos[i] = v;
                x = v + 1;
                break;
            }
            r -= c;
        }
    }
}

/**
 * @brief Computes the slice index of a given cube state.
 *
 * The slice index is a unique identifier for the positions of the E-slice edges (FR, FL, BL, BR).
 * It is calculated by determining which four edge positions (out of twelve) are occupied by these edges.
 *
 * @param c The cube state.
 * @return The slice index (0 to 494).
 */
static int slice_index(const Cube& c) {
    std::array<uint8_t,4> pos{};
    int k = 0;
    for(int i = 0; i < 12; i++)
        if (c.ep[i] >= 8 && c.ep[i] <= 11)
            pos[k++] = i;
    std::sort(pos.begin(), pos.end());
    return subset_rank_12_4(pos);
}

/**
 * @brief Creates a cube state from a given slice index.
 *
 * This function initializes a solved cube and then sets its edge positions
 * based on the provided slice index, placing the E-slice edges (FR, FL, BL, BR)
 * in the appropriate positions and filling the remaining positions with the UD-slice edges.
 * The slice coordinate is defined by which positions (indices 0..11) are occupied by E-slice edges.
 * It doesn’t care which specific E-edge (FR/FL/BL/BR) is in which of those positions,
 * nor about any corner data or orientations.
 *
 * @param idx The slice index (0 to 494).
 * @return The cube state corresponding to the slice index.
 */
static Cube cube_from_slice(const int idx) {
    Cube c = get_solved_cube();
    std::array<uint8_t, 4> pos{};
    subset_unrank_12_4(idx, pos); // place E-slice edges (FR=8, FL=9, BL=10, BR=11) into these positions
    std::array<uint8_t, 12> used{};
    used.fill(0);
    // Fill E-slice positions with FR, FL, BL, BR in order; others with remaining UD edges
    std::array<uint8_t,4> es = {8, 9, 10, 11};
    for(int i = 0; i < 4; i++) {
        const int p = pos[i];
        c.ep[p] = es[i];
        used[p] = 1;
    }
    int e = 0;
    for(int i = 0; i < 12; i++) {
        if(!used[i]){ // fill the rest with the non-E-slice edges in order 0..7
            c.ep[i] = e;
            e++;
            if (e == 8)
                e = 0;
        }
    }
    return c;
}

/**
 * @brief Builds the slice move table.
 *
 * This function precomputes the results of applying each possible move to every
 * slice state and stores them in a 2D array. Each entry in the array represents
 * the new slice index after applying a specific move to a specific slice state.
 *
 * @return A 2D array where each row corresponds to a slice state and each column
 *         corresponds to a move, containing the resulting slice index.
 */
static std::array<std::array<uint16_t, MOVE_N>, SLICE_N> build_slice_moves() {
    std::array<std::array<uint16_t, MOVE_N>, SLICE_N> sliceMove{};
    for(int idx = 0; idx < SLICE_N; idx++) {
        const Cube c = cube_from_slice(idx);
        for(int m = 0; m < MOVE_N; m++) {
            Cube d=c;
            apply_move(d, static_cast<Move>(m));
            sliceMove[idx][m] = slice_index(d);
        }
    }
    return sliceMove;
}


// ****************************************************************************
// *                             CPerm table                                  *
// ****************************************************************************

///< @brief Factorials from 0! to 8!
static std::array<int, 9> fact = [] {
    std::array<int, 9> f{};
    f[0] = 1;
    for (int i = 1; i < 9; i++)
        f[i] = f[i - 1] * i;
    return f;
}();

/**
 * @brief Ranks a permutation of 8 elements.
 *
 * This function computes the rank of a given permutation of 8 elements
 * using the factorial number system (Lehmer code). The rank is a unique integer
 * that represents the position of the permutation in lexicographic order.
 *
 * @param p An array representing the permutation of 8 elements (0 to 7).
 * @return The rank of the permutation (0 to 40319).
 */
static int perm_rank_8(const std::array<uint8_t,8>& p) {
    int r = 0;
    for(int i = 0; i < 8; i++) {
        int s = 0;
        for(int j = i + 1; j < 8; j++)
            s += p[j] < p[i];
        r += s * fact[7 - i];
    }
    return r;
}

/**
 * @brief Unranks a permutation of 8 elements.
 *
 * This function computes the permutation corresponding to a given rank
 * using the factorial number system. The resulting permutation is returned
 * in an array.
 *
 * @param r The rank of the permutation (0 to 40319).
 * @param p An array to store the resulting permutation of 8 elements (0 to 7).
 */
static void perm_unrank_8(int r, std::array<uint8_t,8>& p) {
    std::vector<uint8_t> elems(8);
    std::iota(elems.begin(), elems.end(), 0);
    for(int i = 0; i < 8; i++) {
        const int q = r / fact[7 - i];
        r %= fact[7 - i];
        p[i] = elems[q];
        elems.erase(elems.begin() + q);
    }
}

/**
 * @brief Computes the corner permutation index of a given cube state.
 *
 * The corner permutation index is a unique identifier for the arrangement of the cube's corners.
 * It is calculated by ranking the corner permutation array.
 *
 * @param c The cube state.
 * @return The corner permutation index (0 to 40319).
 */
static int cperm_index(const Cube& c) {
    return perm_rank_8(c.cp);
}

/**
 * @brief Sets the corner permutation of a cube based on a given index.
 *
 * This function decodes the corner permutation index back into the corner permutation array.
 *
 * @param c The cube state to modify.
 * @param idx The corner permutation index (0 to 40319).
 */
static void cperm_set(Cube& c, const int idx) {
    perm_unrank_8(idx, c.cp);
}

/**
 * @brief Builds the corner permutation move table.
 *
 * This function precomputes the results of applying each possible move to every
 * corner permutation state and stores them in a 2D array. Each entry in the array represents
 * the new corner permutation index after applying a specific move to a specific corner permutation state.
 *
 * @return A 2D array where each row corresponds to a corner permutation state and each column
 *         corresponds to a move, containing the resulting corner permutation index.
 */
static std::array<std::array<uint16_t, MOVE_N>, FACT8> build_cperm_moves() {
    std::array<std::array<uint16_t, MOVE_N>, FACT8> cpermMove{};
    for(int idx = 0; idx < FACT8; idx++) {
        Cube c = get_solved_cube();
        cperm_set(c, idx);
        for(int m = 0; m < MOVE_N; m++) {
            Cube d = c;
            apply_move(d, static_cast<Move>(m));
            cpermMove[idx][m] = cperm_index(d);
        }
    }
    return cpermMove;
}

// ****************************************************************************
// *                            EPermUD table                                 *
// ****************************************************************************

/**
 * @brief Computes the edge permutation index of the UD-slice edges in a given cube state.
 *
 * The edge permutation index for the UD-slice edges (edges 0 to 7) is a unique identifier
 * for the arrangement of these edges. It is calculated by ranking the permutation of the
 * UD-slice edges, assuming that the E-slice edges (edges 8 to 11) are in the middle layer
 * (by definition of the P2 states in Kociemba algorithm).
 *
 * @param c The cube state.
 * @return The edge permutation index for the UD-slice edges (0 to 40319).
 */
static int epermUD_index(const Cube& c) {
    std::array<uint8_t,8> p{};
    for(int i = 0; i < 8; i++){ // positions for UD-layer are 0..7
        const uint8_t eid = c.ep[i];
        p[i] = eid;
    }
    return perm_rank_8(p);
}

/**
 * @brief Builds the edge permutation move table for UD-slice edges.
 *
 * This function precomputes the results of applying each possible move to every
 * edge permutation state of the UD-slice edges and stores them in a 2D array.
 * Each entry in the array represents the new edge permutation index after applying
 * a specific move to a specific edge permutation state.
 *
 * @return A 2D array where each row corresponds to an edge permutation state of the UD-slice edges
 *         and each column corresponds to a move, containing the resulting edge permutation index.
 */
static std::array<std::array<uint16_t, MOVE_N>, FACT8> build_epermUD_moves(){
    std::array<std::array<uint16_t, MOVE_N>, FACT8> epermUDMove{};
    for(int idx = 0; idx < FACT8; idx++){
        Cube c = get_solved_cube();
        std::array<uint8_t, 8> p{};
        perm_unrank_8(idx, p);
        for(int i = 0; i < 8; i++)
            c.ep[i] = p[i];
        for(int i = 0; i < 4; i++)
            c.ep[8 + i] = 8 + i; // E-slice solved
        for(int m = 0; m < MOVE_N; m++) {
            Cube d = c;
            apply_move(d, static_cast<Move>(m));
            epermUDMove[idx][m] = epermUD_index(d);
        }
    }
    return epermUDMove;
}

// ****************************************************************************
// *                             EPermE table                                 *
// ****************************************************************************

/**
 * @brief Ranks a permutation of 4 elements.
 *
 * This function computes the rank of a given permutation of 4 elements
 * using the factorial number system (Lehmer code). The rank is a unique integer
 * that represents the position of the permutation in lexicographic order.
 *
 * @param p An array representing the permutation of 4 elements (0 to 3).
 * @return The rank of the permutation (0 to 23).
 */
static int perm_rank_4(const std::array<uint8_t,4>& p) {
    int r = 0;
    for(int i = 0; i < 4; i++) {
        int s = 0;
        for(int j = i + 1; j < 4; j++)
            s += p[j] < p[i];
        r += s * fact[3 - i];
    } return r;
}

/**
 * @brief Unranks a permutation of 4 elements.
 *
 * This function computes the permutation corresponding to a given rank
 * using the factorial number system. The resulting permutation is returned
 * in an array.
 *
 * @param r The rank of the permutation (0 to 23).
 * @param p An array to store the resulting permutation of 4 elements (0 to 3).
 */
static void perm_unrank_4(int r, std::array<uint8_t,4>& p) {
    std::vector<uint8_t> elems(4);
    std::iota(elems.begin(), elems.end(), 0);
    for(int i = 0; i < 4; i++) {
        const int q = r / fact[3 - i];
        r %= fact[3 - i];
        p[i] = elems[q];
        elems.erase(elems.begin() + q);
    }
}


/**
 * @brief Computes the edge permutation index of the E-slice edges in a given cube state.
 *
 * The edge permutation index for the E-slice edges (edges 8 to 11) is a unique identifier
 * for the arrangement of these edges. It is calculated by ranking the permutation of the
 * E-slice edges, assuming that the UD-slice edges (edges 0 to 7) are in the top and bottom layers
 * (by definition of the P2 states in Kociemba algorithm).
 *
 * @param c The cube state.
 * @return The edge permutation index for the E-slice edges (0 to 23).
 */
static int epermE_index(const Cube& c) {
    std::array<uint8_t, 4> p{};
    for(int i = 0; i < 4; i++) {
        const uint8_t pos = 8 + i; // positions 8..11
        const uint8_t eid = c.ep[pos]; // among {8..11}
        p[i] = eid - 8; // map to 0..3
    }
    return perm_rank_4(p);
}

/**
 * @brief Builds the edge permutation move table for E-slice edges.
 *
 * This function precomputes the results of applying each possible move to every
 * edge permutation state of the E-slice edges and stores them in a 2D array.
 * Each entry in the array represents the new edge permutation index after applying
 * a specific move to a specific edge permutation state.
 *
 * @return A 2D array where each row corresponds to an edge permutation state of the E-slice edges
 *         and each column corresponds to a move, containing the resulting edge permutation index.
 */
static std::array<std::array<uint8_t, MOVE_N>, FACT4> build_epermE_moves() {
    std::array<std::array<uint8_t, MOVE_N>, FACT4> epermEMove{};
    for(int idx = 0; idx < FACT4; ++idx) {
        Cube c = get_solved_cube();
        std::array<uint8_t, 4> p{};
        perm_unrank_4(idx, p);
        for(int i = 0; i < 4; i++)
            c.ep[8 + i] = 8 + p[i];
        for(int m = 0; m < MOVE_N; m++) {
            Cube d = c;
            apply_move(d, static_cast<Move>(m));
            epermEMove[idx][m] = static_cast<uint8_t>(epermE_index(d));
        }
    }
    return epermEMove;
}

// ****************************************************************************
// *                        Coords Tables Builder                             *
// ****************************************************************************

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
CoordTables build_coords_tables() {
    CoordTables tables{};
    tables.twistMove = build_twist_moves();
    tables.flipMove = build_flip_moves();
    tables.sliceMove = build_slice_moves();
    tables.cpermMove = build_cperm_moves();
    tables.epermUDMove = build_epermUD_moves();
    tables.epermEMove = build_epermE_moves();
    return tables;
}
