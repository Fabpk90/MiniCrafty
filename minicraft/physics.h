#pragma once
#include "engine/utils/types_3d.h"

class Physics
{
public:


	static bool isIntersecting(YVec3f startLine, YVec3f endLine, YVec3f pointInPlan, YVec3f planeNormal, float& t)
	{
		//planeNormal = a,b,c
		// a*x+b*y+z*c+d = 0
		//on prend un point de dans le plan et on fait un produit scalaire
		//on neg ce produit pour avoir d
		//
		//en gros t = -beta/alpha  (c.f. la ptite feuille blanche sur le bureau, en gros on développe l'équation du plan)

		float d = -planeNormal.dot(pointInPlan);

		float beta = planeNormal.dot(startLine) + d;
		float alpha = planeNormal.dot(endLine-startLine);

		if (alpha == 0) return false;
		
		t = -beta / alpha;

		return t > 0 && t <= 1;
	}

	static bool isIntersectingTriangle(YVec3f startLine, YVec3f endLine, YVec3f A, YVec3f B, YVec3f C, YVec3f D)
	{
		float t = 0;

		YVec3f planNormal = (A - B).cross(C - B);
		
		if (isIntersecting(startLine, endLine, A, planNormal, t))
		{
			YVec3f P = startLine + ((endLine - startLine) * t);

			YVec3f ABAP = (B - A).cross(P - A);
			YVec3f BCBP = (C - B).cross(P - B);
			YVec3f CDCP = (D - C).cross(P - C);
			YVec3f DADP = (A - D).cross(P - D);

			if (ABAP.dot(BCBP) >= 0 && BCBP.dot(CDCP) >= 0 && CDCP.dot(DADP) >= 0)
				return true;
			return false;
		}
		else
			return false;
	}
};
