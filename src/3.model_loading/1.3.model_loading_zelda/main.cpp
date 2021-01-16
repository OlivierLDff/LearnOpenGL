// https://learnopengl.com/Model-Loading/Model

#include <learnopengl/window.hpp>
#include <learnopengl/shader.hpp>
#include <learnopengl/camera.hpp>
#include <learnopengl/cameracontroller.hpp>
#include <learnopengl/fpscounter.hpp>
#include <learnopengl/directionlight.hpp>
#include <learnopengl/pointlight.hpp>
#include <learnopengl/mesh.hpp>
#include <learnopengl/model.hpp>

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
    if (!window)
        return -1;

    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // SHADER PROGRAM
    auto shaderProgram = learnopengl::Shader("shader.vs", "shader.fs");

    learnopengl::Model ourModel("resources/objects/zelda/scene.gltf", false);

    camera.setFovDegrees(70.f);
    camera.setCameraPos(glm::vec3(2.f, 2.f, 2.f));
    camera.setCameraFront(glm::normalize(glm::vec3(-1.f, -1.f, -1.f)));

    learnopengl::PointLight pointLight;
    pointLight.setAmbient(glm::vec3(0.01f));
    pointLight.setDiffuse(glm::vec3(0.5f));
    pointLight.setPosition(glm::vec3(0.7f, 0.2f, 2.0f));
    pointLight.setAttenuation(1.f, 0.09f, 0.032f);

    learnopengl::DirectionLight directionLight;
    directionLight.setAmbient(glm::vec3(0.1f));
    directionLight.setDiffuse(glm::vec3(1.f));
    directionLight.setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));

    // Enable fragment depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main window render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        const auto& view = camera.viewMatrix();

        // Project from View Space (3D) to Clip Space (2D)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        camera.setAspect(height ? float(width) / float(height) : 1.f);
        const auto& projection = camera.projectionMatrix();

        shaderProgram.setMat4("projection", glm::value_ptr(projection));
        shaderProgram.setMat4("view", glm::value_ptr(view));
        shaderProgram.setVec3("cameraPos", camera.cameraPos().x, camera.cameraPos().y, camera.cameraPos().z);
        shaderProgram.setDirectionLight("directionLight", directionLight);
        shaderProgram.setPointLight("pointLight", pointLight);
        shaderProgram.setFloat("material.shininess", 32.f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
        //model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
        //model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.0f, 0.f, 1.f));
        model = glm::scale(model, glm::vec3(0.015f));
        shaderProgram.setMat4("model", glm::value_ptr(model));

        glm::mat3 normalModelMatrix = glm::mat3(glm::inverseTranspose(glm::mat3(model)));
        shaderProgram.setMat3("normalModelMatrix", glm::value_ptr(normalModelMatrix));

        ourModel.draw(shaderProgram);

        // Show rendered buffer in screen
        glfwPollEvents();
        glfwSwapBuffers(window);

        learnopengl::showFPS(window);
    }

    glfwTerminate();

    return 0;
}
