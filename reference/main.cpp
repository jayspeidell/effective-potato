#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "window.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Define a vertex shader. This is in the GLSL language and needs to
// be compiled. It's defined as a string.
/*
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main()
{
  // process input(s) and do some weird graphics stuff
  ...
  // output processed stuff to output variable
  out_variable_name = weird_stuff_we_processed;
}
*/
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n" // Input comes straight from vertex data
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n" // output variable
    "in vec4 vertexColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n" // RGBA color data
    "}\0";

//==============================================================
//=============== MAIN IS HERE =================================
//==============================================================

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ==========================================================
    // ==============Shader Program==============================
    // ==========================================================

    // Vertex shader
    unsigned int vertexShader; // address
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment shader, next stage
    unsigned int fragmentShader; // address
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create a shader program and attach the compiled shaders.
    unsigned int shaderProgram; // address
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ==========================================================
    // ==============Buffers=====================================
    // ==========================================================

    // ******** Single VBO *************
    // Create Vertex Buffer Object, this is just an unsigned int
    // for storing memory addresses.
    unsigned int VBO; // address
    // Create an array of buffer objects at address &VBO.
    // Type is GLuint, an unsigned into with the address
    // of a buffer object or VBO.
    glGenBuffers(1, &VBO);
    // Bind a hint to VBO so the program knows how to interpret
    // the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ^ Any time I make a call on a GL_ARRAY_BUFFER,
    // it's made to the currently bound buffer VBO

    // ******** Define Vertex ***************
    //Tell openGL how to handle the vertex data.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Starting index, 0
    // number of values in set, 3 for 3 dimensions
    // type of data, GL_FLOAT
    // normalize?, special boolean
    // stride in bits, 3 x size of float to iterate through sets
    // start at beginning
    glEnableVertexAttribArray(0);
    // Enable vertex attribute features, off by default.
    // Arg is vertex attribute location.

    // ******************************************************
    // ******** VAO - Array of VBOs, easier access **********
    // ******************************************************

    // Vertex array object
    /*
    A vertex array object stores the following:
    -Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
    -Vertex attribute configurations via glVertexAttribPointer.
    -Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.
    */

    float vertices2 [] {
        0.5,0.5,0.0,
        -0.5,0.5,0.0,
        0.5,-0.5,0.0,
        -0.5,-0.5,0.0
    };

    unsigned int VAO; // address
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    // 4th arg tells the graphics card how to manage the data.
    // Determines write speed based on storage location.
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.


    // ==========================================================
    // ================= Element Buffer Objects  - EBO ==========
    // ==========================================================
    // Builds off of VAO

    unsigned int indices[] {
        0,2,1//,
        //2,3,1
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Copy data into the buffer memory. Doesn't need VBO
        // reference because VBO is bound.
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 4th arg tells the graphics card how to manage the data.
        // Determines write speed based on storage location.
        // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        // GL_STATIC_DRAW: the data is set only once and used many times.
        // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

        glUseProgram(shaderProgram);


        // ****** Accessing uniform variables **************
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // testing shaders
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

        }
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
