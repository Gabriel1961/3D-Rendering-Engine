#pragma once
#include "3DRenderingEngine.h"
#include "./Modules/Mesh.h"
#include "Modules/Physics/BoundingBox.h"
#include <unordered_set>
class Scene;
class Gizmo
{

public:
	vec3 pos{};
	Gizmo( );
	~Gizmo();
	bool enabled = true;
	virtual void DrawGizmo(const Camera& cam) = 0;
	static void DrawAllGizmos(const Camera& cam,Scene* s);
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
	vec3 prevInputPos = vec3(INFINITY, INFINITY, INFINITY);
	bool enabled = true;
	mat4 scaleMat = scale(mat4(1), { .2,.2,.2 });
	
	std::weak_ptr<Model> parentModel ;

	TransformGizmo3D();
	/// Returns false if ray missed
	bool HandleMouseInput(shared_ptr<Model> m, const Physics::Ray& r, Camera& cam);
	void DrawGizmo(const Camera& cam) override;

};