#pragma once
#include "../Gizmos/Gizmo2D.h"
enum class LightType
{
	PointLight=1
};

class Light
{
public:
	struct GPU_Light // struct used on the gpu for storage of the light data
	{
		vec4 pos;
		int type;
		float intensity = 1;
	};
	LightType type;
	vec4 pos;
	float intensity = 1;
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
		return { pos,(int)type,intensity};
	}

	void RenderGizmo(Camera& cam)
	{
		gizmo.SetPosition(pos);
		gizmo.Render(cam);
	}
};

