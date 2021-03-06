#version 410 core

layout (location=0) in vec2 inPos;
layout (location=1) in vec2 inUV;

out vec2 uv;

uniform mat4 projection;

void main(){
    gl_Position=projection*vec4(inPos, 0, 1);
    uv=inUV;
}