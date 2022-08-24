#include "Gizmo3d.h"
#include "../Scenes/Scene.h"
shared_ptr<Shader> Gizmo3D::gizmoShader = 0;


Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

void Gizmo::DrawAllGizmos(const Camera& cam,Scene* s)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	std::vector<shared_ptr<Gizmo>> sorted(s->gizmos.begin(), s->gizmos.end());
	std::sort(sorted.begin(), sorted.end(),
		[&cam](const auto& a, const auto& b) {
			vec3 d1 = vec3(a->pos) - cam.position, d2 = vec3(b->pos) - cam.position;
			return length(d1) > length(d2);
		});
	for (auto g : sorted)
		g->DrawGizmo(cam);

}

shared_ptr<Shader> Gizmo3D::GetSh()
{
	if (gizmoShader == 0)
		gizmoShader = make_shared<Shader>(SHADER_PATH "Gizmo3d.shader");
	return gizmoShader;
}

Gizmo3D::Gizmo3D(const std::string& modelName) : Model((MODEL_PATH "Gizmo/" + modelName).c_str(), GetSh())
{

}

void Gizmo3D::DrawGizmo(const Camera& cam)
{
	
	this->Render(cam);
}

TransformGizmo3D::TransformGizmo3D() : axis("ArrowGizmo.fbx")
{
	rotMat = glm::rotate(mat4(1), pi / 2, { 0,1,0 });
}
void TransformGizmo3D::DrawGizmo(const Camera& cam) 
{
	if (!enabled)
		return;

	if (shared_ptr<Model> r = parentModel.lock()) {
		pos = vec3(r->modelMat[3]);
	}
	
	//x
	mat4 rot = glm::rotate(rotMat, 0.0f, { 0,1,0 });
	axis.meshes[0].mat->diffuse = { 1,0,0,1 };
	axis.meshes[0].modelMat = translate(mat4(1), pos)*translate(scaleMat * rot, { 0,0,7 });
	axis.Render(cam);
	//y
	rot = glm::rotate(rotMat, 2 * pi - pi / 2, { 1,0,0 });
	axis.meshes[0].mat->diffuse = { 0,1,0,1 };
	axis.meshes[0].modelMat = translate(mat4(1), pos)*translate(scaleMat * rot, { 0,0,7 });
	axis.Render(cam);
	//z
	rot = glm::rotate(rotMat, 2 * pi - pi / 2, { 0,1,0 });
	axis.meshes[0].mat->diffuse = { 0,0,1,1 };
	axis.meshes[0].modelMat = translate(mat4(1),pos) * translate(scaleMat * rot, {0,0,7});
	axis.Render(cam);
}

using namespace Physics;
bool TransformGizmo3D::HandleMouseInput(shared_ptr<Model> m,const Ray& r, Camera& cam)
{
	BoundingBox bbs[3];
	mat4 rot = glm::rotate(rotMat, 0.0f, { 0,1,0 });
	axis.meshes[0].modelMat = translate(mat4(1), pos) * translate(scaleMat * rot, { 0,0,7 });
	axis.meshes[0].CalculateBoundingBox();
	bbs[0] = axis.meshes[0].boundingBox;

	rot = glm::rotate(rotMat, 2 * pi - pi / 2, { 1,0,0 });
	axis.meshes[0].modelMat = translate(mat4(1), pos) * translate(scaleMat * rot, { 0,0,7 });
	axis.meshes[0].CalculateBoundingBox();
	bbs[1] = axis.meshes[0].boundingBox;

	rot = glm::rotate(rotMat, 2 * pi - pi / 2, { 0,1,0 });
	axis.meshes[0].modelMat = translate(mat4(1), pos) * translate(scaleMat * rot, { 0,0,7 });
	axis.meshes[0].CalculateBoundingBox();
	bbs[2] = axis.meshes[0].boundingBox;
	

	float dist = INFINITY;
	int idx = -1;
	Rayhit rhd;
	for (int i = 0; i < 3; i++) {
		Rayhit rh;
		if (bbs[i].Intersect(r, rh))
		{
			if (dist > rh.dist) {
				dist = rh.dist;
				idx = i;
				rhd = rh;
			}
		}
	}

	static int prevAxis = -1;

	if (idx == -1)
		return idx;
	if (idx != prevAxis)
	{
		prevInputPos = vec3{ INFINITY,INFINITY ,INFINITY };
		prevAxis = idx;
		return -1;
	}
	if(isinf(prevInputPos.x))
		prevInputPos = rhd.pos;

	vec2 speed = { Input::Mouse::SpeedX,Input::Mouse::SpeedY };
	vec3 vs[] = { {1,0,0},{0,1,0},{0,0,1} };
	vec3 v = vs[idx];
	vec3 delta = rhd.pos - prevInputPos;
	prevInputPos = rhd.pos;
	
	prevAxis = idx;
	m->modelMat = translate(m->modelMat, delta * v);
	return idx;
}
