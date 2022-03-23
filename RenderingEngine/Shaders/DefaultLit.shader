#Vertex Shader
#version 430 core
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
	v_normal = -u_normalMVMat * normal;
	v_fragPos = (u_view*u_model*vec4(position,1)).xyz;
	v_uvCoords = uvCoords;
	gl_Position = u_projection* u_camMat *vec4((u_view * u_model * vec4(position,1)).xyz - u_camPos,1);
}

#Fragment Shader
#version 430 core
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
in vec2 v_uvCoords;
in vec3 v_fragPos;
in vec3 v_normal;

struct Light
{
	vec4 pos;
	int type;
	float intensity;
};

layout(std430, binding = 1) buffer LightList
{
	Light Lights[];
};



void main()
{
	if(Lights[0].intensity ==1 )
		FragColor = vec4(1,1,1,1);
	else 
		FragColor = vec4(1,0,1,1);
}
