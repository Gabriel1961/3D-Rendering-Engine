#pragma once
#include <Quad.h>
#include "../Camera.h"
#define GIZMOS_PATH "RenderingEngine/Modules/Gizmos/"
using namespace glm;
enum class GizmoType
{
	None,
	PointLight,
	DirectionalLight
};

class Gizmo2D
{
	Texture* tex;
	Quad* quad;
public:

	mat4 viewMat = mat4(1);
	void operator=(Gizmo2D&& o)
	{
		quad =o. quad;
		tex = o.tex;

		o.quad = 0;
		o.tex = 0;
	}

	Gizmo2D(Gizmo2D&& o)
	{
		quad =o. quad;
		tex = o.tex;

		o.quad = 0;
		o.tex = 0;
	}
	
	void SetPosition(const vec3& pos)
	{
		viewMat = translate(mat4(1), pos);
	}
	vec3 GetPosition()
	{
		return { viewMat[0][3],viewMat[1][3],viewMat[2][3] };
	}
	Gizmo2D(GizmoType type = GizmoType::None)
	{
		quad = new Quad(1, 0);
		quad->transform.Transforms().push_back(MatrixTransform(mat4(1)));
		switch (type)
		{
		case GizmoType::PointLight:
			tex = new Texture( GIZMOS_PATH "PointLightGizmo.png");
			break;
		case GizmoType::DirectionalLight:
			tex = new Texture(GIZMOS_PATH "DirectionalLightGizmo.png");
			break;
		default:
			// type none 
			quad->useTex = false;
			quad->color = { 1,0,1,1 };
			break;
		}
	}
	void Render(Camera& cam)
	{
		quad->transform.Transforms()[0].Matrix() = cam.projMat * cam.GetCamRotMat() * viewMat * translate(mat4(1), -cam.position) * inverse(cam.GetCamRotMat());
		tex->Bind(0);
		quad->Render();
	}
	~Gizmo2D()
	{
		delete tex;
		delete quad;
	}
};

