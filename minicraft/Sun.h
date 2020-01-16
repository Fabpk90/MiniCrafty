#pragma once
#include "Cube.h"
class Sun :
	public Cube
{
private:
	YColor backgroundColor;

public:
	Sun(YColor background, YVec3f position = YVec3f(0, 0, 0), float size = 1.0f, YColor color = YColor(1.0f, 1.0f, 1.0f, 1.0f), const char* shaderPath = "shaders/cube_debug");

	void draw() override;
};

