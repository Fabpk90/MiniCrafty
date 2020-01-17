#pragma once

#include "Renderable.h"
#include "external/gl/glew.h"
#include <engine\utils\types_3d.h>
#include <engine/render/vbo.h>


class Cube : public Renderable
{
private:
	float size;
	YVec3f position;
	YColor color;
	GLuint shaderId;
	YVbo* vbo;

	void createVBO();

public:
	Cube(YVec3f position = YVec3f(0, 0, 0), float size = 1.0f, YColor color = YColor(1.0f, 1.0f, 1.0f, 1.0f) , const char* shaderPath = "shaders/cube");
	~Cube();

	YVbo* GetVbo() { return vbo; }

	virtual void render() override;
};

