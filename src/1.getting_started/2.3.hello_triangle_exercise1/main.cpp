// https://learnopengl.com/Getting-started/Hello-Triangle
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int initGLAD()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 0;
}

int initGLFW()
{
    glfwInit();

    // Use min OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Use OpenGL in Core Profile (vs old opengl, deprecated function are not available here)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

GLFWwindow* createWindowContext(const char* title, int width = 800, int height = 600)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    return window;
}

GLFWwindow* initialize()
{
    if(initGLFW() < 0)
        return nullptr;

    // Create a valid openGl Context
    auto* window = createWindowContext("LearnOpenGL");
    if(!window)
        return nullptr;

    // GLAD required a valid OpenGL context to load opengl functions
    if(initGLAD() < 0)
        return nullptr;

    return window;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool checkShaderCompilationError(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return false;
    }

    return true;
}

bool checkShaderProgramCompilationError(unsigned int shaderProgram)
{
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return false;
    }

    return true;
}

// Applied for each vertices
// Map vertices to screen coordinates
// Vertices outside of -1.0 -> 1.0 are clipped
const char* vertexShaderSource = R"(

#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

)";

// Fragment shader is called for each pixel in our geometry
// vec4 color is RGBA from 0.0 to 1.0
const char* fragmentShaderSource = R"(

#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}

)";

int main()
{
    auto* window = initialize();
    if(!window)
        return -1;

    // SHADER PROGRAM

    // Create a vertex Shader
    // The vertex shader map each of our vertice to screen coordinate (between -1 to 1)
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    // Compile the shader from its source. A compile error can occurs.
    glCompileShader(vertexShader);
    if(!checkShaderCompilationError(vertexShader))
        return -1;

    // Create fragment shader
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    if(!checkShaderCompilationError(fragmentShader))
        return -1;

    // Create our shader program that use the 2 already compiled shaders
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if(!checkShaderProgramCompilationError(shaderProgram))
        return -1;

    // Shaders objects are no longer required onced they have been linked
    // It's liked obj file (.o) for c/cpp
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VERTEX DATA

    // Vertices of triangle shape in 3d. (x, y, z)
    float vertices[] = {-0.75, -0.5, 0, -0.375, 0.5, 0, 0, -0.5, 0, 0.375, 0.5, 0, 0.75, -0.5, 0};

    unsigned int indices[] = {
        // note that we start from 0!
        0,
        1,
        2, // first triangle
        2,
        3,
        4 // second triangle
    };

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
    // Describe our vertex buffer 'vertices' with a vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Index of vertex attributes is 0 because in shader we have 'layout (location = 0)'

    // EBO : Element Buffer Index
    // Allow to reuse vertices by index to create multiple geometry sharing vertices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Unbind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind our VAO
    glBindVertexArray(0);

    // Unbind our EBO (after VAO)
    // A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
    // This also means it stores its unbind calls so make sure you don't unbind the element array buffer
    // before unbinding your VAO, otherwise it doesn't have an EBO configured.
    // https://learnopengl.com/img/getting-started/vertex_array_objects_ebo.png
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Main window render loop
    while(!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // Draw primitive in wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Draw primitive in fill mode
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Show rendered buffer in screen
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
