#pragma once

#include "engine/render/renderer.h"
#include "engine/render/vbo.h"
#include "cube.h"
#include <fstream>
#include <chrono>
#include <list>

/**
  * On utilise des chunks pour que si on modifie juste un cube, on ait pas
  * besoin de recharger toute la carte dans le buffer, mais juste le chunk en question
  */
class MChunk
{
	public :

		bool vboInit;

		static const int CHUNK_SIZE = 64; ///< Taille d'un chunk en nombre de cubes (n*n*n)
		MCube _Cubes[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; ///< Cubes contenus dans le chunk

		YVbo * VboOpaque = NULL;
		YVbo * VboTransparent = NULL;

		MChunk * Voisins[6];

		int _XPos, _YPos, _ZPos; ///< Position du chunk dans le monde

		MChunk(int x, int y, int z)
		{
			memset(Voisins, 0x00, sizeof(void*)* 6);
			_XPos = x;
			_YPos = y;
			_ZPos = z;

			vboInit = false;
		}

		~MChunk()
		{
			delete VboOpaque;
			delete VboTransparent;
		}

		/*
		Creation des VBO
		*/

		//On met le chunk dans son VBO
		void toVbos(void)
		{
			SAFEDELETE(VboOpaque);
			SAFEDELETE(VboTransparent);

			char msg[150];
			sprintf_s(msg, 150, "crea vbo chunk pos %d %d %d", _XPos, _YPos, _ZPos);
			YLog::log(YLog::ENGINE_INFO, msg);

			//Compter les sommets

			int cubeOpaque = 0, cubeTransparent = 0;

			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					for (int z = 0; z < CHUNK_SIZE; z++)
					{
						if(_Cubes[x][y][z].getType() != MCube::CUBE_AIR)
						{
							if (_Cubes[x][y][z].isOpaque()
								&& _Cubes[x][y][z].getDraw())
								cubeOpaque++;
							else if (_Cubes[x][y][z].isTransparent()
								&& _Cubes[x][y][z].getDraw())
								cubeTransparent++;
						}
					}
				}
			}

			sprintf_s(msg, 150, "chunk pos %d %d %d opaque %d  transp %d", _XPos, _YPos, _ZPos, cubeOpaque, cubeTransparent);
			YLog::log(YLog::ENGINE_INFO, msg);

			//Créer les VBO
			VboOpaque = new YVbo(4, cubeOpaque * 36, YVbo::PACK_BY_VERTICE);

			VboOpaque->setElementDescription(0, YVbo::Element(3));
			VboOpaque->setElementDescription(1, YVbo::Element(3));
			VboOpaque->setElementDescription(2, YVbo::Element(2));
			VboOpaque->setElementDescription(3, YVbo::Element(1));

			VboOpaque->createVboCpu();

			VboTransparent = new YVbo(4, cubeTransparent * 36, YVbo::PACK_BY_VERTICE);

			VboTransparent->setElementDescription(0, YVbo::Element(3));
			VboTransparent->setElementDescription(1, YVbo::Element(3));
			VboTransparent->setElementDescription(2, YVbo::Element(2));
			VboTransparent->setElementDescription(3, YVbo::Element(1));

			
			VboTransparent->createVboCpu();

			YVec3f a = YVec3f();
			YVec3f b = YVec3f();
			YVec3f c = YVec3f();
			YVec3f d = YVec3f();
			int iVerticeOpaque = 0;
			int iVerticeTransparent = 0;
			const int size = MCube::CUBE_SIZE;

			//Remplir les VBO
			//TODO: check if face needs to be shown
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					for (int z = 0; z < CHUNK_SIZE; z++)
					{
						float type = (int)_Cubes[x][y][z].getType();
						auto vbo = _Cubes[x][y][z].isOpaque() ? VboOpaque : VboTransparent;
						int& vert = _Cubes[x][y][z].isOpaque() ? iVerticeOpaque : iVerticeTransparent;

						if(_Cubes[x][y][z].getDraw() && _Cubes[x][y][z].getType() != MCube::CUBE_AIR)
						{
							const int trueX = (x * size) + _XPos * CHUNK_SIZE;
							const int trueY = (y * size) + _YPos * CHUNK_SIZE;
							const int trueZ = (z * size) + _ZPos * CHUNK_SIZE;
							// XY
							
							a = YVec3f(trueX, trueY, trueZ + size);
							b = YVec3f(trueX + size, trueY, trueZ + size);
							c = YVec3f(trueX + size, trueY + size, trueZ + size);
							d = YVec3f(trueX, trueY + size, trueZ + size);
							vert += addQuadToVbo(vbo, vert, a, b, c, d, type);

							if(_Cubes[x][y][z].isOpaque())
							{
								b = YVec3f(trueX, trueY, trueZ);
								c = YVec3f(trueX, trueY + size, trueZ);
								d = YVec3f(trueX + size, trueY + size, trueZ);
								a = YVec3f(trueX + size, trueY, trueZ);
								vert += addQuadToVbo(vbo, vert, a, b, c, d, type);

								// XZ
								a = YVec3f(trueX, trueY, trueZ);
								b = YVec3f(trueX + size, trueY, trueZ);
								c = YVec3f(trueX + size, trueY, trueZ + size);
								d = YVec3f(trueX, trueY, trueZ + size);
								vert += addQuadToVbo(vbo, vert, a, b, c, d, type);
								b = YVec3f(trueX, trueY + size, trueZ);
								c = YVec3f(trueX, trueY + size, trueZ + size);
								d = YVec3f(trueX + size, trueY + size, trueZ + size);
								a = YVec3f(trueX + size, trueY + size, trueZ);
								vert += addQuadToVbo(vbo, vert, a, b, c, d, type);

								// YZ
								b = YVec3f(trueX, trueY, trueZ);
								c = YVec3f(trueX, trueY, trueZ + size);
								d = YVec3f(trueX, trueY + size, trueZ + size);
								a = YVec3f(trueX, trueY + size, trueZ);
								vert += addQuadToVbo(vbo, vert, a, b, c, d, type);
								a = YVec3f(trueX + size, trueY, trueZ);
								b = YVec3f(trueX + size, trueY + size, trueZ);
								c = YVec3f(trueX + size, trueY + size, trueZ + size);
								d = YVec3f(trueX + size, trueY, trueZ + size);
								vert += addQuadToVbo(vbo, vert, a, b, c, d, type);
							}
						}
					}
				}
			}

			
		}

		void sendToGPU()
		{
			VboOpaque->createVboGpu();
			VboOpaque->deleteVboCpu();

			VboTransparent->createVboGpu();
			VboTransparent->deleteVboCpu();

			vboInit = true;
		}

		YVec3f GetAtlasPosition(float type)
		{
			//grass	0 0
			//rock	1 0
			//dirt  3 0
			//water 14 0

			if(type == 1)
				return YVec3f(0, 0, 0);
			if (type == 2) //dirt
				return YVec3f(2, 0, 0);
			if (type == 4)
				return YVec3f(1, 0, 0);
			if (type == 5)
				return YVec3f(14, 0, 0);

			return YVec3f(-1, -1, -1);
		}

		//Ajoute un quad du cube. Attention CCW
		int addQuadToVbo(YVbo * vbo, int iVertice, YVec3f & a, YVec3f & b, YVec3f & c, YVec3f & d, float type) {

			YVec3f test = (b - a).normalize().cross((c - a).normalize());
			YVec3f uvPosition = GetAtlasPosition(type) * 0.0625;
			
			vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
			vbo->setElementValue(1, iVertice, test.X, test.Y, test.Z);
			vbo->setElementValue(2, iVertice, uvPosition.X , uvPosition.Y + 0.0625f);
			vbo->setElementValue(3, iVertice, type);
			++iVertice;
			vbo->setElementValue(0, iVertice, b.X, b.Y, b.Z);
			vbo->setElementValue(1, iVertice, test.X, test.Y, test.Z);
			vbo->setElementValue(2, iVertice, uvPosition.X + 0.0625, uvPosition.Y + 0.0625);
			vbo->setElementValue(3, iVertice, type);
			++iVertice;
			vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
			vbo->setElementValue(1, iVertice, test.X, test.Y, test.Z);
			vbo->setElementValue(2, iVertice, uvPosition.X + 0.0625, uvPosition.Y);
			vbo->setElementValue(3, iVertice, type);
			++iVertice;
			
			vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
			vbo->setElementValue(1, iVertice, test.X, test.Y, test.Z);
			vbo->setElementValue(2, iVertice, uvPosition.X, uvPosition.Y + 0.0625);
			vbo->setElementValue(3, iVertice, type);
			++iVertice;
			vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
			vbo->setElementValue(1, iVertice, test.X, test.Y, test.Z);
			vbo->setElementValue(2, iVertice, uvPosition.X + 0.0625, uvPosition.Y);
			vbo->setElementValue(3, iVertice, type);
			++iVertice;
			vbo->setElementValue(0, iVertice, d.X, d.Y, d.Z);
			vbo->setElementValue(1, iVertice, test.X, test.Y, test.Z);
			vbo->setElementValue(2, iVertice, uvPosition.X, uvPosition.Y);
			vbo->setElementValue(3, iVertice, type);
			return 6;
		}

		//Permet de compter les triangles ou des les ajouter aux VBO
		void foreachVisibleTriangle(bool countOnly, int * nbVertOpaque, int * nbVertTransp, YVbo * VboOpaque, YVbo * VboTrasparent)
		{
		}

		/*
		Gestion du chunk
		*/

		void reset(void)
		{
			for(int x=0;x<CHUNK_SIZE;x++)
				for(int y=0;y<CHUNK_SIZE;y++)
					for(int z=0;z<CHUNK_SIZE;z++)
					{
						_Cubes[x][y][z].setDraw(false);
						_Cubes[x][y][z].setType(MCube::CUBE_AIR);
					}
		}

		void setVoisins(MChunk * xprev, MChunk * xnext, MChunk * yprev, MChunk * ynext, MChunk * zprev, MChunk * znext)
		{
			Voisins[0] = xprev;
			Voisins[1] = xnext;
			Voisins[2] = yprev;
			Voisins[3] = ynext;
			Voisins[4] = zprev;
			Voisins[5] = znext;
		}

		void get_surrounding_cubes(int x, int y, int z, MCube ** cubeXPrev, MCube ** cubeXNext,
			MCube ** cubeYPrev, MCube ** cubeYNext,
			MCube ** cubeZPrev, MCube ** cubeZNext)
		{

			*cubeXPrev = NULL;
			*cubeXNext = NULL;
			*cubeYPrev = NULL;
			*cubeYNext = NULL;
			*cubeZPrev = NULL;
			*cubeZNext = NULL;

			if (x == 0 && Voisins[0] != NULL)
				*cubeXPrev = &(Voisins[0]->_Cubes[CHUNK_SIZE - 1][y][z]);
			else if (x > 0)
				*cubeXPrev = &(_Cubes[x - 1][y][z]);

			if (x == CHUNK_SIZE - 1 && Voisins[1] != NULL)
				*cubeXNext = &(Voisins[1]->_Cubes[0][y][z]);
			else if (x < CHUNK_SIZE - 1)
				*cubeXNext = &(_Cubes[x + 1][y][z]);

			if (y == 0 && Voisins[2] != NULL)
				*cubeYPrev = &(Voisins[2]->_Cubes[x][CHUNK_SIZE - 1][z]);
			else if (y > 0)
				*cubeYPrev = &(_Cubes[x][y - 1][z]);

			if (y == CHUNK_SIZE - 1 && Voisins[3] != NULL)
				*cubeYNext = &(Voisins[3]->_Cubes[x][0][z]);
			else if (y < CHUNK_SIZE - 1)
				*cubeYNext = &(_Cubes[x][y + 1][z]);

			if (z == 0 && Voisins[4] != NULL)
				*cubeZPrev = &(Voisins[4]->_Cubes[x][y][CHUNK_SIZE - 1]);
			else if (z > 0)
				*cubeZPrev = &(_Cubes[x][y][z - 1]);

			if (z == CHUNK_SIZE - 1 && Voisins[5] != NULL)
				*cubeZNext = &(Voisins[5]->_Cubes[x][y][0]);
			else if (z < CHUNK_SIZE - 1)
				*cubeZNext = &(_Cubes[x][y][z + 1]);
		}

		void render(bool transparent)
		{
			if(vboInit)
			{
				if (transparent)
					VboTransparent->render();
				else
					VboOpaque->render();
			}
		}

		/**
		  * On verifie si le cube peut être vu
		  */
		bool test_hidden(int x, int y, int z)
		{
			MCube * cubeXPrev = NULL; 
			MCube * cubeXNext = NULL; 
			MCube * cubeYPrev = NULL; 
			MCube * cubeYNext = NULL; 
			MCube * cubeZPrev = NULL; 
			MCube * cubeZNext = NULL; 

			if(x == 0 && Voisins[0] != NULL)
				cubeXPrev = &(Voisins[0]->_Cubes[CHUNK_SIZE-1][y][z]);
			else if(x > 0)
				cubeXPrev = &(_Cubes[x-1][y][z]);

			if(x == CHUNK_SIZE-1 && Voisins[1] != NULL)
				cubeXNext = &(Voisins[1]->_Cubes[0][y][z]);
			else if(x < CHUNK_SIZE-1)
				cubeXNext = &(_Cubes[x+1][y][z]);

			if(y == 0 && Voisins[2] != NULL)
				cubeYPrev = &(Voisins[2]->_Cubes[x][CHUNK_SIZE-1][z]);
			else if(y > 0)
				cubeYPrev = &(_Cubes[x][y-1][z]);

			if(y == CHUNK_SIZE-1 && Voisins[3] != NULL)
				cubeYNext = &(Voisins[3]->_Cubes[x][0][z]);
			else if(y < CHUNK_SIZE-1)
				cubeYNext = &(_Cubes[x][y+1][z]);

			if(z == 0 && Voisins[4] != NULL)
				cubeZPrev = &(Voisins[4]->_Cubes[x][y][CHUNK_SIZE-1]);
			else if(z > 0)
				cubeZPrev = &(_Cubes[x][y][z-1]);

			if(z == CHUNK_SIZE-1 && Voisins[5] != NULL)
				cubeZNext = &(Voisins[5]->_Cubes[x][y][0]);
			else if(z < CHUNK_SIZE-1)
				cubeZNext = &(_Cubes[x][y][z+1]);

			if( cubeXPrev == NULL || cubeXNext == NULL ||
				cubeYPrev == NULL || cubeYNext == NULL ||
				cubeZPrev == NULL || cubeZNext == NULL )
				return false;

			if (cubeXPrev->isOpaque() == true && //droite
				cubeXNext->isOpaque() == true && //gauche
				cubeYPrev->isOpaque() == true && //haut
				cubeYNext->isOpaque() == true && //bas
				cubeZPrev->isOpaque() == true && //devant
				cubeZNext->isOpaque() == true)  //derriere
				return true;
			return false;
		}

		void disableHiddenCubes(void)
		{
			for(int x=0;x<CHUNK_SIZE;x++)
				for(int y=0;y<CHUNK_SIZE;y++)
					for(int z=0;z<CHUNK_SIZE;z++)
					{
						_Cubes[x][y][z].setDraw(true);
						if(test_hidden(x,y,z))
							_Cubes[x][y][z].setDraw(false);
					}
		}

		MCube* getCubeAt(int x, int y, int z)
		{
			return &_Cubes[x][y][z];
		}

		void saveToDisk()
		{

			uint8* buffer = new uint8[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 4];

			MCube::MCubeType cubeType = _Cubes[0][0][0].getType();
			uint16 nbOccur = 0;
			int allCase = 0;
			int posInFile = -1;
			for (int z = 0; z < CHUNK_SIZE; ++z)
				for (int y = 0; y < CHUNK_SIZE; ++y) {
					for (int x = 0; x < CHUNK_SIZE; ++x) {
						++allCase;
						if (_Cubes[x][y][z].getType() == cubeType) ++nbOccur;


						if ((_Cubes[x][y][z].getType() != cubeType || nbOccur == 0xFFFF) || (z + 1 == CHUNK_SIZE && y + 1 == CHUNK_SIZE && x + 1 == CHUNK_SIZE)) {
							bool endnull = (nbOccur & 0xFF00) == 0x0000;
							bool basenull = (nbOccur & 0x00FF) == 0x0000;

							uint8 occurInfo = 0x80 | ((basenull) ? 0x00 : 0x01) | ((endnull) ? 0x00 : 0x02);
							buffer[++posInFile] = (MCube::MCubeType)cubeType;
							buffer[++posInFile] = occurInfo;

							buffer[++posInFile] = (!endnull) ? (nbOccur >> 8) : 0xff;
							buffer[++posInFile] = (!basenull) ? nbOccur : 0xff;

							if (nbOccur < 0xFFFF) nbOccur = 1;
							else nbOccur = 0;
							cubeType = _Cubes[x][y][z].getType();
						}

					}
				}
			buffer[++posInFile] = 0x00;

			char path[50];
			sprintf_s(path, "%d_%d_%d.txt", _XPos, _YPos, _ZPos); 

			ofstream myfile(path, std::ofstream::binary);
			myfile << buffer;

			buffer[posInFile] = 0x01;
			delete[] buffer;
			myfile.close();
		}

		void loadFrom(const ifstream& stream)
		{
			
			std::filebuf* pbuf = stream.rdbuf();
			int size = pbuf->pubseekoff(0, stream.end, stream.in);
			pbuf->pubseekpos(0, stream.in);
			uint8* buffer = new uint8[size];
			pbuf->sgetn((char*)buffer, size);

			int posInFile = 0;
			uint8 cubeType;
			uint16 nbOccur = 0;
			
			for (int z = 0; z < CHUNK_SIZE; ++z)
				for (int y = 0; y < CHUNK_SIZE; ++y) {
					for (int x = 0; x < CHUNK_SIZE; ++x) {


						if (nbOccur == 0) {
							cubeType = buffer[posInFile];
							++posInFile;
							uint8 occurinfo = buffer[posInFile];
							++posInFile;
							nbOccur = ((occurinfo & 2) == 0x02) ? buffer[posInFile] << 8 : 0x00;
							++posInFile;
							nbOccur |= ((occurinfo & 1) == 0x01) ? buffer[posInFile] : 0x00;
							++posInFile;
						}

						nbOccur--;
						_Cubes[x][y][z]._Code = cubeType;
					}
				}
			
			delete[] buffer;
		}
};