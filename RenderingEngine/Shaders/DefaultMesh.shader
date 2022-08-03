#Vertex Shader
#version 330 core
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
	v_normal = u_normalMVMat * normal;
	v_fragPos = (u_view*u_model*vec4(position,1)).xyz;
	v_uvCoords = uvCoords;
	gl_Position = u_projection* u_camMat *vec4((u_view * u_model * vec4(position,1)).xyz - u_camPos,1);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform vec4 u_color = vec4(1);
uniform highp vec3 u_camPos;
//Textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

//Lights
uniform vec3 u_lightColor = vec3(1);
uniform vec3 u_lightPos;
uniform float u_specularStrength = 1; // [0,1]
uniform float u_shininess =32;
in vec2 v_uvCoords;
in vec3 v_fragPos;
in vec3 v_normal;

void main()
{
	vec2 uv = v_uvCoords;
	uv.y = 1 - uv.y; // Flip v

	float ambientStrength = 0.1;
	float roughness = 0.2;
	vec3 fragToLight = normalize(u_lightPos - v_fragPos);
	vec3 fragToCam = normalize(u_camPos - v_fragPos);

	vec3 diffuseColor = max(dot(-fragToLight,v_normal),0.0f) * u_lightColor;

	vec3 sampleColor = texture(texture_diffuse1, uv).rgb;

	vec3 ambientColor = sampleColor * ambientStrength;

	vec3 specular = u_lightColor * pow(max(dot(reflect(fragToLight,v_normal), -fragToCam), 0.0f),u_shininess) * u_specularStrength;

	FragColor.rgb = specular + sampleColor*diffuseColor + ambientColor;
	FragColor.a = 1;
}
