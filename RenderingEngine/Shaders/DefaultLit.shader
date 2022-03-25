#Vertex Shader
#version 430 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_camMat;
uniform mat3 u_normalMVMat;
uniform highp vec3 u_camPos;
out vec2 v_uvCoords;
out vec3 v_fragPos;
out vec3 v_normal;
void main()
{
	v_normal = -u_normalMVMat * normal;
	v_fragPos = (u_view*u_model*vec4(position,1)).xyz;
	v_uvCoords = uvCoords;
	gl_Position = u_projection* u_camMat *vec4((u_view * u_model * vec4(position,1)).xyz - u_camPos,1);
}

#Fragment Shader
#version 430 core
out vec4 FragColor;
uniform vec4 u_color = vec4(1);
uniform bool useTex = false;
uniform highp vec3 u_camPos;
//Textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

in vec2 v_uvCoords;
in vec3 v_fragPos;
in vec3 v_normal;

struct Light
{
	int type;
	vec4 pos;
	vec3 color;
	float shininess;
	float specularStrength;
};

layout(std430, binding = 1) buffer LightList
{
	Light lights[];
};

vec3 GetLightColor(inout Light l,inout vec3 fragToCam,inout vec3 sampleColor)
{
	vec3 fragToLight = normalize(l.pos.xyz - v_fragPos);
	vec3 diffuseColor = max(dot(-fragToLight, v_normal), 0.0f) * l.color;
	vec3 specular = l.color * pow(max(dot(reflect(fragToLight, v_normal), -fragToCam), 0.0f), l.shininess) * l.specularStrength;
	return specular + sampleColor * diffuseColor;
}

void main()
{
	vec2 uv = v_uvCoords;
	float ambientStrength = 0.1;
	vec3 fragToCam = normalize(u_camPos - v_fragPos);
	//uv.y = 1 - uv.y; // Flip v
	vec3 sampleColor;
	if (useTex)
		sampleColor = texture(texture_diffuse1, uv).rgb;
	else
		sampleColor = u_color.rgb;

	vec3 ambientColor = sampleColor * ambientStrength;

	vec3 addedLightColors = vec3(0);

	if (lights[0].shininess == 32)
	{
		FragColor = vec4(1);
		return;
	}
	return;
	for (int i = 0; i < lights.length(); i++) // calculate for all lights
	{
		addedLightColors += GetLightColor(lights[i], fragToCam, sampleColor);

	}
	
	FragColor.rgb = addedLightColors + ambientColor;
	FragColor.a = 1;
}
