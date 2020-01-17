#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine/engine.h"

#include "avatar.h"
#include "world.h"
#include <sysinfoapi.h>
#include "../Cube.h"
#include "world.h"

class MEngineMinicraft : public YEngine {
private:
	Cube* cubeLel;
	Cube* sun;

	GLuint ShaderCubeDebug;
	GLuint ShaderSun;
	GLuint shaderColorLocation;

	YColor sunColor;
	YColor skyColor;

	float rotationValue;

	YVec3f sunPosition;
	YVec3f sunDirection;

	bool isRightClicking = false;
	bool isCtrling = false;
	bool isMiddleClicking = false;

	MWorld* world;

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
		
		cubeLel = new Cube();
		//glDisable(GL_CULL_FACE);

		

		sun = new Cube();

		ShaderSun = Renderer->createProgram("shaders/sun");
		shaderColorLocation = glGetUniformLocation(ShaderSun, "sun_color");

		sunColor = YColor(1.0f, 1.0f, 0.0f, 1.0f);

		world = new MWorld();
		world->init_world(0);
	}


	
	bool getSunDirFromDayTime(YVec3f& sunDir, float mnLever, float mnCoucher, float boostTime)
	{
		bool nuit = false;

		SYSTEMTIME t;
		GetLocalTime(&t);

		//On borne le tweak time à une journée (cyclique)
		while (boostTime > 24 * 60)
			boostTime -= 24 * 60;

		//Temps écoulé depuis le début de la journée
		float fTime = (float)(t.wHour * 60 + t.wMinute);
		fTime += boostTime;
		while (fTime > 24 * 60)
			fTime -= 24 * 60;

		//Si c'est la nuit
		if (fTime < mnLever || fTime > mnCoucher)
		{
			nuit = true;
			if (fTime < mnLever)
				fTime += 24 * 60;
			fTime -= mnCoucher;
			fTime /= (mnLever + 24 * 60 - mnCoucher); // divise par le temps total de jour (levé)
			fTime *= (float)M_PI;
		}
		else
		{
			//c'est le jour
			nuit = false;
			fTime -= mnLever;
			fTime /= (mnCoucher - mnLever);
			fTime *= (float)M_PI;
		}

		//Direction du soleil en fonction de l'heure
		sunDir.X = cos(fTime);
		sunDir.Y = 0.2f;
		sunDir.Z = sin(fTime);
		sunDir.normalize();

		return nuit;
	}

	void updateLights(float boostTime = 0)
	{
		//On recup la direciton du soleil
		bool nuit = getSunDirFromDayTime(sunDirection, 6.0f * 60.0f, 19.0f * 60.0f, boostTime);
		sunPosition = sunDirection * 2;

		//Pendant la journée
		if (!nuit)
		{
			//On definit la couleur
			sunColor = YColor(1.0f, 1.0f, 0.8f, 1.0f);
			skyColor = YColor(0.0f, 181.f / 255.f, 221.f / 255.f, 1.0f);
			YColor downColor(0.9f, 0.5f, 0.1f, 1);

			sunColor = sunColor.interpolate(downColor, (abs(sunDirection.X)));
			skyColor = skyColor.interpolate(downColor, (abs(sunDirection.X)));
		}
		else
		{
			//La nuit : lune blanche et ciel noir
			sunColor = YColor(1, 1, 1, 1);
			skyColor = YColor(0, 0, 0, 1);
		}

		Renderer->setBackgroundColor(skyColor);
	}

	void update(float elapsed) 
	{
		updateLights(DeltaTimeCumul * 100);
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

		
		glPushMatrix();
		
		glTranslatef(sunPosition.X, sunPosition.Y, sunPosition.Z);

		//Exemple d'utilisation d'un shader
		


		glUseProgram(ShaderSun); //Demande au GPU de charger ces shaders
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderSun);
		glUniform3f(shaderColorLocation, sunColor.R, sunColor.V, sunColor.B);
			//Envoie les matrices au shader
		sun->render();

		glPopMatrix();

		world->render_world_basic(ShaderSun, cubeLel->GetVbo());

	}

	void resize(int width, int height) {
	
	}

	/*INPUTS*/

	void keyPressed(int key, bool special, bool down, int p1, int p2)
	{
		YVec3f movementVector = YVec3f();
		switch (key)
		{
		case 122: // z
			movementVector.X = DeltaTime * 10;
			break;

		case 113: //q
			movementVector.Y = -DeltaTime * 10;
			break;

		case 115: // s
			movementVector.X = -DeltaTime * 10;
			break;
		case 100: //d
			movementVector.Y = DeltaTime * 10;
			break;

		case GLUT_KEY_CTRL_L:
			isCtrling = down;
			break;
		default:
			break;
		}

		Renderer->Camera->move(movementVector);
	}

	void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
	{
		//3 avant
		//4 arrière

		auto fov = Renderer->Camera->FovY;
		if (wheel == 2)
		{
			fov += DeltaTime * 100.0f;
		}
		else if (wheel == 3)
		{
			fov -= DeltaTime * 100.0f;
		}


		if (fov < 90.0f && fov > 45.0f)
			Renderer->Camera->FovY = fov;

	}

	void mouseClick(int button, int state, int x, int y, bool inUi)
	{
		isRightClicking = button == 2 && state == 0;
		isMiddleClicking = button == 1 && state == 0;

		if (isRightClicking || isMiddleClicking)
		{
			glutWarpPointer(BaseWidth >> 1, BaseHeight >> 1);
		}
	}

	void mouseMove(int x, int y, bool pressed, bool inUi)
	{
		int deltaX, deltaY;

		deltaX = (BaseWidth >> 1) - x;
		deltaY = (BaseHeight >> 1) - y;

		if (isRightClicking)
		{
			if (isCtrling)
			{
				Renderer->Camera->rotateAround((deltaX / 400.0f));
				Renderer->Camera->rotateUpAround(deltaY / 400.0f);
			}
			else
			{
				Renderer->Camera->rotate((deltaX / 400.0f));
				Renderer->Camera->rotateUp(deltaY / 400.0f);
			}

			glutWarpPointer(BaseWidth >> 1, BaseHeight >> 1);
		}
		else if (isMiddleClicking)
		{
			if (isCtrling)
			{
				Renderer->Camera->moveTo(YVec3f(deltaY / 400.0f, deltaX / 400.0, 0));
			}
			else
			{
				Renderer->Camera->move(YVec3f(deltaY / 400.0f, deltaX / 400.0f, 0));
			}

			glutWarpPointer(BaseWidth >> 1, BaseHeight >> 1);
		}

	}
	
};


#endif