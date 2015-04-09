#ifndef _WRITEVTK_DEF_H_
#define _WRITEVTK_DEF_H_

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

#include "./FiniteVolume/Mesh/FvGrid.h"
#include "./Field/Field.h"

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
template <typename T>
void Write_vtk(std::vector <Fv_CC_Grid> grid, CC_Fv_Field <T>) 
{
	
	int Rank;
	
	//Like in OpenFoam or free-cfd, time steps will be separated into subfolders
	string filePath="./output/"+int2str(timeStep);
	string fileName=filePath+"/grid_" + int2str(gid+1) + "_proc_"+int2str(Rank)+".vtu";
	
	ofstream file;
	file.open((fileName).c_str(),ios::out);
	file << "<?xml version=\"1.0\"?>" << endl;
	file << "<VTKFile type=\"UnstructuredGrid\">" << endl;
	file << "<UnstructuredGrid>" << endl;
	file << "<Piece NumberOfPoints=\"" << grid[gid].nodeCount << "\" NumberOfCells=\"" << grid[gid].cellCount << "\">" << endl;
	file << "<Points>" << endl;
	file << "<DataArray NumberOfComponents=\"3\" type=\"Float32\" format=\"ascii\" >" << endl;
	for (int n=0;n<grid[gid].nodeCount;++n) {
		for (int i=0; i<3; ++i) file<< setw(16) << setprecision(8) << scientific << grid[gid].node[n].comp[i] << endl;
	}
	file << "</DataArray>" << endl;
	file << "</Points>" << endl;
	file << "<Cells>" << endl;
	
	file << "<DataArray Name=\"connectivity\" type=\"Int32\" format=\"ascii\" >" << endl;
	for (int c=0;c<grid[gid].cellCount;++c) {
		for (int n=0;n<grid[gid].cell[c].nodeCount;++n) {
			file << grid[gid].cell[c].nodes[n] << "\t";
		}
		file << endl;
	}
	
	file << "</DataArray>" << endl;
	file << "<DataArray Name=\"offsets\" type=\"Int32\" format=\"ascii\" >" << endl;
	int offset=0;
	for (int c=0;c<grid[gid].cellCount;++c) {
		offset+=grid[gid].cell[c].nodeCount;
		file << offset << endl;
	}
	file << "</DataArray>" << endl;
	
	file << "<DataArray Name=\"types\" type=\"UInt8\" format=\"ascii\" >" << endl;
	for (int c=0;c<grid[gid].Num_Cells();++c) {
		if (grid[gid].Cell(c).Num_Vertex()==4) file << "10" << endl; // Tetra
		if (grid[gid].Cell(c).Num_Vertex()==8) file << "12" << endl; // Hexa
		if (grid[gid].Cell(c).Num_Vertex()==6) file << "13" << endl; // Prism
		if (grid[gid].Cell(c).Num_Vertex()==5) file << "14" << endl; // Pyramid (Wedge)
	}
	file << endl;
	file << "</DataArray>" << endl;;
	
	file << "</Cells>" << endl;
	
	file << "<PointData Scalars=\"scalars\" format=\"ascii\">" << endl;
	
	//Look trough output variables
	for (int ov=0;ov<varList.size();++ov) {
		// Assume outputting a vector variable
		bool scalar=false;
		// Loop vector components
		for (int i=0;i<3;++i) {
			// Write variable name
			file << "<DataArray Name=\"";
			if (var_is_vec3d[ov]) {
				if (i==0) file << varList[ov] << "_x"; 
				if (i==1) file << varList[ov] << "_y"; 
				if (i==2) file << varList[ov] << "_z"; 
			} else {
				file << varList[ov]; 
				scalar=true;
			}
			file << "\" type=\"Float32\" format=\"ascii\" >" << endl;
			if (varList[ov]=="p") for (int n=0;n<grid[gid].Num_Verts();++n) file << ns[gid].p.node(n) << endl;
			else if (varList[ov]=="T") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].T.node(n) << endl;
			else if (varList[ov]=="rho") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].material.rho(ns[gid].p.node(n),ns[gid].T.node(n)) << endl;
			else if (varList[ov]=="dt") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].T.node(n) << endl;
			else if (varList[ov]=="mu") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].material.viscosity(ns[gid].T.node(n)) << endl;
			else if (varList[ov]=="lambda") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].material.therm_cond(ns[gid].T.node(n)) << endl;	
			else if (varList[ov]=="Cp") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].material.Cp(ns[gid].T.node(n)) << endl;
			else if (varList[ov]=="resp") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].update[0].node(n) << endl;
			else if (varList[ov]=="resT") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].update[4].node(n) << endl;
			else if (varList[ov]=="limiterp") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].limiter[0].node(n) << endl; 
			else if (varList[ov]=="limiterT") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].limiter[4].node(n) << endl;
			else if (varList[ov]=="k") for (int n=0;n<grid[gid].nodeCount;++n) file << rans[gid].k.node(n) << endl;
			else if (varList[ov]=="omega") for (int n=0;n<grid[gid].nodeCount;++n) file << rans[gid].omega.node(n) << endl;
			else if (varList[ov]=="mu_t") for (int n=0;n<grid[gid].nodeCount;++n) file << rans[gid].mu_t.node(n) << endl;
			else if (varList[ov]=="Mach") {
				for (int n=0;n<grid[gid].nodeCount;++n) file << fabs(ns[gid].V.node(n))/ns[gid].material.a(ns[gid].p.node(n),ns[gid].T.node(n)) << endl;
			}
			// Vectors
			else if (varList[ov]=="V") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].V.node(n)[i] << endl; 
			else if (varList[ov]=="gradp") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].gradp.node(n)[i] << endl;
			else if (varList[ov]=="gradu") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].gradu.node(n)[i] << endl;
			else if (varList[ov]=="gradv") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].gradv.node(n)[i] << endl;
			else if (varList[ov]=="gradw") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].gradw.node(n)[i] << endl;
			else if (varList[ov]=="gradT") {
				if (equations[gid]==NS) {
					for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].gradT.node(n)[i] << endl;
				} else if (equations[gid]==HEAT) {
					for (int n=0;n<grid[gid].nodeCount;++n) file << hc[gid].gradT.node(n)[i] << endl;
				}
			}				
			else if (varList[ov]=="gradrho") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].gradrho.node(n)[i] << endl;
			else if (varList[ov]=="resV") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].update[1].node(n) << endl;
			else if (varList[ov]=="limiterV") for (int n=0;n<grid[gid].nodeCount;++n) file << ns[gid].limiter[1].node(n) << endl;
			else if (varList[ov]=="gradk") for (int n=0;n<grid[gid].nodeCount;++n) file << rans[gid].gradk.node(n)[i] << endl;
			else if (varList[ov]=="gradomega") for (int n=0;n<grid[gid].nodeCount;++n) file << rans[gid].gradomega.node(n)[i] << endl;	
			file << "</DataArray>" << endl;
			if (scalar) break;
		}
		
	}
	
	file << "</PointData>" << endl;
	
	file << "</Piece>" << endl;
	file << "</UnstructuredGrid>" << endl;
	file << "</VTKFile>" << endl;
	file.close();

	return;
}

}	//End of FluxSol

#endif