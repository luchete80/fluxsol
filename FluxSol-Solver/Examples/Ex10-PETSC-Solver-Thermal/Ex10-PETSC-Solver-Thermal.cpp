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

	//malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	//for (int p=0;p<3;p++)
    //    T.Boundaryfield().PatchField(p).AssignValue(wallvalue);

	T.Boundaryfield().PatchField(1).AssignValue(topvalue);


	// Materiales
	//vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	//Scalar k(1.);	//Difusion, puede ser un escalar
	//Scalar kdiff=material[0].k;
	Scalar kdiff=1.;
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);
//	cout<<"Solving system"<<endl;
//	Solve(TEqn);
//	TEqn.Log("EqLog.txt");
//
//	cout<<"Generating field"<<endl;

    int numberofcomp=pow(3.,TEqn.Dim());

    ///// PETSC SOLVE MANUALLY ///////

	PETSC_KSP_Solver<double> Solver(TEqn.Num_Eqn());


	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
	{
	    //Width Assign

        //cout << "Assemblying Eqn "<<e<<endl;
		vector <double> ap=TEqn.Eqn(e).Ap().Comp();
		Scalar ap_sc=TEqn.Eqn(e).Ap();
		Scalar value;

		int row=e*numberofcomp;

        vector <double> col;
        //CENTRAL COEFFS
        col=ap;
        for (int dim=0;dim<numberofcomp;dim++)
            Solver.SetMatVal(row+dim, row+dim, col[0]);    //An is scalar


		//Look trough entire width for neighbours id
		//The main idea is to look through eqn width
		//Real cell id is taken, and then are watched all neighbours to check if each real cell id belongs to neighbours vector
		for (int nc=0;nc<TEqn.Eqn(e).Num_Neighbours();nc++)
		{
			int realcellid=TEqn.Eqn(e).Neighbour(nc);   //Wich cell

			col=TEqn.Eqn(e).An(nc).Comp();
			int columnid=numberofcomp*realcellid;

            //cout << "Found Cell " <<endl;
            for (int dim=0;dim<numberofcomp;dim++)
                Solver.SetMatVal(row+dim, columnid+dim, col[0]);    //An is scalar

		}//En of neighbours


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

