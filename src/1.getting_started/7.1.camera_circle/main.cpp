// https://learnopengl.com/Getting-started/Camera

#include <learnopengl/window.hpp>
#include <learnopengl/shader.hpp>
#include <learnopengl/texture.hpp>
#include <learnopengl/fpscounter.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char** argv)
{
    auto* window = learnopengl::createWindow();
    if(!window)
        return -1;

    // SHADER PROGRAM
    auto shaderProgram = learnopengl::Shader("shader.vs", "shader.fs");

    // TEXTURE
    learnopengl::Texture texture1("resources/textures/container.jpg", {});
    learnopengl::Texture texture2("resources/textures/awesomeface.png",
        {
            .verticalFlip = true,
        });

    // VERTEX DATA

    // Vertices of triangle shape in 3d. (x, y, z)
    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // clang-format on

    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Index of vertex attributes is 2 because in shader we have 'layout (location = 2)'
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind our VAO
    glBindVertexArray(0);

    // Enable fragment depth testing
    glEnable(GL_DEPTH_TEST);

    // Create camera local coordinate system following Gram–Schmidt process https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process
    // Create a look at matrix
    {
        // World Position of the camera
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        // Make the camera point to the origin
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

        // Get the right by doing cross product from up and cameraDirection
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    }

    // Main window render loop
    while(!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        const int texture1Unit = 0;
        const int texture2Unit = 1;

        shaderProgram.setInt("texture1", texture1Unit);
        shaderProgram.setInt("texture2", texture2Unit);

        texture1.use(texture1Unit);
        texture2.use(texture2Unit);

        glBindVertexArray(VAO);

        const float radius = std::fabs(std::cosf(float(glfwGetTime()))) * 10.f + 2;
        float camX = std::sinf(float(glfwGetTime())) * radius;
        float camZ = std::cosf(float(glfwGetTime())) * radius;
        glm::mat4 view;
        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        // Project from View Space (3D) to Clip Space (2D)
        glm::mat4 projection;
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

        shaderProgram.setMat4("view", glm::value_ptr(view));
        shaderProgram.setMat4("projection", glm::value_ptr(projection));

        int i = 0;
        for(const auto& position: cubePositions)
        {
            // Model Matrix : Transform from Local Space to World Space
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            shaderProgram.setMat4("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            ++i;
        }

        // Show rendered buffer in screen
        glfwPollEvents();
        glfwSwapBuffers(window);

        learnopengl::showFPS(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
