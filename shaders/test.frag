#version 410 core
out vec4 FragColor;
in vec2 uv;
uniform sampler2D tex;
void main(){
  FragColor=texture(tex,uv);
//  FragColor=vec4(uv,0,1);
}