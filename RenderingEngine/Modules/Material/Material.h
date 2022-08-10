#include <Renderer.h>
#pragma once
class Material
{
public:
	vec4 diffuse{1,1,1,1};
	Texture* diffuseTex = 0;
	vec3 ambient{1,1,1};
	Texture* ambientTex = 0;
	vec3 specular{1,1,1};
	Texture* specularTex = 0;
	float shininess{128};

	vec2 texScale = {1,1};
	vec2 texOffset = { 0.1,0.1 };
	float texAngle = 0;
	GLenum texScalingMode = GL_REPEAT;

	void Bind(Shader* sh) const;
};

