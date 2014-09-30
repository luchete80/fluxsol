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
	    for (int f=0;f<VolField.ConstGrid().Num_Faces();f++)
        {
            _FvFace face=VolField.ConstGrid().Face(f);

            //Eqn eq[2];
            int cell[2];


            //Check sign of inner product

            //eqn[0].An()face.Cell(0);
            //Take the field norm

            T coeff_ap=-1.;
            T coeff_an= 1.;
            const Scalar zero(0.);
            if (!face.Boundaryface())
            {
                for (int i=0;i<2;i++)cell[i]=face.Cell(i);
                //TO MODIFY-> CHANGE INNER FIELD AND BOUNDARY FIELD
                //Changed

                if(FluxField.Val(f).Norm()>zero)
                {
                    coeff_ap=-1.;
                    coeff_an= 1.;
                else
                {
                    coeff_ap= 1.;
                    coeff_an=-1.;
                }
                    //Check cell 0
                    //If inner prod > 0, then flux is going out face cell 0,
                    //then
                    eqnsys.Eqn(cell[0]).Ap(-1.);
                    //Search global neighbour cell
                    int neigh=VolField.ConstGrid().Cell(cell[0]).SearchNeighbour(cell[1]);
                    if (neigh>0)
                        eqnsys.Eqn(cell[1]).An(neigh,1.);
            }
            else             //Boundary face
                             //If any value is set
            {

            }

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

//TO SEE IN OPENFOAM HOW TO RELATE CONVECTION SCHEME WITH PLAIN DIVERGENCE

//Flux Field may have been not related to volume field (such convection-diffusion test)

template <class T>
EqnSystem < typename innerProduct < Vec3D, T> ::type >
FvImp::Div(_Surf_Fv_Field<Scalar> fi,_CC_Fv_Field <T> VolField)
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
	    for (int f=0;f<VolField.ConstGrid().Num_Faces();f++)
        {
            _FvFace face=VolField.ConstGrid().Face(f);

            Eqn eq[2];
            int cell[2];


            //Check sign of inner product

            //eqn[0].An()face.Cell(0);
            //Take the field norm

            T coeff_ap=-1.;
            T coeff_an= 1.;
            if (!face.Boundaryface())
            {
                for (int i=0;i<2;i++)cell[i]=face.Cell(i);
                //TO MODIFY-> CHANGE INNER FIELD AND BOUNDARY FIELD
                //Changed
                if(FluxField.Val(f).Norm()>0.)
                {
                    coeff_ap=-1.;
                    coeff_an= 1.;
                else
                {
                    coeff_ap= 1.;
                    coeff_an=-1.;
                }
                    //Check cell 0
                    //If inner prod > 0, then flux is going out face cell 0,
                    //then
                    eqnsys.Eqn(cell[0]).Ap(coeff_ap);
                    //Search global neighbour cell
                    int neigh=VolField.ConstGrid().Cell(cell[0]).SearchNeighbour(cell[1]);
                    if (neigh>0)
                        eqnsys.Eqn(cell[1]).An(neigh,coeff_an);
            }
            else             //Boundary face
                             //If any value is set
            {

            }

        }
	//    for (cellit=;icell<VolField.GridPtr->num_cells;icell++)
	//    {
	//
	//    }

//	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
//	{
//		for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//		{
//			int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
//			_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
//			//Boundary type
//			//Instead of if sentence it is convenient to use inheritance
//			if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
//			{
//			    //cout <<"source"<<endl;
//				ap=-face.Norm_ad()/fabs(face.Dist_pf_LR(0))*fi;
//				source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;
//				//cout <<"created" <<endl;
//				eqnsys.Eqn(face.Cell(0)).Ap()+=ap;
//				eqnsys.Eqn(face.Cell(0)).Source()+=source;
//			}
//			else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
//			{
//				source=VolField.Boundaryfield().PatchField(p).Val(f)*fi;
//				eqnsys.Eqn(face.Cell(0)).Source()+=source;
//			}
//		}
//
//	}
	    return eqnsys;
}



}//Fin FluxSol

#endif
