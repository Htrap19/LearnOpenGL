#version 330

layout(location = 0) out vec4 o_Color;

in vec4 v_VCol;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;
in vec4 v_DirectionalLightSpacePos;

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

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;
uniform vec3 u_EyePosition;
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINTLIGHTS];
uniform int u_SpotLightCount;
uniform SpotLight u_SpotLights[MAX_SPOTLIGHTS];
uniform OmniShadowMap u_OmniShadowMaps[MAX_POINTLIGHTS + MAX_SPOTLIGHTS];

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

float CalcDirectionalShadow(DirectionalLight light)
{
	vec3 projCoords = v_DirectionalLightSpacePos.xyz / v_DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float currentDepth = projCoords.z;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_DirectionalLight.direction);

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);

	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	float shadow = 0.0;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9;

	if (currentDepth > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

float CalcOmniShadow(PointLight light, int shadowMapIndex)
{
	vec3 fragToLight = v_FragPos - light.position;
	float currentDepth = length(fragToLight);
	float bias = 0.05;
	float shadow = 0.0;
	
	// === Simple shadow =======
	/* float closestDepth = texture(u_OmniShadowMaps[shadowMapIndex].shadowMap, fragToLight).r;
	   closestDepth *= u_OmniShadowMaps[shadowMapIndex].farPlane; */

	// === PCF =================
	/* 
	float offset = 0.1;
	float samples = 4.0;

	for (float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for (float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for (float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float closestDepth = texture(u_OmniShadowMaps[shadowMapIndex].shadowMap, fragToLight + vec3(x, y, z)).r;
				closestDepth *= u_OmniShadowMaps[shadowMapIndex].farPlane;
				if ((currentDepth - bias) > closestDepth)
				{
					shadow += 1.0;
				}
			}
		}
	}

	shadow /= (samples * samples * samples);
	*/

	// === Predefined samples ==
	int samples = 20;
	float viewDistance = length(u_EyePosition - v_FragPos);
	float diskRadius = (1.0 + (viewDistance / u_OmniShadowMaps[shadowMapIndex].farPlane)) / 25.0;
	for (int i = 0; i < samples; i++)
	{
		float closestDepth = texture(u_OmniShadowMaps[shadowMapIndex].shadowMap, fragToLight + (sampleOffsetDirections[i] * diskRadius)).r;
		closestDepth *= u_OmniShadowMaps[shadowMapIndex].farPlane;
		if ((currentDepth - bias) > closestDepth)
		{
			shadow += 1.0;
		}
	}
	shadow /= float(samples);

	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
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
	
	return (ambientColor + (1 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadow(u_DirectionalLight);
	return CalcLightByDirection(u_DirectionalLight.base, u_DirectionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pointLight, int shadowMapIndex)
{
	vec3 direction = v_FragPos - pointLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	float shadowFactor = CalcOmniShadow(pointLight, shadowMapIndex);
	vec4 color = CalcLightByDirection(pointLight.base, direction, shadowFactor);
	float attenuation = pointLight.exponent * (distance * distance) +
						pointLight.linear * distance +
						pointLight.constant;
	return (color / attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0.0);

	for (int i = 0; i < u_PointLightCount; i++)
		totalColor += CalcPointLight(u_PointLights[i], i);

	return totalColor;
}

vec4 CalcSpotLight(SpotLight spotLight, int shadowMapIndex)
{
	vec3 rayDirection = normalize(v_FragPos - spotLight.base.position);
	float slFactor = dot(rayDirection, normalize(spotLight.direction));

	if (slFactor > spotLight.edge)
	{
		vec4 color = CalcPointLight(spotLight.base, shadowMapIndex);
		return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - spotLight.edge)));
	}

	return vec4(0.0);
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0.0);

	for (int i = 0; i < u_SpotLightCount; i++)
		totalColor += CalcSpotLight(u_SpotLights[i], u_PointLightCount + i);

	return totalColor;
}

void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();
	o_Color = texture(u_Texture, v_TexCoord) * finalColor;
}