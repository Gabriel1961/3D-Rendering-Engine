#pragma once
#include "Common.h"
class FrameBuffer
{
private:
	uint renderId = 0;
public:
	uint width, height;
	FrameBuffer(int _witdth = 1000, int _height = 1000)
	{
		width = _witdth;
		height = _height;
		gc(glGenFramebuffers(1, &renderId));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT     ));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT     ));
	}
	void Bind()
	{
		gc(glBindFramebuffer(GL_FRAMEBUFFER,renderId));
	}
	void Unbind()
	{
		gc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	~FrameBuffer()
	{
		gc(glDeleteBuffers(1, &renderId));
	}
};

