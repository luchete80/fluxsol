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
#ifndef _GRAD_EXP_H_
#define _GRAD_EXP_H_

#include "./SistEcuac/SistEcuac.h"
#include "./Type/Products.h"
#include "../../Type/Operations.h"
#include "../../Field/Field.h"
#include "./Type/Vec3d.h"
#include "./Interpolation/CenterToVertexInterpolation.h"
#include "FvExp.h"

#include <set>

using namespace std;
namespace FluxSol
{
    namespace FvExp
    {

    template<class T>
    _CC_Fv_Field
    < typename outerProduct<Vec3D, T>::type >
        Grad (const _CC_Fv_Field <T>& field);


     template<class T>
    _CC_Fv_Field
    < typename outerProduct<Vec3D, T>::type >
        GradV (const _CC_Fv_Field <T>& field);


    //Face gradient
    //Explicit form of FaceGrad
    //Like Laplacian, (N-P)dist + Non-Orthogonal
    //TO MODIFY!!!!
    //CHANGE BY SnGradf
    template<class T>
    _Surf_Fv_Field
    //< typename outerProduct<Vec3D, T>::type >
    //TILL NOW IS NOT PASSED BY REF
    < T >
        SnGrad (const _CC_Fv_Field <T> &VolField);



	// New function for non-Orthogonal correction
	template<class T>
	_CC_Fv_Field
	<typename outerProduct<Vec3D, T>::type >
	NonOrthGrad(const _CC_Fv_Field <T> & field);

    }//Fin de FvExp
}//Fin de namespace FluxSol

#endif
