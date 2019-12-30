#version 410 core

in vec2 uv;

uniform sampler2D tex;
uniform vec2 inUV;
out vec4 fc;

void main(){
    fc=texture(tex,uv/16.0+inUV);
//    fc.w=1.0;
//    fc.b=1.0;
//    fc=vec4(1,0,0,1);
}