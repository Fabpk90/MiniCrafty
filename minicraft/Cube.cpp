#include "Cube.h"
#include "src/engine_minicraft.h"

void Cube::createVBO()
{
	// Creation du VBO
	vbo = new YVbo(3, 36, YVbo::PACK_BY_ELEMENT_TYPE);

	// Définition du contenu VBO
	vbo->setElementDescription(0, YVbo::Element(3)); //Sommet
	vbo->setElementDescription(1, YVbo::Element(3)); //Normale
	vbo->setElementDescription(2, YVbo::Element(2)); //UV

	vbo->createVboCpu();

	// On ajoute les sommets
	int iVertice = 0;

	// Surface 1
	vbo->setElementValue(0, iVertice, 0, 0, 0);
	vbo->setElementValue(1, iVertice, 0, -1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 0, 0);
	vbo->setElementValue(1, iVertice, 0, -1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 0, 1);
	vbo->setElementValue(1, iVertice, 0, -1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	// Surface 1
	vbo->setElementValue(0, iVertice, 1, 0, 0);
	vbo->setElementValue(1, iVertice, 0, -1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 0, 1);
	vbo->setElementValue(1, iVertice, 0, -1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 0, 1);
	vbo->setElementValue(1, iVertice, 0, -1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;

	// Surface 2
	vbo->setElementValue(0, iVertice, 0, 0, 0);
	vbo->setElementValue(1, iVertice, -1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 0, 1);
	vbo->setElementValue(1, iVertice, -1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 0);
	vbo->setElementValue(1, iVertice, -1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	// Surface 2
	vbo->setElementValue(0, iVertice, 0, 1, 0);
	vbo->setElementValue(1, iVertice, -1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 0, 1);
	vbo->setElementValue(1, iVertice, -1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 1);
	vbo->setElementValue(1, iVertice, -1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;


	// Surface 3
	vbo->setElementValue(0, iVertice, 0, 0, 0);
	vbo->setElementValue(1, iVertice, 0, 0, -1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 0);
	vbo->setElementValue(1, iVertice, 0, 0, -1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 0, 0);
	vbo->setElementValue(1, iVertice, 0, 0, -1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	// Surface 3
	vbo->setElementValue(0, iVertice, 1, 0, 0);
	vbo->setElementValue(1, iVertice, 0, 0, -1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 0);
	vbo->setElementValue(1, iVertice, 0, 0, -1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 1, 0);
	vbo->setElementValue(1, iVertice, 0, 0, -1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;


	// Surface 1
	vbo->setElementValue(0, iVertice, 1, 1, 0);
	vbo->setElementValue(1, iVertice, 0, 1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 0);
	vbo->setElementValue(1, iVertice, 0, 1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 1);
	vbo->setElementValue(1, iVertice, 0, 1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	// Surface 1
	vbo->setElementValue(0, iVertice, 0, 1, 1);
	vbo->setElementValue(1, iVertice, 0, 1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 1, 1);
	vbo->setElementValue(1, iVertice, 0, 1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 1, 0);
	vbo->setElementValue(1, iVertice, 0, 1, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;


	// Surface 2
	vbo->setElementValue(0, iVertice, 1, 0, 0);
	vbo->setElementValue(1, iVertice, 1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 1, 0);
	vbo->setElementValue(1, iVertice, 1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 0, 1);
	vbo->setElementValue(1, iVertice, 1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	// Surface 2
	vbo->setElementValue(0, iVertice, 1, 1, 0);
	vbo->setElementValue(1, iVertice, 1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 1, 1);
	vbo->setElementValue(1, iVertice, 1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 0, 1);
	vbo->setElementValue(1, iVertice, 1, 0, 0);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;


	// Surface 3
	vbo->setElementValue(0, iVertice, 0, 0, 1);
	vbo->setElementValue(1, iVertice, 0, 0, 1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 0, 1);
	vbo->setElementValue(1, iVertice, 0, 0, 1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 1);
	vbo->setElementValue(1, iVertice, 0, 0, 1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	// Surface 3
	vbo->setElementValue(0, iVertice, 1, 0, 1);
	vbo->setElementValue(1, iVertice, 0, 0, 1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 1, 1, 1);
	vbo->setElementValue(1, iVertice, 0, 0, 1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;
	vbo->setElementValue(0, iVertice, 0, 1, 1);
	vbo->setElementValue(1, iVertice, 0, 0, 1);
	vbo->setElementValue(2, iVertice, 0, 0);
	++iVertice;

	// On envoie le contenu au GPU
	vbo->createVboGpu();
	// On clean le contenu du CPU
	vbo->deleteVboCpu();
}

Cube::Cube(YVec3f position, float size, YColor color, const char* shaderPath)
	: position(position), color (color), size(size)
{
	shaderId = MEngineMinicraft::getInstance()->Renderer->createProgram(shaderPath);
	createVBO();
}

Cube::~Cube()
{
	glDeleteProgram(shaderId);
	delete vbo;
}

void Cube::draw()
{
	auto renderer = MEngineMinicraft::getInstance()->Renderer;
	glUseProgram(shaderId); //Demande au GPU de charger ces shaders
	renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
	renderer->sendMatricesToShader(shaderId); //Envoie les matrices au shader
	vbo->render(); //Demande le rendu du VBO
}
