#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, unsigned int DrawMode)
{
	shader.Bind();
	if(shader.preRender) 
		shader.preRender(&shader);
	va.Bind();
	ib.Bind();
	gc(glDrawElements(DrawMode, ib.GetCount(), ib.GetType(), nullptr));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const Texture& tex, std::pair<std::string, int> SamplerNameAndSlot, unsigned int DrawMode)
{
	shader.Bind();
	if(shader.preRender) shader.preRender(&shader);
	shader.SetUniform1i(SamplerNameAndSlot.first, SamplerNameAndSlot.second);
	tex.Bind(SamplerNameAndSlot.second);
	va.Bind();
	ib.Bind();
	gc(glDrawElements(DrawMode, ib.GetCount(), ib.GetType(), nullptr));
}

void Renderer::DrawArray(const VertexArray& va, Shader& shader, uint count, GLenum drawMode)
{
	shader.Bind();
	if(shader.preRender) shader.preRender(&shader);
	va.Bind();
	glDrawArrays(drawMode, 0, count);
}
