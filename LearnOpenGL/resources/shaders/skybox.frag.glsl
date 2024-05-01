#version 330

layout(location = 0) out vec4 o_Color;

in vec3 v_TexCoord;

uniform samplerCube u_Skybox;

void main()
{
	o_Color = texture(u_Skybox, v_TexCoord);
}