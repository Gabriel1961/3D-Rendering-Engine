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
		mat4 matl;
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
	
	
	/// Shadows
	std::shared_ptr<Gizmo2D> gizmo=0;
	
	vec3 pos{};
	vec3 color{};

protected:
	LightType type;
	
	Light(LightType type,vec3 color = {1,1,1}, vec3 pos={})
		:type(type),pos(pos),color(color)
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
	virtual GPU_Light GetGpuLight() 
	{
		GPU_Light l;
		l.pos = pos;
		l.color = color;
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
	float lin, qua;
	PointLight(vec3 pos, vec3 color = {1,1,1}, float lin = 1, float qua = .1f) : Light(LightType::PointLight,color,pos),lin(lin),qua(qua)
	{
	
	}

	GPU_Light GetGpuLight() override
	{
		GPU_Light l = Light::GetGpuLight();
		l.lin = lin;
		l.qua = qua;
		return l;
	}
};

class IShadowCaster
{
public:
	FrameBuffer shadowFrameBuffer;
	mat4 projMat = ortho<float>(-20, 20, -20, 20, 1, 30);	
	float range = 10.f;
	virtual mat4 GetViewProj() = 0;
	virtual void EnableShadows() = 0;
};

class DirectionalLight : public Light,public IShadowCaster
{
public:
	vec3 dir;
	DirectionalLight(vec3 pos, vec3 dir, vec3 color ={1,1,1}) :Light(LightType::DirectionalLight,color,pos) {
		this->pos = pos;
		this->color = color;
		this->dir = normalize(dir);
	}

	mat4 GetViewProj() override
	{
		return projMat * lookAt(10.f*dir, {}, {0,1,0});
	}

	GPU_Light GetGpuLight() override
	{
		GPU_Light l = Light::GetGpuLight();
		l.dir = dir;
		return l;
	}

	void EnableShadows() override
	{

	}
};