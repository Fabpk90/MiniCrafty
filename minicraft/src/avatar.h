#ifndef __AVATAR__
#define __AVATAR__

#include "engine/utils/types_3d.h"
#include "engine/render/camera.h"
#include "engine/utils/timer.h"
#include "world.h"

class MAvatar
{
public:
	YVec3f Position;
	YVec3f Speed;

	bool Move;
	bool Jump;
	float Height;
	float CurrentHeight;
	float Width;
	bool avance;
	bool recule;
	bool gauche;
	bool droite;
	bool Standing;
	bool InWater;
	bool Crouch;
	bool Run;

	YCamera * Cam;
	MWorld * World;

	YTimer _TimerStanding;

	YVbo* vbo;

	YVec3f gravity;
	float mass;

	YVec3f velocity;
	YVec3f sumForces;
	YVec3f damping;

	MAvatar(YCamera * cam, MWorld * world)
	{
		Position = YVec3f(10, 10, 100);
		Height = 1.8f;
		CurrentHeight = Height;
		Width = 0.3f;
		Cam = cam;
		avance = false;
		recule = false;
		gauche = false;
		droite = false;
		Standing = false;
		Jump = false;
		World = world;
		InWater = false;
		Crouch = false;
		Run = false;

		mass = 75.0f;

		gravity = YVec3f(0, 0, -1) * 9.81f * mass;
		damping = YVec3f(1, 1, 1);
	}

	void AddForce(YVec3f force)
	{
		sumForces += force;
	}

	void resetDirections()
	{
		avance = false;
		recule = false;
		gauche = false;
		droite = false;
	}

	void update(float elapsed)
	{
		if (elapsed > 1.0f / 60.0f)
			elapsed = 1.0f / 60.0f;

		if (avance)
			sumForces += Cam->Direction * 100;
		else if(recule)
			sumForces -= Cam->Direction * 100;
		if(gauche)
			sumForces -= Cam->RightVec * 100;
		else if(droite)
			sumForces += Cam->RightVec * 100;
		
		sumForces /= mass;

		velocity = velocity + sumForces * elapsed;
		velocity = velocity * damping;
		YVec3f newPos;
		YVec3f direction;
		newPos = Position + velocity * elapsed;

		direction = Position - newPos;
		Position = newPos;
		
		float value = 0;
		
		bool resolved = World->chunks.empty();

		if (resolved)
			sumForces += gravity;

		while(!resolved)
		{
			MWorld::MAxis m = World->getMinCol(Position, direction.normalize(), Width, CurrentHeight, value, true);

			if (m & MWorld::AXIS_X)
			{
				cout << "Collision X" << endl;
				Position -= YVec3f(1, 0, 0) * value;
			}
			else if (m & MWorld::AXIS_Y)
			{
				cout << "Collision Y" << endl;
				Position -= YVec3f(0, 1, 0) * value;
			}
			else if (m & MWorld::AXIS_Z)
			{
				cout << "Collision Z" << endl;
				Position -= YVec3f(0, 0, 1) * value;
			}
			else
				resolved = true;
		}
		
		
		Cam->moveTo(Position - Cam->Direction);

		resetDirections();
		
		sumForces *= 0;
	}
};

#endif