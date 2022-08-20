#pragma once
#include "Renderer.h"
class DebugLine
{
private:
	Shader* sh;
	uint ibData[2]{ 0,1 };
	IndexBuffer ib = IndexBuffer(2, ibData);
	static std::vector<DebugLine*>* lines;
	VertexArray* va = 0;
	void Draw(const mat4& view,const mat4& proj)
	{
		float vbData[]{ start.x,start.y,start.z,end.x,end.y,end.z };
		VertexBufferLayout vbl;
		vbl.Push<float>(3);
		VertexBuffer vb(sizeof(vbData), vbData);
		delete va;
		va = new VertexArray;
		va->AddLayout(vb, vbl);

		sh->SetUniformMat4f("u_model", modelMat);
		sh->SetUniformMat4f("u_view", view);
		sh->SetUniformMat4f("u_proj", proj);
		sh->SetUniform3f("color", color);
		Renderer::Draw(*va, ib, *sh, GL_LINES);
	}
public:
	mat4 modelMat = mat4(1);
	glm::vec3 color = { 1,0,1 };
	static void RenderLines(const mat4& view, const mat4& proj)
	{
		for (auto x : *lines)
			x->Draw(view,proj);
	}
	glm::vec3 start = { 0,0,0 }, end = { 1,1,1 };
	DebugLine(glm::vec3 start, glm::vec3 end, glm::vec4 color = { 1,1,1,1 })
		:start(start), end(end), color(color)
	{
		sh = new Shader("OpenGLWrapper/Line.shader");
		lines->push_back(this);
	}
	~DebugLine()
	{
		delete sh;
		delete va;
	}
};

