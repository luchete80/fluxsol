/************************************************************************
	
	Copyright 2012-2013 Luciano Buglioni
 
	Contact: luciano.buglioni@gmail.com

	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version. CFD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#include "./Mesh/MeshInfo.h"

using namespace std;
namespace FluxSol
{

MeshInfo::MeshInfo(const string name, _Grid &malla)
{
	filename=name;
	
	char *n=name.c_str();

	this->ios.open(n, std::ofstream::out | std::ofstream::app);
	
	ios<<"Number of Vertices: "	<<malla.Num_Verts()<<endl;
	ios<<"Number of Cells:    "	<<malla.Num_Cells()<<endl;

	for (int i=0;i<malla.Num_Cells();i++)
	{
		ios<<"Cell " << i << " Neighbours"<<endl;
		
	}
}

};	//Fin namespace FluxSol