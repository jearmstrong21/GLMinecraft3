#version 410 core

in vec2 uv;

uniform sampler2D tex;

out vec4 fc;

void main(){
//    fc=vec4(uv,0,1);
    fc=texture(tex,uv);
}