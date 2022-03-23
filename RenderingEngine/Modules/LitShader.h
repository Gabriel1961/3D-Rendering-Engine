#pragma once
#include <3DRenderingEngine.h>
#include "Lights/Light.h"
class LitShader : public Shader
{
	SSBO_ObjectList<Light::GPU_Light> gpuLights;
public:

	std::vector<Light> lights;
	void ApplyLights()
	{
		std::vector<Light::GPU_Light> newGpuLights;
		for (auto& l : lights)
			newGpuLights.push_back(l.GetGpuLight());
		gpuLights = SSBO_ObjectList<Light::GPU_Light>(newGpuLights, GL_STATIC_READ, 1);
	}
	LitShader() : Shader(SHADER_PATH "DefaultLit.shader")
	{

	}
	void RenderLightGizmos(Camera& cam)
	{
		for (auto& l : lights)
			l.RenderGizmo(cam);
	}
	void Bind() const override
	{
		gpuLights.Bind();
		Shader::Bind();
	}
};

