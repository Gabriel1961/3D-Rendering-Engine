#Vertex Shader
#version 330 core
layout(location = 0) in vec3 position;
out vec3 texCoords;
uniform mat4 camMat;

void main()
{
	texCoords = position;
	gl_Position = camMat * vec4( position, 1.);
}

#Fragment Shader
#version 330 core

in vec3 texCoords;
out vec4 fragColor;
uniform samplerCube cubeMapSamp;

void main()
{
	fragColor = texture(cubeMapSamp, texCoords);
}
