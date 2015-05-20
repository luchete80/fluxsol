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
GeomSurfaceField <T> Interpolate(const _CC_Fv_Field <T> &field)
    {

        //Pending to Generate constructor
        GeomSurfaceField <T> r(field.ConstGrid().Num_Faces());
        r.AssignGrid(&field.ConstGrid());

        //Loop throug faces
        Scalar fp[2];
        T prom;
        for (int f = 0; f<field.ConstGrid().Num_Faces(); f++)
        {
            //_FvFace face = field.ConstGrid().Face(f);
            //Scalar fp
            prom=0.;
            //cout <<"Face "<< f<< " cell 0 Field Value: "<< field[face.Cell(0)].outstr() << endl;
            //if (!field.ConstGrid().Face(f).Boundaryface())
            //    cout <<" "<< field[face.Cell(1)].outstr() <<endl;
            //cout << "Fp "<< face.Fp().outstr()<<endl;


            //MODIFICATED
            fp[0]=(1.0 - field.ConstGrid().Face(f).Fp());fp[1]=field.ConstGrid().Face(f).Fp();

            for (int fc=0;fc<field.ConstGrid().Face(f).NumCells();fc++)prom += field[field.ConstGrid().Face(f).Cell(fc)]*fp[fc];

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
    _CC_Fv_Field < T > ret (phi.Grid());

    int numcells = phi.Grid().Num_Cells();
//
    Scalar afdir[2];afdir[0]=1.;afdir[1]=-1.;
//    for (int c=0;c<numcells;c++)
//    {
//        //cout << "Cell "<<c<<endl;
//        for (int fi=0;fi<phi.Grid().Cell(c).Num_Faces();fi++)
//        {
//            //cout << "fi "<<fi<<endl;
//            int f=phi.Grid().Cell(c).Id_Face(fi);
//            //T res=phi.Val(f)*phi.Grid().CellFaceSign(c,fi);
//            //cout << "Face "<<f<<" Flux: "<<res.outstr()<<endl;
//            ret[c]+=phi.Val(f)*phi.Grid().CellFaceSign(c,fi);
//        }
//    }

    for (int f=0;f<phi.Grid().Num_Faces();f++)
    {
        for (int fc=0;fc<phi.Grid().Face(f).NumCells();fc++)
        {

            //Originally ret[c]+=phi.Val(f)*phi.Grid().CellFaceSign(c,fi);
            //int c=r.Grid().Face(f).Cell(fc);
            //grad was
            //r[c]+=r.Grid().Face(f).Af()*facefi[f]*afdir[fc];
            ret[phi.Grid().Face(f).Cell(fc)]+=phi.Val(f)*afdir[fc];
        }
    }


    return ret;
}

};//FvExp
};//FluxSol

#endif
