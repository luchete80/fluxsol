/************************************************************************
	
	Copyright 2012-2013 Luciano Buglioni
 
	Contact: luciano.buglioni@gmail.com

	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    Free CFD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/

#ifndef _CENTERTOVERTEX_INTERPOLATION_H_
#define _CENTERTOVERTEX_INTERPOLATION_H_

#include "Interpolation.h"
#include "../Type/Scalar.h"
#include "../Field/Field.h"
#include "../FiniteVolume/Mesh/FvGrid.h"
#include "../Type/Vec3d.h"


namespace FluxSol 
{

// Class can be a template class or not
//And could be inherited from FvGrid
//If is inherited it will cost duplication of entire mesh information
// Same occursr If a class member is a vertex field
template <typename T>
class CenterToVertexInterpolation
	:public Interpolation<T>
{

	//Fv_CC_Grid &grid;

	public:
	CenterToVertexInterpolation():Interpolation<T>(){};
	//references must be set in list
	CenterToVertexInterpolation(Fv_CC_Grid &grida):grid(grida){};
	CenterToVertexInterpolation(_CC_Fv_Field<T> &fi) :Interpolation<T>(fi){};

	Vertex_Fv_Field <T> Interpolate(_CC_Fv_Field<T> &);

	//Named Like in Open Foam
	//NOW IS A NEW CLASS
    _Surf_Fv_Field <T>
	FaceInterpolate (const _CC_Fv_Field <T>&);


	//Vertex_Fv_Field & VertexField(const CC_Fv_Field<T> &);
};

}; //FluxSol
#endif