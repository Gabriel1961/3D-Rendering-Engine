#include <Renderer.h>
#pragma once
class Material
{
public:
	vec4 diffuse{1,1,1,1};
	shared_ptr<Texture> diffuseTex = 0;
	vec3 ambient{1,1,1};
	shared_ptr<Texture> ambientTex = 0;
	vec3 specular{1,1,1};
	shared_ptr<Texture> specularTex = 0;
	float shininess{128};

	vec2 texScale = {1,1};
	vec2 texOffset = { 0.1,0.1 };
	float texAngle = 0;
	GLenum texScalingMode = GL_REPEAT;

	Material(vec4 dif = {1,.6,0,1}, float amb = .1, float shiny = 128.0f, vec3 spec = {1,1,1})
		:diffuse( dif), ambient(amb* dif),shininess(shiny),specular(spec)
	{
	}

	void GetImgui(const std::string& name)
	{
		ImGui::Text("name");
		ImGui::SliderFloat3("diffuse", &diffuse.x, 0, 1);
		ImGui::SliderFloat3("ambient", &ambient.x , 0, 1);
		ImGui::SliderFloat3("specular", &specular.x , 0, 1);
		ImGui::SliderFloat("shiny", &shininess, 0, 256);
	}
	void Bind(shared_ptr<Shader> sh) const;
};

