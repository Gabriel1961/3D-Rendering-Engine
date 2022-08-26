#include "EngineMath.h"
using namespace glm;
vec3 GetPosFromMat(const mat4& m)
{
	return { m[3][0],m[3][1],m[3][2] };
}

float SumComponents(const glm::vec3& v)
{
	return v.x + v.y + v.z;
}
