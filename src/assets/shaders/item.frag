#version 410 core

in vec2 uv;

uniform sampler2D tex;
uniform vec2 uv_pos;
uniform vec2 uv_size;
uniform vec3 tint;

out vec4 fc;

void main(){
    fc=vec4(tint, 1)*texture(tex, uv_pos+uv_size*uv);
    if (fc.w<0.5)discard;
    fc.w=0.5;
}