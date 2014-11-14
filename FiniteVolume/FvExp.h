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


template<class T>
_Surf_Fv_Field
< typename outerProduct<Vec3D, T>::type >
Gradf (const _CC_Fv_Field <T>&);

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


template<class T>
_CC_Fv_Field
< T >
Div (const _CC_Fv_Field <T>& field)
{

    _CC_Fv_Field < T > ret;




    return ret;
}

//ATENTION!!!
//THIS FUNCTION IS NOT LIKE OPENFOAM SINTAX, IS SIMPLY DIVErGENCe
template<class T>
_CC_Fv_Field
< T >
Div (const _Surf_Fv_Field <T>& phi)
{
    _CC_Fv_Field < T > ret;

//    const int numcells = phi.GridPtr->Num_Cells();
//
//    for (int f=0;f<phi.ConstGrid().Num_Faces();f++)
//    {
//        _FvFace face=phi.ConstGrid().Face(f);
// 		if (!face.Is_Null_Flux_Face())
//		{
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
//
//            cell[0]=face.Cell(0);
//            Scalar coeff_ap, coeff_an;
//            //TO MODIFY-> CHANGE INNER FIELD AND BOUNDARY FIELD
//            //Changed
//            //Check cell 0
//            //If inner prod > 0, then flux is going out face cell 0,
//            //then phi_face=phi_cell0
//            //TO MODIFY: CHANGE FOR (Min[-mf,0])
//            //THESE COEFFS REFERS TO CELL 0
//            //IF FACE IS NOT BOUNDARY, THEN COEFFS IN CELL 1 ARE INVERTED
//            double d=FluxField.Val(f).Val();
//            //cout << "f"<< f << " " <<"FluxField Val " <<FluxField.Val(f).Val() <<endl;
//            if(FluxField.Val(f)>0.)
//            {
//                coeff_ap= FluxField.Val(f);
//                coeff_an= 0.;
//            }
//            else
//            {
//                coeff_ap= 0.;
//                coeff_an= FluxField.Val(f);
//            }
//            //If face is internal, assign to cell 0 neighbour an coefficient
//            eqnsys.Eqn(cell[0]).Ap()+=coeff_ap;
//
//            if (!face.Boundaryface())
//            {
//                //Contribution of central coefficient, flux field if the flux is outwards from cell
//                //eqnsys.Eqn(cell[0]).Ap()+=coeff_ap;
//
//                cell[1]=face.Cell(1);
////                cout << "Global Cell 1: "<< cell[1]<<endl;
//                //Search global neighbour cell
//                int neigh=phi.ConstGrid().Cell(cell[0]).SearchNeighbour(cell[1]);
//                int neigh2=phi.ConstGrid().Cell(cell[1]).SearchNeighbour(cell[0]);
////                cout << "Cell "<<cell[0]<< "neighbour: "<<neigh<<endl;
// // TO MODIFY!!!!
// //This Way is too expensive, only for evaluation
//                if (neigh>=0 /*&& neigh <VolField.ConstGrid().Cell(cell[0]).Num_Neighbours()*/)
//                    //eqnsys.Eqn(cell[0]).An(neigh,coeff_an);
//                    eqnsys.Eqn(cell[0]).An(neigh)+=coeff_an;
//
//                //Cell 1 has opposite flux direction
//                eqnsys.Eqn(cell[1]).Ap()-=coeff_an;
//
//                if (neigh2>=0 /*&& neigh <VolField.ConstGrid().Cell(cell[1]).Num_Neighbours()*/)
//                    //eqnsys.Eqn(cell[1]).An(neigh2,coeff_ap);
//                    eqnsys.Eqn(cell[1]).An(neigh2)-=coeff_ap;
//            }
//		}//End if !NullFluxFace
//    }
////    ------------------
////    Loop through faces
////    ------------------
////    TO MODIFY, CHAMGE ORDER BETWEEN IF AND FOR
//    for (int p=0;p<phi.Grid().vBoundary().Num_Patches();p++)
//    {
//        for (int f=0;f<phi.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//        {
//            int idface=phi.Grid().vBoundary().vPatch(p).Id_Face(f);
//            _FvFace bface=phi.Grid().Face(idface);  //TO MODIFY idface or face pos??
//            //Boundary type
//            //Instead of if sentence it is convenient to use inheritance
//            if (phi.Boundaryfield()._PatchField(p).Type()==FIXEDVALUE)
//            {
//
//                if(FluxField.Val(idface)<0.)
//                    //If flux is inwards, source is positive (is RHS)
//                    //cout << "Boundary Field Value"<<phi.Boundaryfield()._PatchField(p).Val(f).Val()<<endl;
//                    eqnsys.Eqn(bface.Cell(0)).Source()-=phi.Boundaryfield()._PatchField(p).Val(f)*FluxField.Val(idface);
//            }
//            else if (phi.Boundaryfield()._PatchField(p).Type()==FIXEDGRADIENT)
//            {
//                //TO MODIFY
//                //source=VolField.Boundaryfield()._PatchField(p).Val(f)*fi;
//                //eqnsys.Eqn(face.Cell(0)).Source()+=source;
//            }
//        }
//
//    }


    return ret;
}

};//FvExp
};//FluxSol

#endif
