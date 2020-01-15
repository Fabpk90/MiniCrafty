#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine/engine.h"

#include "avatar.h"
#include "world.h"
#include <sysinfoapi.h>

class MEngineMinicraft : public YEngine {
private:
	YVbo* cubeLel;
	YVbo* sun;

	GLuint ShaderCubeDebug;
	GLuint ShaderSun;
	GLuint shaderColorLocation;

	YColor sunColor;
public :
	//Gestion singleton
	static YEngine * getInstance()
	{
		if (Instance == NULL)
			Instance = new MEngineMinicraft();
		return Instance;
	}

	

	/*HANDLERS GENERAUX*/
	void loadShaders() {
		
	}

	void init() 
	{
		YLog::log(YLog::ENGINE_INFO,"Minicraft Started : initialisation");

		Renderer->setBackgroundColor(YColor(0.0f,0.0f,0.0f,1.0f));
		Renderer->Camera->setPosition(YVec3f(10, 10, 10));
		Renderer->Camera->setLookAt(YVec3f());
		
		cubeLel = new YVbo(3, 36, YVbo::PACK_BY_ELEMENT_TYPE);
		cubeLel->setElementDescription(0, YVbo::Element(3));
		cubeLel->setElementDescription(1, YVbo::Element(3));
		cubeLel->setElementDescription(2, YVbo::Element(2));

		cubeLel->createVboCpu();

		fillVBOCube(cubeLel);
		//glDisable(GL_CULL_FACE);

		
		ShaderCubeDebug = Renderer->createProgram("shaders/cube_debug");

		sun = new YVbo(3, 36, YVbo::PACK_BY_ELEMENT_TYPE);
		sun->setElementDescription(0, YVbo::Element(3));
		sun->setElementDescription(1, YVbo::Element(3));
		sun->setElementDescription(2, YVbo::Element(2));

		sun->createVboCpu();

		fillVBOCube(sun, 1.f);

		ShaderSun = Renderer->createProgram("shaders/sun");
		shaderColorLocation = glGetUniformLocation(ShaderSun, "sun_color");

		sunColor = YColor(1.0f, 1.0f, 0.0f, 1.0f);
		SYSTEMTIME s;
		GetLocalTime(&s);

		//sunColor.interpolate()
	}

	int addQuadToVbo(YVbo* vbo, int iVertice, YVec3f& a, YVec3f& b, YVec3f& c, YVec3f& d)
	{
		YVec3f normal = (b - a).cross(d - a);
		normal.normalize();

		vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 0, 0);

		iVertice++;

		vbo->setElementValue(0, iVertice, b.X, b.Y, b.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 1, 0);

		iVertice++;

		vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 1, 1);

		iVertice++;

		vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 0, 0);

		iVertice++;

		vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 1, 1);

		iVertice++;

		vbo->setElementValue(0, iVertice, d.X, d.Y, d.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 0, 1);

		iVertice++;

		return 6;
	}

	void fillVBOCube(YVbo* vbo, float size = 5.0f)
	{
		int iVertice = 0;

		YVec3f a(size / 2.0f, -size / 2.0f, -size / 2.0f);
		YVec3f b(size / 2.0f, size / 2.0f, -size / 2.0f);
		YVec3f c(size / 2.0f, size / 2.0f, size / 2.0f);
		YVec3f d(size / 2.0f, -size / 2.0f, size / 2.0f);
		YVec3f e(-size / 2.0f, -size / 2.0f, -size / 2.0f);
		YVec3f f(-size / 2.0f, size / 2.0f, -size / 2.0f);
		YVec3f g(-size / 2.0f, size / 2.0f, size / 2.0f);
		YVec3f h(-size / 2.0f, -size / 2.0f, size / 2.0f);

		iVertice += addQuadToVbo(vbo, iVertice, a, b, c, d); //x+
		iVertice += addQuadToVbo(vbo, iVertice, f, e, h, g); //x-
		iVertice += addQuadToVbo(vbo, iVertice, b, f, g, c); //y+
		iVertice += addQuadToVbo(vbo, iVertice, e, a, d, h); //y-
		iVertice += addQuadToVbo(vbo, iVertice, c, g, h, d); //z+
		iVertice += addQuadToVbo(vbo, iVertice, e, f, b, a); //z-
	}


	YVbo* createGPUCube() {
		// Creation du VBO
		auto VboCube = new YVbo(3, 36, YVbo::PACK_BY_ELEMENT_TYPE);

		// Définition du contenu VBO
		VboCube->setElementDescription(0, YVbo::Element(3)); //Sommet
		VboCube->setElementDescription(1, YVbo::Element(3)); //Normale
		VboCube->setElementDescription(2, YVbo::Element(2)); //UV

		VboCube->createVboCpu();

		// On ajoute les sommets
		int iVertice = 0;

		// Surface 1
		VboCube->setElementValue(0, iVertice, 0, 0, 0);
		VboCube->setElementValue(1, iVertice, 0, -1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 0, 0);
		VboCube->setElementValue(1, iVertice, 0, -1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 0, 1);
		VboCube->setElementValue(1, iVertice, 0, -1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		// Surface 1
		VboCube->setElementValue(0, iVertice, 1, 0, 0);
		VboCube->setElementValue(1, iVertice, 0, -1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 0, 1);
		VboCube->setElementValue(1, iVertice, 0, -1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 0, 1);
		VboCube->setElementValue(1, iVertice, 0, -1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;

		// Surface 2
		VboCube->setElementValue(0, iVertice, 0, 0, 0);
		VboCube->setElementValue(1, iVertice, -1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 0, 1);
		VboCube->setElementValue(1, iVertice, -1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 0);
		VboCube->setElementValue(1, iVertice, -1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		// Surface 2
		VboCube->setElementValue(0, iVertice, 0, 1, 0);
		VboCube->setElementValue(1, iVertice, -1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 0, 1);
		VboCube->setElementValue(1, iVertice, -1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 1);
		VboCube->setElementValue(1, iVertice, -1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;


		// Surface 3
		VboCube->setElementValue(0, iVertice, 0, 0, 0);
		VboCube->setElementValue(1, iVertice, 0, 0, -1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 0);
		VboCube->setElementValue(1, iVertice, 0, 0, -1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 0, 0);
		VboCube->setElementValue(1, iVertice, 0, 0, -1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		// Surface 3
		VboCube->setElementValue(0, iVertice, 1, 0, 0);
		VboCube->setElementValue(1, iVertice, 0, 0, -1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 0);
		VboCube->setElementValue(1, iVertice, 0, 0, -1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 1, 0);
		VboCube->setElementValue(1, iVertice, 0, 0, -1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;


		// Surface 1
		VboCube->setElementValue(0, iVertice, 1, 1, 0);
		VboCube->setElementValue(1, iVertice, 0, 1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 0);
		VboCube->setElementValue(1, iVertice, 0, 1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 1);
		VboCube->setElementValue(1, iVertice, 0, 1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		// Surface 1
		VboCube->setElementValue(0, iVertice, 0, 1, 1);
		VboCube->setElementValue(1, iVertice, 0, 1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 1, 1);
		VboCube->setElementValue(1, iVertice, 0, 1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 1, 0);
		VboCube->setElementValue(1, iVertice, 0, 1, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;


		// Surface 2
		VboCube->setElementValue(0, iVertice, 1, 0, 0);
		VboCube->setElementValue(1, iVertice, 1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 1, 0);
		VboCube->setElementValue(1, iVertice, 1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 0, 1);
		VboCube->setElementValue(1, iVertice, 1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		// Surface 2
		VboCube->setElementValue(0, iVertice, 1, 1, 0);
		VboCube->setElementValue(1, iVertice, 1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 1, 1);
		VboCube->setElementValue(1, iVertice, 1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 0, 1);
		VboCube->setElementValue(1, iVertice, 1, 0, 0);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;


		// Surface 3
		VboCube->setElementValue(0, iVertice, 0, 0, 1);
		VboCube->setElementValue(1, iVertice, 0, 0, 1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 0, 1);
		VboCube->setElementValue(1, iVertice, 0, 0, 1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 1);
		VboCube->setElementValue(1, iVertice, 0, 0, 1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		// Surface 3
		VboCube->setElementValue(0, iVertice, 1, 0, 1);
		VboCube->setElementValue(1, iVertice, 0, 0, 1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 1, 1, 1);
		VboCube->setElementValue(1, iVertice, 0, 0, 1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;
		VboCube->setElementValue(0, iVertice, 0, 1, 1);
		VboCube->setElementValue(1, iVertice, 0, 0, 1);
		VboCube->setElementValue(2, iVertice, 0, 0);
		++iVertice;

		// On envoie le contenu au GPU
		VboCube->createVboGpu();
		// On clean le contenu du CPU
		VboCube->deleteVboCpu();

		return VboCube;
	}

	void update(float elapsed) 
	{
			
	}

	void renderObjects() 
	{
		glUseProgram(0);
		//Rendu des axes
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(10000, 0, 0);
		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 10000, 0);
		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 10000);
		glEnd();		



		glRotatef(this->DeltaTimeCumul / 10.0f * 360, -1, 1, 0);

		//Exemple d'utilisation d'un shader
		glUseProgram(ShaderCubeDebug); //Demande au GPU de charger ces shaders
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderCubeDebug); //Envoie les matrices au shader
		cubeLel->render(); //Demande le rendu du VBO


		glUseProgram(ShaderSun); //Demande au GPU de charger ces shaders
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderSun);
		glUniform3f(shaderColorLocation, sunColor.R, sunColor.V, sunColor.B);
			//Envoie les matrices au shader
		sun->render();

	}

	void resize(int width, int height) {
	
	}

	/*INPUTS*/

	void keyPressed(int key, bool special, bool down, int p1, int p2) 
	{	

	}

	void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
	{
		
	}

	void mouseClick(int button, int state, int x, int y, bool inUi)
	{
		
	}

	void mouseMove(int x, int y, bool pressed, bool inUi)
	{
		
	}
	
};


#endif