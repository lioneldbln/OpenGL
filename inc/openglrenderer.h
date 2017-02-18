#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <cmath>

// OpenGL expects you to send all of your vertices in a single array.
// This is the vertex data.
float vertices[] = {
    -0.5f,  0.5f, // Top-left
     0.5f,  0.5f, // Top-right
     0.5f, -0.5f, // Bottom-right
    -0.5f, -0.5f  // Bottom-left
};

GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
};

class OpenGLRenderer {
private:
  GLuint _vao;
  GLuint _vbo;
  GLuint _ebo;
  GLuint _shaderProgram;
  GLuint _vertexShader;
  GLuint _fragmentShader;
  GLint _uniColor;

public:
  void render(auto t_start) {
    // Clear the screen to black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

    // Change the color of the triangle using a sine function.
    glUniform3f(_uniColor, (std::sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
  }

  GLuint buildVAO(void) {
    // Create Vertex Array Object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create a Vertex Buffer Object and copy the vertex data to it.
    glGenBuffers(1, &_vbo); // generates 1 buffer.
    glBindBuffer(GL_ARRAY_BUFFER, _vbo); // makes it the active object.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // uploads the actual data.
                                                                               // note that this function doesn't refer to the id of our VBO.
                                                                               // but instead to the active buffer.

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  }

  GLuint buildProgramWithVertexSources(const char *vertexSource, const char *fragmentSource) {
    // Create and compile the vertex shader.
    _vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates a shader object.
    glShaderSource(_vertexShader, 1, &vertexSource, NULL); // loads data into it.
    glCompileShader(_vertexShader);

    // Create and compile the fragment shader.
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(_fragmentShader);

    // Link the vertex and fragment shader into a shader program.
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, _vertexShader);
    glAttachShader(_shaderProgram, _fragmentShader);
    glBindFragDataLocation(_shaderProgram, 0, "outColor"); // need to explicitily specify which output is written to which buffer.
    glLinkProgram(_shaderProgram);
    glUseProgram(_shaderProgram);
  }

  void setupParameters() {
    // Specify the layout of the vertex data.
    GLint posAttrib = glGetAttribLocation(_shaderProgram, "position"); // retrieves a reference to the position input in the vertex shader.
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0); // specifies how the data for that input is retrieved from the array.
    glEnableVertexAttribArray(posAttrib); // the vertex attribute array needs to be enabled.

    // Retrieve the location (or reference) to the uniform.
    _uniColor = glGetUniformLocation(_shaderProgram, "triangleColor");
  }

  void deleteBuffers(void) {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vao);
  }
  
  void deleteProgram(void) {
    glDeleteProgram(_shaderProgram);
    glDeleteShader(_fragmentShader);
    glDeleteShader(_vertexShader);
  }
};

