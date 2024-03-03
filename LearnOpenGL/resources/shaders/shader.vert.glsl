#version 330

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

out vec4 v_VCol;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
	v_VCol = vec4(clamp(a_Pos, 0.0, 1.0), 1.0);
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	v_FragPos = (u_Model * vec4(a_Pos, 1.0)).xyz;
}