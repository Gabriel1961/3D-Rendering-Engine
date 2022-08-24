#pragma once
#include "Common.h"
#include "Texture.h"
#include "../ScreenSize.h"
class FrameBuffer
{
private:
	uint renderId = 0;
public:
	shared_ptr<DepthTexture> depthTexture;
	FrameBuffer()
	{
		gc(glGenFramebuffers(1, &renderId));
	}

	void AttachDepthTexture(shared_ptr<DepthTexture> tex)
	{
		depthTexture = tex;
		gc(glBindFramebuffer(GL_FRAMEBUFFER, renderId));
		gc(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->renderId, 0));
		
		gc(glDrawBuffer(GL_NONE));
		gc(glReadBuffer(GL_NONE)); // explicitly tell OpenGL not to render any color data

		gc(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 
	}

	void Bind()
	{
		gc(glViewport(0, 0, depthTexture->GetWidth(), depthTexture->GetHeight()));
		gc(glBindFramebuffer(GL_FRAMEBUFFER,renderId));
	}
	void Clear()
	{
		gc(glClear(GL_DEPTH_BUFFER_BIT));
	}

	void Unbind()
	{
		gc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		gc(glViewport(0, 0, Window_Width, Window_Height));
	}
	~FrameBuffer()
	{
		gc(glDeleteFramebuffers(1, &renderId));
	}
};

