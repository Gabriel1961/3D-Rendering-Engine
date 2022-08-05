/// Using viewportshading to display positive normals with blue and negative normals with red

#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_proj;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_camMat;
uniform mat3 u_normalMVMat = mat3(1);
uniform highp vec3 u_camPos;
out vec3 FragPos;
flat out vec3 Norm;
void main()
{
	Norm = u_normalMVMat * normal;
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
uniform int u_mode = 0;
void main()
{
	float ambientStrength = 0.3;
	vec3 fragToCam = u_camPos - FragPos;

	vec3 color;
	vec3 diffuse;
	if (dot(fragToCam, Norm) > 0) {

		color = vec3(0, 0, 1);
		diffuse = color * max(dot(normalize(fragToCam), Norm), 0);
	}
	else {

		color = vec3(1, 0, 0);
		diffuse = color * max(dot(normalize(fragToCam), -Norm), 0);
	}
	vec3 ambient = color * ambientStrength;

	if (u_mode == 0)
		FragColor.xyz = ambient + diffuse * (1 - ambientStrength);
	else
		FragColor.xyz = abs(Norm);
	FragColor.w = 1;

}