#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "texture.h"

int main()
{
    // ---------------- GLFW ----------------
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "Textured Quad", nullptr, nullptr);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // ---------------- GLAD ----------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    // ---------------- Geometry ----------------
    float vertices[] = {
        // positions        // tex coords
        -0.5f,-0.5f,0.0f,   0.0f,0.0f,
         0.5f,-0.5f,0.0f,   1.0f,0.0f,
         0.5f, 0.5f,0.0f,   1.0f,1.0f,
        -0.5f, 0.5f,0.0f,   0.0f,1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        0,2,3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // tex coords
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // ---------------- Shaders ----------------
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;

        uniform sampler2D texture1;

        void main()
        {
            FragColor = texture(texture1, TexCoord);
        }
    )";

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // ---------------- Texture (AFTER GLAD) ----------------
    Texture cat("assets/textures/cat.png");

    glUseProgram(shaderProgram);
    glUniform1i(
        glGetUniformLocation(shaderProgram, "texture1"),
        0
    );

    // ---------------- Render Loop ----------------
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        cat.Bind(0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
