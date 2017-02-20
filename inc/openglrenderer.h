#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <cmath>
#include <iostream>

#include "vertices.h"
#include "shaders.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    createTransformations();
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
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat) , 0); // specifies how the data for that input is retrieved from the array.
    glEnableVertexAttribArray(posAttrib); // the vertex attribute array needs to be enabled.

    GLint texAttrib = glGetAttribLocation(_shaderProgram, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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

  void createTransformations() {
    // Camera/View transformation
    glm::mat4 view;
    GLfloat radius = 10.0f;
    GLfloat camX = sin(glfwGetTime()) * radius;
    GLfloat camZ = cos(glfwGetTime()) * radius;
    view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), // position in world space.
                       glm::vec3(0.0f, 0.0f, 0.0f), // target.
                       glm::vec3(0.0f, 1.0f, 0.0f)); // up vector.
    // Projection
    glm::mat4 projection;
    projection = glm::perspective(45.0f, // defines the FOV value.
                                  (GLfloat)WIDTH / (GLfloat)HEIGHT, // sets the aspect ratio.
                                  0.1f, // near plane.
                                  100.0f); // far plane.
    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(_shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(_shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(_shaderProgram, "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(_vao);
    for (GLuint i = 0; i < 10; i++)
    {
        // Calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        GLfloat angle = 20.0f * i;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0); 
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

