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
#include "../../Interpolation/CenterToVertexInterpolation.h"

namespace FluxSol
{
    namespace FvExp
    {
    //////////////////////
    // FUNCIONES DE GRADIENTE EXPLICITO
    ///////////////////////////////////

    //
    //Para que sigue OpenFoam con los tipos en la template??
    //(const GeometricField <Type, fvsPatchField, surfaceMesh>&)
    template<class T>
    _CC_Fv_Field
    < typename outerProduct<Vec3D, T>::type >
	Grad (const _CC_Fv_Field <T>& field)
	{

		//Field to return
		_CC_Fv_Field <T> fieldnc=field;
		 _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(fieldnc.Grid());
		//Paso el flujo a las caras, a un campo de faces

		//Tengo que hacer el prod "externo" entre un T a izquierda y un vector (rank=1) a derecha

		 //Like the open Foam functions
		 //_Surf_Fv_Field < typename outerProduct<Vec3D, T>::type > facefield=field.FaceInterpolate();
        SurfaceField <T> facefi=Interpolate(field);

		 bool end = false;
		 //Begin Main Loop
		 //facefi and r (which is the corrected gauss gradient of fieldnc) are changing
		 _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > rant;
		 while (!end)
		 {
			 r=0.;
			 //Loop through cells to calculate Gauss Gradient
			 int c;
			 for (c=0,r.Grid().cellit=r.Grid().BeginCell();r.Grid().cellit!=r.Grid().EndCell();r.Grid().cellit++,c++)
			 {
				//External product between a T field and a vector
				//Grad Cell Center = 1/Vp * Sum_faces(external prod(field fi * FaceArea))
				//Loop trough cell faces
				for (int cellface=0;cellface<r.Grid().cellit->Num_Faces();cellface++)
				{
					int f = r.Grid().cellit->Id_Face(cellface);
					//This is fi_f Outer Af
					r[c]+=(facefi[f]*r.Grid().Face(f).Af());
				}
				//Divide by cell volume
				r[c]=r[c]/r.Grid().cellit->Vp();

			 }

			 //Average of fi and fi (fio) gradient (gradfio)
			 //Look throug faces to obtain the facefield
			 //fi_f = fio + gradfio & fo-f//Here is inner product between different types
			 //With fo-f = Pf - Pfo = Pf - (Pf&ePN)ePN is the projection
			 //Dist_pf_LR

			 for (int f=0;f<r.Grid().Num_Faces();f++)
			 {
				 //f-fo is  unique for each face, but it can be calculated either with P or N cells
				 _FvFace face = r.Grid().Face(f);
				 Vec3D fof=face.Dist_pf_LR(0)-(face.Dist_pf_LR(0)&face.e_PN())*face.e_PN();

				//Variable and gradient averages
				 T fifo = face.Fp()*fieldnc[face.Cell(0)]+(1.0-face.Fp())*fieldnc[face.Cell(1)];
				typename outerProduct<Vec3D, T>::type grad_fio=(r[face.Cell(0)]*(1-face.Fp())+r[face.Cell(1)]*face.Fp());
				//typename outerProduct<Vec3D, T>::type grad_fio=r[face.Cell(0)];

				//facefi [f] = (fifo+(grad_fio&fof));
				facefi [f] = fifo;
				T s1,s2;
				s1=s2;

			 }


			 //Compare r against rant
			 //With MaxDiff Field template function
			 Scalar maxdif=MaxDiff(r,rant);
			 rant = r;

		 }//While End



		 return r;

	}


    }//Fin de FvExp
}//Fin de namespace FluxSol

#endif
