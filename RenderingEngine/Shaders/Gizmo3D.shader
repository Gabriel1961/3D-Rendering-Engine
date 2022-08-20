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
out vec3 v_normal;
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


struct Material {
	vec4 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	mat3 texTileMat;
};

uniform Material mat = Material(vec4(1, 1, 1, 1), vec3(1, 1, 1), vec3(1, 1, 1), 32, mat3(1));

in vec2 v_uvCoords;
in vec3 v_fragPos; // not affected by the camera position
in vec3 v_normal;
vec3 normal;

#define pi 3.14159265359
void main()
{
	normal = normalize(v_normal);
	
	vec3 fragToView = normalize(u_viewPos - v_fragPos);
	vec3 diffuse = mat.diffuse.rgb, ambient = diffuse.rgb * .1;

	FragColor.rgb = ambient + diffuse * max(dot(normal,fragToView),0);
	FragColor.a = 1;
}
