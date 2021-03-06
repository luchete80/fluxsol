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
#ifndef _SOLVER_H
#define _SOLVER_H

#include "./Type/Vec3d.h"
#include "./Field/Field.h"

#include "SistEcuac.h"

//Sparse libraries
//#include "laspack.h"
//#include "./Nastran/Varios.h"
#include "Utils.h"

#include <time.h>


namespace FluxSol{

template <typename number>
class Solver{


    protected:
    Vec3D vtol;     //Tolerancia de velocidades
    double ptol;    //Tolerancia de presiones
    double maxiter; //maxximo numero de iteraciones
    double actualiter;  //iteracion actual


	number rtol,abstol;
	const int matdim;		//CONST?
	int maxits;


    public:

    //ES VIRTUAL!!
    //Ojo que si no la defino, si no coloco nada y solo la declaro tengo problemas con la vtable en compilacion
    virtual void Resolver_Iteracion(){};

	Solver<number>():matdim(0)
	{
	    maxiter=100;
        ptol=1.0e-3;
        vtol=1.e-03;    //Todos los valores iguales
	}
	Solver<number>(const int &d):
	matdim(d)
	{}


};

	template <typename T>
    void Solve(EqnSystem <T> &);


// //TO MODIFY: CALL CONSTRUCTOR FROM CLAS ACORDING TO SCHEME
// template <typename T>
// void Solve (EqnSystem <T> &eq)
// {

    // //TO MODIFY, DELETE
    // //ofstream fitlog;
	// //fitlog.open("Iteration-Log.txt");

    // clock_t ittime_begin, ittime_end, temp;
    // double ittime_spent;

    // ittime_begin = clock();


	// QMatrix K;
	// Vector U,R;
	// int numberofcomp=pow(3.,eq.Dim());
	// int totrows=numberofcomp*eq.Num_Eqn();
	// Q_Constr(&K, "K", totrows, False, Rowws, Normal, True);
	// V_Constr(&U,"U", totrows, Normal,True);
	// V_Constr(&R,"R", totrows, Normal,True);

	// //Look trhoug equations (rows)
	// //cout << "[Matrix] [Row] [Col]" << numberofcomp<<endl;
	// for (int e=0;e<eq.Num_Eqn();e++)	//Aca voy con las filas de a 2
	// {
        // //cout << "eqn "<<e<<endl;
		// vector <double> ap=eq.Eqn(e).Ap().Comp();
		// Scalar ap_sc=eq.Eqn(e).Ap();
		// Scalar value;
		// int width=(eq.Eqn(e).Width()-1)*numberofcomp+1;
		// //int width=eq.Eqn(e).Width()*numberofcomp;

		// int sparsecol=0;
		// int row=e*numberofcomp;

// //        cout << "Eqn " <<e << "Width: "<<eq.Eqn(e).Width()<<endl;

		// for (int dim=0;dim<numberofcomp;dim++)
        // {
            // //cout <<"Row "<< row+dim+1<<" length: "<<width<<endl;
            // Q_SetLen(&K,row+dim+1,width);
        // }

		// vector <double> nullval;
		// nullval.assign(numberofcomp,0.);

		// //This id is relative to the cell, not the column
		// int fisrt_nonzero_col=eq.Eqn(e).MinNeigbourId();
		// int realcellid;



		// //Look trough entire width for neighbours id
		// for (int width_cells=0;width_cells<eq.Eqn(e).Width();width_cells++)
		// {
			// realcellid=width_cells+fisrt_nonzero_col;

			// vector <double> col;
			// int columnid;
			// //Found central
			// bool foundcell=false;
			// int localneighbourfound=SearchVal(realcellid,eq.Eqn(e).NeighboursIds());
			// if (eq.Eqn(e).Id()==realcellid)
			// {col=ap;columnid=row;foundcell=true;}//row is equal to
			// //Neighbours ids are not neccesarily ordered, then must search for cellid in all neighbours
			// //else if (eq.Eqn(e).Neighbour(neighb)==realcellid) //Found an
			// else if(localneighbourfound>-1)
			// {col=eq.Eqn(e).An(localneighbourfound).Comp();columnid=numberofcomp*realcellid;foundcell=true;}
			// else //column index is not a neighbour neither central cell
			// {columnid=numberofcomp*realcellid;}

			// //Write Matrix
			// if (foundcell)
			// {
                // //cout << "Found Cell " <<endl;
				// for (int dim=0;dim<numberofcomp;dim++)
				// {
					// //Rows and columns start at 1 but sparse index start at 1
					// //Look throug neighbours
					// //cout << "row + dim" << row+dim<<endl;
					// //cout << "col [dim]" <<col[dim]<<endl;
					// //cout << "number of comp"<<numberofcomp<<endl;
                    // //cout << "width_cells: "<< width_cells <<endl;
                    // //cout << "column id: "<<columnid<<endl;

                    // //cout << "Sparse index" <<numberofcomp*width_cells+dim<<endl;

                    // //Q_SetEntry(Matrix,RoC,Entry,Pos,Val)
                    // //First argument is matrix , second is row (begining from 1),
                    // //third is sparse column begining from zero
                    // //fourth is real column id (begining from 1)
                    // //Last is value
                    // //cout << "(Indexes From 1) K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << col[0]<<endl;
                    // //cout << "(From zero) Sparse col: " << numberofcomp*width_cells<<endl;
					// //Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells+dim,columnid+dim+1,col[0]);
					// //Modified, block rows are moved to right in block when increment the matrix row
					// //So the sparse index is equal to all dim components
					// Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,col[0]);
                    // //cout <<row+dim<<" "<<columnid+dim<<" "<<col[0]<<endl;
				// }

			// }
			// else
			// {
                // //cout << "Cell Not Found" <<endl;
				// for (int dim=0;dim<numberofcomp;dim++)
                // {
					// //Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells+dim,columnid+dim+1,0.0);
					// Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,0.0);
                    // //INFO
                    // //cout << "(Indexes From 1)  K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << 0.0<<endl;
                    // //cout << "(From zero) Sparse col: " << numberofcomp*width_cells <<endl;

                // }

			// }


		// }//En of width

// //		vector<double> val;
// //		vector<int> pos;
// //		for (int width_cells=0;width_cells<eq.Eqn(e).Width();width_cells++)
// //		{
// //			for (int dim=0;dim<numberofcomp;dim++)
// //			{
// //			val.push_back(K.El[row+dim+1][width_cells].Val);
// //			pos.push_back(K.El[row+dim+1][width_cells].Pos);
// //
// //			}
// //		}

	// }//End of cells

    // //cout << "R vector (from zero)"<<endl;
	// V_SetAllCmp(&R,0.0);
	// for (int e=0;e<eq.Num_Eqn();e++)
	// {
	    // //cout << "Eqn " << e<<endl;
	    // //cout << "[" <<e<<"]: "  ;
		// vector <double> source=eq.Eqn(e).Source().Comp();
		// for (int dim=0;dim<numberofcomp;dim++)
        // {
			// V_SetCmp(&R,e*numberofcomp+dim+1,source[dim]);
            // //cout << R.Cmp[e*numberofcomp+dim+1]<< " ";
        // }
        // //cout << endl;

	// }
	// //cout << "tot rows" << totrows<<endl;
	// std::vector <double> Ui,Ri;
	// Ui.assign(totrows,0.);
	// Ri.assign(totrows,0.);
	// V_SetAllCmp(&U,0.0);
	// SetRTCAccuracy(1e-5);

    // ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;
    // cout << "assembly time spent "<< ittime_spent <<endl;
    // temp= clock();
    // BiCGSTABIter (&K,&U,&R,1000,SSORPrecond,1.2);

    // ittime_spent = (double)(clock() - temp) / CLOCKS_PER_SEC;
    // cout << "solving time "<< ittime_spent <<endl;
    // temp= clock();


    // for (int j=0;j<totrows;j++)
    // {
        // //cout << "row "<< j<<endl;
        // Ui[j]=U.Cmp[j+1];
        // Ri[j]=R.Cmp[j+1];
    // }

    // //cout <<"Solver Results "<<endl;
	// for (int e=0;e<eq.Num_Eqn();e++)
	// {
		// vector <double> r;
		// for (int dim=0;dim<numberofcomp;dim++)
        // {
 			// r.push_back(U.Cmp[numberofcomp*e+dim+1]);
            // //cout <<U.Cmp[numberofcomp*e+dim+1]<<" ";
        // }
        // //cout <<endl;
		// eq.Eqn(e).X()=r;
		// //cout << " X Copied Vector: "<<eq.Eqn(e).X().outstr()<<endl;
    // }

	// Q_Destr(&K);
	// V_Destr(&U);
	// V_Destr(&R);

    // ittime_spent = (double)(clock() - temp) / CLOCKS_PER_SEC;
    // cout << "post solving time "<< ittime_spent <<endl;

    // ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;
    // cout << "total time "<< ittime_spent <<endl;
// }

}

#endif
