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
		float lin{.7};
		float qua{.2};
		float _p4{33};
	};
public:
	
	
	FrameBuffer fb;
	/// Shadows
	std::shared_ptr<Gizmo2D> gizmo=0;
	
	vec3 pos{};
	vec3 color = {1,1,1};
	vec3 dir{}; // for directional light
	void EnableShadows(ivec2 res)
	{
		castShadows = true;
		fb.AttachDepthTexture(make_shared<DepthTexture>(res));
	}
protected:
	LightType type;
	bool castShadows = false;
	
	Light(LightType type)
		:type(type)
	{
		switch (type)
		{
		case LightType::PointLight:
			gizmo = make_shared<Gizmo2D>(Gizmo2DType::PointLight);
			break;
		case LightType::DirectionalLight:
			gizmo = make_shared<Gizmo2D>(Gizmo2DType::DirectionalLight);
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

	void UpdateGizmoPos()
	{
		gizmo->SetPosition(pos);
	}
	~Light() {
	}
};

class PointLight : public Light 
{
public:
	PointLight(vec3 pos, vec3 color = {1,1,1}) :Light(LightType::PointLight) {
		this->color = color;
		this->pos = pos;
	}
};

class DirectionalLight : public Light
{
public:
	vec3 dir;
	DirectionalLight(vec3 pos, vec3 dir, vec3 color ={1,1,1}) :Light(LightType::DirectionalLight) {
		this->pos = pos;
		this->color = color;
		this->dir = normalize(dir);
	}

	mat4 GetWP()
	{
		return ortho<float>(-20, 20, -20, 20, 1, 30) * lookAt(10.f*dir, {}, {0,1,0});
	}
};