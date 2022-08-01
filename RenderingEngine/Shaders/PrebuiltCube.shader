#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_camMat;
uniform vec3 u_camPos;
out vec2 v_uvCoords;
void main()
{
	v_uvCoords = uvCoords;
	gl_Position = u_projection * u_camMat * vec4((u_view * u_model * vec4(position, 1)).xyz - u_camPos, 1);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
in vec2 v_uvCoords;
uniform vec4 u_color = vec4(1);
uniform sampler2D albedo;
void main()
{
	FragColor.xyz = texture(albedo, v_uvCoords).xyz;
	FragColor.w = 1;
}
