#version 330

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_Model;
uniform mat4 u_DirectionalLightTransform;

void main()
{
	gl_Position = u_DirectionalLightTransform * u_Model * vec4(a_Pos, 1.0);
}