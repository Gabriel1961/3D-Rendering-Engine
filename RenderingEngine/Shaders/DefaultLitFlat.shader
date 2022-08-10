#Vertex Shader
#version 430 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat3 u_normalMat;
out vec2 v_uvCoords;
out vec3 v_fragPos;
flat out vec3 v_normal;
void main()
{
	v_uvCoords = uvCoords;
	v_normal = u_normalMat * normal;
	v_fragPos = (u_model * vec4(position, 1)).xyz;
	gl_Position = u_proj * u_view * vec4(v_fragPos, 1);
}

#Fragment Shader
#version 430 core
out vec4 FragColor;

uniform highp vec3 u_viewPos;
//Textures
uniform sampler2D diffuseTex;
uniform sampler2D ambientTex;
uniform sampler2D specularTex;
uniform int useDiffuseTex = 0;
uniform int useAmbientTex = 0;
uniform int useSpecularTex = 0;


struct Material {
	vec4 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	mat3 texTileMat;
};

#define LIGHT_TYPE_NONE 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_DIRECTIONAL 2
#define LIGHT_TYPE_SPOT 3
uniform Material mat = Material(vec4(1, 1, 1, 1), vec3(1, 1, 1), vec3(1, 1, 1), 32, mat3(1));

in vec2 v_uvCoords;
in vec3 v_fragPos; // not affected by the camera position
flat in vec3 v_normal;

struct Light
{
	vec4 pos;
	vec4 color;
	vec4 dir; // for directional light
	int type;
};

layout(std140, binding = 1) buffer LightList
{
	Light lights[];
};

vec3 GetLightColor(inout Light l, vec3 fragToView, vec3 diffuse, vec3 ambient, vec3 specular)
{
	if (l.type == LIGHT_TYPE_POINT)
	{
		ambient = ambient * diffuse;
		float attenuation = 1.0f;// TODO (Distance based)
		vec3 fragToLight = normalize(l.pos.xyz - v_fragPos);
		vec3 diff = max(dot(fragToLight, v_normal), 0.0f) * diffuse;
		vec3 spec = pow(max(dot(-reflect(fragToLight, v_normal), fragToView), 0.0f), mat.shininess) * (specular);

		return (spec + diff + ambient) * l.color.rgb * attenuation;
	}
	else if (l.type == LIGHT_TYPE_DIRECTIONAL)
	{
		ambient = ambient * diffuse;
		vec3 fragToLight = -l.dir.xyz;
		vec3 diff = max(dot(fragToLight, v_normal), 0.0f) * diffuse;
		vec3 spec = pow(max(dot(-reflect(fragToLight, v_normal), fragToView), 0.0f), mat.shininess) * (specular);

		return (spec + diff + ambient) * l.color.rgb;
	}

	return vec3(1, 0, 1);
}
#define pi 3.14159265359
void main()
{

	vec2 uv = ((mat.texTileMat * vec3(v_uvCoords, 1)).xy);
	vec3 fragToView = normalize(u_viewPos - v_fragPos);
	vec3 diffuse, ambient, specular;
	if (useDiffuseTex == 1)
		diffuse = texture(diffuseTex, uv).rgb;
	else
		diffuse = mat.diffuse.rgb;

	if (useAmbientTex == 1)
		ambient = texture(ambientTex, uv).rgb;
	else
		ambient = mat.ambient;
	//FragColor = vec4(ambient, 1);
	//return;
	if (useSpecularTex == 1)
		specular = texture(specularTex, uv).rgb;
	else
		specular = mat.specular;

	// calculate for all lights
	vec3 addedLightColors = vec3(0, 0, 0);

	for (int i = 0; i < lights.length(); i++) {
		addedLightColors += GetLightColor(lights[i], fragToView, diffuse, ambient, specular);
	}
	FragColor.rgb = addedLightColors;
	if (length(FragColor.rgb )> 1) {
		FragColor = vec4(1, 0, 1, 1);
	}
	FragColor.a = 1;
}
