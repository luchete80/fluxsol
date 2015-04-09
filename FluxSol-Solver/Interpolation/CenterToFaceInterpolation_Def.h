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

#ifndef _CENTERTOFACE_INTERPOLATION_DEF_H_
#define _CENTERTOFACE_INTERPOLATION_DEF_H_

#include "Interpolation_.h"
#include "CenterToFaceInterpolation.h"

//template<typename T>
//CenterToFaceInterpolation<T>::CenterToFaceInterpolation<T>(const _CC_Fv_Field <T> &fi)
//:field(fi),
// grid(fi.GridPtr())
//{


//}

//Return the initial interpolated field
template<typename T>
const GeomSurfaceField <T> &
CenterToFaceInterpolation<T>::Interpolate()
{

	return intfield;
}


template<typename T>
GeomSurfaceField <T> &
CenterToFaceInterpolation<T>::Interpolate(const _CC_Fv_Field <T> &fi)
{
	_CC_Fv_Field <T> field = fi;
	//Pending to Generate constructor
	_Surf_Fv_Field <T> r(field.Grid().Num_Faces());


	//Loop throug faces
	cout << " field faces " <<field.Grid().Num_Faces()<<endl;
	for (int f = 0; f<field.Grid().Num_Faces(); f++)
	{
		_FvFace &face = field.Grid().Face(f);
		T prom;
		//Scalar fp
		if (!field.Grid().Face(f).Boundaryface())
			prom = (field[face.Cell(0)] * (1.0 - face.Fp()) + field[face.Cell(1)] * face.Fp())*0.5;
		else
			prom = field.Val(field.Grid().Face(f).Cell(0));

		r[f] = prom;
	}

	intfield = r;

}



#endif
