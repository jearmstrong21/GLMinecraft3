#version 410 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec2 inUV0;
layout (location=2) in vec2 inUV1;
layout (location=3) in vec3 inCol0;
layout (location=4) in vec3 inCol1;
layout (location=5) in float inAO;

out vec2 uv0;
out vec2 uv1;
out vec3 col0;
out vec3 col1;
out float ao;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 model;

void main(){
    gl_Position=perspective*view*model*vec4(inPos, 1.0);
    uv0=inUV0;
    uv1=inUV1;
    col0=inCol0;
    col1=inCol1;
    ao=inAO;
    //    uv=inUV/16.0;
    //    uv.y=1.0-uv.y;
}