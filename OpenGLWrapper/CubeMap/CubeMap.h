#pragma once
#include "../Common.h"
#include "../Renderer.h"
#include "../vendor/stb_image/stb_image.h"

class CubeMap
{
	uint renderId;
	static const std::vector<std::string> defaultCubeMapPaths;
public:
	Shader* shader{};
	CubeMap(const std::vector<std::string>& textures, const std::string& shader );
	void Render();
	const uint& GetRenderId() { return renderId; }
	~CubeMap();
};

