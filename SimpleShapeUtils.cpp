#include "stdafx.h"
#include "Common.h"
#include "FiringSolutionThruster.h"
#include "Rotations.h"
#include "TestUtils.h"
#include "SimpleShapeUtils.h"
#include "CppUnitTest.h"


SimpleShapeUtils::SimpleShapeUtils()
{
}


SimpleShapeUtils::~SimpleShapeUtils()
{
}


string SimpleShapeUtils::VertexListToString(vector<VECTOR3> vertexlist)
{
	stringstream list;
	list << "Number of vertices: " << vertexlist.size() << "; ";
	
	for (UINT i = 0; i < vertexlist.size(); ++i)
	{
		list << TestUtils::V3ToString(vertexlist[i]) << "; ";
	}
	list << "\n";
	return list.str();
}