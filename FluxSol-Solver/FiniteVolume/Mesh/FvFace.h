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
#ifndef _FV_FACE_H
#define _FV_FACE_H

//Posee la clase cara que tiene todas las variables que corresponden
#include "../../FiniteVolume/Mesh/CC_Cell.h"
#include "../../Mesh/Face.h"
#include "../../Type/Vec3d.h"
#include "../../Type/Scalar.h"
#include "../../Mesh/Vertex.h"
#include "../../Mesh/Node.h"
#include "../../Type/Operations.h"

namespace FluxSol{

class _FvFace:public _Face{

	Vec3D vec_pn;											//Vector entre baricentros
	Vec3D e_pn;											//Vector normalizado entre baricentros
	double dist_pn;										//Distancia entre baricentros, antes se llamaba l

	double dist_pncorr;                                 //Orthogonal correction.
                                                        //This is the dist_pn projection to normal direction
                                                        //Is vec_pn*


	std::vector <Vec3D>  vec_pf_LR;				//Distancia del baricentro del cell al de la cara
	std::vector <Vec3D>	 vec_normal_LR;		    //Vector normalizado y su opuesto, el primero apunta P-L y el otro hacia el vecino N-R

	std::vector <double> f_pn;                           //factor de interpolación a ambos lados
    std::vector <Cell_CC> *pcell;                       //Puntero a cells

    //Valores de Caras
    Vec3D ad,at;                                //Ad: Area en direccion pn
                                                //Af: Area en direccion normal //DEFINIDA EN LA CLASE BASE
                                                //At=Ad+At:Relaciona las anteriores
	// FUNCIONES //
    Scalar norm_ad,norm_at;
    Scalar fp,fn;                               //fp y su complemento

    //Gradientes
    std::vector <double> GradCoeff;             //Coeficientes de gradientes para cada cell
                                                //PUEDEN SER 2 LOS VECINOS SI ES UN GRADIENTE SIMPLE (PN-PP)/d O MUCHOS MAS
                                                //SI ES UN GRADIENTE NORMAL


	public:
		_FvFace(){};
		_FvFace(const int &nom,const vector<int> & cells, const vector<int> & verts);  //


		//Importante la relacion entre los nodos pn y los vertices, aca es donde entra la conectividad
        _FvFace(const int &Id, std::vector <int> &idverts, const std::vector<_Vertex> & verts,
                                 std::vector <Node> &nodospn, bool bf);  //Stencil de nodos p y n


		double & Dist_pn(){return dist_pn;}
		Vec3D & e_PN(){return e_pn;}
		Vec3D  &  Dist_pf_LR(const int &i){return this->vec_pf_LR[i];}
		Scalar & Norm_ad(){return norm_ad;}
		Vec3D & Ad(){return ad;}

		Scalar & Fp(){return fp;}

		//Gradientes
		void GradCoeffs_Central();              //Calculo los coeficientes que necesito para los gradientes de cara
		//~_FvFace(){};



};

//State que tiene cada una de las caras
class _State {


	};

}//Fin FluxSol

#endif
