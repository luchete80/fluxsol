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
#ifndef _CENTERTOVERTEXINTERPOLATION_DEF_H_
#define _CENTERTOVERTEXINTERPOLATION_DEF_H

#include "ap.h"
#include "interpolation.h" //ALGLIB
#include <math.h>
#include "CenterToVertexInterpolation.h"
#include "Field.h"



using namespace FluxSol;
//using namespace alglib_impl;
using namespace alglib;	//real_XX_array
using namespace std;


template <typename T>
Vertex_Fv_Field<T>
CenterToVertexInterpolation<T>::Interpolate(_CC_Fv_Field<T> &field)
{
	Vertex_Fv_Field<T> vfield(field.Grid());
	//Depending on Field dimension, rbf model varies
	rbfreport rep;

	//Interpolation result
	double vscalar;

	int numberofcomp = (int)pow(3.0,pTraits<T>::rank);
	//For each vertex commonvertcells details each vertex neighbour cells
	vector<vector<int> > commonvertcells;
	vector<int> vi;
	commonvertcells.assign(this->grid.Num_Verts(),vi);
	for (int c=0;c<this->grid.Num_Cells();c++)
	{
		for (int v=0; v<this->grid.Cell(c).Num_Vertex();v++)
		{
			int globalv=this->grid.Cell(c).Vert(v);
			commonvertcells[globalv].push_back(c);
		}
	}

	//Look trough vertices
	for (int v=0;v<this->grid.Num_Verts();v++)
	{
		rbfmodel model;
		alglib_impl::ae_state state;
		//Dimensionality of space (always 3) and field (1 or 3)
		rbfcreate(3, numberofcomp, model);
		//real_2d_array xy="[[-2,0,1],[-1,0,0],[0,0,1],[+1,0,-1],[+2,0,1],[+3,0,2]]";
		//Columns are space components, rows are elements of array domain
		//
		real_2d_array xy;
		//Simplest Case: only vertex common cells are considered for weighting
		xy.setlength(commonvertcells[v].size(), 3 + numberofcomp);


		for (int f=0;f<commonvertcells[v].size();f++)
		{
			int globalcell = commonvertcells[v][f];
			vector<double> cellfieldval = field.Val(globalcell).Comp();	//Neighbourfield val

			//Coordinates values
			for (int c=0 ;c<3;c++)
				xy[f][  c]=this->grid.Node_(globalcell).comp[c];	//Assuming node id is equal to cell id

			//Field Values
			for (int c=0;c<numberofcomp;c++)
				xy[f][3+c]=cellfieldval[c];


		}
		//Field values

		rbfsetpoints(model, xy);


		rbfsetalgoqnn(model);
		//rbfsetalgomultilayer(model, 5.0, 1, 1.0e-3);
		rbfbuildmodel(model, rep);

		//

		real_1d_array vertcoord,vertfieldval;
		vertcoord.setlength(3);
		for (int c=0;c<3;c++) vertcoord[c]=this->grid.Vertex(v).Comp()[c];

		vertfieldval.setlength(numberofcomp);
		//Calculate interp value
		rbfcalc(model, vertcoord, vertfieldval);

		//Passing vertfieldval from real_1d_array to T
		T val;

		vector<double> vecval;
		for (int c=0;c<numberofcomp;c++)
			vecval.push_back(vertfieldval[c]);
		val=vecval;

		vfield.Val(v,val);

	}//for grid vertex v

	return vfield;
}

//FUNCTION NOT IN USE BECAUSE IS A NEW CLASS
template<class T>
_Surf_Fv_Field <T>
CenterToVertexInterpolation<T>::FaceInterpolate(const _CC_Fv_Field <T> &fi)
{
	_CC_Fv_Field <T> field=fi;
	//Pending to Generate constructor
	_Surf_Fv_Field <T> r(field.Grid().Num_Faces());


	//Loop throug faces
	for (int f=0; f<field.Grid().Num_Faces();f++)
	{
		_FvFace &face = field.Grid().Face(f);
		T prom;
		//Scalar fp
		if(!field.Grid().Face(f).Boundaryface())
		prom = (field[face.Cell(0)]* (1.0-face.Fp())  + field[face.Cell(1)] * face.Fp())*0.5;
		else
			prom = field.Val(field.Grid().Face(f).Cell(0));

		r[f]=prom;
	}

	return r;
}



#endif
