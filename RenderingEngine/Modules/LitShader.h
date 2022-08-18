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
			newGpuLights.push_back(l->GetGpuLight());
		
		gpuLights = SSBO_ObjectList<Light::GPU_Light>(newGpuLights, GL_DYNAMIC_COPY, 1);
	}
	LitShader() : Shader(SHADER_PATH "DefaultLit.shader")
	{

	}
	LitShader(const std::string& path) : Shader(SHADER_PATH + path)
	{

	}
	void RenderLightGizmos(Camera& cam)
	{
		std::vector<std::shared_ptr<Light>> lightsSorted = lights;
		sort(lightsSorted.begin(), lightsSorted.end(), [&cam](const auto& a, const auto& b) {
			vec3 d1 = vec3(a->pos) - cam.position, d2 = vec3(b->pos) - cam.position;
			return length(d1) > length(d2);
			});

		for (auto& l : lightsSorted)
			l->RenderGizmo(cam);
	}
	void Bind() override
	{
		gpuLights.Bind();
		Shader::Bind();
	}
};

