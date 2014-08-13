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
#ifndef FV_IMP_H
#define FV_IMP_H

#include "./Type/Products.h"
#include "./SistEcuac/SistEcuac.h"
#include "./Field/Field.h"
#include "./Field/FvField.h"
#include "./Field/GeometricField.h"

//Este archivo tiene la definicion de la namespace
//Las definiciones de ada una de las funciones las puedo hacer aparte
//Definicion de calculo de volumenes dfinitos implicito
//FUNCIONES QUE DEVUELVEN UN SISTEMA DE ECUACIONES
namespace FluxSol {
namespace FvImp {

////////////////
// GRADIENTE  //
////////////////


////// DIVERGENCIA ////////
// Ejemplo de divergencia es Div (rho * U), el termino convectivo
//Toma un campo centrado en el cuerpo

//RECORDAR QUE EL GRADINTE, COMO EN OPENFOAM, NO ES IMPLICITO, SOLO ES EXPLICITO
// Ejemplo de divergencia es Div (rho * U), el termino convectivo
template <typename T>
EqnSystem < typename innerProduct<Vec3D, T>::type > //typename is written because it can not be compiled in gcc
Div(_Surf_Fv_Field<Scalar> fi,_CC_Fv_Field <T> VolField);


//Operadores laplacianos

template<typename T>
EqnSystem <T> Laplacian(Scalar fi,_CC_Fv_Field <T> &VolField);

template<typename T>
EqnSystem <T> Laplacian(_Surf_Fv_Field<Scalar> fi,_CC_Fv_Field <T> &VolField);

//-- DIVERGENCE
template<typename T>
EqnSystem <T> Div(Scalar fi, _CC_Fv_Field <T> &VolField);

template<typename T>
EqnSystem < typename innerProduct < Vec3D, T> ::type >
Div(const _CC_Fv_Field <T> &VolField);

};//Fin de FvImp

}//Fin de FluxSol


#endif
