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

#include <time.h>

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

//THIS IS THE ONE WHICH IS ACTUALLY WORKING
template <class T>
//EqnSystem < typename innerProduct < Vec3D, T> ::type >
//First arg is flux
EqnSystem < T >
FvImp::Div(GeomSurfaceField<Scalar> FluxField,_CC_Fv_Field <T> phi)
{
        clock_t ittime_begin, ittime_end;
    double ittime_spent;
    ittime_end = clock();

// TO MODIFY!! CONVECTION SCHEME
//OpenFoam Style
//00052     return fv::convectionScheme<Type>::New
//00053     (
//00054         vf.mesh(),
//00055         flux,
//00056         vf.mesh().divScheme(name)
//00057     )().fvmDiv(flux, vf);

    //EqnSystem <T> eqnsys(VolField.GridPtr->Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones
    //EqnSystem <T> < typename innerProduct < Vec3D, T> ::type > eqnsys(VolField.Grid());
    EqnSystem < T > eqnsys(phi.Grid());

    //cout << "calculating div"<<endl;
    //Interpolate face fluxes and then upwind

    //A continuacion se muestra una forma de calculo de divergencia
    //Sum_f (Sf * (ro * U)f * fi(f) )
    //En OpenFoam ro*U es fi
    //Esta forma de calculo es para cuando las partes de la cara no son iguales
    //--------------
    //RECORRO CELLS



    const int numcells = phi.Grid().Num_Cells();


    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "div field gen "<<ittime_spent <<endl;


    ittime_end = clock();
    Scalar coeff_ap, coeff_an;
    int cell[2];
    int neigh,neigh2;

    std::set<int>::iterator it;
    std::set<int> *nff=&phi.IntNetFluxFaces();
	for (it=nff->begin(); it!=nff->end(); ++it)
    {
        int f=*it;

        cell[0]=phi.ConstGrid().Face(*it).Cell(0);

            //TO MODIFY-> CHANGE INNER FIELD AND BOUNDARY FIELD
            //Changed
            //Check cell 0
            //If inner prod > 0, then flux is going out face cell 0,
            //then phi_face=phi_cell0
            //TO MODIFY: CHANGE FOR (Min[-mf,0])
            //THESE COEFFS REFERS TO CELL 0
            //IF FACE IS NOT BOUNDARY, THEN COEFFS IN CELL 1 ARE INVERTED
            //cout << "Obtaining Flux Value"<<endl;

            //cout << "Value "<< endl;
            //cout << "f"<< f << " " <<"FluxField Val " <<FluxField.Val(f).Val() <<endl;
            if(FluxField.Val(*it)>0.)
            {
                coeff_ap= FluxField.Val(*it);
                coeff_an= 0.;
            }
            else
            {
                coeff_ap= 0.;
                coeff_an= FluxField.Val(*it);
            }
            //cout << "coeff ap" << coeff_ap.Val()<<endl;
            //cout << "coeff an" << coeff_an.Val()<<endl;
           // cout << "Creating Eqn"<<endl;
            //If face is internal, assign to cell 0 neighbour an coefficient
            eqnsys.Eqn(cell[0]).Ap()+=coeff_ap;


            cell[1]=phi.ConstGrid().Face(*it).Cell(1);
            //cout << "Global Cell 1: "<< cell[1]<<endl;
            //Search global neighbour cell
            neigh=phi.ConstGrid().Cell(cell[0]).SearchNeighbour(cell[1]);
            neigh2=phi.ConstGrid().Cell(cell[1]).SearchNeighbour(cell[0]);
                //cout << "Cell "<<cell[0]<< "neighbour: "<<neigh<<endl;
 // TO MODIFY!!!!
 //This Way is too expensive, only for evaluation
            //if (neigh>=0 /*&& neigh <VolField.ConstGrid().Cell(cell[0]).Num_Neighbours()*/)
                //cout << "Cell 0 Neigh (Nrigh 1)" <<neigh<<endl;
                //eqnsys.Eqn(cell[0]).An(neigh,coeff_an);
                eqnsys.Eqn(cell[0]).An(neigh)+=coeff_an;

            //Cell 1 has opposite flux direction
            eqnsys.Eqn(cell[1]).Ap()-=coeff_an;

            //if (neigh2>=0 /*&& neigh <VolField.ConstGrid().Cell(cell[1]).Num_Neighbours()*/)
                //eqnsys.Eqn(cell[1]).An(neigh2,coeff_ap);
                //cout << "Cell 1 Neigh (Nrigh 2)" <<neigh2<<endl;
                eqnsys.Eqn(cell[1]).An(neigh2)-=coeff_ap;


    }//End Faces Loop
//    ------------------
//    Loop through faces
//    ------------------
//    TO MODIFY, CHAMGE ORDER BETWEEN IF AND FOR
    //cout <<"boundary"<<endl;

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "divergence interior faces loop "<<ittime_spent <<endl;
     ittime_end = clock();

    for (int p=0;p<phi.Grid().vBoundary().Num_Patches();p++)
    {
        //for (int f=0;f<phi.Grid().vBoundary().vPatch(p).Num_Faces();f++)
        //{
            //int idface=phi.Grid().vBoundary().vPatch(p).Id_Face(f);
            //_FvFace bface=phi.Grid().Face(idface);  //TO MODIFY idface or face pos??
            //if (!bface.Is_Null_Flux_Face())
            //{
                //Boundary type�
                //Instead of if sentence it is convenient to use inheritance
                if (phi.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
                {
                    for (int f=0;f<phi.Grid().vBoundary().vPatch(p).Num_Faces();f++)
                    {
                        int idface=phi.Grid().vBoundary().vPatch(p).Id_Face(f);
                        //_FvFace bface=phi.Grid().Face(idface);  //TO MODIFY idface or face pos??

                        //if(FluxField.Val(idface)<0.)
                        //If flux is inwards, source is positive (is RHS)
                        //cout << "Boundary Field Value"<<phi.Boundaryfield()._PatchField(p).Val(f).Val()<<endl;
                        //cout << "id_face" << idface<<endl;
                        //cout << "Value "<< phi.Boundaryfield().PatchField(p).Val(f).outstr()<<endl;

                        eqnsys.Eqn(phi.Grid().Face(idface).Cell(0)).Source()-=phi.Boundaryfield().PatchField(p).Val(f)*FluxField.Val(idface);
                    }
                }
                else if (phi.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
                {
                    //TO MODIFY
                    //source=VolField.Boundaryfield()._PatchField(p).Val(f)*fi;
                    //eqnsys.Eqn(face.Cell(0)).Source()+=source;
                }


    }
    //cout << "Returning laplacian"<<endl;
    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "div boundary faces loop "<<ittime_spent <<endl;


    //cout << "returning div"<<endl;
    return eqnsys;
}

template <class T>
//EqnSystem < typename innerProduct < Vec3D, T> ::type >
//First arg is flux
//TO MODIFY, CALL Convection Scheme
EqnSystem < T >
FvImp::Div_CDS(GeomSurfaceField<Scalar> FluxField,_CC_Fv_Field <T> phi)
{

    EqnSystem < T > eqnsys(phi.Grid());

    //cout << "calculating div"<<endl;
    //Interpolate face fluxes and then upwind

    //Flux, inner producto
    //Can be a vector, or a scalar
    //Sf is a vector
//    cout << "VolField number of vals"<<VolField.Numberofvals()<<endl;
    //TO MODIFY, CORRECT NUMBER OF VALS
//    SurfaceField <typename innerProduct < Vec3D, Vec3D> ::type> FluxField(VolField.ConstGrid().Num_Faces());
    //_CC_Fv_Field <typename innerProduct < Vec3D, Vec3D> ::type> Prueba(VolField.Numberofvals(),0.);
//    FluxField= VolField.Grid().Sf() & interp.Interpolate();

//    cout << "Sf values "<<endl;

    //for (int f=0;f<VolField.GridPtr->Num_Faces();f++)
    //    cout << "Face "<< f<< " " <<VolField.Grid().Sf().Val(f).outstr() <<endl;

//    cout << "Interp values "<<endl;
   // for (int f=0;f<VolField.GridPtr->Num_Faces();f++)
    //   cout << "Face "<< f<< " " <<interp.Interpolate().Val(f).outstr() <<endl;
    //A continuacion se muestra una forma de calculo de divergencia
    //Sum_f (Sf * (ro * U)f * fi(f) )
    //En OpenFoam ro*U es fi
    //Esta forma de calculo es para cuando las partes de la cara no son iguales
    //--------------
    //RECORRO CELLS
    const int numcells = phi.Grid().Num_Cells();

    for (int f=0;f<phi.ConstGrid().Num_Faces();f++)
    {
        //cout << "Face: "<<f <<endl;
        _FvFace face=phi.ConstGrid().Face(f);
 		if (!face.Is_Null_Flux_Face())
		{
            //cout << "No null flux"<<endl;
            //Eqn eq[2];
            int cell[2];


            //Check sign of inner product

            //eqn[0].An()face.Cell(0);
            //Take the field norm


            cell[0]=face.Cell(0);
            Scalar coeff_ap, coeff_an;
            //TO MODIFY-> CHANGE INNER FIELD AND BOUNDARY FIELD
            //Changed
            //Check cell 0
            //If inner prod > 0, then flux is going out face cell 0,
            //then phi_face=phi_cell0
            //TO MODIFY: CHANGE FOR (Min[-mf,0])
            //THESE COEFFS REFERS TO CELL 0
            //IF FACE IS NOT BOUNDARY, THEN COEFFS IN CELL 1 ARE INVERTED
            //cout << "Obtaining Flux Value"<<endl;
            double d=FluxField.Val(f).Val();
            //cout << "Value "<< endl;
            //cout << "f"<< f << " " <<"FluxField Val " <<FluxField.Val(f).Val() <<endl;
                coeff_ap= FluxField.Val(f);
                coeff_an= 0.;

            //cout << "coeff ap" << coeff_ap.Val()<<endl;
            //cout << "coeff an" << coeff_an.Val()<<endl;
           // cout << "Creating Eqn"<<endl;
            //If face is internal, assign to cell 0 neighbour an coefficient
            eqnsys.Eqn(cell[0]).Ap()+=coeff_ap;

            if (!face.Boundaryface())
            {
                //Contribution of central coefficient, flux field if the flux is outwards from cell
                //eqnsys.Eqn(cell[0]).Ap()+=coeff_ap;

                cell[1]=face.Cell(1);
                //cout << "Global Cell 1: "<< cell[1]<<endl;
                //Search global neighbour cell
                int neigh=phi.ConstGrid().Cell(cell[0]).SearchNeighbour(cell[1]);
                int neigh2=phi.ConstGrid().Cell(cell[1]).SearchNeighbour(cell[0]);
                //cout << "Cell "<<cell[0]<< "neighbour: "<<neigh<<endl;
 // TO MODIFY!!!!
 //This Way is too expensive, only for evaluation
                if (neigh>=0 /*&& neigh <VolField.ConstGrid().Cell(cell[0]).Num_Neighbours()*/)
                    cout << "Cell 0 Neigh (Nrigh 1)" <<neigh<<endl;
                    //eqnsys.Eqn(cell[0]).An(neigh,coeff_an);
                    eqnsys.Eqn(cell[0]).An(neigh)+=coeff_ap;

                //Cell 1 has opposite flux direction
                eqnsys.Eqn(cell[1]).Ap()-=coeff_ap;

                if (neigh2>=0 /*&& neigh <VolField.ConstGrid().Cell(cell[1]).Num_Neighbours()*/)
                    //eqnsys.Eqn(cell[1]).An(neigh2,coeff_ap);
                    cout << "Cell 1 Neigh (Nrigh 2)" <<neigh2<<endl;
                    eqnsys.Eqn(cell[1]).An(neigh2)-=coeff_ap;
            }
		}//End if !NullFluxFace
    }
//    ------------------
//    Loop through faces
//    ------------------
//    TO MODIFY, CHAMGE ORDER BETWEEN IF AND FOR
    //cout <<"boundary"<<endl;
    for (int p=0;p<phi.Grid().vBoundary().Num_Patches();p++)
    {
        for (int f=0;f<phi.Grid().vBoundary().vPatch(p).Num_Faces();f++)
        {
            int idface=phi.Grid().vBoundary().vPatch(p).Id_Face(f);
            _FvFace bface=phi.Grid().Face(idface);  //TO MODIFY idface or face pos??

            if (!bface.Is_Null_Flux_Face())
            {
                //Boundary type
                //Instead of if sentence it is convenient to use inheritance
                if (phi.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
                {

                    //if(FluxField.Val(idface)<0.)
                        //If flux is inwards, source is positive (is RHS)
                        //cout << "Boundary Field Value"<<phi.Boundaryfield()._PatchField(p).Val(f).Val()<<endl;
                        //cout << "id_face" << idface<<endl;
                        //cout << "Value "<< phi.Boundaryfield().PatchField(p).Val(f).outstr()<<endl;

                    eqnsys.Eqn(bface.Cell(0)).Source()-=phi.Boundaryfield().PatchField(p).Val(f)*FluxField.Val(idface);
                }
                else if (phi.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
                {
                    //TO MODIFY
                    //source=VolField.Boundaryfield()._PatchField(p).Val(f)*fi;
                    //eqnsys.Eqn(face.Cell(0)).Source()+=source;
                }
            }
        }

    }
    //cout << "returning div"<<endl;
    return eqnsys;
}

//Div fi = Sum_f (Sf * fi_f)
//Here must be set an upwind scheme
//TO MODIFY, MUST CALL UPWIND SCHEME
template<typename T>
EqnSystem < typename innerProduct < Vec3D, T> ::type >
FvImp::Div(const _CC_Fv_Field <T> &VolField)
{
	    EqnSystem <T> eqnsys;            //Ecuacion para cada una de las cells
//	                            //No hace falta construirla porque la devuelve el VolField
//	    //EqnSystem <T> eqnsys(VolField.GridPtr->Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones
//		//EqnSystem <T> < typename innerProduct < Vec3D, T> ::type > eqnsys(VolField.Grid());
//		EqnSystem <typename innerProduct < Vec3D, T> ::type> eqnsys(VolField.ConstGrid());
//
//
//
//		//Interpolate face fluxes and then upwind
//		CenterToFaceInterpolation <T> interp(VolField);
//		//Flux, inner producto
//		//Can be a vector, or a scalar
//		//Sf is a vector
//		SurfaceField <typename innerProduct < Vec3D, Vec3D> ::type> FluxField(VolField.Numberofvals());
//		//_CC_Fv_Field <typename innerProduct < Vec3D, Vec3D> ::type> Prueba(VolField.Numberofvals(),0.);
//		FluxField= VolField.GridPtr->Sf() & interp.Interpolate();
//
//
//	    //A continuacion se muestra una forma de calculo de divergencia
//	    //Sum_f (Sf * (ro * U)f * fi(f) )
//	    //En OpenFoam ro*U es fi
//	    //Esta forma de calculo es para cuando las partes de la cara no son iguales
//	    //--------------
//	    //RECORRO CELLS
//	    const std::vector<Cell_CC>::iterator cellit;
//
//		const int numcells = VolField.GridPtr->Num_Cells();
//		for (int cid=0; cid<numcells; cid++)
//	    {
//	        //const int &icell, std::vector <Scalar> flux
//	        //Tengo que pasar a vector el campo de flujo
//			const int nfaces = VolField.GridPtr->Cell(cid).Num_Faces();
//
//			//Calculate Face Flux
//	        //eqn=VolField.ToFaces(cid,flux); //Puedo hacer una funcion que no itere por elemento pero esta la memoria
//
//	    }
//	    for (int f=0;f<VolField.ConstGrid().Num_Faces();f++)
//        {
//            _FvFace face=VolField.ConstGrid().Face(f);
//
//            //Eqn eq[2];
//            int cell[2];
//
//
//            //Check sign of inner product
//
//            //eqn[0].An()face.Cell(0);
//            //Take the field norm
//
//            T coeff_ap=-1.;
//            T coeff_an= 1.;
//
//            if (!face.Boundaryface())
//            {
//                for (int i=0;i<2;i++)cell[i]=face.Cell(i);
//                //TO MODIFY-> CHANGE INNER FIELD AND BOUNDARY FIELD
//                //Changed
//
//                if(FluxField.Val(f).Norm()>0.)
//                {
//                    coeff_ap=-1.;
//                    coeff_an= 1.;
//                }
//                else
//                {
//                    coeff_ap= 1.;
//                    coeff_an=-1.;
//                }
//                    //Check cell 0
//                    //If inner prod > 0, then flux is going out face cell 0,
//                    //then
//                    eqnsys.Eqn(cell[0]).Ap(-1.);
//                    //Search global neighbour cell
//                    int neigh=VolField.ConstGrid().Cell(cell[0]).SearchNeighbour(cell[1]);
//                    if (neigh>0)
//                        eqnsys.Eqn(cell[1]).An(neigh,1.);
//            }
//            else             //Boundary face
//                             //If any value is set
//            {
//
//            }
//
//        }
//	//    for (cellit=;icell<VolField.GridPtr->num_cells;icell++)
//	//    {
//	//
//	//    }

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
//

}//Fin FluxSol

#endif
