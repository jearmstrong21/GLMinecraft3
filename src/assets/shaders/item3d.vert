#version 410 core

layout (location=0) in vec3 inVec;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 model;

uniform float scale;

out vec2 uv;

void main(){
    gl_Position=perspective*view*model*vec4(inVec*scale, 1);
    uv=inVec.xy;
}