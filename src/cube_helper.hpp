#pragma once
#include "glwrapper.hpp"
#include "textureface.hpp"
#include <vector>

enum CubeFace {
    TOP = 0, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

class Cube {
public:

    static const GLfloat faces[6][18];

    static void stamp_face(CubeFace face,
                           int x, int y, int z,
                           TextureFace& tex,
                           std::vector<GLfloat>& verts,
                           std::vector<GLfloat>& cols,
                           std::vector<GLfloat>& uvs);
};

#ifdef CUBE_HELPER_IMPLEMENTATION

const GLfloat Cube::faces[6][18] = {
    {
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f
    },
    {
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f
    },
    {
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f
    },
    {
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f
    },
    {
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f
    },
    {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,

        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f
    }
};

void Cube::stamp_face(CubeFace face,
                      int x, int y, int z,
                      TextureFace& tex,
                      std::vector<GLfloat>& verts,
                      std::vector<GLfloat>& cols,
                      std::vector<GLfloat>& uvs)
{
    verts.insert(verts.end(), {
        Cube::faces[face][0] + (float)x, Cube::faces[face][1] + (float)y, Cube::faces[face][2] + (float)z,
        Cube::faces[face][3] + (float)x, Cube::faces[face][4] + (float)y, Cube::faces[face][5] + (float)z,
        Cube::faces[face][6] + (float)x, Cube::faces[face][7] + (float)y, Cube::faces[face][8] + (float)z,

        Cube::faces[face][9] + (float)x, Cube::faces[face][10] + (float)y, Cube::faces[face][11] + (float)z,
        Cube::faces[face][12] + (float)x, Cube::faces[face][13] + (float)y, Cube::faces[face][14] + (float)z,
        Cube::faces[face][15] + (float)x, Cube::faces[face][16] + (float)y, Cube::faces[face][17] + (float)z,
    });

    float bri = face == TOP ? 1.0f : face == BOTTOM ? 0.4f : (face == LEFT || face == RIGHT) ? 0.6f : 0.8f;

    cols.insert(cols.end(), {
        bri, bri, bri,
        bri, bri, bri,
        bri, bri, bri,

        bri, bri, bri,
        bri, bri, bri,
        bri, bri, bri
    });

    uvs.insert(uvs.end(), {
        tex.bl.x, tex.bl.y,
        tex.br.x, tex.br.y,
        tex.tr.x, tex.tr.y,

        tex.tr.x, tex.tr.y,
        tex.tl.x, tex.tl.y,
        tex.bl.x, tex.bl.y
    });
}

#endif