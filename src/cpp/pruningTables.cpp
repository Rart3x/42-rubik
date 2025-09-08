/*!
 *  @file pruningTables.cpp
 *  @brief Source file for pruning tables used in Rubik's Cube algorithms.
 *
 *  This file contains the implementation of precomputed pruning tables that assist in
 *  Rubik's Cube solving algorithms.
 */

#include "include/pruningTables.hpp"
#include <deque>
#include <algorithm>
#include <cstdint>
#include <iostream>


void build_P1_slice_twist(PrunP1 &P1, const CoordTables &T) {
    std::cerr << "BFS P1: slice_twist...";

    auto &dst = P1.slice_twist;

    const Cube S = get_solved_cube();
    const int sSolved = slice_index(S); // Doesn't care about E-Slice edges permutations
    const int tSolved = twist_index(S); // 0

    std::deque<std::pair<int, int>> q;
    const int root = sSolved * TWIST_N + tSolved;
    dst.set(root, 0);
    q.push_back({sSolved, tSolved});


    uint8_t maxDepth = 0;
    while(!q.empty()){
        const std::pair<int, int> st = q.front();
        const int slice = st.first;
        const int twist = st.second;
        q.pop_front();
        const uint8_t d = dst.get(slice * TWIST_N + twist);
        maxDepth = std::max<uint8_t>(maxDepth, d);
        for(int m = 0; m < MOVE_N; m++){
            const int slice2 = T.sliceMove[slice][m];
            const int twist2 = T.twistMove[twist][m];
            const int id = slice2 * TWIST_N + twist2;
            if (dst.get(id) == 0xFF) {
                dst.set(id, d + 1);
                q.push_back({slice2, twist2});
            }
        }
    }
    std::cerr << " done. radius=" << maxDepth << "\n";
}

void build_P1_slice_flip(PrunP1 &P1, const CoordTables &T) {
    std::cerr << "BFS P1: slice_flip...";

    auto &dst = P1.slice_flip;

    const Cube S = get_solved_cube();
    const int sSolved = slice_index(S);
    const int fSolved = flip_index(S); // 0

    std::deque<std::pair<int, int>> q;
    const int root = sSolved * FLIP_N + fSolved;
    dst.set(root, 0);
    q.push_back({sSolved, fSolved});

    uint8_t maxDepth = 0;
    while(!q.empty()){
        const std::pair<int, int> sf = q.front();
        const int slice = sf.first;
        const int flip = sf.second;
        q.pop_front();
        const uint8_t d = dst.get(slice * FLIP_N + flip);
        maxDepth = std::max<uint8_t>(maxDepth, d);
        for(int m = 0; m < MOVE_N; m++){
            const int slice2 = T.sliceMove[slice][m];
            const int flip2 = T.flipMove[flip][m];
            const int id = slice2 * FLIP_N + flip2;
            if (dst.get(id) == 0xFF){
                dst.set(id, d + 1);
                q.push_back({slice2, flip2});
            }
        }
    }
    std::cerr << " done. radius=" << maxDepth << "\n";
}

void build_P2_eE_c(PrunP2 &P2, const CoordTables &T) {
    std::cerr << "BFS P2: eE_c...";

    auto &dst = P2.eE_c;

    auto isP2Legal = [](const int m){
        return m==U || m==U2 || m==Up ||
               m==D || m==D2 || m==Dp ||
               m==R2 || m==L2 || m==F2 || m==B2;
    };

    const Cube S = get_solved_cube();
    const int eSolved = epermE_index(S); // 0
    const int cSolved = cperm_index(S);  // 0

    std::deque<std::pair<int, int>> q;
    const int root = eSolved * FACT8 + cSolved;
    dst.set(root, 0);
    q.push_back({eSolved, cSolved});

    uint8_t maxDepth = 0;
    while(!q.empty()){
        const std::pair<int, int> ec = q.front();
        const int ePermE = ec.first;
        const int cPerm = ec.second;
        q.pop_front();
        const uint8_t d = dst.get(ePermE * FACT8 + cPerm);
        maxDepth = std::max<uint8_t>(maxDepth, d);
        for(int m = 0; m < MOVE_N; m++) {
            if (!isP2Legal(m)) continue;
            const int ePermE2 = T.epermEMove[ePermE][m];
            const int cPerm2 = T.cpermMove[cPerm][m];
            const int id = ePermE2 * FACT8 + cPerm2;
            if (dst.get(id) == 0xFF){
                dst.set(id, d + 1);
                q.push_back({ePermE2, cPerm2});
            }
        }
    }
    std::cerr << " done. radius=" << maxDepth << "\n";
}

void build_P2_eE_eU(PrunP2 &P2, const CoordTables &T){
    std::cerr << "BFS P2: eE_eU...";

    auto &dst = P2.eE_eU;

    auto isP2Legal = [](const int m){
        return m==U || m==U2 || m==Up ||
               m==D || m==D2 || m==Dp ||
               m==R2 || m==L2 || m==F2 || m==B2;
    };

    const Cube S = get_solved_cube();
    const int eSolved  = epermE_index(S);  // 0
    const int uSolved  = epermUD_index(S); // 0

    std::deque<std::pair<int,int>> q;
    const int root = eSolved * FACT8 + uSolved;
    dst.set(root, 0);
    q.push_back({eSolved, uSolved});

    uint8_t maxDepth = 0;
    while(!q.empty()){
        const std::pair<int, int> ee = q.front();
        const int ePermE = ee.first;
        const int ePermUD = ee.second;
        q.pop_front();
        const uint8_t d = dst.get(ePermE * FACT8 + ePermUD);
        maxDepth = std::max<uint8_t>(maxDepth, d);
        for(int m = 0; m < MOVE_N; m++){
            if (!isP2Legal(m)) continue;
            const int ePermE2 = T.epermEMove[ePermE][m];
            const int ePermUD2 = T.epermUDMove[ePermUD][m];
            const int id = ePermE2 * FACT8 + ePermUD2;
            if (dst.get(id) == 0xFF){
                dst.set(id, d + 1);
                q.push_back({ePermE2, ePermUD2});
            }
        }
    }
    std::cerr << "  done. radius=" << maxDepth << "\n";
}