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
#ifndef _SOLVER_H
#define _SOLVER_H

#include "./Type/Vec3d.h"
#include "./Field/Field.h"

#include "SistEcuac.h"

//Sparse libraries
#include "laspack.h"
//#include "./Nastran/Varios.h"
#include "Utils.h"
namespace FluxSol{
class _Solver{


    protected:
    Vec3D vtol;     //Tolerancia de velocidades
    double ptol;    //Tolerancia de presiones
    double maxiter; //maxximo numero de iteraciones
    double actualiter;  //iteracion actual


    public:
	_Solver();
    _Solver(const int  ){}; //Numero de iteraciones
    //ES VIRTUAL!!
    //Ojo que si no la defino, si no coloco nada y solo la declaro tengo problemas con la vtable en compilacion
    virtual void Resolver_Iteracion(){};
};

template <typename T>
void Solve (EqnSystem <T> &eq)
{
	QMatrix K;
	Vector U,R;
	int numberofcomp=pow(3.,eq.Dim());
	int totrows=numberofcomp*eq.Num_Eqn();
	Q_Constr(&K, "K", totrows, False, Rowws, Normal, True);
	V_Constr(&U,"U", totrows, Normal,True);
	V_Constr(&R,"R", totrows, Normal,True);

	//Look trhoug equations (rows)
	for (int e=0;e<eq.Num_Eqn();e++)	//Aca voy con las filas de a 2
	{
		vector <double> ap=eq.Eqn(e).Ap().Comp();
		int width=(eq.Eqn(e).Width()-1)*numberofcomp+1;

		int sparsecol=0;
		int row=e*numberofcomp;

		for (int dim=0;dim<numberofcomp;dim++)	Q_SetLen(&K,row+dim+1,width);

		vector <double> nullval;
		nullval.assign(numberofcomp,0.);

		//This id is relative to the cell, not the column
		int fisrt_nonzero_col=eq.Eqn(e).MinNeigbourId();
		int realcellid;

		//Look trough entire width for neighbours id
		for (int width_cells=0;width_cells<eq.Eqn(e).Width();width_cells++)
		{
			realcellid=width_cells+fisrt_nonzero_col;

			vector <double> col;
			int columnid;
			//Found central
			bool foundcell=false;
			int localneighbourfound=SearchVal(realcellid,eq.Eqn(e).NeighboursIds());
			if (eq.Eqn(e).Id()==realcellid)
			{col=ap;columnid=row;foundcell=true;}//row is equal to
			//Neighbours ids are not neccesarily ordered, then must search for cellid in all neighbours
			//else if (eq.Eqn(e).Neighbour(neighb)==realcellid) //Found an
			else if(localneighbourfound>-1)
			{col=eq.Eqn(e).An(localneighbourfound).Comp();columnid=numberofcomp*realcellid;foundcell=true;}
			else //column index is not a neighbour neither central cell
			{columnid=numberofcomp*realcellid;}

			//Write Matrix
			if (foundcell)
			{
				for (int dim=0;dim<numberofcomp;dim++)
				{
					//Rows and columns start at 1 but sparse index start at 1
					//Look throug neighbours
					Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells+dim,columnid+dim+1,col[dim]);
				}

			}
			else
			{
				for (int dim=0;dim<numberofcomp;dim++)
					Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells+dim,columnid+dim+1,0.0);

			}


		}//En of width

		vector<double> val;
		vector<int> pos;
		for (int width_cells=0;width_cells<eq.Eqn(e).Width();width_cells++)
		{
			for (int dim=0;dim<numberofcomp;dim++)
			{
			val.push_back(K.El[row+dim+1][width_cells].Val);
			pos.push_back(K.El[row+dim+1][width_cells].Pos);

			}
		}

	}//End of cells

	V_SetAllCmp(&R,0.0);
	for (int e=0;e<eq.Num_Eqn();e++)
	{
		vector <double> source=eq.Eqn(e).Source().Comp();
		for (int dim=0;dim<numberofcomp;dim++)
			V_SetCmp(&R,e*numberofcomp+dim+1,source[dim]);

	}
	std::vector <double> Ui,Ri;
	Ui.assign(totrows,0.);
	Ri.assign(totrows,0.);
	V_SetAllCmp(&U,0.0);
	SetRTCAccuracy(1e-5);


	for (int i=0;i<10;i++)
	{
		V_SetAllCmp(&U,0.0);
		BiCGSTABIter (&K,&U,&R,1000,SSORPrecond,1.0);
		cout<<"It "<<i<<endl;
		for (int j=0;j<totrows;j++)
		{
			Ui[j]=U.Cmp[j+1];
			Ri[j]=R.Cmp[j+1];
		}

	}

	for (int e=0;e<eq.Num_Eqn();e++)
	{
		vector <double> r;
		for (int dim=0;dim<numberofcomp;dim++)
			r.push_back(U.Cmp[numberofcomp*e+dim+1]);
		eq.Eqn(e).X()=r;
	}
}

}

#endif
