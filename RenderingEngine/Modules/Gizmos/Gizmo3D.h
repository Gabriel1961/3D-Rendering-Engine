#pragma once
#include "3DRenderingEngine.h"
#include "./Modules/Mesh.h"
class Gizmo3D : Model
{
	static shared_ptr<Shader> gizmoShader;

public:
	shared_ptr<Shader> GetSh()
	{
		if (gizmoShader == 0)
			gizmoShader = make_shared<Shader>(SHADER_PATH "Gizmo3d.shader");
		return gizmoShader;
	}
	Gizmo3D(const std::string& modelName) : Model((MODEL_PATH "Gizmo/"+ modelName).c_str(),GetSh())
	{

	}
};