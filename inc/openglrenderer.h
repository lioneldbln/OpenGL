#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// OpenGL expects you to send all of your vertices in a single array.
// This is the vertex data.
float vertices[] = {
  0.0f,  0.5f,
  0.5f, -0.5f,
 -0.5f, -0.5
};

class OpenGLRenderer {
private:
  GLuint _programName;
  GLuint _vao;
  GLuint _vbo;
  GLuint _viewWidth;
  GLuint _viewHeight;

public:
  void render(void);

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
  }

  GLuint buildProgramWithVertexSources(const char *vertexSource, const char *fragmentSource);

  void deleteBuffers(void) {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vao);
  }
};

