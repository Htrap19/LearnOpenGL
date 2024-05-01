#version 330

layout(location = 0) in vec3 a_Pos;

out vec3 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Projection * u_View * vec4(a_Pos, 1.0);
	v_TexCoord = a_Pos;
}