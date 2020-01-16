#pragma once
#include <engine\utils\types_3d.h>
#include <gl\GL.h>

#include "Renderable.h"

class Cube : Renderable
{
private:
	float size;
	YVec3f position;
	YColor color;
	GLuint shaderId;
	YVbo* vbo;

	void createVBO();

public:
	Cube(YVec3f position = YVec3f(0, 0, 0), float size = 1.0f, YColor color = YColor(1.0f, 1.0f, 1.0f, 1.0f) , const char* shaderPath = "shaders/cube_debug");
	~Cube();

	virtual void draw() override;
};

