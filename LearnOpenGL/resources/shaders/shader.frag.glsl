#version 330

layout(location = 0) out vec4 o_Color;

in vec4 v_VCol;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

const int MAX_POINTLIGHTS = 3;
const int MAX_SPOTLIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;
uniform vec3 u_EyePosition;
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINTLIGHTS];
uniform int u_SpotLightCount;
uniform SpotLight u_SpotLights[MAX_SPOTLIGHTS];

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(v_Normal), normalize(direction)), 0.0);
	vec4 diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0.0);

	if (diffuseFactor > 0.0)
	{
		vec3 fragToEye = normalize(u_EyePosition - v_FragPos);
		vec3 reflectedDirection = normalize(reflect(direction, normalize(v_Normal)));

		float specularFactor = dot(reflectedDirection, fragToEye);
		if (specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, u_Material.shininess);
			specularColor = vec4((light.color * u_Material.specularIntensity * specularFactor), 1.0);
		}
	}
	
	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(u_DirectionalLight.base, u_DirectionalLight.direction);
}

vec4 CalcPointLight(PointLight pointLight)
{
	vec3 direction = v_FragPos - pointLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	vec4 color = CalcLightByDirection(pointLight.base, direction);
	float attenuation = pointLight.exponent * (distance * distance) +
						pointLight.linear * distance +
						pointLight.constant;
	return (color / attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0.0);

	for (int i = 0; i < u_PointLightCount; i++)
		totalColor += CalcPointLight(u_PointLights[i]);

	return totalColor;
}

vec4 CalcSpotLight(SpotLight spotLight)
{
	vec3 rayDirection = normalize(v_FragPos - spotLight.base.position);
	float slFactor = dot(rayDirection, normalize(spotLight.direction));

	if (slFactor > spotLight.edge)
	{
		vec4 color = CalcPointLight(spotLight.base);
		return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - spotLight.edge)));
	}

	return vec4(0.0);
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0.0);

	for (int i = 0; i < u_SpotLightCount; i++)
		totalColor += CalcSpotLight(u_SpotLights[i]);

	return totalColor;
}

void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();
	o_Color = texture(u_Texture, v_TexCoord) * finalColor;
}