#version 410 core
out vec4 frag_color;
in vec2 uv0;
in vec2 uv1;
in vec3 col0;
in vec3 col1;
in float ao;
uniform sampler2D tex;
void main(){
    vec4 first=texture(tex, uv0)*vec4(col0, 1);
    vec4 second=texture(tex, uv1)*vec4(col1, 1);
    vec4 result=first;
    if (second.w>0.2){
        result=second;
    }
    //    frag_color=vec4(ao,ao,ao,1);
    frag_color=vec4(result.xyz, 1);
    frag_color*=max(0, min(1, ao));
    //  FragColor=texture(tex,uv);
    //  FragColor=vec4(uv,0,1);
}