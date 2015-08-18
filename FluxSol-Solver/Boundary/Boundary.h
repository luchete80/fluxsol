/************************************************************************

	Copyright 2012-2014 Luciano Buglioni - Pablo Zitelli

	Contacts:
        Luciano Buglioni: luciano.buglioni@gmail.com
        Pablo Zitelli:    zitelli.pablo@gmail.com
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
#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <vector>
#include <list>
#include "../Nastran/Varios.h"
#include "Boundary_Decl.h"
#include "FvGrid_Decl.h"
#include "FvFace.h"

namespace FluxSol{

//Varios patch forman un boundary
class Patch{

    int id;
	int num_cells;
	int num_faces;

	std::vector <int> id_face;
	std::vector <int> id_cell;	//Este puede ddeducirse del anterior pero no la inversa

	std::string name;

    Fv_CC_Grid *grid;   //Which belongs, to return faces as example


	public:
		Patch(){};
		Patch(const int &Id){id=Id;num_cells=num_faces=0;}
		Patch(const std::vector <int> cell, const std::vector <int> face){};
		//template <typename T,unsigned S> Patch( T (&v)[S]);
		Patch(std::list <int> &idfaces);
		Patch(const std::string s){name=s;}

		Patch(const std::string s, std::list <int> &idfaces);

		void AddFace(const int &f){id_face.push_back(f);id_face[num_faces]=f;num_faces++;}
		int & Num_Faces(){return num_faces;};
		const int & Id_Face(const int &i)const{return this->id_face[i];}

		void AddGrid(const Fv_CC_Grid &fvgrid){grid=&fvgrid;}

		const Fv_CC_Grid &Grid()const{return *this->grid;}

		const _FvFace & Face(const int &i)const;

		// OJO QUE ESTA FUNCION DEBE CREARSE, VER EN DONDE
		// LO PIDE EL COMPILADOR
		Patch & operator=(const Patch&){return *this;}

		const string & Name()const {return name;}


	};

class Boundary{

    std::vector<Patch> patch;
	std::list <int> idpatch;	//This is used in order to use Field::ApplyBC
                                //
	std::vector <int> e;

	std::vector <string> patchname;

	int num_faces;			//En la construccion se suma todo
	int num_patches;


    public:
    Boundary(){};
	//Boundary(std::vector<Patch> vp){patch=vp;}		Esto no lo puedo hacer porque no tengo un operador de patches
	Boundary(std::vector<int> vp){e=vp;}
	void AddPatch(Patch p){patch.push_back(p);}
	Boundary (std::vector <Patch> patchlist);
	int & Num_Faces(){return num_faces;}
	int & Num_Patches(){return num_patches;}
	const Patch & vPatch(const int &i)const {return patch[i];}
	void AddGridPtr(const Fv_CC_Grid &grid)
	{
        //NEW->Assgnind Grid refs to patches
        for (int p=0;p<this->num_patches;p++)   this->patch[p].AddGrid(grid);

	}

	const string & PatchName(const int &id)const{return patchname[id];}
};

}

#endif
