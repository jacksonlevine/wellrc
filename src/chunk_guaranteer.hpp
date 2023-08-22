#pragma once
#include "glwrapper.hpp"
#include "chunk_handler.hpp"
#include "glwrapper.hpp"
#include <unordered_map>
#include "inttup.hpp"

#ifndef LOAD_WIDTH
#define LOAD_WIDTH 16
#endif

class ChunkGuaranteer {
public:
    static glm::vec3 positions[LOAD_WIDTH*LOAD_WIDTH];

    Chunk chunks[LOAD_WIDTH*LOAD_WIDTH];
    entt::entity entities[LOAD_WIDTH*LOAD_WIDTH];

    

    ChunkGuaranteer(GLWrapper& wr, std::unordered_map<IntTup, int>& w);
private:
    GLWrapper& m_wrap;
    std::unordered_map<IntTup, int>& m_world;
};

#ifdef CHUNKGUARANTEER_IMP

ChunkGuaranteer::ChunkGuaranteer(GLWrapper& wr, std::unordered_map<IntTup, int>& w)
: m_wrap(wr), m_world(w)
{

    //Initialize "positions" array
    int half = LOAD_WIDTH / 2;
    int ind = 0;
    for(int i = -half; i < half; ++i)
    {
        for(int k = -half; k < half; ++k)
        {
            this->positions[ind] = glm::vec3(i, k); //The chunk positions we will be guaranteeing coverage of
            ind++;
        }
    }

}

#endif