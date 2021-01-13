// https://learnopengl.com/Lighting/Basic-Lighting

#include <iostream>
#include <learnopengl/window.hpp>
#include <learnopengl/shader.hpp>
#include <learnopengl/texture.hpp>
#include <learnopengl/camera.hpp>
#include <learnopengl/fpscounter.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

learnopengl::Camera camera;

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    static float deltaTime = 0.0f; // Time between current frame and last frame
    static float lastFrame = 0.0f; // Time of last frame

    const float currentFrame = float(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    const float cameraSpeed = 2.5f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(learnopengl::Camera::Movement::Forward, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(learnopengl::Camera::Movement::Backward, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(learnopengl::Camera::Movement::Left, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(learnopengl::Camera::Movement::Right, deltaTime);
}

float lastX = 0;
float lastY = 0;
bool firstMouse = true;
bool middleMousePressed = false;

enum class MouseMode
{
    None,
    Orbit,
    Pan,
    OrbitCenter,
    Zoom
};
MouseMode mousePressed = MouseMode::None;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if(action == GLFW_PRESS)
        {
            if(mousePressed == MouseMode::None)
            {
                firstMouse = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                if(button == GLFW_MOUSE_BUTTON_RIGHT)
                {
                    mousePressed = MouseMode::Orbit;
                }
                else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
                {
                    if(mods & GLFW_MOD_SHIFT)
                        mousePressed = MouseMode::Pan;
                    else if(mods & GLFW_MOD_CONTROL)
                        mousePressed = MouseMode::Zoom;
                    else
                        mousePressed = MouseMode::OrbitCenter;
                }
            }
        }
        else if(action == GLFW_RELEASE)
        {
            if((mousePressed == MouseMode::Orbit && button == GLFW_MOUSE_BUTTON_RIGHT) ||
                ((mousePressed == MouseMode::Pan || mousePressed == MouseMode::Zoom || mousePressed == MouseMode::OrbitCenter) &&
                    button == GLFW_MOUSE_BUTTON_MIDDLE))
            {
                mousePressed = MouseMode::None;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(mousePressed == MouseMode::None)
        return;

    if(firstMouse)
    {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }

    float xoffset = float(xpos) - lastX;
    float yoffset = lastY - float(ypos);
    lastX = float(xpos);
    lastY = float(ypos);

    if(mousePressed == MouseMode::Orbit)
        camera.orbit(xoffset, yoffset);
    else if(mousePressed == MouseMode::Pan)
        camera.pan(xoffset, yoffset);
    else if(mousePressed == MouseMode::Zoom)
        camera.zoom(yoffset);
    else if(mousePressed == MouseMode::OrbitCenter)
        camera.orbitCenter(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) { camera.zoomFov(float(yoffset)); }

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
    auto lightShaderProgram = learnopengl::Shader("shader.vs", "light.fs");

    // VERTEX DATA

    // Vertices of triangle shape in 3d. (x, y, z)
    // clang-format off
    float vertices[] = {
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
         -0.5f,  0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,

         -0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,

         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,

         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f, -0.5f,

         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f
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

    // Index of vertex attributes is 0 because in shader we have 'layout (location = 0)'
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Enable fragment depth testing
    glEnable(GL_DEPTH_TEST);

    // Main window render loop
    while(!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float timeAsRad = glm::radians(float(glfwGetTime()) * 100.f);
        const auto lightPos = glm::vec3(std::cosf(timeAsRad), 0, std::sinf(timeAsRad)) * 1.2f;

        shaderProgram.use();

        const auto& view = camera.viewMatrix();

        // Project from View Space (3D) to Clip Space (2D)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        camera.setAspect(height ? float(width) / float(height) : 1.f);
        const auto& projection = camera.projectionMatrix();

        shaderProgram.setMat4("view", glm::value_ptr(view));
        shaderProgram.setMat4("projection", glm::value_ptr(projection));

        {
            glm::mat4 model = glm::mat4(1.0f);
            shaderProgram.setMat4("model", glm::value_ptr(model));

            shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

            //shaderProgram.setMat4("model", glm::value_ptr(model));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShaderProgram.use();
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            lightShaderProgram.setMat4("view", glm::value_ptr(view));
            lightShaderProgram.setMat4("projection", glm::value_ptr(projection));
            lightShaderProgram.setMat4("model", glm::value_ptr(model));

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
