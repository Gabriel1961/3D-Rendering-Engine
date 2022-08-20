#pragma once
#include <3DRenderingEngine.h>
#include "Lights/Light.h"
class LitShader : public Shader
{
	SSBO_ObjectList<Light::GPU_Light> gpuLights;
public:
	vec4 color = {1,1,1,1};
	std::vector<std::shared_ptr<Light>> lights;
	void ApplyLights(const Camera& cam)
	{
		std::vector<Light::GPU_Light> newGpuLights;
		for (auto& l : lights)
			newGpuLights.push_back(l->GetGpuLight()),l->UpdateGizmoPos();
		
		gpuLights = SSBO_ObjectList<Light::GPU_Light>(newGpuLights, GL_DYNAMIC_COPY, 1);
	}
	LitShader() : Shader(SHADER_PATH "DefaultLit.shader")
	{

	}
	LitShader(const std::string& path) : Shader(SHADER_PATH + path)
	{

	}
	void Bind() override
	{
		gpuLights.Bind();
		Shader::Bind();
	}
};

