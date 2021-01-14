// https://learnopengl.com/Lighting/Materials

#include <learnopengl/window.hpp>
#include <learnopengl/shader.hpp>
#include <learnopengl/camera.hpp>
#include <learnopengl/cameracontroller.hpp>
#include <learnopengl/fpscounter.hpp>
#include <learnopengl/material.hpp>
#include <learnopengl/materialcollection.hpp>
#include <learnopengl/pointlight.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>
#include <cmath>

learnopengl::Camera camera;
learnopengl::CameraController cameraController(&camera);

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    cameraController.processInput(window);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    cameraController.mouseButtonCallback(window, button, action, mods);
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) { cameraController.mouseMoveCallback(float(xpos), float(ypos)); }

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) { cameraController.scrollCallback(float(yoffset)); }

int main(int argc, char** argv)
{
    auto* window = learnopengl::createWindow();
    if(!window)
        return -1;

    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // SHADER PROGRAM
    auto shaderProgram = learnopengl::Shader("shader.vs", "shader.fs");
    auto lightShaderProgram = learnopengl::Shader("light.vs", "light.fs");

    // VERTEX DATA

    // Vertices of triangle shape in 3d. (x, y, z)
    // clang-format off
    float vertices[] = {
        // Coordinates       // Normal
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };
    // clang-format on

    // VAO: Vertex Array Object
    // It's a list of vertex attribute pointer
    // A vertex attribute pointer reference and describe a VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // bind Vertex Array Object
    glBindVertexArray(VAO);

    // VBO : Vertex Buffer Object (Allow to send data to GPU)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Set the type of our VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy our data to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind our VAO
    glBindVertexArray(0);

    unsigned int lightVAO;
    {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        // we only need to bind to the VBO, the container's VBO's data already contains the data.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // set the vertex attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Enable fragment depth testing
    glEnable(GL_DEPTH_TEST);

    camera.setCameraPos(glm::vec3(2.2f, 1.f, 6.f));
    camera.setCameraFront(glm::normalize(glm::vec3(0.f, 0.f, -1.f)));

    std::vector<learnopengl::Material> materials = {
        learnopengl::MaterialCollection::emerald(),
        learnopengl::MaterialCollection::pearl(),
        learnopengl::MaterialCollection::bronze(),
        learnopengl::MaterialCollection::gold(),
        learnopengl::MaterialCollection::cyanPlastic(),
        learnopengl::MaterialCollection::redPlastic(),
        learnopengl::MaterialCollection::greenRubber(),
        learnopengl::MaterialCollection::yellowRubber(),
    };

    learnopengl::PointLight pointLight;

    // Main window render loop
    while(!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float timeAsRad = glm::radians(float(glfwGetTime()) * 100.f);
        pointLight.setPosition(glm::vec3(2.25, 1, 4));

        glm::vec3 lightColor;
        lightColor.x = std::sinf(float(glfwGetTime()) * 2.0f);
        lightColor.y = std::sinf(float(glfwGetTime()) * 0.7f);
        lightColor.z = std::sinf(float(glfwGetTime()) * 1.3f);

        pointLight.setDiffuse(lightColor * glm::vec3(0.5f));
        pointLight.setAmbient(pointLight.diffuse() * glm::vec3(0.2f));
        pointLight.setSpecular(glm::vec3(glm::length(lightColor)));

        shaderProgram.use();

        const auto& view = camera.viewMatrix();

        // Project from View Space (3D) to Clip Space (2D)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        camera.setAspect(height ? float(width) / float(height) : 1.f);
        const auto& projection = camera.projectionMatrix();

        shaderProgram.setMat4("projection", glm::value_ptr(projection));

        {
            shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

            int row = 0;
            int column = 0;
            const int maxColumns = 4;

            for(const auto& material: materials)
            {
                auto pointLightView = pointLight;
                pointLightView.setPosition(view * glm::vec4(pointLightView.position(), 1));

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(float(column) * 1.5f, float(row) * 1.5f, 0.f));

                const glm::mat4 modelView = view * model;
                shaderProgram.setMat4("modelView", glm::value_ptr(modelView));

                glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(glm::mat3(modelView)));
                shaderProgram.setMat3("normalMatrix", glm::value_ptr(normalMatrix));

                shaderProgram.setMaterial("material", material);
                shaderProgram.setPointLight("light", pointLightView);

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                ++column;
                if(column >= maxColumns)
                {
                    ++row;
                    column = 0;
                }
            }
        }

        lightShaderProgram.use();

        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLight.position());
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            lightShaderProgram.setMat4("view", glm::value_ptr(view));
            lightShaderProgram.setMat4("projection", glm::value_ptr(projection));
            lightShaderProgram.setMat4("model", glm::value_ptr(model));
            lightShaderProgram.setVec3("color", lightColor.r, lightColor.g, lightColor.b);

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Show rendered buffer in screen
        glfwPollEvents();
        glfwSwapBuffers(window);

        learnopengl::showFPS(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
