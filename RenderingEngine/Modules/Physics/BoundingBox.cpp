#include "BoundingBox.h"
#include "../Mesh.h"

Physics::BoundingBox::BoundingBox()
{

}

Physics::BoundingBox::BoundingBox(vec3 minp, vec3 maxp) : minp(minp), maxp(maxp)
{

}

Physics::BoundingBox::BoundingBox(const std::vector<Vertex>& verts, const glm::mat4& modelMat)
{
	maxp = { -INFINITY,-INFINITY ,-INFINITY };
	minp = {  INFINITY, INFINITY, INFINITY };
	for (int i = 0; i < verts.size(); i++)
	{
		vec3 pos = modelMat * vec4(verts[i].position, 1);
		maxp = glm::max(maxp, pos);
		minp = glm::min(minp, pos);
	}
}

Physics::BoundingBox::BoundingBox(const std::vector<BoundingBox*>&bbs)
{
	maxp = { -INFINITY,-INFINITY ,-INFINITY };
	minp = { INFINITY, INFINITY, INFINITY };
	for (auto r : bbs)
	{
		maxp = glm::max(maxp, r->maxp);
		minp = glm::min(minp, r->minp);
	}
}

bool Physics::BoundingBox::Intersect(const Ray& r, Rayhit& hit)
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
	hit.dist = ans;
	return hitr;
}

std::pair<float, vec3> Physics::BoundingBox::IntersectZPlane(const Ray& r, float t)
{
	if (t < 0)
		return { t,{ 0,0,0 } };
	vec3 p = r.GetPosOnRay(t);
	if (p.x > minp.x && p.x < maxp.x && p.y > minp.y && p.y < maxp.y)
		return { t,p };
	return { -1,{ 0,0,0 } };
}

std::pair<float, vec3> Physics::BoundingBox::IntersectXPlane(const Ray& r, float t)
{
	if (t < 0)
		return { t,{ 0,0,0 } };
	vec3 p = r.GetPosOnRay(t);
	if (p.z > minp.z && p.z < maxp.z && p.y > minp.y && p.y < maxp.y)
		return { t,p };
	return { -1,{ 0,0,0 } };
}

std::pair<float, vec3> Physics::BoundingBox::IntersectYPlane(const Ray& r, float t)
{
	if (t < 0)
		return { t,{ 0,0,0 } };
	vec3 p = r.GetPosOnRay(t);
	if (p.x > minp.x && p.x < maxp.x && p.z > minp.z && p.z < maxp.z)
		return { t,p };
	return { -1,{ 0,0,0 } };
}

vec3 Physics::Ray::GetPosOnRay(float t) const
{
	return pos + dir * t;
}

Physics::Ray::Ray(vec3 pos, vec3 dir)
	:pos(pos), dir(normalize(dir))
{
}
