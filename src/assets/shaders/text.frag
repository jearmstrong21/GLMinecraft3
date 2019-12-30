#version 410 core

in vec2 uv;

uniform sampler2D tex;

out vec4 fc;

void main(){
    fc=texture(tex,uv);
//    fc.w=1.0;
//    fc.b=1.0;
//    fc=vec4(1,0,0,1);
}