#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_MVP;
uniform vec3 u_camPos;

void main()
{
	gl_Position = u_MVP * vec4(position + u_camPos, 1);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform vec4 u_color = vec4(1);
void main()
{
	FragColor = vec4(1, 0, 1, 1);
}
