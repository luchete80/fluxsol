/************************************************************************

	Copyright 2012-2013 Luciano Buglioni

	Contact: luciano.buglioni@gmail.com

	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    FluxSol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#ifndef _WRITEVTK_FVGRID_DEF_H_
#define _WRITEVTK_FVGRID_DEF_H_

//Entirely based in free-cfd
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>      // std::setw

//For int2string
#include <string>
#include <sstream>
#include <limits>

#include "../FiniteVolume/Mesh/FvGrid.h"


namespace FluxSol
{
using namespace std;
//using namespace FluxSol;

//Variables to write VTK
int timeStep,gid;
vector<string> varList;
vector<bool> var_is_vec3d;

string int2str(int number) {
        std::stringstream ss;
        ss << number;
        return ss.str();
}

//Initially it cat write several meshes
void Fv_CC_Grid::Write_vtk()
{

	int Rank=0;

	//Like in OpenFoam or free-cfd, time steps will be separated into subfolders
	string filePath="./output/"+int2str(timeStep);
	//string fileName=filePath+"/grid_" + int2str(gid+1) + "_proc_"+int2str(Rank)+".vtu";
	string fileName="Hola.txt";
	ofstream file;
	file.open((fileName).c_str(),ios::out);
	file << "<?xml version=\"1.0\"?>" << endl;
	file << "<VTKFile type=\"UnstructuredGrid\">" << endl;
	file << "<UnstructuredGrid>" << endl;
	file << "<Piece NumberOfPoints=\"" << this->Num_Verts() << "\" NumberOfCells=\"" << this->Num_Cells() << "\">" << endl;
	file << "<Points>" << endl;
	file << "<DataArray NumberOfComponents=\"3\" type=\"Float32\" format=\"ascii\" >" << endl;
	for (int n=0;n<this->Num_Verts();++n) {
		for (int i=0; i<3; ++i) file<< setw(16) << setprecision(8) << scientific << vert[n].Comp()[i] << endl;
	}
	file << "</DataArray>" << endl;
	file << "</Points>" << endl;
	file << "<Cells>" << endl;

	file << "<DataArray Name=\"connectivity\" type=\"Int32\" format=\"ascii\" >" << endl;
	for (int c=0;c<this->Num_Cells();++c) {
		for (int n=0;n<Cell(c).Num_Vertex();++n) {
			file << Cell(c).Vert(n) << "\t";
		}
		file << endl;
	}

	file << "</DataArray>" << endl;
	file << "<DataArray Name=\"offsets\" type=\"Int32\" format=\"ascii\" >" << endl;
	int offset=0;
	for (int c=0;c<this->Num_Cells();++c) {
		offset+=Cell(c).Num_Vertex();
		file << offset << endl;
	}
	file << "</DataArray>" << endl;

	file << "<DataArray Name=\"types\" type=\"UInt8\" format=\"ascii\" >" << endl;
	for (int c=0;c<Num_Cells();++c) {
		if (Cell(c).Num_Vertex()==4) file << "10" << endl; // Tetra
		if (Cell(c).Num_Vertex()==8) file << "12" << endl; // Hexa
		if (Cell(c).Num_Vertex()==6) file << "13" << endl; // Prism
		if (Cell(c).Num_Vertex()==5) file << "14" << endl; // Pyramid (Wedge)
	}
	file << endl;
	file << "</DataArray>" << endl;;

	file << "</Cells>" << endl;

	file << "<PointData Scalars=\"scalars\" format=\"ascii\">" << endl;


	file << "</PointData>" << endl;

	file << "</Piece>" << endl;
	file << "</UnstructuredGrid>" << endl;
	file << "</VTKFile>" << endl;
	file.close();

	return;
}

}	//End of FluxSol

#endif
