#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_proj;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_camMat;
uniform mat3 u_normalMVMat;
uniform highp vec3 u_camPos;
out vec3 FragPos;
flat out vec3 Norm;
void main()
{
	Norm = u_normalMVMat *normal;
	vec3 worldPos = (u_view * u_model * vec4(position, 1)).xyz;
	FragPos = worldPos;
	gl_Position = u_proj * u_camMat * vec4(worldPos - u_camPos, 1);
}

#Fragment Shader
#version 330 core
in vec3 FragPos;
flat in vec3 Norm;
out vec4 FragColor;
uniform highp vec3 u_camPos;
uniform vec4 u_color = vec4(1);

void main()
{
	float ambientStrength = 0.3;
	vec3 ambient = u_color.xyz * ambientStrength;
	vec3 diffuse = u_color.xyz * max(dot(normalize(u_camPos - FragPos), Norm),0);
	
	FragColor.xyz = ambient + diffuse*(1-ambientStrength);
	FragColor.w = 1;
}