#version 460 core

in vec3 v_vectexColors;

out vec4 color;

void main() {
    color = vec4(v_vectexColors.r, v_vectexColors.g, v_vectexColors.b, 1.0f); // you can refer to indices 0,1,2, with x,y,z or r,g,b
}
