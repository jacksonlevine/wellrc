#pragma once
#include "glwrapper.hpp"
#include <entt/entt.hpp>

#include "cube_helper.hpp"
#include "mesh_component.hpp"


#include "inttup.hpp"
#include "inttup_neighbor.hpp"

#ifndef CHUNK_WIDTH
#define CHUNK_WIDTH 16
#endif

#ifndef CHUNK_HEIGHT
#define CHUNK_HEIGHT 64
#endif

class Chunk {

public:
    entt::entity id;
    glm::vec2 chunk_position;
    Chunk(glm::vec2 c_pos, entt::registry& r, GLWrapper& w, std::unordered_map<IntTup, int, IntTupHash>& wo);

    void rebuild();

private:
    entt::registry& m_reg;
    GLWrapper& m_wrap;
    std::unordered_map<IntTup, int, IntTupHash>& m_world;
};

#ifdef CHUNK_IMPLEMENTATION

Chunk::Chunk(glm::vec2 c_pos, entt::registry& r, GLWrapper& w, std::unordered_map<IntTup, int, IntTupHash>& wo)
: m_reg(r), m_wrap(w), m_world(wo), chunk_position(c_pos) {
    this->id = m_reg.create();
}

void Chunk::rebuild()
{
    std::vector<GLfloat> verts;
    std::vector<GLfloat> cols;
    std::vector<GLfloat> uvs;
    verts.reserve(10000);
    cols.reserve(10000);
    uvs.reserve(10000);
    int half = CHUNK_WIDTH / 2;
    TextureFace face(4,3);

    IntTup tup(0,0,0);
    for(int i = -half; i < half; ++i)
    {
        for(int j = 0; j < CHUNK_HEIGHT; ++j)
        {
            for(int k = -half; k < half; ++k)
            {
                tup.set(
                    (int)(this->chunk_position.x * CHUNK_WIDTH) + i,
                    0 + j,
                    (int)(this->chunk_position.y * CHUNK_WIDTH) + k
                );
                if(m_world.find(tup) != m_world.end())
                {
                    for(Neighbor& n : get_neighbors(tup))
                    {
                        if(m_world.find(n.tup) == m_world.end())
                        {
                            Cube::stamp_face(n.face, tup.x, tup.y, tup.z, face, verts, cols, uvs);
                        }
                    }
                }
            }
        }
    }
    std::cout << verts.size() << " " << cols.size() << " " << uvs.size() << std::endl;

    if (m_reg.all_of<MeshComponent>(this->id))
	{
		MeshComponent m;
		m.length = verts.size();
		m_wrap.bindGeometry(
			m.vbov,
			m.vboc,
			m.vbouv,
			verts.data(),
			cols.data(),
			uvs.data(),
			verts.size() * sizeof(GLfloat),
			cols.size() * sizeof(GLfloat),
			uvs.size() * sizeof(GLfloat)
		);
		m_reg.emplace<MeshComponent>(this->id, m);
	}
	else {
		MeshComponent& m = m_reg.get<MeshComponent>(this->id);
		m.length = verts.size();
		m_wrap.bindGeometry(
			m.vbov,
			m.vboc,
			m.vbouv,
			verts.data(),
			cols.data(),
			uvs.data(),
			verts.size() * sizeof(GLfloat),
			cols.size() * sizeof(GLfloat),
			uvs.size() * sizeof(GLfloat)
		);
	}
}

#endif