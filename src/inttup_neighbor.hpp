#pragma once
#include "inttup.hpp"
#include "cube_helper.hpp"
struct Neighbor {
    CubeFace face;
    IntTup tup;
    Neighbor(CubeFace f, IntTup t);
    Neighbor();
};

std::vector<Neighbor> get_neighbors(IntTup tup);

#ifdef NEIGHBORS_IMPLEMENTATION
Neighbor::Neighbor(CubeFace f, IntTup t) : face(f), tup(t) {};
Neighbor::Neighbor() : face(TOP), tup(IntTup(0,0,0)) {};

std::vector<Neighbor> get_neighbors(IntTup tup) {
    std::vector<Neighbor> result(6);
    result.insert(result.end(), {
        Neighbor(TOP, tup + IntTup(0, 1, 0)),
        Neighbor(BOTTOM, tup + IntTup(0, -1, 0)),
        Neighbor(LEFT, tup + IntTup(-1, 0, 0)),
        Neighbor(RIGHT, tup + IntTup(1, 0, 0)),
        Neighbor(FRONT, tup + IntTup(0, 0, 1)),
        Neighbor(BACK, tup + IntTup(0, 0, -1))
    });
    return result;
}
#endif