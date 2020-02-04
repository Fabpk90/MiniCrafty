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

	float maxSpeed = 70;

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
		Position = YVec3f(10, 10, 150);
		Height = 1.f;
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

		gravity = YVec3f(0, 0, -1) * 9.81f;
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
			sumForces += Cam->Direction;
		else if(recule)
			sumForces -= Cam->Direction;
		if(gauche)
			sumForces -= Cam->RightVec;
		else if(droite)
			sumForces += Cam->RightVec;

		if(!Standing)
			sumForces += gravity;

		if (Jump)
		{
			sumForces += YVec3f(0, 0, 1) * 2.5f / elapsed;
			Jump = false;
		}

		//On met une limite a sa vitesse horizontale
		float speedmax = 70;
		if (Crouch)
			speedmax = 45;
		if (!Standing)
			speedmax = 70;
		if (Run)
			speedmax = 140;

		YVec3f horSpeed = velocity;
		horSpeed.Z = 0;
		if (horSpeed.getSize() > speedmax)
		{
			horSpeed.normalize();
			horSpeed *= speedmax;
			velocity.X = horSpeed.X;
			velocity.Y = horSpeed.Y;
		}
			
		
		//sumForces /= mass;

		velocity = velocity + sumForces * elapsed;
		velocity = velocity * damping;

		
		Position = Position + (velocity * elapsed);

		
		

		//todo: clamp vertical and horizontal speed
		
		if (_TimerStanding.getElapsedSeconds() > 0.01f)
			Standing = false;


		for (int pass = 0; pass < 2; ++pass)
		{
			//3 times seeing the future and 3 times normal
			for (int i = 0; i < 6; ++i)
			{
				float value = 0;
				MWorld::MAxis m = World->getMinCol(Position, velocity, Width, CurrentHeight, value, i < 3);

				if (m & MWorld::AXIS_X)
				{
					YLog::log(YLog::ENGINE_INFO, ("x " + toString(value)).c_str());
					Position.X += value;
					velocity.X = 0;
				}
				if (m & MWorld::AXIS_Y)
				{
					YLog::log(YLog::ENGINE_INFO, ("y " + toString(value)).c_str());
					Position.Y += value;
					velocity.Y = 0;
				}
				if (m & MWorld::AXIS_Z)
				{
					YLog::log(YLog::ENGINE_INFO, ("z " + toString(value)).c_str());
					Position.Z += value;
					velocity.Z = 0;

					Standing = true;
					_TimerStanding.start();
				}
				
			}
			
		}
		
		
		Cam->moveTo(Position - (Cam->Direction * 2.0f));

		resetDirections();
		
		sumForces.X = 0;
		sumForces.Y = 0;
		sumForces.Z = 0;
	}
};
#endif