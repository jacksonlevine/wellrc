#pragma once
#define JACKS_GL_WRAPPER_IMPLEMENTATION
#include "glwrapper.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TEXTURE_FACE_IMPLEMENTATION
#include "textureface.hpp"

#define INTTUP_IMPLEMENTATION
#include "inttup.hpp"

#define CUBE_HELPER_IMPLEMENTATION
#include "cube_helper.hpp"

#define NEIGHBORS_IMPLEMENTATION
#include "inttup_neighbor.hpp"

#define COMPONENT_IMPLEMENTATIONS
#include "mesh_component.hpp"

#define CHUNK_IMPLEMENTATION
#include "chunk_handler.hpp"

#include <entt/entt.hpp>
#include <cstdlib>

unsigned int texture;
GLWrapper wrap;
int waterHeight = -200;

std::unordered_map<IntTup, int, IntTupHash> worldmap;

entt::registry registry;

void prepare_texture();

int main() {

    wrap.initializeGL();
    wrap.setupVAO();

    prepare_texture();

    glClearColor(0.4f, 0.3f, 0.5f, 1.0f);
    float deltaTime = 0;
    float lastFrame = 0;

    int prevUwv = 0;

    for(int i = -50; i < 50; i++)
    {
        for(int j = 0; j < 50; j++)
        {
            for(int k = -50; k < 50; k++)
            {
                if(rand() < 100)
                {
                    worldmap.insert_or_assign(IntTup(i, j, k), 0);
                }
            }
        }
    }

    Chunk test_chunk(glm::vec2(0,0), registry, wrap, worldmap);
    test_chunk.rebuild();

    auto meshes_view = registry.view<MeshComponent>();

    while (!glfwWindowShouldClose(wrap.window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (wrap.activeState.forward)
        {

            wrap.activeState.forwardVelocity = std::min(wrap.activeState.forwardVelocity + deltaTime, 2.0f);
        } else {
            wrap.activeState.forwardVelocity *= .600f;
        }

        if (wrap.activeState.forwardVelocity > 0)
        {
            auto dir = wrap.cameraDirection;
            dir.y = 0;
            wrap.cameraPos += (dir * wrap.activeState.forwardVelocity) * 0.65f;
        }



        int uwV = (wrap.cameraPos.y - 0.15f < waterHeight) ? 1 : 0;

        
        wrap.updateOrientation();
        glBindVertexArray(wrap.vao);
        glUseProgram(wrap.shaderProgram);

        GLuint mvpLoc = glGetUniformLocation(wrap.shaderProgram, "mvp");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(wrap.mvp));

        GLuint camPosLoc = glGetUniformLocation(wrap.shaderProgram, "camPos");
        glUniform3f(camPosLoc, wrap.cameraPos.x, wrap.cameraPos.y, wrap.cameraPos.z);

        GLuint uwLoc = glGetUniformLocation(wrap.shaderProgram, "underWater");
        // int uwFeet = (wrap.cameraPos.y - 2 < waterHeight) ? 1 : 0;
        glUniform1i(uwLoc, uwV);
//draw here


        for (const entt::entity entity : meshes_view)
        {
            MeshComponent& m = registry.get<MeshComponent>(entity);
            wrap.bindGeometryNoUpload(
                m.vbov,
                m.vboc,
                m.vbouv);

            glDrawArrays(GL_TRIANGLES, 0, m.length);
            
        }
        


    //std::cout << wrap.cameraPos.x << " " << wrap.cameraPos.y << " " << std::endl;

        glfwSwapBuffers(wrap.window);




        int queueFOV = 0;

        if (prevUwv != uwV)
        {
            if (uwV == 1)
            {
                queueFOV = 70;
            }
            else
            {
                queueFOV = 90;
            }
            prevUwv = uwV;
        }

        if (queueFOV != 0)
        {
            wrap.setFOV(queueFOV);
        }

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        wrap.deltaTime = deltaTime;
        lastFrame = currentFrame;

        /*if (wrap.activeState.forwardVelocity > 0)
        {
            auto dir = wrap.cameraDirection;
            dir.y = 0;
            wrap.cameraPos += (dir * wrap.activeState.forwardVelocity) * 3.0f;
            wrap.activeState.forwardVelocity *= friction;
        }*/
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void prepare_texture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("src/assets/texture.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}