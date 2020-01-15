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
	Cube(YVec3f position, float size, YColor color, const char* shaderPath);
	~Cube();
};

