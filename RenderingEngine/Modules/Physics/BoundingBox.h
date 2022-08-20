#pragma once
#include "3DRenderingEngine.h"
#include "../Mesh.h"
namespace Physics
{
	struct Ray
	{
		vec3 pos;
		vec3 dir;
		vec3 GetPosOnRay(float t) const
		{
			return pos + dir * t;
		}
		Ray(vec3 pos, vec3 dir)
			:pos(pos),dir(normalize(dir))
		{
		}
	};

	struct Rayhit
	{
		vec3 pos;
	};

	class BoundingBox
	{
	public:
		vec3 maxp = {INFINITY,INFINITY ,INFINITY };
		vec3 minp = {-INFINITY,-INFINITY,-INFINITY};
		BoundingBox()
		{

		}

		BoundingBox(vec3 minp,vec3 maxp) : minp(minp), maxp(maxp)
		{

		}

		BoundingBox(const std::vector<Vertex>& verts)
		{
			for (int i = 0; i < verts.size(); i++)
			{
				maxp = glm::max(maxp, verts[i].position);
				minp = glm::max(maxp, verts[i].position);
			}
		}

		bool Intersect(const Ray& r, Rayhit& hit)
		{
			vec3 t1 = (minp - r.pos) / r.dir;
			vec3 t2 = (maxp - r.pos) / r.dir;
			std::pair<float, vec3> res[6];
			res[0] = IntersectXPlane(r, t1.x);
			res[1] = IntersectXPlane(r, t2.x);
			res[2] = IntersectYPlane(r, t1.y);
			res[3] = IntersectYPlane(r, t2.y);
			res[4] = IntersectZPlane(r, t1.z);
			res[5] = IntersectZPlane(r, t2.z);
			bool hitr = 0;
			float ans = INFINITY;
			vec3 ansp{};
			for (int i = 0; i < 6; i++)
				if (res[i].first > 0) {
					if (ans > res[i].first) {
						ans = res[i].first;
						ansp = res[i].second;
					}
					hitr = 1;
				}
			hit.pos = ansp;
			return hitr;
		}
	protected:
		std::pair<float, vec3> IntersectZPlane(const Ray& r, float t)
		{
			if(t<0)
				return { t,{0,0,0} };
			vec3 p = r.GetPosOnRay(t);
			if (p.x > minp.x && p.x < maxp.x && p.y > minp.y && p.y < maxp.y)
				return { t,p };
			return { -1,{0,0,0} };
		}

		std::pair<float, vec3> IntersectXPlane(const Ray& r, float t)
		{
			if (t < 0)
				return { t,{0,0,0} };
			vec3 p = r.GetPosOnRay(t);
			if (p.z > minp.z && p.z < maxp.z && p.y > minp.y && p.y < maxp.y)
				return { t,p };
			return { -1,{0,0,0} };
		}

		std::pair<float, vec3> IntersectYPlane(const Ray& r, float t)
		{
			if (t < 0)
				return { t,{0,0,0} };
			vec3 p = r.GetPosOnRay(t);
			if (p.x > minp.x && p.x < maxp.x && p.z > minp.z && p.z < maxp.z)
				return { t,p };
			return { -1,{0,0,0} };
		}
	};

}