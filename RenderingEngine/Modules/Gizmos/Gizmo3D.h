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


class IMovableGizmo
{
public:
	inline virtual void SetParentModel(weak_ptr<Model> v) = 0;
	inline virtual void SetEnabled(bool v) = 0;
	virtual int GetIntersectedAxis(const Physics::Ray& r) = 0;
	/// <param name="moveVec"> in world space </param>
	/// <param name="axis"></param>
	virtual void UpdateModelTransform(glm::vec3 moveVec,const glm::mat4& cameraMat,int axis) = 0;
};

class TransformGizmo3D : public Gizmo,public IMovableGizmo
{
	Gizmo3D axis;
	mat4 rotMat;
public:
	bool enabled = true;
	mat4 scaleMat = scale(mat4(1), { .2,.2,.2 });
	
	std::weak_ptr<Model> parentModel;

	TransformGizmo3D();
	/// Returns false if ray missed
	int GetIntersectedAxis(const Physics::Ray& r);
	void DrawGizmo(const Camera& cam) override;
	void UpdateModelTransform(glm::vec3 moveVec, const glm::mat4& cameraMat, int axis);
	inline void SetEnabled(bool v)
	{
		enabled = v;
	}
	inline void SetParentModel(weak_ptr<Model> v)
	{
		parentModel = v;
	}

};