#include "CubeMap.h"

static const char* SHADER_PATH = "OpenGLWrapper/PixelDisplay.shader";

const std::vector<std::string> CubeMap::defaultCubeMapPaths = {
	"posx.jpg",
	"negx.jpg",
	"posy.jpg",
	"negy.jpg",
	"posz.jpg",
	"negz.jpg"
};

CubeMap::CubeMap(const std::vector<std::string>& textures = defaultCubeMapPaths,const std::string& shaderPath = SHADER_PATH)
{	
	gc(glGenTextures(1, &renderId));
	gc(glBindTexture(GL_TEXTURE_CUBE_MAP, renderId));
	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		data = stbi_load(textures[i].c_str(), &width, &height, &nrChannels, 0);
		gc(glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		));
		stbi_image_free(data);
	}
	gc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	gc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	gc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	

	/// Load the shader
	shader = new Shader(shaderPath);
}

void CubeMap::Render()
{
	
	gc(glDepthMask(false));
	
}

CubeMap::~CubeMap()
{
	delete shader;
}
