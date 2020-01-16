#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine/engine.h"

#include "avatar.h"
#include "world.h"
#include <sysinfoapi.h>

class MEngineMinicraft : public YEngine {
private:
	Cube cubeLel;
	Cube sun;

	GLuint ShaderCubeDebug;
	GLuint ShaderSun;
	GLuint shaderColorLocation;

	YColor sunColor;
	YColor skyColor;

	float rotationValue;

	YVec3f sunPosition;
	YVec3f sunDirection;
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
		
		cubeLel = Cube();
		//glDisable(GL_CULL_FACE);

		

		sun = createGPUCube();

		ShaderSun = Renderer->createProgram("shaders/sun");
		shaderColorLocation = glGetUniformLocation(ShaderSun, "sun_color");

		sunColor = YColor(1.0f, 1.0f, 0.0f, 1.0f);
		SYSTEMTIME s;
		GetLocalTime(&s);

		//sunColor.interpolate()
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