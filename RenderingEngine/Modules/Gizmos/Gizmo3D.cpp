#include "Gizmo3d.h"
std::unordered_set<Gizmo*> Gizmo::gizmos;
shared_ptr<Shader> Gizmo3D::gizmoShader = 0;

Gizmo::Gizmo()
{
	gizmos.insert(this);
}

Gizmo::~Gizmo()
{
	gizmos.erase(this);
}

void Gizmo::DrawAllGizmos(const Camera& cam)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	std::vector<Gizmo*> sorted(gizmos.begin(), gizmos.end());
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

	static float x = 0;
	axis.meshes[0].mat->diffuse = { 1,0,0,1 };
	ImGui::SliderFloat("sd", &x, 0, 10);
	//x
	mat4 rot = glm::rotate(rotMat, 0.0f, { 0,1,0 });
	axis.meshes[0].mat->diffuse = { 1,0,0,0 };
	axis.modelMat = translate(mat4(1), pos)*translate(scaleMat * rot, { 0,0,7 });
	axis.Render(cam);
	//y
	rot = glm::rotate(rotMat, 2 * pi - pi / 2, { 1,0,0 });
	axis.meshes[0].mat->diffuse = { 0,1,0,0 };
	axis.modelMat = translate(mat4(1), pos)*translate(scaleMat * rot, { 0,0,7 });
	axis.Render(cam);
	//z
	rot = glm::rotate(rotMat, 2 * pi - pi / 2, { 0,1,0 });
	axis.meshes[0].mat->diffuse = { 0,0,1,0 };
	axis.modelMat = translate(mat4(1),pos) * translate(scaleMat * rot, {0,0,7});
	axis.Render(cam);
}