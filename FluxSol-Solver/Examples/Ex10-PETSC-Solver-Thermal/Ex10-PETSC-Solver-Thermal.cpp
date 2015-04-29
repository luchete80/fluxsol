/************************************************************************

	Copyright 2012-2014 Luciano Buglioni - Pablo Zitelli

	Contacts:
        Luciano Buglioni: luciano.buglioni@gmail.com
        Pablo Zitelli:    zitelli.pablo@gmail.com
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
#include "FluxSol.h"

//Former temp test
// TEMPERATURE EXAMPLE 2
// READS FROM A FILE

int main(int argc, char *argv[])

//int main()
{

    cout << "Input file: "<< argv[1]<<endl;

    cout << "Reading square.cgns ..."<<endl;
	Fv_CC_Grid malla(argv[1]);
	//malla.ReadCGNS();

	malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
        T.Boundaryfield().PatchField(p).AssignValue(wallvalue);

	T.Boundaryfield().PatchField(3).AssignValue(topvalue);


	// Materiales
	//vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	//Scalar k(1.);	//Difusion, puede ser un escalar
	//Scalar kdiff=material[0].k;
	Scalar kdiff=1.;
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);
	cout<<"Solving system"<<endl;
	Solve(TEqn);
	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;



    ///// PETSC SOLVE MANUALLY ///////

	PETSC_KSP_Solver<double> Solver(TEqn.Num_Eqn());


    //solver.SetMatVal(vn[r],vn[c],0.);

	int numberofcomp=pow(3.,TEqn.Dim());
	int totrows=numberofcomp*TEqn.Num_Eqn();

    vector <int> nonzerosperrow;
	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
    {
        int width=(TEqn.Eqn(e).Width()-1)*numberofcomp+1;
        //int nz=(TEqn.Eqn(e).An().size()-1)*numberofcomp+1;
        for (int dim=0;dim<numberofcomp;dim++)  nonzerosperrow.push_back(5);
    }


    Solver.PreAllocateRows(nonzerosperrow);

	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
	{
	    //Width Assign

        //cout << "Assemblying Eqn "<<e<<endl;
		vector <double> ap=TEqn.Eqn(e).Ap().Comp();
		Scalar ap_sc=TEqn.Eqn(e).Ap();
		Scalar value;
		int width=(TEqn.Eqn(e).Width()-1)*numberofcomp+1;
//		//int width=TEqn.Eqn(e).Width()*numberofcomp;   // THIS IS WRONG
//
		int sparsecol=0;
		int row=e*numberofcomp;

//        cout << "Eqn " <<e << "Width: "<<TEqn.Eqn(e).Width()<<endl;

		for (int dim=0;dim<numberofcomp;dim++)
        {
            //cout <<"Row "<< row+dim+1<<" length: "<<width<<endl;
            //Q_SetLen(&K,row+dim+1,width);
        }

		vector <double> nullval;
		nullval.assign(numberofcomp,0.);

		//This id is relative to the cell, not the column
		int fisrt_nonzero_col=TEqn.Eqn(e).MinNeigbourId();
		int realcellid;



		//Look trough entire width for neighbours id
		for (int width_cells=0;width_cells<TEqn.Eqn(e).Width();width_cells++)
		{
			realcellid=width_cells+fisrt_nonzero_col;

			vector <double> col;
			int columnid;
			//Found central
			bool foundcell=false;
			int localneighbourfound=FluxSol::SearchVal(realcellid,TEqn.Eqn(e).NeighboursIds());
			if (TEqn.Eqn(e).Id()==realcellid)
			{col=ap;columnid=row;foundcell=true;}//row is equal to
			//Neighbours ids are not neccesarily ordered, then must search for cellid in all neighbours
			//else if (TEqn.Eqn(e).Neighbour(neighb)==realcellid) //Found an
			else if(localneighbourfound>-1)
			{col=TEqn.Eqn(e).An(localneighbourfound).Comp();columnid=numberofcomp*realcellid;foundcell=true;}
			else //column index is not a neighbour neither central cell
			{columnid=numberofcomp*realcellid;}

			//Write Matrix
			if (foundcell)
			{
                //cout << "Found Cell " <<endl;
				for (int dim=0;dim<numberofcomp;dim++)
				{
                    //INFO
                    //cout << "(Indexes From 1)  K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << 0.0<<endl;
                    //cout << "(From zero) Sparse col: " << numberofcomp*width_cells <<endl;

					//Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,col[0]);
					Solver.SetMatVal(row+dim, columnid+dim, col[0]);

				}

			}
			else
			{
                //cout << "Cell Not Found" <<endl;
				for (int dim=0;dim<numberofcomp;dim++)
                {
                    //Solver.SetMatVal(row+dim, columnid+dim, 0.);
					//Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,0.0);
                    //INFO
                    //cout << "(Indexes From 1)  K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << 0.0<<endl;
                    //cout << "(From zero) Sparse col: " << numberofcomp*width_cells <<endl;

                }

			}


		}//En of width


	}//End of cells

    //cout << "R vector (from zero)"<<endl;
	//V_SetAllCmp(&R,0.0);
	for (int e=0;e<TEqn.Num_Eqn();e++)
	{
	    //cout << "Eqn " << e<<endl;
	    //cout << "[" <<e<<"]: "  ;
		vector <double> source=TEqn.Eqn(e).Source().Comp();
		for (int dim=0;dim<numberofcomp;dim++)
        {
            Solver.SetbValues(e*numberofcomp+dim, source[dim]);
			//V_SetCmp(&R,e*numberofcomp+dim+1,source[dim]);
            //cout << R.Cmp[e*numberofcomp+dim+1]<< " ";
        }
        //cout << endl;

	}
	//cout << "tot rows" << totrows<<endl;
	//std::vector <double> Ui,Ri;
	//Ui.assign(totrows,0.);
	//Ri.assign(totrows,0.);
	//V_SetAllCmp(&U,0.0);
	//SetRTCAccuracy(1e-5);

    clock_t ittime_begin, ittime_end;
    double ittime_spent;

    ittime_begin = clock();

    Solver.Solve();

    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;

    cout << "PETSC Solving elapsed time: "<<ittime_spent<<endl;


    ofstream file;     //Previously this was inside each function
    file.open("Out.3D");
    file << " x y z val"<<endl;
    vector <double> sol=Solver.X();

    for (int i=0;i<sol.size();i++)
    {
        for (int c=0;c<3;c++)   file << malla.Node_(i).comp[c]<<" ";
        file << sol[i]<<endl;
    }

    file.close();
	//////////// RESULTS ///////
	CenterToVertexInterpolation <Scalar> interp(malla);

	Vertex_Fv_Field<Scalar> vT;

    _CC_Fv_Field <Scalar> U(malla);

	T=TEqn.Field();

	cout<<"Interpolating to vertices"<<endl;
	vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
	OutputFile("VertexField.vtu",vT);

	return 0;
}

