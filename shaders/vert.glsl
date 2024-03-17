#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vectexColor;

out vec3 v_vectexColors; // v_ represents that we are passing colors from this to fragment shader

void main() {
    v_vectexColors = vectexColor;

    gl_Position = vec4(position.x, position.y, position.z, 1.0); // you can refer to indices 0,1,2, with x,y,z or r,g,b
}
