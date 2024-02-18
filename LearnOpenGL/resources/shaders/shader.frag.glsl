#version 330

in vec4 v_VCol;
in vec2 v_TexCoord;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
};

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;

void main()
{
	vec4 ambientColor = vec4(u_DirectionalLight.color, 1.0) * u_DirectionalLight.ambientIntensity;
	o_Color = texture(u_Texture, v_TexCoord) * ambientColor;
}