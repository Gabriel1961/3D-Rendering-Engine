#Vertex Shader
#version 430 core
layout(location = 0)in vec3 position;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * u_model* vec4(position, 1);
}

#Fragment Shader
#version 430 core

out vec4 FragColor;
uniform vec3 color = vec3(1,1,1);

void main()
{
	FragColor.rgb = color;
	FragColor.a = 1;
}
