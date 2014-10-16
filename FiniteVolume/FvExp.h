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
#ifndef FV_EXP_H
#define FV_EXP_H


//Incluyo a todos los archivos que tengan funciones explicitas
#include "../FiniteVolume/Grad/FvExpGrad.h"
#include "../Type/Operations.h"
#include "../Type/Products.h"

namespace FluxSol
{
namespace FvExp
{

//Gauss Gradient with non orthogonality correction
template<class T>
_CC_Fv_Field
< typename outerProduct<Vec3D, T>::type >
Grad (const _CC_Fv_Field <T>&);

//Like CenterToFaceInterpolation
template<typename T>
SurfaceField <T> Interpolate(const _CC_Fv_Field <T> &field)
    {

        //Pending to Generate constructor
        cout << "Grid Faces " <<field.GridPtr->Num_Faces()<<endl;
        SurfaceField <T> r(field.GridPtr->Num_Faces());


        //Loop throug faces
        for (int f = 0; f<field.GridPtr->Num_Faces(); f++)
        {
            _FvFace face = field.GridPtr->Face(f);
            T prom;
            //Scalar fp
            cout <<"Face "<< f<< " "<< field[face.Cell(0)].outstr() << " "<< field[face.Cell(1)].outstr() <<endl;
            cout << "Fp "<< face.Fp().outstr()<<endl;
            if (!field.GridPtr->Face(f).Boundaryface())
                prom = field[face.Cell(0)] * (1.0 - face.Fp()) + field[face.Cell(1)] * face.Fp();
            else
                prom = field[face.Cell(0)];

            r[f] = prom;
        }

        return r;
    };

};//FvExp
};//FluxSol

#endif
