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

#ifndef _CENTERTOFACE_INTERPOLATION_H_
#define _CENTERTOFACE_INTERPOLATION_H_

#include "Interpolation_.h"
#include "../Type/Scalar.h"
#include "../Field/Field.h"
#include "../FiniteVolume/Mesh/FvGrid.h"
#include "../Type/Vec3d.h"

#include "../Field/GeometricField.h"


namespace FluxSol
{

// Class can be a template class or not
//And could be inherited from FvGrid
//If is inherited it will cost duplication of entire mesh information
// Same occursr If a class member is a vertex field
template <typename T>
class CenterToFaceInterpolation
	:public Interpolation<T>
{
	protected:
		SurfaceField <T> intfield;	//Not constant because can be reinterpolated

	public:
		CenterToFaceInterpolation():Interpolation<T>(){};
		CenterToFaceInterpolation(const _CC_Fv_Field <T> &fi) :Interpolation<T>(fi)
		{

			//Pending to Generate constructor
//			cout << "Grid Faces " <<this->field.GridPtr->Num_Faces()<<endl;
			SurfaceField <T> r(this->field.GridPtr->Num_Faces());


			//Loop throug faces
			for (int f = 0; f<this->field.GridPtr->Num_Faces(); f++)
			{
				_FvFace face = this->field.GridPtr->Face(f);
				T prom;
				//Scalar fp
				//cout <<"Face "<< f<< " "<< this->field[face.Cell(0)].outstr() << " "<< this->field[face.Cell(1)].outstr() <<endl;
				//cout << "Fp "<< face.Fp().outstr()<<endl;
				if (!this->field.GridPtr->Face(f).Boundaryface())
					prom = this->field[face.Cell(0)] * (1.0 - face.Fp()) + this->field[face.Cell(1)] * face.Fp();
				else
					prom = this->field[face.Cell(0)];

				r[f] = prom;
			}

			intfield = r;
		};

		const SurfaceField <T> & Interpolate();
		SurfaceField <T> & Interpolate(const _CC_Fv_Field<T> &fi);

		//Base classes
		SurfaceField <T> operator &(const SurfaceField <T> &right) const{};

};




};

#endif
