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
#ifndef _OPERATIONS_H
#define _OPERATIONS_H

#include "Scalar.h"
#include "Vec3d.h"
#include "Matrix.h"
//#include "../SistEcuac/SistEcuac.h"

// Ac� se definen operadores que involucran diferentes clases
// SE MOFDIFICAN RESPECTO
namespace FluxSol{

//Outer Product
	//template explicit
//Outer Prod between Scalar and Vector
//both operands MUST NOT BE CONST to avoid following: scalar * const = vector

Vec3D& operator/(const Vec3D &left, const Scalar &right);


template <typename T>
Scalar Norm (T);
//Operations between different types




///////////////////////
// OPERADORES VECTORIALES //
////////////////////////////
//Producto externo
//inline Matrix& OuterProd(const Vec3D &v1, const Vec3D &v2)
//{
//    Vec3D r;
//
//    r=v1.x();
//
//    return r;
//}



///////////////////////////////////////////////////
// OPERADORES CON SISTEMAS DE ECUACIONES Y TYPES //
///////////////////////////////////////////////////

//Aca es como el outer product est�ndar, no se sab
//Aca no hay template porque no conviene, hay que ver cu�ndo incluyo a una matriz ac�
//La cantidad de ecuaciones
//make inline!!
// IF THESE FUNCTIONS ARE DONE MUST BE LOCATED IN ANOTHER PLACE BECAUSE ARE
//CROSS CALLING BETWEEN EQN AND MESH
//inline Eqn <Vec3D>& OuterProd(const Eqn <Vec3D> &eqn ,  const Vec3D &vec)
//{
//    //u x v=[u1 u2 u3][v1 v2 v3]=[u1v1 u1v2 u1v3],[u2v1..]
//    //la multiplica por el componente en esa diension
//   // Eqn <Vec3D> eqn;//El valor que devuelve
//
//}
////Es como el outer product
//inline Eqn <Vec3D>& operator^(const Eqn <Vec3D> &eqn ,  const Vec3D &vec)
//{
//
//}



///////////////////////////////////////////////////
// OPERACIONES EXPLICITAS ENTRE TIPOS DISTINTOS ///
///////////////////////////////////////////////////

//PRODUCTO EXTERNO

//Es como el outer product
//es simplemente el producto por un escalar
//inline Matrix& operator *(const Vec3D &left,  const Vec3D &right)
//{
//	Matrix res;
//
//}


};//Fin de namespace FluxSol


#endif
