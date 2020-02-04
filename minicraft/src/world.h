#ifndef __WORLD_H__
#define __WORLD_H__

#include "external/gl/glew.h"
#include "external/gl/freeglut.h" 
#include "engine/utils/types_3d.h"
#include "cube.h"
#include "chunk.h"
#include "engine/noise/perlin.h"
#include <thread>
#include <mutex>

#include "engine_minicraft.h"
#include "../dumber_perlin.h"

class MWorld
{
public :
	typedef uint8 MAxis;
	static const int AXIS_X = 0b00000001;
	static const int AXIS_Y = 0b00000010;
	static const int AXIS_Z = 0b00000100;

	#ifdef _DEBUG
	static const int MAT_SIZE = 1; //en nombre de chunks
	#else
	static const int MAT_SIZE = 3; //en nombre de chunks
	#endif // DEBUG

	static const int MAT_HEIGHT = 1; //en nombre de chunks
	static const int MAT_SIZE_CUBES = (MAT_SIZE * MChunk::CHUNK_SIZE);
	static const int MAT_HEIGHT_CUBES = (MAT_HEIGHT * MChunk::CHUNK_SIZE);
	static const int MAT_SIZE_METERS = (MAT_SIZE * MChunk::CHUNK_SIZE * MCube::CUBE_SIZE);
	static const int MAT_HEIGHT_METERS = (MAT_HEIGHT * MChunk::CHUNK_SIZE  * MCube::CUBE_SIZE);

	MChunk * Chunks[MAT_SIZE][MAT_SIZE][MAT_HEIGHT];

	list<MChunk*> chunks;

	GLuint shaderProgram;

	YTexFile* texture;
	std::thread chunker1;
	bool chunker1Done;
	
	std::thread chunker2;
	bool chunker2Done;
	std::list<MChunk*> chunkier;
	std::list<YVec3<int>> chunkList;

	std::mutex chunkierMutex;
	std::mutex chunkListMutex;

	int seed;

	YVec3<int> playerChunkPosition;
	
	MWorld()
	{
		chunkier = std::list<MChunk*>();
		chunkList = std::list<YVec3<int>>();
		chunks = std::list<MChunk*>();

		chunker1Done = true;
		chunker2Done = true;

		playerChunkPosition = YVec3<int>(0, 0, 0);
		
		texture = YTexManager::getInstance()->loadTexture("terrain.png");

		shaderProgram = YEngine::getInstance()->Renderer->createProgram("shaders/world");

		//On crée les chunks
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
					Chunks[x][y][z] = new MChunk(x,y,z);	
	}

	//call this before adding it to the list
	void SetVoisins(MChunk* chunk)
	{
		for (auto val : chunks)
		{
			if(chunk->Voisins[0] == nullptr && chunk->_XPos - 1 == val->_XPos)
			{
				if (chunk->_YPos == val->_YPos && chunk->_ZPos == val->_ZPos)
				{
					chunk->Voisins[0] = val;
					val->Voisins[1] = chunk;
				}
					
			}
			else if (chunk->Voisins[1] == nullptr && chunk->_XPos + 1 == val->_XPos)
			{
				if (chunk->_YPos == val->_YPos && chunk->_ZPos == val->_ZPos)
				{
					chunk->Voisins[1] = val;
					val->Voisins[0] = chunk;
				}
					
			}
			else if (chunk->Voisins[2] == nullptr && chunk->_YPos - 1 == val->_YPos)
			{
				if (chunk->_XPos == val->_XPos && chunk->_ZPos == val->_ZPos)
				{
					chunk->Voisins[2] = val;
					val->Voisins[3] = chunk;
				}
					
			}
			else if (chunk->Voisins[3] == nullptr && chunk->_YPos + 1 == val->_YPos)
			{
				if (chunk->_XPos == val->_XPos && chunk->_ZPos == val->_ZPos)
				{
					chunk->Voisins[3] = val;
					val->Voisins[2] = chunk;
				}
					
			}
			else if (chunk->Voisins[4] == nullptr && chunk->_ZPos - 1 == val->_ZPos)
			{
				if (chunk->_XPos == val->_XPos && chunk->_YPos == val->_YPos)
				{
					chunk->Voisins[4] = val;
					val->Voisins[5] = chunk;
				}
					
			}
			else if (chunk->Voisins[5] == nullptr && chunk->_ZPos + 1 == val->_ZPos)
			{
				if (chunk->_XPos == val->_XPos && chunk->_YPos == val->_YPos)
				{
					chunk->Voisins[5] = val;
					val->Voisins[4] = chunk;
				}
			}
		}
	}

	//TODO: change this to the current chunk !

	/*inline MCube * getCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * MChunk::CHUNK_SIZE) x = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * MChunk::CHUNK_SIZE) y = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if(z >= MAT_HEIGHT * MChunk::CHUNK_SIZE) z = (MAT_HEIGHT * MChunk::CHUNK_SIZE)-1;

		return &(Chunks[x / MChunk::CHUNK_SIZE][y / MChunk::CHUNK_SIZE][z / MChunk::CHUNK_SIZE]->_Cubes[x % MChunk::CHUNK_SIZE][y % MChunk::CHUNK_SIZE][z % MChunk::CHUNK_SIZE]);
	}*/

	inline MCube* getCube(int x, int y, int z)
	{
		if (x < 0)x = 0;
		if (y < 0)y = 0;
		if (z < 0)z = 0;

		int xIndex = x / MChunk::CHUNK_SIZE;
		int yIndex = y / MChunk::CHUNK_SIZE;
		int zIndex = z / MChunk::CHUNK_SIZE;

		for (auto chunk : chunks)
		{
			if(chunk->_XPos == xIndex 
				&& chunk->_YPos == yIndex
				&& chunk->_ZPos == zIndex)
			{
				return chunk->getCubeAt(x, y, z);
			}
		}

		return nullptr;
	}
	//todo: update this
	void updateCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * MChunk::CHUNK_SIZE)x = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * MChunk::CHUNK_SIZE)y = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if (z >= MAT_HEIGHT * MChunk::CHUNK_SIZE)z = (MAT_HEIGHT * MChunk::CHUNK_SIZE) - 1; {
			Chunks[x / MChunk::CHUNK_SIZE][y / MChunk::CHUNK_SIZE][z / MChunk::CHUNK_SIZE]->disableHiddenCubes();
			Chunks[x / MChunk::CHUNK_SIZE][y / MChunk::CHUNK_SIZE][z / MChunk::CHUNK_SIZE]->toVbos();
		}
		
	}

	void deleteCube(int x, int y, int z)
	{
		MCube * cube = getCube(x,y,z);
		cube->setType(MCube::CUBE_AIR);
		cube->setDraw(false);
		cube = getCube(x-1,y,z);
		updateCube(x,y,z);	
	}

	MChunk* getChunkAt(int x, int y, int z)
	{
		for (auto chunk : chunks)
		{
			if (x == chunk->_XPos && y == chunk->_YPos && z == chunk->_ZPos)
				return chunk;
		}

		return nullptr;
	}

	void threadChunkHandler(bool* done)
	{
		*done = false;
		YDumberPerlin n = YDumberPerlin();
		n.updateVecs();

		//n.setZDecay(MWorld::MAT_HEIGHT_CUBES - 5, 0.5f);
		
		while(!chunkList.empty())
		{
			YVec3<int> chunkPos;
			{
				std::lock_guard<std::mutex> lock1(chunkListMutex);

				chunkPos = chunkList.front();
				chunkList.remove(chunkPos);
			}

			MChunk* chunk = getChunkAt(chunkPos.X, chunkPos.Y, chunkPos.Z);

			if(chunk)
			{
				YLog::log(YLog::ENGINE_INFO, "loading chunk from list");
				
				chunk->disableHiddenCubes();
				chunk->toVbos();

				std::lock_guard<std::mutex> lock(chunkierMutex);
				SetVoisins(chunk);
				chunkier.push_back(chunk);
			}
			else
			{
				char path[50];

				sprintf_s(path, "%d_%d_%d.txt", chunkPos.X, chunkPos.Y, chunkPos.Z);
				ifstream i(path, ifstream::binary);

				MChunk* chunk = new MChunk(chunkPos.X, chunkPos.Y, chunkPos.Z);

				//if the file exists, the chunk has already been computed
				if (false)//i.good())
				{
					YLog::log(YLog::ENGINE_INFO, "loading chunk from file");
					chunk->loadFrom(i);
					
					i.close();
				}
				else
				{
					i.close();

					YLog::log(YLog::ENGINE_INFO, "starting chunk");


					for (int x = 0; x < MChunk::CHUNK_SIZE; x++)
						for (int y = 0; y <  MChunk::CHUNK_SIZE; y++)
							for (int z = 0; z < MChunk::CHUNK_SIZE; z++)
							{
								auto cube = chunk->getCubeAt(x, y, z);

								float perlin = n.sample((x + chunk->_XPos * MChunk::CHUNK_SIZE),
									(y + chunk->_YPos * MChunk::CHUNK_SIZE),
									(z + chunk->_ZPos * MChunk::CHUNK_SIZE));

								if (perlin > 0.5f)
									cube->setType(MCube::CUBE_PIERRE);
							}
				}

				SetVoisins(chunk);
				chunks.push_back(chunk);
				chunk->disableHiddenCubes();

				chunk->toVbos();

				const std::lock_guard<std::mutex> lock(chunkierMutex);
				chunkier.push_back(chunk);
			}
		}
		
		YLog::log(YLog::ENGINE_INFO, "ending thread");
		*done = true;
	}

	void addChunkToGenerate(int x, int y, int z)
	{
		{
			const std::lock_guard<std::mutex> lock1(chunkListMutex);
			chunkList.emplace_back(x, y, z);
		}

		if(chunker1Done)
		{
			chunker1Done = false;
			YLog::log(YLog::ENGINE_INFO, "waiting for thread 1");
			chunker1.join();
			chunker1 = std::thread(&MWorld::threadChunkHandler, this, &chunker1Done);
		}

		if(chunker2Done)
		{
			chunker2Done = false;
			YLog::log(YLog::ENGINE_INFO, "waiting for thread 2");
			chunker2.join();
			chunker2 = std::thread(&MWorld::threadChunkHandler, this, &chunker2Done);
		}
		
	}
			
	void init_world(int seed)
	{
		YLog::log(YLog::USER_INFO,(toString("Creation du monde seed ")+toString(seed)).c_str());

		srand(seed);

		this->seed = seed;
		
		//Reset du monde
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
					Chunks[x][y][z]->reset();

		/*for (int x = 0; x < MAT_SIZE; x++)
			for (int y = 0; y < MAT_SIZE; y++)
				for (int z = 0; z < MAT_HEIGHT; z++)
					chunkList.push_back(YVec3f(x, y, z));*/

		for (int x = 0; x < MAT_SIZE; x++)
			for (int y = 0; y < MAT_SIZE; y++)
				for (int z = 0; z < MAT_HEIGHT; z++)
					chunkList.emplace_back(x, y, z);

		bool yes = false;
		for (int i = 0; i < chunkList.size(); ++i)
		{
			threadChunkHandler(&yes);
		}
	}

	void update()
	{
		//lock the chunkier to see if any vbo is to be added
		//add the vbo if so
		const std::lock_guard<std::mutex> lock(chunkierMutex);

		if(!chunkier.empty())
		{
			auto front = chunkier.front();
			front->sendToGPU();

			front->saveToDisk();

			chunkier.erase(chunkier.begin());
		}

		auto position = YEngine::getInstance()->Renderer->Camera->Position;
		position.X = (int)position.X / MChunk::CHUNK_SIZE;
		position.Y = (int)position.Y / MChunk::CHUNK_SIZE;
		position.Z = (int)position.Z / MChunk::CHUNK_SIZE;

		YVec3<int> pos = YVec3<int>(position.X, position.Y, position.Z);

		
		/*if(!(pos == playerChunkPosition))
		{
			
			//check what chunk needs to be loaded
			vector<YVec3<int>> chunkPosToAdd;
			
			chunkPosToAdd.emplace_back(pos);
			auto pos1 = YVec3<int>(pos.X + 1, pos.Y, pos.Z);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X + 1, pos.Y + 1, pos.Z);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X + 1, pos.Y + 1, pos.Z + 1);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X + 1, pos.Y, pos.Z + 1);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X, pos.Y, pos.Z + 1);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X, pos.Y + 1, pos.Z);
			chunkPosToAdd.emplace_back(pos1);

			pos1 = YVec3<int>(pos.X - 1, pos.Y, pos.Z);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X - 1, pos.Y - 1, pos.Z);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X - 1, pos.Y - 1, pos.Z - 1);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X - 1, pos.Y, pos.Z - 1);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X, pos.Y, pos.Z - 1);
			chunkPosToAdd.emplace_back(pos1);
			pos1 = YVec3<int>(pos.X, pos.Y - 1, pos.Z);
			chunkPosToAdd.emplace_back(pos1);
			

			//we use the neighbors map to see what chunk to delete
			// delete and save the unwanted chunks

			vector<YVec3<int>> chunkPosToAddFinal;
			vector<YVec3<int>> chunkToKeep;
			for (int i = 0; i < chunkPosToAdd.size(); ++i)
			{
				MChunk* chunk = getChunkAt(chunkPosToAdd[i].X, chunkPosToAdd[i].Y, chunkPosToAdd[i].Z);

				if(!chunk)
				{
					chunkPosToAddFinal.push_back(chunkPosToAdd[i]);
				}
				else
				{
					chunkToKeep.push_back(chunkPosToAdd[i]);
				}
			}

			vector<MChunk*> chunkToDelete(chunks.begin(), chunks.end());

			for (int i = 0; i < chunkToKeep.size(); ++i)
			{
				for (int j = 0; j < chunkToDelete.size(); ++j)
				{
					MChunk* chunk = chunkToDelete[j];

					if (chunk->_XPos == chunkToKeep[i].X
						&& chunk->_YPos == chunkToKeep[i].Y
						&& chunk->_ZPos == chunkToKeep[i].Z)
					{
						chunkToDelete.erase(chunkToDelete.begin() + j);
						break;
					}
				}
				
			}

			for (int i = 0; i < chunkToDelete.size(); ++i)
			{
				auto chunk = chunkToDelete.at(i);

				chunk->saveToDisk();
				chunks.remove(chunk);

				delete chunk;
			}

			for (int i = 0; i < chunkPosToAddFinal.size(); ++i)
			{
				addChunkToGenerate(chunkPosToAddFinal[i].X, chunkPosToAddFinal[i].Y, chunkPosToAddFinal[i].Z);
			}
			
			
			//set the neighbors
		}*/

		playerChunkPosition = pos;
	}

	
	//Boites de collisions plus petites que deux cubes
	MAxis getMinCol(YVec3f pos, YVec3f dir, float width, float height, float & valueColMin, bool oneShot)
	{

		int x = (int)(pos.X / MCube::CUBE_SIZE);
		int y = (int)(pos.Y / MCube::CUBE_SIZE);
		int z = (int)(pos.Z / MCube::CUBE_SIZE);
		
		if (getCube(x, y, z) == nullptr) return 0;

		
		

		int xNext = (int)((pos.X + width / 2.0f) / MCube::CUBE_SIZE);
		int yNext = (int)((pos.Y + width / 2.0f) / MCube::CUBE_SIZE);
		int zNext = (int)((pos.Z + height / 2.0f) / MCube::CUBE_SIZE);

		int xPrev = (int)((pos.X - width / 2.0f) / MCube::CUBE_SIZE);
		int yPrev = (int)((pos.Y - width / 2.0f) / MCube::CUBE_SIZE);
		int zPrev = (int)((pos.Z - height / 2.0f) / MCube::CUBE_SIZE);
		
		if (x < 0)	x = 0;
		if (y < 0)	y = 0;
		if (z < 0)	z = 0;

		if (xPrev < 0)	xPrev = 0;
		if (yPrev < 0)	yPrev = 0;
		if (zPrev < 0)	zPrev = 0;

		if (xNext < 0)	xNext = 0;
		if (yNext < 0)	yNext = 0;
		if (zNext < 0)	zNext = 0;

		if (x >= MAT_SIZE_CUBES)	x = MAT_SIZE_CUBES - 1;
		if (y >= MAT_SIZE_CUBES)	y = MAT_SIZE_CUBES - 1;
		if (z >= MAT_HEIGHT_CUBES)	z = MAT_HEIGHT_CUBES - 1;

		if (xPrev >= MAT_SIZE_CUBES)	xPrev = MAT_SIZE_CUBES - 1;
		if (yPrev >= MAT_SIZE_CUBES)	yPrev = MAT_SIZE_CUBES - 1;
		if (zPrev >= MAT_HEIGHT_CUBES)	zPrev = MAT_HEIGHT_CUBES - 1;

		if (xNext >= MAT_SIZE_CUBES)	xNext = MAT_SIZE_CUBES - 1;
		if (yNext >= MAT_SIZE_CUBES)	yNext = MAT_SIZE_CUBES - 1;
		if (zNext >= MAT_HEIGHT_CUBES)	zNext = MAT_HEIGHT_CUBES - 1;

		//On fait chaque axe
		MAxis axis = 0x00;
		valueColMin = oneShot ? 0.5f : 10000.0f;
		float seuil = 0.0000001f;
		float prodScalMin = 1.0f;
		if (dir.getSqrSize() > 1)
			dir.normalize();

		//On verif tout les 4 angles de gauche
		if (getCube(xPrev, yPrev, zPrev)->isSolid() ||
			getCube(xPrev, yPrev, zNext)->isSolid() ||
			getCube(xPrev, yNext, zPrev)->isSolid() ||
			getCube(xPrev, yNext, zNext)->isSolid())
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev + 1, yPrev, zPrev)->isSolid() ||
				getCube(xPrev + 1, yPrev, zNext)->isSolid() ||
				getCube(xPrev + 1, yNext, zPrev)->isSolid() ||
				getCube(xPrev + 1, yNext, zNext)->isSolid()) || !oneShot)
			{
				float depassement = ((xPrev + 1) * MCube::CUBE_SIZE) - (pos.X - width / 2.0f);
				float prodScal = abs(dir.X);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_X;
					}
			}
		}

		//float depassementx2 = (xNext * NYCube::CUBE_SIZE) - (pos.X + width / 2.0f);

		//On verif tout les 4 angles de droite
		if (getCube(xNext, yPrev, zPrev)->isSolid() ||
			getCube(xNext, yPrev, zNext)->isSolid() ||
			getCube(xNext, yNext, zPrev)->isSolid() ||
			getCube(xNext, yNext, zNext)->isSolid())
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xNext - 1, yPrev, zPrev)->isSolid() ||
				getCube(xNext - 1, yPrev, zNext)->isSolid() ||
				getCube(xNext - 1, yNext, zPrev)->isSolid() ||
				getCube(xNext - 1, yNext, zNext)->isSolid()) || !oneShot)
			{
				float depassement = (xNext * MCube::CUBE_SIZE) - (pos.X + width / 2.0f);
				float prodScal = abs(dir.X);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_X;
					}
			}
		}

		//float depassementy1 = (yNext * NYCube::CUBE_SIZE) - (pos.Y + width / 2.0f);

		//On verif tout les 4 angles de devant
		if (getCube(xPrev, yNext, zPrev)->isSolid() ||
			getCube(xPrev, yNext, zNext)->isSolid() ||
			getCube(xNext, yNext, zPrev)->isSolid() ||
			getCube(xNext, yNext, zNext)->isSolid())
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yNext - 1, zPrev)->isSolid() ||
				getCube(xPrev, yNext - 1, zNext)->isSolid() ||
				getCube(xNext, yNext - 1, zPrev)->isSolid() ||
				getCube(xNext, yNext - 1, zNext)->isSolid()) || !oneShot)
			{
				float depassement = (yNext * MCube::CUBE_SIZE) - (pos.Y + width / 2.0f);
				float prodScal = abs(dir.Y);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Y;
					}
			}
		}

		//float depassementy2 = ((yPrev + 1) * NYCube::CUBE_SIZE) - (pos.Y - width / 2.0f);

		//On verif tout les 4 angles de derriere
		if (getCube(xPrev, yPrev, zPrev)->isSolid() ||
			getCube(xPrev, yPrev, zNext)->isSolid() ||
			getCube(xNext, yPrev, zPrev)->isSolid() ||
			getCube(xNext, yPrev, zNext)->isSolid())
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yPrev + 1, zPrev)->isSolid() ||
				getCube(xPrev, yPrev + 1, zNext)->isSolid() ||
				getCube(xNext, yPrev + 1, zPrev)->isSolid() ||
				getCube(xNext, yPrev + 1, zNext)->isSolid()) || !oneShot)
			{
				float depassement = ((yPrev + 1) * MCube::CUBE_SIZE) - (pos.Y - width / 2.0f);
				float prodScal = abs(dir.Y);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Y;
					}
			}
		}

		//On verif tout les 4 angles du haut
		if (getCube(xPrev, yPrev, zNext)->isSolid() ||
			getCube(xPrev, yNext, zNext)->isSolid() ||
			getCube(xNext, yPrev, zNext)->isSolid() ||
			getCube(xNext, yNext, zNext)->isSolid())
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yPrev, zNext - 1)->isSolid() ||
				getCube(xPrev, yNext, zNext - 1)->isSolid() ||
				getCube(xNext, yPrev, zNext - 1)->isSolid() ||
				getCube(xNext, yNext, zNext - 1)->isSolid()) || !oneShot)
			{
				float depassement = (zNext * MCube::CUBE_SIZE) - (pos.Z + height / 2.0f);
				float prodScal = abs(dir.Z);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Z;
					}
			}
		}

		//On verif tout les 4 angles du bas
		if (getCube(xPrev, yPrev, zPrev)->isSolid() ||
			getCube(xPrev, yNext, zPrev)->isSolid() ||
			getCube(xNext, yPrev, zPrev)->isSolid() ||
			getCube(xNext, yNext, zPrev)->isSolid())
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yPrev, zPrev + 1)->isSolid() ||
				getCube(xPrev, yNext, zPrev + 1)->isSolid() ||
				getCube(xNext, yPrev, zPrev + 1)->isSolid() ||
				getCube(xNext, yNext, zPrev + 1)->isSolid()) || !oneShot)
			{
				float depassement = ((zPrev + 1) * MCube::CUBE_SIZE) - (pos.Z - height / 2.0f);
				float prodScal = abs(dir.Z);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Z;
					}
			}
		}

		return axis;
	}
		
	void render_world_basic(GLuint shader, YVbo * vboCube) 
	{
		glUseProgram(shader);

		for (int i = 0; i < MAT_SIZE_CUBES; i++)
		{
			for (int j = 0; j < MAT_SIZE_CUBES; j++)
			{
				for (int z = 0; z < MAT_HEIGHT_CUBES; z++)
				{
					auto cube = getCube(i, j, z);
					
					if (cube->getType() != MCube::CUBE_AIR 
						&& cube->getDraw())
					{
						glPushMatrix();

						glTranslatef(i * MCube::CUBE_SIZE, j * MCube::CUBE_SIZE, z * MCube::CUBE_SIZE);
						GLuint var = glGetUniformLocation(shader, "cube_color");
						glUniform3f(var, 0.5, 0.5f, 0.5f);

						YEngine::getInstance()->Renderer->updateMatricesFromOgl();
						YEngine::getInstance()->Renderer->sendMatricesToShader(shader);

						vboCube->render();

						glPopMatrix();
					}

					
				}
			}
		}
	}

	void render_world_vbo(bool debug,bool doTransparent)
	{
		const std::lock_guard<std::mutex> lock(chunkierMutex);
		glUseProgram(shaderProgram);
		
		texture->setAsShaderInput(shaderProgram);
		YEngine::getInstance()->Renderer->updateMatricesFromOgl();
		YEngine::getInstance()->Renderer->sendMatricesToShader(shaderProgram);
		

		glDisable(GL_BLEND);
		//Dessiner les chunks opaques
				
		/*for (int i = 0; i < MAT_SIZE; i++)
		{
			for (int j = 0; j < MAT_SIZE; j++)
			{
				for (int k = 0; k < MAT_HEIGHT; k++)
				{
					Chunks[i][j][k]->render(false);
				}
			}
		}*/

		
		std::list<MChunk*>::const_iterator iterator;
		for (iterator = chunks.begin(); iterator != chunks.end(); ++iterator) {
			(*iterator)->render(false);
		}

		if (doTransparent)
		{
			glEnable(GL_BLEND);
			//Dessiner les chunks transparents
			/*for (int i = 0; i < MAT_SIZE; i++)
			{
				for (int j = 0; j < MAT_SIZE; j++)
				{
					for (int k = 0; k < MAT_HEIGHT; k++)
					{
						Chunks[i][j][k]->render(true);
					}
				}
			}*/

			std::list<MChunk*>::const_iterator iterator;
			for (iterator = chunks.begin(); iterator != chunks.end(); ++iterator) {
				(*iterator)->render(true);
			}
		}
	}

	/**
	* Attention ce code n'est pas optimal, il est compréhensible. Il existe de nombreuses
	* versions optimisées de ce calcul.
	*/
	inline bool intersecDroitePlan(const YVec3f & debSegment, const  YVec3f & finSegment,
		const YVec3f & p1Plan, const YVec3f & p2Plan, const YVec3f & p3Plan,
		YVec3f & inter)
	{
		
		return true;
	}

	/**
	* Attention ce code n'est pas optimal, il est compréhensible. Il existe de nombreuses
	* versions optimisées de ce calcul. Il faut donner les points dans l'ordre (CW ou CCW)
	*/
	inline bool intersecDroiteCubeFace(const YVec3f & debSegment, const YVec3f & finSegment,
		const YVec3f & p1, const YVec3f & p2, const YVec3f & p3, const  YVec3f & p4,
		YVec3f & inter)
	{
		
		return false;
	}

	bool getRayCollision(const YVec3f & debSegment, const YVec3f & finSegment,
		YVec3f & inter,
		int &xCube, int&yCube, int&zCube)
	{
		
		return false;
	}

	/**
	* De meme cette fonction peut être grandement opitimisée, on a priviligié la clarté
	*/
	bool getRayCollisionWithCube(const YVec3f & debSegment, const YVec3f & finSegment,
		int x, int y, int z,
		YVec3f & inter)
	{

		return true;
	}
};



#endif