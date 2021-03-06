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
#include "FvField.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
//#include <chrono>
#include <string>
#include <cstring>

#include <time.h>
//
//
//#define PROFILE(BLOCK, ROUTNAME) ProfilerRun([&](){do {BLOCK;} while(0);}, \
//        ROUTNAME, __FILE__, __LINE__);
//
//template <typename T>
//void ProfilerRun (T&& func, const std::string& routine_name = "unknown",
//                  const char* file = "unknown", unsigned line = 0)
//{
//    using std::cerr;
//    using std::endl;
//
//    clock_t t_begin = clock();
//
//    // Call the function
//    func();
//
//    clock_t t_end = clock();
//    cerr << "[" << std::setw (20)
//         << (std::strrchr (file, '/') ?
//             std::strrchr (file, '/') + 1 : file)
//         << ":" << std::setw (5) << line << "]   "
//         << std::setw (10) << std::setprecision (6) << std::fixed
////         << static_cast<float> (duration_cast<microseconds>
//         <<(double)(t_end - t_begin) / CLOCKS_PER_SEC
//         //<< (t_end - t_begin).count() / 1e6
//         << "s  --> " << routine_name << endl;
//
//    cerr.unsetf (std::ios_base::floatfield);
//}

namespace FluxSol
{

//Inner net flux faces
template <typename T>
void _CC_Fv_Field<T>::Create_IntNetFluxFaces()
{
	for (int f=0;f<this->Grid().Num_Faces();f++)
	{
	    //cout << "Face "<<f<<endl;
		_FvFace face=this->Grid().Face(f);
		if (!face.Is_Null_Flux_Face() && !this->Grid().Face(f).Boundaryface())
            this->int_netflux_faces.insert(f);

	}
}


template <typename T>
_CC_Fv_Field<T>::_CC_Fv_Field(const Fv_CC_Grid &grid)
{
        //TO MODIFY: CALL BASE FIELD CONSTRUCTOR
		this->GridPtr=&grid;
		this->value.clear();
		T val;
		this->value=vector<T>(this->GridPtr->Num_Cells());
		this->prev_value=vector<T>(this->GridPtr->Num_Cells());

		//Inicio variables PARA TODOS LOS CELLS (ver luego el borde
        //cout << "Cell Number" << GridPtr->Num_Cells()<<endl;
//		for (int c=0;c<this->GridPtr->Num_Cells();c++)
//		{
//			T val;
//			//this->value.push_back(val);
//			this->value[c]=val;
//		}

    // Original
    _BoundaryField <T> bf(grid.vBoundary());
    //_BoundaryField <T> bf;
	//PROFILE({bf=grid.vBoundary();}, "BF Create");
	//Para igualar los boundaryfield debo ver el Scalar =
	this->BoundaryField=bf;
    this->numberofvals=this->GridPtr->Num_Cells();
    //this->Create_IntNetFluxFaces();
    this->int_netflux_faces=grid.IntFaces();
}

template <typename T>
_CC_Fv_Field<T>::_CC_Fv_Field(const Fv_CC_Grid *grid)
{
    this->GridPtr=grid;
    this->value.clear();
    T val;
    this->value=vector<T>(this->GridPtr->Num_Cells());

	_BoundaryField <T> bf(grid->vBoundary());
	//Para igualar los boundaryfield debo ver el Scalar =
	this->BoundaryField=bf;
    this->numberofvals=this->GridPtr->Num_Cells();
    this->Create_IntNetFluxFaces();
}

template <typename T>
_CC_Fv_Field<T>::_CC_Fv_Field(const Fv_CC_Grid &grid, const vector <T*> vp)
:_CC_Fv_Field(grid)
{
		for (int c=0;c<this->GridPtr->Num_Cells();c++)
			this->value[c]=*vp[c];
}

template <typename T>
_CC_Fv_Field<T>::_CC_Fv_Field(const Fv_CC_Grid *grid, const vector <T*> vp)
:_CC_Fv_Field(grid)
{
		for (int c=0;c<this->GridPtr->Num_Cells();c++)
			this->value[c]=*vp[c];
}


//Must be verified the patch fields against mesh patches
template <typename T>
_CC_Fv_Field<T>::_CC_Fv_Field(const Fv_CC_Grid &grid, const _BoundaryField <T> &bfield)
{
	_BoundaryField <T> bf = bfield;
        T val;
		this->GridPtr=&grid;
        this->value=vector<T>(this->GridPtr->Num_Cells());
		//Inicio variables PARA TODOS LOS CELLS (ver luego el borde
//		for (int c=0;c<this->GridPtr->Num_Cells();c++)
//		{
//			T val;
//			//this->value.push_back(val);
//			this->value[c]=val;
//		}

	//Para igualar los boundaryfield debo ver el Scalar =
	this->BoundaryField=bf;
    this->numberofvals=this->GridPtr->Num_Cells();
    this->Create_IntNetFluxFaces();
}

//
//template <typename T>
//void _CC_Fv_Field<T>::ToCellCenters(EqnSystem <T> &eqnsys)
//{
//	//Can compare number of cells vs number of eqn
//	for (int e=0;e<eqnsys.Num_Eqn();e++)
//	{
//		this->value[e]=eqnsys.Eqn(e).X();
//
//	}
//
//}

//template<typename T>
//_CC_Fv_Field <T>::_CC_Fv_Field(InputFile &inputfile)
//{
//
//
//
//}


}

#include "_CC_Fv_field.inst"