#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine/engine.h"

#include "avatar.h"
#include "world.h"
#include <windows.h>
#include "world.h"
#include "../physics.h"

class MEngineMinicraft : public YEngine {
private:

	YVbo* vbo;

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
	bool isLeftClicking = false;

	bool isZing;
	bool isSing;
	bool isQing;
	bool isDing;

	MWorld* world;

	GUILabel* camPosition;
	GUILabel* crosshair;

	MAvatar* avatar;

	YVbo* cubeDebug;
	
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

	YVbo* createVBO()
	{
		// Creation du VBO
		auto vbo = new YVbo(3, 36, YVbo::PACK_BY_ELEMENT_TYPE);

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

		return vbo;
	}

	void init() 
	{
		YLog::log(YLog::ENGINE_INFO,"Minicraft Started : initialisation");

		

		Renderer->setBackgroundColor(YColor(0.0f,0.0f,0.0f,1.0f));
		Renderer->Camera->setPosition(YVec3f(10, 10, 10));
		Renderer->Camera->setLookAt(YVec3f());

		vbo = createVBO();

		ShaderSun = Renderer->createProgram("shaders/sun");
		shaderColorLocation = glGetUniformLocation(ShaderSun, "sun_color");

		sunColor = YColor(1.0f, 1.0f, 0.0f, 1.0f);

		world = new MWorld();
		world->init_world(rand());
		camPosition = new GUILabel();
		auto pos = Renderer->Camera->Position;
		camPosition->Text.clear();
		camPosition->Text.append(std::to_string(pos.X));
		camPosition->Text.append(" ");

		camPosition->Text.append(std::to_string(pos.Y));
		camPosition->Text.append(" ");

		camPosition->Text.append(std::to_string(pos.Z));
		camPosition->Text.append(" ");

		camPosition->X = 150;

		crosshair = new GUILabel();
		crosshair->Text = "+";
		crosshair->X = BaseWidth / 2;
		crosshair->Y = BaseHeight / 2;

		this->ScreenStats->addElement(camPosition);
		ScreenJeu->addElement(crosshair);

		avatar = new MAvatar(Renderer->Camera, world);
		avatar->vbo = createVBO();

		cubeDebug = createVBO();

		ShaderCubeDebug = Renderer->createProgram("shaders/cube_debug");
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

	void CheckForInputs()
	{

		if (isZing)
			avatar->avance = true;
		else if (isSing)
			avatar->recule = true;
		if (isQing)
			avatar->gauche = true;
		else if (isDing)
			avatar->droite = true;
	}

	void update(float elapsed) 
	{
		updateLights();
		

		if (ScreenStats->_Active)
		{
			auto pos = Renderer->Camera->Position;
			camPosition->Text.clear();
			camPosition->Text.append(std::to_string(pos.X));
			camPosition->Text.append(" ");

			camPosition->Text.append(std::to_string(pos.Y));
			camPosition->Text.append(" ");

			camPosition->Text.append(std::to_string(pos.Z));
			camPosition->Text.append(" ");
		}
		
		CheckForInputs();

		world->update();
		avatar->update(elapsed);
		Renderer->Camera->moveTo(avatar->Position + Renderer->Camera->Direction + YVec3f(0, 0, avatar->CurrentHeight * 0.75f));

		if (isLeftClicking)
			avatar->RayCast();
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

		glUseProgram(ShaderSun); //Demande au GPU de charger ces shaders
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderSun);
		glUniform3f(shaderColorLocation, sunColor.R, sunColor.V, sunColor.B);
			//Envoie les matrices au shader
		vbo->render();
		glPopMatrix();
		

		world->render_world_vbo(true, true);

		glPushMatrix();
	
		glTranslatef(avatar->Position.X - avatar->Width / 2, avatar->Position.Y - avatar->Width / 2, avatar->Position.Z - avatar->CurrentHeight / 2);
		glScalef(avatar->Width, avatar->Width, avatar->CurrentHeight);
		glUseProgram(ShaderCubeDebug);
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderCubeDebug);
		avatar->vbo->render();
		
		glPopMatrix();
	}

	void resize(int width, int height) {
	
	}

	/*INPUTS*/

	void keyPressed(int key, bool special, bool down, int p1, int p2)
	{

		if(!special)
		{
			switch (key)
			{
			case 122: // z
				isZing = down;
				break;

			case 113: //q
				isQing = down;
				break;

			case 115: // s
				isSing = down;
				break;
			case 100: //d
				isDing = down;
				break;
			case 32: //space
				avatar->Jump = down;
				break;
			}
		}
		else
		{
			switch (key)
			{
			case GLUT_KEY_CTRL_L:
				isCtrling = down;
				break;

			case GLUT_KEY_SHIFT_L:
				//Renderer->Camera->boost = down;
				avatar->Run = down;
				break;
			default:
				break;
			}
		}
	}

	void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
	{
		//2 avant
		//3 arrière

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
		isLeftClicking = button == 0 && state == 0;
		if (isRightClicking)
		{
			showMouse(false);
			glutWarpPointer(BaseWidth >> 1, BaseHeight >> 1);
		}
	}

	void mouseMove(int x, int y, bool pressed, bool inUi)
	{
		int deltaX, deltaY;

		deltaX = (BaseWidth >> 1) - x;
		deltaY = (BaseHeight >> 1) - y;

		showMouse(true);

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

			showMouse(false);
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
				//Renderer->Camera->move(YVec3f(deltaY / 400.0f, deltaX / 400.0f, 0));
				YVec3f delta(deltaY, deltaX, 0);
				YVec3f position;
				position += Renderer->Camera->Direction * delta.X;
				position += Renderer->Camera->RightVec * delta.Y;
				position += Renderer->Camera->UpVec * delta.Z;
				avatar->AddForce(position);
			}
			showMouse(false);
			glutWarpPointer(BaseWidth >> 1, BaseHeight >> 1);
		}

	}
	
};


#endif