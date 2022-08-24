#pragma once
#include <string>
#include <Common.h>
#include <unordered_map>
#include <../ScreenSize.h>
using namespace glm;
class Texture
{
protected:
	int height{}, width{}, bpp{};
public:
	static std::unordered_map<std::string, std::pair<Texture*,int>> cachedTextures;
	std::string filePath = "";
	uint renderId{};

	std::string type = "texture_diffuse";

	Texture(const std::string& _FilePath);
	Texture() {}
	Texture(uint heigth, uint witdth, GLenum antiAliasing = GL_NEAREST, GLenum format = GL_RGBA32F);
	Texture(ARGBColor* buffer, GLenum antiAliasing = GL_NEAREST);
	~Texture();

	Texture(Texture&& o) noexcept;
	Texture(const Texture& tex) = default;
	Texture& operator=(const Texture&) = default;
	void SetScalingMode(GLenum wrapS, GLenum wrapT);
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int  GetHeight()const { return height; }
	int  GetWidth()const { return width; }
	ivec2 GetSize() const { return { width,height }; }
};
class DepthTexture : public Texture
{
public:
	DepthTexture(ivec2 size)
	{
		width = size.x;
		height = size.y;

		gc(glGenTextures(1, &renderId));
		gc(glBindTexture(GL_TEXTURE_2D, renderId));
		gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		vec4 clampColor = { 1,1,1,1 };
		gc(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &clampColor.x));
	}
};