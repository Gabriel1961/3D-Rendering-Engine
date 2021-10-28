#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
uniform mat4 u_MVP;
uniform vec3 u_camPos;
out vec2 v_uvCoords;
void main()
{
	v_uvCoords = uvCoords;
	gl_Position = u_MVP * vec4(position + u_camPos, 1);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform vec4 u_color = vec4(1);
//Textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

in vec2 v_uvCoords;

void main()
{
	vec2 uv = v_uvCoords;
	uv.y = 1 - uv.y; // Flip v
	FragColor = texture(texture_diffuse1,uv);
}
