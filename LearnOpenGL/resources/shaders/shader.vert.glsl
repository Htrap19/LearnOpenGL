#version 330

layout(location = 0) in vec3 a_Pos;

out vec4 v_VCol;						

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
	v_VCol = vec4(clamp(a_Pos, 0.0, 1.0), 1.0);
}