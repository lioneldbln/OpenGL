#pragma once

// Shader sources.
const GLchar* vertexSource =
    "#version 150\n"
    "in vec3 position;"
    "in vec2 texcoord;"
    "out vec2 Texcoord;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"
    "void main()"
    "{"
    "    Texcoord = texcoord;"
    "    gl_Position = projection * view * model * vec4(position, 1.0);" // remember that our vertex position is already specified as device coordinates.
    "}";

const GLchar* fragmentSource =
    "#version 150\n"
    "in vec2 Texcoord;"
    "out vec4 outColor;" // the fragment shader has one mandatory output, the final color of a fragment.
    "uniform sampler2D tex1;"
    "uniform sampler2D tex2;"
    "void main()"
    "{"
    "    outColor = mix(texture(tex1, Texcoord), texture(tex2, Texcoord), 0.2);"
    "}";

