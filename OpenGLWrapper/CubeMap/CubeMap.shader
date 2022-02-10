#Vertex Shader
#version 330 core
layout(location = 0) in vec3 position;
out vec3 texCoords;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	texCoords = position;
	gl_Position = projection*view*vec4(position,1.);
}

#Fragment Shader
#version 330 core

in vec3 texCoords;
out vec4 fragColor;
sampler samplerCube cubeMap;

void main()
{
	fragColor = texture(cubeMap, texCoords);
}
