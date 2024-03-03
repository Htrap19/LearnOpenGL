#version 330

in vec4 v_VCol;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;
uniform vec3 u_EyePosition;

void main()
{
	vec4 ambientColor = vec4(u_DirectionalLight.color, 1.0) * u_DirectionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(v_Normal), normalize(u_DirectionalLight.direction)), 0.0);
	vec4 diffuseColor = vec4(u_DirectionalLight.color, 1.0) * u_DirectionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0.0);

	if (diffuseFactor > 0.0)
	{
		vec3 fragToEye = normalize(u_EyePosition - v_FragPos);
		vec3 reflectedDirection = normalize(reflect(u_DirectionalLight.direction, normalize(v_Normal)));

		float specularFactor = dot(reflectedDirection, fragToEye);
		if (specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, u_Material.shininess);
			specularColor = vec4((u_DirectionalLight.color * u_Material.specularIntensity * specularFactor), 1.0);
		}
	}
	
	o_Color = texture(u_Texture, v_TexCoord) * (ambientColor + diffuseColor + specularColor);
}