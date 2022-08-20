#pragma once
#include "3DRenderingEngine.h"
#include "./Modules/Mesh.h"
#include <unordered_set>

class Gizmo
{
public:
	vec3 pos;
	static std::unordered_set<Gizmo*> gizmos;
	Gizmo();
	~Gizmo();
	bool enabled = true;
	virtual void DrawGizmo(const Camera& cam) = 0;
	static void DrawAllGizmos(const Camera& cam);
};

class Gizmo3D :public Gizmo, public Model
{
	static shared_ptr<Shader> gizmoShader;
public:
	shared_ptr<Shader> GetSh();
	Gizmo3D(const std::string& modelName);
	void DrawGizmo(const Camera& cam) override;
};


class TransformGizmo3D : public Gizmo
{
	Gizmo3D axis;
	mat4 rotMat;
public:
	bool enabled = true;
	mat4 scaleMat = scale(mat4(1), { .2,.2,.2 });
	mat4 posMat = mat4(1);
	TransformGizmo3D();

	void DrawGizmo(const Camera& cam) override;

};