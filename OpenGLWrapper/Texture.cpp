#include "Texture.h"
#include "Common.h"
#include "Shader.h"
#include "vendor/stb_image/stb_image.h"

//#define TEXTURE_DEBUG
#ifdef TEXTURE_DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif // DEBUG

std::unordered_map < std::string, std::pair<Texture*,int>> Texture::cachedTextures;

using std::cout;
/// <summary>
/// format GL_RGBA8
/// </summary>
/// <param name="_FilePath"></param>
Texture::Texture(const std::string& _filePath)
{

	auto it = cachedTextures.find(_filePath);
	if (it != cachedTextures.end())
	{
		*this = *(it->second).first;
		(it->second).second++;
		return;
	}
	else
	{ // Do normal init 
		filePath = _filePath;
		DBG(std::cout << "Loaded " << filePath << " " << renderId << endl);
		if (_filePath.find(".bmp") == _filePath.npos)
			stbi_set_flip_vertically_on_load(1);
		uchar* tempBuf = stbi_load(_filePath.c_str(), &width, &height, &bpp, 4);
		if (!tempBuf ) {
			cout << "Texture file not found\n";
			assert(false);
		}
		gc(glGenTextures(1, &renderId));
		gc(glBindTexture(GL_TEXTURE_2D, renderId));

		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempBuf));
		gc(glBindTexture(GL_TEXTURE_2D, 0));
		if (tempBuf)
		{
			stbi_image_free(tempBuf);
		}

	}
}


/// <summary>
/// Generate arbitrary size buffer
/// Format : GL_RGBA32F
/// </summary>
Texture::Texture(uint heigth, uint witdth, GLenum antiAliasing, GLenum format)
	:height(heigth), width(witdth)
{
	gc(glActiveTexture(GL_TEXTURE0));
	gc(glGenTextures(1, &renderId));
	gc(glBindTexture(GL_TEXTURE_2D, renderId));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, witdth, heigth, 0, GL_RGBA, GL_FLOAT,
		NULL));
	gc(glBindImageTexture(0, renderId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));
}
/// <summary>
/// Format ARGB
/// </summary>
Texture::Texture(ARGBColor* buffer, GLenum antiAliasing)
{
	height = Window_Height;
	width = Window_Width;
	uchar* tempBuffer = (unsigned char*)buffer;
	gc(glGenTextures(1, &renderId));
	gc(glBindTexture(GL_TEXTURE_2D, renderId));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempBuffer));
	gc(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	if (!isAppClosed)
	{
		auto it = cachedTextures.find(filePath);
		if (it != cachedTextures.end())
		{
			it->second.second--;
			if(it->second.second == 0)
				cachedTextures.erase(it);
		}
	}
	gc(glDeleteTextures(1, &renderId));
}


Texture::Texture(Texture&& o) noexcept
{
	height = o.height;
	width = o.width;
	bpp = o.bpp;
	renderId = o.renderId;

	o.renderId = 0;
}

void Texture::SetScalingMode(GLenum wrapS, GLenum wrapT)
{
	Bind();
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
}

void Texture::Bind(unsigned int slot) const
{
	gc(glActiveTexture(GL_TEXTURE0 + slot));
	gc(glBindTexture(GL_TEXTURE_2D, renderId));
}

void Texture::Unbind() const
{
	gc(glBindTexture(GL_TEXTURE_2D, 0));
}



