#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aColor;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
out vec3 v_Color;
out vec2 v_Tex;
void main(){ v_Color=aColor; v_Tex=aTex; gl_Position = u_Proj*u_View*u_Model*vec4(aPos,1.0); }

#shader fragment
#version 330 core
in vec3 v_Color;
in vec2 v_Tex;
out vec4 FragColor;
uniform vec4 u_Color;
uniform int u_UseTexture;
uniform sampler2D u_Diffuse;
void main() {
    vec4 base = vec4(v_Color,1.0) * u_Color;
    if (u_UseTexture==1) FragColor = texture(u_Diffuse, v_Tex) * base;
    else FragColor = base;
}
