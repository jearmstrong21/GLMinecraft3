#version 410 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inCol;

out vec3 col;

uniform mat4 perspective;
uniform mat4 view;

void main(){
    gl_Position=perspective*view*vec4(inPos,1.0);
    col=inCol;
}