#pragma once
#include "inttup.hpp"
#include <unordered_map>
#include "perlin.h"

void generate_world(std::unordered_map<IntTup, int, IntTupHash>& worldmap);

#ifdef WORLDGEN_IMP

perlin p;
float noise_func(int x, int y, int z)
{
    return p.noise(((float)x)/20.35f, 30.23, ((float)z)/20.35f) * 60.0f;
}
void generate_world(std::unordered_map<IntTup, int, IntTupHash>& worldmap)
{
    for(int i = -50; i < 50; i++)
    {
        
        for(int k = -50; k < 50; k++)
        {
            int height = (int)noise_func(i,0,k);
            
            for(int j = 0; j < height; j++)
            {
                worldmap.insert_or_assign(IntTup(i, j, k), 0);
            }
        }
    }
}
#endif