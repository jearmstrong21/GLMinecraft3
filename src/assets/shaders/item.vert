#version 410 core

layout (location=0) in vec2 inVec;

uniform mat4 ortho;
uniform vec2 pos;
uniform vec2 size;

out vec2 uv;

void main(){
    gl_Position=ortho*vec4(pos+size*inVec, 0, 1);
    uv=inVec;
}