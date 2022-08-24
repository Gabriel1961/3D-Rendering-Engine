#pragma once
#include "3DRenderingEngine.h"
struct Vertex;
namespace Physics
{
	struct Ray
	{
		vec3 pos;
		vec3 dir;
		vec3 GetPosOnRay(float t) const;
		Ray(vec3 pos, vec3 dir);
	};

	struct Rayhit
	{
		vec3 pos;
		float dist;
	};

	class BoundingBox
	{
	public:
		vec3 maxp = {INFINITY,INFINITY ,INFINITY };
		vec3 minp = {-INFINITY,-INFINITY,-INFINITY};
		BoundingBox();

		BoundingBox(vec3 minp, vec3 maxp);

		BoundingBox(const std::vector<Vertex>& verts, const glm::mat4& modelMat);

		BoundingBox(const std::vector<BoundingBox*>& bbs);

		bool Intersect(const Ray& r, Rayhit& hit);
	protected:
		std::pair<float, vec3> IntersectZPlane(const Ray& r, float t);

		std::pair<float, vec3> IntersectXPlane(const Ray& r, float t);

		std::pair<float, vec3> IntersectYPlane(const Ray& r, float t);
	};

}