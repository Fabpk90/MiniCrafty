#ifndef __AVATAR__
#define __AVATAR__

#include "engine/utils/types_3d.h"
#include "engine/render/camera.h"
#include "engine/utils/timer.h"
#include "world.h"
#include "../physics.h"

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

	const int maxRaycastDistance = 5;

	YCamera * Cam;
	MWorld * World;

	YTimer _TimerStanding;

	YVbo* vbo;

	YVec3f gravity;
	float mass;

	YVec3f velocity;
	YVec3f sumForces;
	YVec3f damping;

	YVec3f startPos;
	YVec3f endPos;

	MAvatar(YCamera * cam, MWorld * world)
	{
		Position = YVec3f(10, 10, 120);
		Height = 1.8f;
		CurrentHeight = Height;
		Width = 0.5f;
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


	void RayCast()
	{
		startPos = Cam->Position;
		endPos = (Cam->Position + Cam->Direction * 5);

		int xAvatar, yAvatar, zAvatar;

		xAvatar = Position.X / MChunk::CHUNK_SIZE;
		yAvatar = Position.Y / MChunk::CHUNK_SIZE;
		zAvatar = Position.Z / MChunk::CHUNK_SIZE;
		
		MChunk* chunk = World->getChunkAt(xAvatar, yAvatar, zAvatar);

		

		if(chunk != nullptr) // if in a chunk
		{
			int xDeb = ( (int)Position.X % MChunk::CHUNK_SIZE )- maxRaycastDistance;
			int yDeb = ((int)Position.Y % MChunk::CHUNK_SIZE) - maxRaycastDistance;
			int zDeb = ((int)Position.Z % MChunk::CHUNK_SIZE) - maxRaycastDistance;

			
			int xFin = ((int)Position.X % MChunk::CHUNK_SIZE) + maxRaycastDistance;
			int yFin = ((int)Position.Y % MChunk::CHUNK_SIZE) + maxRaycastDistance;
			int zFin = ((int)Position.Z % MChunk::CHUNK_SIZE) + maxRaycastDistance;
			
			//todo: discard cube that the avatar is not facing towards

			const int size = MCube::CUBE_SIZE;
			bool found = false;
			for (int x = xDeb; x <= xFin && !found; x++)
				for (int y = yDeb; y <= yFin && !found; y++)
					for (int z = zDeb; z <= zFin && !found; z++)
					{
						MCube* cube = chunk->getCubeAt(x, y, z);

						if(cube != nullptr)
						{
							if (cube->isSolid() && cube->getDraw())
							{
								//Check for the faces

								const int trueX = (x * size);
								const int trueY = (y * size);
								const int trueZ = (z * size);

								//Face 1
								YVec3f a = YVec3f(trueX, trueY, trueZ + size);
								YVec3f b = YVec3f(trueX + size, trueY, trueZ + size);
								YVec3f c = YVec3f(trueX + size, trueY + size, trueZ + size);
								YVec3f d = YVec3f(trueX, trueY + size, trueZ + size);

								if (Physics::isIntersectingTriangle(startPos, endPos, a, b, c, d))
								{
									World->deleteCube(x, y, z);
									found = true;
									break;
								}

								b = YVec3f(trueX, trueY, trueZ);
								c = YVec3f(trueX, trueY + size, trueZ);
								d = YVec3f(trueX + size, trueY + size, trueZ);
								a = YVec3f(trueX + size, trueY, trueZ);

								if (Physics::isIntersectingTriangle(startPos, endPos, a, b, c, d))
								{
									World->deleteCube(x, y, z);
									found = true;
									break;
								}

								// XZ
								a = YVec3f(trueX, trueY, trueZ);
								b = YVec3f(trueX + size, trueY, trueZ);
								c = YVec3f(trueX + size, trueY, trueZ + size);
								d = YVec3f(trueX, trueY, trueZ + size);

								if (Physics::isIntersectingTriangle(startPos, endPos, a, b, c, d))
								{
									World->deleteCube(x, y, z);
									found = true;
									break;
								}

								b = YVec3f(trueX, trueY + size, trueZ);
								c = YVec3f(trueX, trueY + size, trueZ + size);
								d = YVec3f(trueX + size, trueY + size, trueZ + size);
								a = YVec3f(trueX + size, trueY + size, trueZ);

								if (Physics::isIntersectingTriangle(startPos, endPos, a, b, c, d))
								{
									World->deleteCube(x, y, z);
									found = true;
									break;
								}

								// YZ
								b = YVec3f(trueX, trueY, trueZ);
								c = YVec3f(trueX, trueY, trueZ + size);
								d = YVec3f(trueX, trueY + size, trueZ + size);
								a = YVec3f(trueX, trueY + size, trueZ);

								if (Physics::isIntersectingTriangle(startPos, endPos, a, b, c, d))
								{
									World->deleteCube(x, y, z);
									found = true;
									break;
								}

								a = YVec3f(trueX + size, trueY, trueZ);
								b = YVec3f(trueX + size, trueY + size, trueZ);
								c = YVec3f(trueX + size, trueY + size, trueZ + size);
								d = YVec3f(trueX + size, trueY, trueZ + size);

								if (Physics::isIntersectingTriangle(startPos, endPos, a, b, c, d))
								{
									World->deleteCube(x, y, z);
									found = true;
									break;
								}
							}
						
						}
					}
		}
		
		
	}
	
	void update(float elapsed)
	{
		if (elapsed > 1.0f / 60.0f)
			elapsed = 1.0f / 60.0f;

		//Par defaut, on applique la gravité (-100 sur Z), la moitie si dans l'eau
		YVec3f force = YVec3f(0, 0, -1) * 9.81f;
		if (InWater)
			force = YVec3f(0, 0, -1) * 0.5f;

		float lastheight = CurrentHeight;
		CurrentHeight = Height;
		if (Crouch)
			CurrentHeight = Height / 2;

		//Pour ne pas s'enfoncer dans le sol en une frame quand on se releve
		if (CurrentHeight > lastheight)
			Position.Z += Height / 4;

		//Si l'avatar n'est pas au sol, alors il ne peut pas sauter
		/*if (!Standing && !InWater) //On jump tout le temps
		Jump = false;*/

		float accel = 40;
		if (Crouch)
			accel = 20;
		if (!Standing)
			accel = 5;
		if (Run)
			accel = 80;

		YVec3f forward(Cam->Direction.X, Cam->Direction.Y, 0);
		forward.normalize();
		YVec3f right(Cam->RightVec.X, Cam->RightVec.Y, 0);
		right.normalize();

		//On applique les controles en fonction de l'accélération
		if (avance)
			force += forward * accel;
		if (recule)
			force += forward * -accel;
		if (gauche)
			force += right * -accel;
		if (droite)
			force += right * accel;


		//On applique le jump
		if (Jump)
		{
			force += YVec3f(0, 0, 1) * 5.0f / elapsed; //(impulsion, pas fonction du temps)
			Jump = false;
		}

		//On applique les forces en fonction du temps écoulé
		velocity += force * elapsed;

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

		//On le déplace, en sauvegardant son ancienne position
		YVec3f oldPosition = Position;
		Position += (velocity * elapsed);

		//YLog::log(YLog::ENGINE_INFO, ("zS " + toString(Speed.Z)).c_str());

		if (_TimerStanding.getElapsedSeconds() > 0.01)
			Standing = false;
		for (int pass = 0; pass < 2; pass++)
		{
			for (int i = 0; i < 6; i++)
			{
				float valueColMin = 0;
				MWorld::MAxis axis = World->getMinCol(Position, velocity, Width, CurrentHeight, valueColMin, i < 3);
				//YLog::log(YLog::ENGINE_INFO,"check");
				if (axis != 0)
				{
					//valueColMin = nymax(nyabs(valueColMin), 0.0001f) * (valueColMin > 0 ? 1.0f : -1.0f);
					if (axis & MWorld::AXIS_X)
					{
						//YLog::log(YLog::ENGINE_INFO,("x " + toString(valueColMin)).c_str());
						Position.X += valueColMin + 0.001 * sign(valueColMin);
						velocity.X = 0;
					}
					if (axis & MWorld::AXIS_Y)
					{
						//YLog::log(YLog::ENGINE_INFO, ("y " + toString(valueColMin)).c_str());
						Position.Y += valueColMin + 0.001 * sign(valueColMin);
						velocity.Y = 0;
					}
					if (axis & MWorld::AXIS_Z)
					{
						//YLog::log(YLog::ENGINE_INFO, ("z " + toString(valueColMin)).c_str());
						velocity.Z = 0;
						Position.Z += valueColMin + 0.001 * sign(valueColMin);
						Standing = true;
						_TimerStanding.start();
					}
				}
			}
		}

		int x = (int)(Position.X / MCube::CUBE_SIZE);
		int y = (int)(Position.Y / MCube::CUBE_SIZE);
		int z = (int)(Position.Z / MCube::CUBE_SIZE);

		//Escaliers
		float floatheight = 1.0f;
		float zpied = Position.Z - (Height / 2.0f);
		float zfloatpied = zpied - floatheight;
		int izCubeDessousFloat = (int)((zfloatpied) / MCube::CUBE_SIZE);
		float zCubeDessous2Float = zfloatpied - MCube::CUBE_SIZE;
		int izCubeDessous2Float = (int)(zCubeDessous2Float / MCube::CUBE_SIZE);


		//Si on est dans l'eau
		InWater = false;
		if (World->getCube(x, y, z) != nullptr && 
			World->getCube(x, y, z)->getType() == MCube::CUBE_EAU)
			InWater = true;

		if (InWater)
		{
			//Standing = true;
			velocity *= pow(0.2f, elapsed);
		}
		else if (Standing)
			velocity *= pow(0.01f, elapsed);

		resetDirections();
	}
};
#endif