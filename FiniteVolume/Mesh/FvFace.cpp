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
#include "FvFace.h"

using namespace std;
using namespace FluxSol;

//Deriva el constructor de la clase base que ya inicia los vertices
//Veo que el idcells es como los nodos en las Fv_CC
//Si pongo el const adelante del vector no puedo llamar a los miembros con el corchete
_FvFace::_FvFace(const int &Id, std::vector<int> &idverts,const vector <_Vertex> &verts,
                                 vector <Node> & nodospn, bool bf):_Face(Id,verts)//El primer nodo es el p, el otro el n
{
    num_cells=0;
	int idnode[2];
	boundaryface=bf;
	//Es una celda interior
	for (int i=0;i<this->NumVerts();i++)
		this->vertex.push_back(idverts[i]);

	af=0.;
	//Af = 0.5 * Sum_triangles( (ri-1 - r1) x (ri - r1) )
	for (int nv=2;nv<verts.size();nv++)
		af+=0.5*( (verts[nv-1]-verts[0]) ^ (verts[nv]-verts[0]) );

	//If boundary face or boundary cell with dummy boundary treatment
	if (nodospn.size()==2)
	{
		if (af.dot(nodospn[1]-nodospn[0])>0.)	{idnode[0]=0;idnode[1]=1;}
		else									{idnode[0]=1;idnode[1]=0;}

		//According to normal orientation the nodes are oriented
		//Inicio la informacion de nodos
		vec_pn=nodospn[idnode[1]]-nodospn[idnode[0]];
		//cout << "Vec_pn" << vec_pn.outstr()<<endl;
		cout <<"Face vec_pn"<<vec_pn.outstr()<<endl;
		dist_pn=fabs(vec_pn);
		e_pn=vec_pn.VecNorm(); //CORRECTED, IS NOT NORMALIZE
		//cout << "Vec_pn norm" << vec_pn.outstr()<<endl;
		//Relacion con vertice central de la cara
		//Vector que va de P/N al baricentro de la cara
		vec_pf_LR.push_back(sm-nodospn[idnode[0]]);
		vec_pf_LR.push_back(sm-nodospn[idnode[1]]);

		this->cell.push_back(nodospn[idnode[0]].Id()); num_cells++;
		this->cell.push_back(nodospn[idnode[1]].Id()); num_cells++;


		//norm_ad=Af.Af/(Af.ePN)
		//Ad is ths component collineal to pn
		//af is the norm of ad vector
		norm_ad=af.Norm();
		ad=norm_ad*e_pn;


		at=af-ad;   norm_at=fabs(at);

		//Factores
		//Aca puedo hacer mas ameno el exponente
		cout <<"Face pf LR" <<vec_pf_LR[idnode[0]].outstr()<<endl;
		cout <<"Face e_pn" <<e_pn.outstr()<<endl;
        cout <<"Face vec_pn"<<vec_pn.outstr()<<endl;

        cout <<"vec_pn norm"<<vec_pn.Norm().outstr()<<endl;
        cout << "fp . epn" << (vec_pf_LR[idnode[0]].dot(e_pn))<<endl;
        //fp=Pf/Norm(PN)=Pf.ePN/(PN.PN)^0.5
		fp=vec_pf_LR[idnode[0]].dot(e_pn)/vec_pn.Norm();
		fn=1.0-fp;

	}
	else //celda del borde, tiene solo el nodo interior
	{
		//Calculo la distancia entre el punto y el face
		//Relacion con vertice central de la cara
		//Vector que va de P/N al baricentro de la cara
		vec_pf_LR.push_back(sm-nodospn[0]);
		this->cell.push_back(nodospn[0].Id()); num_cells++;

		//Case first order
		Vec3D e_pf_LR=vec_pf_LR[0]/fabs(vec_pf_LR[0]);

		norm_ad=(af.dot(af)/af.dot(e_pf_LR));
		ad=norm_ad*vec_pf_LR[0];

	}

}


