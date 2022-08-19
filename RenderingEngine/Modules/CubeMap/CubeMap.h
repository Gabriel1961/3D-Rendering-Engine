#pragma once
#include "Common.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"
#include "Modules/Camera.h"
#include "Models/PreBuilt/CubeModel.h"
static const char* CUBE_SHADER_PATH = SHADER_PATH "CubeMap.shader";

class CubeMap
{
	uint renderId;
	static const std::vector<std::string> defaultCubeMapPaths;
public:
	CubeModel* cube;
	shared_ptr<Shader>shader{};
	CubeMap(const std::vector<std::string>& textures = defaultCubeMapPaths, const std::string& shader = CUBE_SHADER_PATH);
	void Draw(const Camera& cam);
	const uint& GetRenderId() { return renderId; }
	~CubeMap();
};

