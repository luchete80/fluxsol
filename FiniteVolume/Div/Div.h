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
#ifndef DIV_H
#define DIV_H

#include "./FiniteVolume/FvImp.h"
#include "./FiniteVolume/FvExp.h"
#include "./Type/Products.h"
#include "./Type/Vec3d.h"
#include "./Type/Scalar.h"
#include "./Interpolation/CenterToFaceInterpolation.h"

#include "DivScheme.h"

#include "FieldOperations.h"
#include "GeometricField.h"

using namespace std;

namespace FluxSol{


//Esta funcion es para un simpe valor de campo
//Atencion colocarle el nombre a la funcion
//template<typename T>
//EqnSystem <T> FvImp::Div(_CC_Fv_Field <T> )
//{
//
//    //A continuacion se muestra una forma de calculo de divergencia
//    //Sum_f (Sf * (ro * U)f * fi(f) )
//    //En OpenFoam ro*U es fi
//
//
//}
//


//Div fi = Sum_f (Sf * fi_f)
//Here must be set an upwind scheme
template<typename T>
EqnSystem < typename innerProduct < Vec3D, T> ::type >
FvImp::Div(const _CC_Fv_Field <T> &VolField)
{
	    Eqn <T> eqn;            //Ecuacion para cada una de las cells
	                            //No hace falta construirla porque la devuelve el VolField
	    //EqnSystem <T> eqnsys(VolField.GridPtr->Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones
		//EqnSystem <T> < typename innerProduct < Vec3D, T> ::type > eqnsys(VolField.Grid());
		EqnSystem <typename innerProduct < Vec3D, T> ::type> eqnsys(VolField.ConstGrid());

		//Interpolate face fluxes and then upwind 
		CenterToFaceInterpolation <T> interp(VolField);
		//Flux, inner producto
		//Can be a vector, or a scalar
		//Sf is a vector
		SurfaceField <typename innerProduct < Vec3D, Vec3D> ::type> FluxField(VolField.Numberofvals());
		//_CC_Fv_Field <typename innerProduct < Vec3D, Vec3D> ::type> Prueba(VolField.Numberofvals(),0.);
		FluxField= VolField.GridPtr->Sf() & interp.Interpolate();


	    //A continuacion se muestra una forma de calculo de divergencia
	    //Sum_f (Sf * (ro * U)f * fi(f) )
	    //En OpenFoam ro*U es fi
	    //Esta forma de calculo es para cuando las partes de la cara no son iguales
	    //--------------
	    //RECORRO CELLS
	    const std::vector<Cell_CC>::iterator cellit;
		vector <Scalar> flux;	//Flujo en la celda y en las vecina
		const int numcells = VolField.GridPtr->Num_Cells();
		for (int cid=0; cid<numcells; cid++)
	    {
	        //const int &icell, std::vector <Scalar> flux
	        //Tengo que pasar a vector el campo de flujo
			const int nfaces = VolField.GridPtr->Cell(cid).Num_Faces();

			
			//Calculate Face Flux
	        //eqn=VolField.ToFaces(cid,flux); //Puedo hacer una funcion que no itere por elemento pero esta la memoria

	    }
	//    for (cellit=;icell<VolField.GridPtr->num_cells;icell++)
	//    {
	//
	//    }
	
	    return eqnsys;
}

//Transporte Div (fi,U)=Div (ro * (U.n),U vector)
//Aca el flujo ya tiene que estar calculado e interpolado en las caras
//template<typename T>
//EqnSystem <T> FvImp::Div(_Surf_Fv_Field<Scalar> fi,_CC_Fv_Field <T> VolField)
//{
//    Eqn <T> eqn;            //Ecuacion para cada una de las cells
//                            //No hace falta construirla porque la devuelve el VolField
//    EqnSystem <T> eqnsys(VolField.GridPtr->Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones
//
//    //A continuacion se muestra una forma de calculo de divergencia
//    //Sum_f (Sf * (ro * U)f * fi(f) )
//    //En OpenFoam ro*U es fi
//    //Esta forma de calculo es para cuando las partes de la cara no son iguales
//    //--------------
//    //RECORRO CELLS
//    std::vector<Cell_CC>::iterator cellit;
//    int cid=0;
//	vector <Scalar> flux;	//Flujo en la celda y en las vecinas
//    for (cellit=VolField.GridPtr->BeginCell();cellit!=VolField.GridPtr->EndCell();cellit++)
//    {
//        //const int &icell, std::vector <Scalar> flux
//        //Tengo que pasar a vector el campo de flujo
//        eqn=VolField.ToFaces(cid,flux); //Puedo hacer una funcion que no itere por elemento pero esta la memoria
//        cid++;
//    }
////    for (cellit=;icell<VolField.GridPtr->num_cells;icell++)
////    {
////
////    }
//
//    return eqnsys;
//}

//Flux Field may have been not related to volume field (such convection-diffusion test)

template <class T>
EqnSystem < typename innerProduct < Vec3D, T> ::type > 
FvImp::Div(_Surf_Fv_Field<Scalar> fi,_CC_Fv_Field <T> VolField)
{
	//Defined in Products.h
	EqnSystem < typename innerProduct < Vec3D, T> ::type > eqnsys(VolField.Grid());
	//eqnsys.Insertar(0,ec);
	//EqnSystem <Vec3D> eqnsys;

	//Interpolate face fluxes and then upwind 
	CenterToFaceInterpolation <T> interp(VolField);
	//Flux, inner producto
	//Can be a vector, or a scalar
	//Sf is a vector
	SurfaceField <typename innerProduct < Vec3D, Vec3D> ::type> FluxField(VolField.Numberofvals());
	//_CC_Fv_Field <typename innerProduct < Vec3D, Vec3D> ::type> Prueba(VolField.Numberofvals(),0.);
	FluxField = VolField.GridPtr->Sf() & interp.Interpolate();


return eqnsys;

}



}//Fin FluxSol

#endif
