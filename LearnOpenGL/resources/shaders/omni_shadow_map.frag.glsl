#version 330

in vec4 v_FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{
	float dist = length(v_FragPos.xyz - u_LightPos);
	dist = dist / u_FarPlane;
	gl_FragDepth = dist;
}