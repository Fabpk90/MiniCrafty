#include "Sun.h"

Sun::Sun(YColor background, YVec3f position, float size, YColor color, const char* shaderPath ) 
	: Cube(position, size, color, shaderPath)
{
	backgroundColor = background;
}

void Sun::render()
{
	
}
