#version 330

in vec4 v_VCol;
in vec2 v_TexCoord;
in vec3 v_Normal;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;

void main()
{
	vec4 ambientColor = vec4(u_DirectionalLight.color, 1.0) * u_DirectionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(v_Normal), normalize(u_DirectionalLight.direction)), 0.0);
	vec4 diffuseColor = vec4(u_DirectionalLight.color, 1.0) * u_DirectionalLight.diffuseIntensity * diffuseFactor;
	
	o_Color = texture(u_Texture, v_TexCoord) * (ambientColor + diffuseColor);
}