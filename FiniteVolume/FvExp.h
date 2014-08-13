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

};//FvExp
};//FluxSol

#endif
