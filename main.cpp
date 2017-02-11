#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <cmath>

// OpenGL expects you to send all of your vertices in a single array.
// This is the vertex data.
float vertices[] = {
  -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
   0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
};

// An element array is filled with unsigned integers referring to vertices bound to GL_ARRAY_BUFFER
// Using an element buffer allows to reuse data
GLuint elements[] = {
  0, 1, 2,
  2, 3, 0
};

// Shader sources.
const GLchar* vertexSource =
    "#version 150\n"
    "in vec2 position;"
    "in vec3 color;"
    "out vec3 Color;"
    "void main()"
    "{"
    "    Color = color;"
    "    gl_Position = vec4(position, 0.0, 1.0);" // remember that our vertex position is already specified as device coordinates.
    "}";
const GLchar* fragmentSource =
    "#version 150\n"
    "in vec3 Color;" // make sure that the output of the vertex shader and the input of the fragment shader have the same name.
    "out vec4 outColor;" // the fragment shader has one mandatory output, the final color of a fragment.
    "void main()"
    "{"
    "    outColor = vec4(Color, 1.0);"
    "}";
 
int main() {
  // GLFW needs to be initialized when the program starts.
  glfwInit();
  
  // Create and configure the window.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // windowed mode.
  glfwMakeContextCurrent(window); // the OpenGL context has to be made active

  // Initialize GLEW.
  glewExperimental = GL_TRUE;
  glewInit();

  // Create Vertex Array Object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a Vertex Buffer Object and copy the vertex data to it.
  GLuint vbo;
  glGenBuffers(1, &vbo); // generates 1 buffer.
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // makes it the active object.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // uploads the actual data.
                                                                             // note that this function doesn't refer to the id of our VBO.
                                                                             // but instead to the active buffer.

  // Create an element array
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Create and compile the vertex shader.
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates a shader object.
  glShaderSource(vertexShader, 1, &vertexSource, NULL); // loads data into it.
  glCompileShader(vertexShader);

  // Create and compile the fragment shader.
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Link the vertex and fragment shader into a shader program.
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor"); // need to explicitily specify which output is written to which buffer.
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  // Specify the layout of the vertex data.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position"); // retrieves a reference to the position input in the vertex shader.
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0); // specifies how the data for that input is retrieved from the array.
  glEnableVertexAttribArray(posAttrib); // the vertex attribute array needs to be enabled.
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(colAttrib);

  // Retrieve the location (or reference) to the uniform.
  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
  
  // The Event-Loop...
  while (!glfwWindowShouldClose(window)) {
    // Clear the screen to black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // These two functions are required.
    glfwSwapBuffers(window); // swaps the back buffer and front buffer after you've finished drawing.
    glfwPollEvents(); // retrieve window events.
  }

  // Clean up.
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &vao);

  // Clean up when program closes.
  glfwTerminate();
}

