#pragma once 
#include <3DRenderingEngine.h>
#include "Modules/Camera.h"
#include "Modules/Material/Material.h"
#include "Modules/Mesh.h"
class CubeModel : public Model
{

public:
	CubeModel(std::shared_ptr<Material> mat = make_shared<Material>(), std::shared_ptr<Shader>shader = 0);
	Mesh* m;
};
