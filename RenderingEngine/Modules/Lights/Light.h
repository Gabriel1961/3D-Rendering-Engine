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
		vec3 pos;
		float _p1{33};
		vec3 color;
		float _p2{33};
		vec3 dir; // for directional light
		float _p3{33};
		int type;
		float lin{1};
		float qua{1};
		float _p4{33};
	};
public:
	/// Shadows
	Gizmo2D gizmo;
	FrameBuffer* shadowMapFrameBuffer=0;
	
	vec3 pos{};
	vec3 color = {1,1,1};
	vec3 dir{}; // for directional light
protected:
	LightType type;

	
	Light(LightType type)
		:type(type)
	{
		switch (type)
		{
		case LightType::PointLight:
			gizmo = Gizmo2D(Gizmo2DType::PointLight);
			break;
		case LightType::DirectionalLight:
			gizmo = Gizmo2D(Gizmo2DType::DirectionalLight);
			break;
		default:
			break;
		}
	}

public:
	/// Gets coresponding gpu struct that contains the data in this class
	GPU_Light GetGpuLight()
	{
		GPU_Light l;
		l.pos = pos;
		l.color = color;
		l.dir = dir;
		l.type = (int)type;
		return l;
	}

	void EnableShadow(std::function<void()> renderFunc, ivec2 shadowRes = {1024,1024}) {
		delete shadowMapFrameBuffer;
		shadowMapFrameBuffer = new FrameBuffer();
		shadowMapFrameBuffer->AttachDepthTexture(new DepthTexture(shadowRes));

	}

	void UpdateGizmoPos()
	{
		gizmo.SetPosition(pos);
	}
	~Light() {
		delete shadowMapFrameBuffer;
	}
};

class PointLight : public Light 
{
public:
	PointLight(vec3 pos, vec3 color = {1,1,1}) :Light(LightType::PointLight) {
		this->color = color;
		this->pos = vec3(pos);
	}
};