#pragma once
#include "inttup.hpp"
#include <unordered_map>
#include "perlin.h"

void generate_world(std::unordered_map<IntTup, int, IntTupHash>& worldmap);

#ifdef WORLDGEN_IMP

perlin p;
float noise_func(int x, int y, int z)
{
    return 20.0f + p.noise(((float)x)/15.35f, ((float)y)/15.35f, ((float)z)/15.35f) * 10.0f;
}
void generate_world(std::unordered_map<IntTup, int, IntTupHash>& worldmap)
{
    for(int i = -100; i < 100; i++)
    {

        for(int k = -100; k < 100; k++)
        {
            

            for(int j = 0; j < CHUNK_HEIGHT; j++)
            {
                int density = std::max(0.0f, noise_func(i,j,k) - ((float)j/2.0f));
                if(density > 10)
                {
                    worldmap.insert_or_assign(IntTup(i, j, k), 0);
                }
            }
        }
    }
}
#endif