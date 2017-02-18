#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <chrono>
#include <cmath>
#include <iostream>

// OpenGL expects you to send all of your vertices in a single array.
// This is the vertex data.
float vertices[] = {
//  Position      Texcoords
    -0.5f,  0.5f, 0.0f, 0.0f, // Top-left
     0.5f,  0.5f, 1.0f, 0.0f, // Top-right
     0.5f, -0.5f, 1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 0.0f, 1.0f  // Bottom-left
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
  GLuint _tex1;
  GLuint _tex2;

public:
  void render() {
    // Clear the screen to black.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    GLint logLength;
    // Create and compile the vertex shader.
    _vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates a shader object.
    glShaderSource(_vertexShader, 1, &vertexSource, NULL); // loads data into it.
    glCompileShader(_vertexShader);
    glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	  if (logLength > 0) 
	  {
		  GLchar *log = (GLchar*) malloc(logLength);
		  glGetShaderInfoLog(_vertexShader, logLength, &logLength, log);
		  std::cout << "Vtx Shader compile log: " << log << std::endl;
		  free(log);
    }

    // Create and compile the fragment shader.
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(_fragmentShader);
    glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
	  if (logLength > 0) 
	  {
		  GLchar *log = (GLchar*) malloc(logLength);
		  glGetShaderInfoLog(_fragmentShader, logLength, &logLength, log);
		  std::cout << "Frag Shader compile log: " << log << std::endl;
		  free(log);
    }

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
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat) , 0); // specifies how the data for that input is retrieved from the array.
    glEnableVertexAttribArray(posAttrib); // the vertex attribute array needs to be enabled.

    GLint texAttrib = glGetAttribLocation(_shaderProgram, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texAttrib);
  }

  void loadTextures(void) {
    {
      glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture.
      glGenTextures(1, &_tex1);
      glBindTexture(GL_TEXTURE_2D, _tex1);

      int width, height;
      unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      glGenerateMipmap(GL_TEXTURE_2D);
      SOIL_free_image_data(image);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      GLuint tex1Pos = glGetUniformLocation(_shaderProgram, "tex1");
      glUniform1i(tex1Pos, 0);
    }
    {
      glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture.
      glGenTextures(1, &_tex2);
      glBindTexture(GL_TEXTURE_2D, _tex2);

      int width, height;
      unsigned char* image = SOIL_load_image("smile.png", &width, &height, 0, SOIL_LOAD_RGB);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      glGenerateMipmap(GL_TEXTURE_2D);
      SOIL_free_image_data(image);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      GLuint tex2Pos = glGetUniformLocation(_shaderProgram, "tex2");
      glUniform1i(tex2Pos, 1);
    }
  }

  void deleteTexture(void) {
    glDeleteTextures(1, &_tex1);
    glDeleteTextures(1, &_tex2);
  }

  void deleteBuffers(void) {
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vao);
  }

   void deleteProgram(void) {
    glDeleteProgram(_shaderProgram);
    glDeleteShader(_fragmentShader);
    glDeleteShader(_vertexShader);
  }
};

