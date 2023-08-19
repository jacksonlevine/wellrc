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

#define GRAV 9.81f


#define COMPONENT_IMPLEMENTATIONS
#include "mesh_component.hpp"



#define CHUNK_IMPLEMENTATION
#include "chunk_handler.hpp"

#define PERLIN_IMP
#include "perlin.h"

#define WORLDGEN_IMP
#include "worldgen.hpp"

#define COLLCAGE_IMP
#include "collision_cage.hpp"

#define GUITEXT_IMP
#include "gui_text.hpp"

#include <entt/entt.hpp>
#include <cstdlib>

unsigned int texture;
GLWrapper wrap;
int waterHeight = -200;

std::unordered_map<IntTup, int, IntTupHash> worldmap;

void prepare_texture();

std::string show_vars;

bool grounded = false;
bool prev_grounded = true;

void update_show_vars()
{
    if (grounded != prev_grounded)
    {
        show_vars = "grounded: ";
        show_vars += grounded ? "true" : "false";
        show_vars += "\n";
        prev_grounded = grounded;
    }
}


int main() {

    entt::registry registry;
    wrap.initializeGL();
    wrap.setupVAO();
    wrap.cameraPos = glm::vec3(0,80,0);
    prepare_texture();

    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
    float deltaTime = 0;
    float lastFrame = 0;

    int prevUwv = 0;
    generate_world(worldmap);


    for(int i = -3; i < 3; ++i)
    {
        for(int k = -3; k < 3; ++k)
        {
Chunk test_chunk(glm::vec2(i,k), registry, wrap, worldmap);

    test_chunk.rebuild();
        }
    }
    

    CollisionCage cage(wrap, worldmap, registry);

    BoundingBox user(wrap.cameraPos, glm::vec3(0,0,0));

    auto meshes_view = registry.view<MeshComponent>();

    while (!glfwWindowShouldClose(wrap.window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (wrap.activeState.forward)
        {

            wrap.activeState.forwardVelocity = std::min(wrap.activeState.forwardVelocity + (25.0f*deltaTime), 5.0f);
        } else {
            wrap.activeState.forwardVelocity *= .400f;
        }
        update_show_vars();
        cage.update_readings(wrap.cameraPos + glm::vec3(0, -1.0, 0));
        if(wrap.activeState.jump && grounded)
        {
            wrap.activeState.upVelocity += 5.0f;
            grounded = false;
            wrap.activeState.jump = false;
        }








        if(std::find(cage.solid.begin(), cage.solid.end(), FLOOR) == cage.solid.end())
    {
        grounded = false;
    }









        if(!grounded)
        {
            wrap.activeState.upVelocity = std::max(wrap.activeState.upVelocity + ((GRAV * -deltaTime) * 0.020f), -(GRAV * deltaTime));
            wrap.activeState.upVelocity = std::min(wrap.activeState.upVelocity, (GRAV * deltaTime)*2.0f);
        }

        if (wrap.activeState.forwardVelocity != 0 || wrap.activeState.upVelocity != 0)
        {
            
            glm::vec3 up_dir(0,1,0);
            glm::vec3 forward_dir = wrap.cameraDirection;

            forward_dir.y = 0;
            //wrap.cameraPos += (dir * wrap.activeState.forwardVelocity) * 0.65f;
            glm::vec3 upward_change = (up_dir * wrap.activeState.upVelocity);
            glm::vec3 forward_change = ((forward_dir * wrap.activeState.forwardVelocity) * deltaTime);
            glm::vec3 desired_movement = upward_change + forward_change;
            //std::cout << "DESIRED MOVEMENT: " << desired_movement.x << " " << desired_movement.y << " " << desired_movement.z << std::endl;
            glm::vec3 user_center = wrap.cameraPos + glm::vec3(0, -0.5, 0);
            //std::cout << "USER CENTER " << user_center.x << " " << user_center.y << " " << user_center.z << std::endl;
            //std::cout << "USER CENTER WITH RAW MOVE APPLIED: " << (user_center + desired_movement).x << " " << (user_center + desired_movement).y << " " << (user_center + desired_movement).z << std::endl;
            user.set_center(user_center + desired_movement,  0.85f , 0.3f);

            cage.update_colliding(user);

            if(cage.colliding.size() > 0)
            {
                for(Side& side : cage.colliding)
                {
                    //std::cout << "penet: " << cage.penetration[side] << std::endl;
                    desired_movement += CollisionCage::normals[side] * (float)cage.penetration[side];
                    
                    if(side == FLOOR)
                    {
                        //std::cout << "FLOOR";
                        wrap.activeState.upVelocity = 0;
                        desired_movement -= upward_change;
                        grounded = true;
                    }
                    //std::cout << "change being made: " << (CollisionCage::normals[side] * (float)cage.penetration[side]).x << " " << (CollisionCage::normals[side] * (float)cage.penetration[side]).y << " " << (CollisionCage::normals[side] * (float)cage.penetration[side]).z << std::endl;
                }
                user.set_center(user_center + desired_movement, 0.85f , 0.3f);

            }
            //if(glm::distance(user.center + glm::vec3(0, 0.5, 0),wrap.cameraPos) < 0.5f )
                wrap.cameraPos = user.center + glm::vec3(0, 0.5, 0);



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
        draw_text((std::string("Wellrc v0.1\n") + show_vars).c_str(), -0.95f, 0.95f);


        //std::cout << wrap.cameraPos.x << " " << wrap.cameraPos.y << " " << std::endl;
        glBindVertexArray(0);
        glfwSwapBuffers(wrap.window);




        int queueFOV = 0;

        if (prevUwv != uwV)
        {
            if (uwV == 1)
            {
                queueFOV = 40;
            }
            else
            {
                queueFOV = 75;
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