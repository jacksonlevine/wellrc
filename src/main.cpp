#define JACKS_GL_WRAPPER_IMPLEMENTATION
#include "glwrapper.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TEXTURE_FACE_IMPLEMENTATION
#include "textureface.hpp"

unsigned int texture;
GLWrapper wrap;
int waterHeight = -200;

int main() {

    wrap.initializeGL();
    wrap.setupVAO();

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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    float deltaTime = 0;
    float lastFrame = 0;

    int prevUwv = 0;


    GLuint test_vbov = 0;
    GLuint test_vboc = 0;
    GLuint test_vbouv = 0;
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
                    std::cout << "NEW POS: " << wrap.cameraPos.x << " " << wrap.cameraPos.z << " " << std::endl;
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



        if(test_vbov == 0)
        {
            glGenBuffers(1, &test_vbov);
            glGenBuffers(1, &test_vboc);
            glGenBuffers(1, &test_vbouv);

            GLfloat verts[] = {
                0.0f, 0.0f, 0.0f,
                3.0f, 0.0f, 0.0f,
                3.0f, 3.0f, 0.0f,

                3.0f, 3.0f, 0.0f,
                0.0f, 3.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
            };

            GLfloat cols[] = {
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 
                1.0f, 1.0f, 1.0f, 

                1.0f, 1.0f, 1.0f, 
                1.0f, 1.0f, 1.0f, 
                1.0f, 1.0f, 1.0f, 
            };

            TextureFace tex(4,3);

            GLfloat uvs[] = {
                tex.bl.x, tex.bl.y,
                tex.br.x, tex.br.y,
                tex.tr.x, tex.tr.y,

                tex.tr.x, tex.tr.y,
                tex.tl.x, tex.tl.y,
                tex.bl.x, tex.bl.y,
            };

            wrap.bindGeometry(
                test_vbov,
                test_vboc,
                test_vbouv,
                &verts[0],
                &cols[0],
                &uvs[0],
                18 * sizeof(GLfloat),
                18 * sizeof(GLfloat),
                12 * sizeof(GLfloat)
            );

        } else {
            wrap.bindGeometryNoUpload(
                test_vbov,
                test_vboc,
                test_vbouv
            );
        }
        glDrawArrays(GL_TRIANGLES, 0, 18);


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