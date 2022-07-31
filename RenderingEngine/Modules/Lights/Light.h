#pragma once
#include "../Gizmos/Gizmo2D.h"
#include "FrameBuffer.h"
#include <functional>

enum class LightType
{
	None=0,
	PointLight=1,
	DirectionalLight=2,
};

class Light
{
public:
	struct GPU_Light // struct used on the gpu for storage of the light data
	{
		vec4 pos;
		vec3 color;
		int type;
		float shininess;
		float specularStrength;
	};
	/// Shadows
	FrameBuffer* shadowMapFrameBuffer=0;
	std::function<void()> renderFunc;

	LightType type;
	vec4 pos;
	vec3 color = {1,1,1};
	float shininess=32;
	float specularStrength=1;

	Gizmo2D gizmo;
	Light(LightType type,vec3 pos)
		:type(type), pos(pos,1)
	{
		switch (type)
		{
		case LightType::PointLight:
			gizmo = Gizmo2D(GizmoType::PointLight);
			break;
		default:
			break;
		}
	}

	/// Gets coresponding gpu struct that contains the data in this class
	GPU_Light GetGpuLight()
	{
		return { pos,color,(int)type,shininess,specularStrength};
	}

	void EnableShadow(std::function<void()> renderFunc, ivec2 shadowRes = {1024,1024}) {
		delete shadowMapFrameBuffer;
		shadowMapFrameBuffer = new FrameBuffer();
		this->renderFunc = renderFunc;
		shadowMapFrameBuffer->AttachDepthTexture(new DepthTexture(shadowRes));

	}

	void RenderGizmo(Camera& cam)
	{
		gizmo.SetPosition(pos);
		gizmo.Render(cam);
	}
	~Light() {
		delete shadowMapFrameBuffer;
	}
};

