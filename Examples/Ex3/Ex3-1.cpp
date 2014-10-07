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

#include <iostream>
#include "FluxSol.h"

using namespace std;
using namespace FluxSol;

int main()
{

    Fv_CC_Grid mesh(5,1,1.0,1.0);
//	MyList<int>wallfaces = { 1, 2};
//	MyVector<int> hola={1,2};
	int wallfaces[]={2,5,8,10,13,18};
	int movwallfaces[]={15,19};
	list <int> lmovwallfaces;	for (int i=0;i<2;i++)	lmovwallfaces.push_back(movwallfaces[i]);
	list <int> lwallfaces;	for (int i=0;i<6;i++)	lwallfaces.push_back(wallfaces[i]);
	Patch fwall(lwallfaces);
	Patch mwall(lmovwallfaces);
	vector<Patch> vpatch;
	//Must be a verification - Debe haber una verificacion
	vpatch.push_back(lwallfaces); 	vpatch.push_back(lmovwallfaces);
	//(fwall,mwall);

	Boundary bound(vpatch);
	mesh.AddBoundary(bound);
	//Campo de temperatura
	_CC_Fv_Field <Scalar> phi(mesh);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	phi.Boundaryfield().PatchField(0).AssignValue(wallvalue);
	phi.Boundaryfield().PatchField(1).AssignValue(topvalue);


	//string inputFileName="Input.in";
	//InputFile input(inputFileName);

    //cout << "Creating Mesh..."<<endl;
	//Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));

	_CC_Fv_Field<Vec3D> U(mesh);

    U=1.0;

	mesh.Log("Log.txt");


	EqnSystem <Scalar> CDEqn;

	Scalar k(1.);	//Diffusion

	cout<<"Generating Eqn system"<<endl;

    cout << "Field U Number of Values" << U.Numberofvals()<<endl;

	CDEqn = (FvImp::Laplacian(k, phi) == FvImp::Div(phi,U));

	cout << "Writing log.."<<endl;
	CDEqn.Log("CDEqLog.txt");

	cout<<"Solving system"<<endl;
	Solve(CDEqn);
    phi=CDEqn.Field();

    cout << "Results"<<endl;
	for (int e=0;e<CDEqn.Num_Eqn();e++)
    {
        cout << CDEqn.Eqn(e).X().Val()<<endl;
        cout << phi.Val(e).Val()<<endl;
    }

	//CDEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp(phi);

	Vertex_Fv_Field<Scalar> intphi;    //interpolated phi


	//phi.ToCellCenters(CDEqn);


	cout<<"Interpolating to vertices"<<endl;
	//intphi=interp.Interpolate(phi);

	cout<<"Writing files"<<endl;
	OutputFile("CellFieldCD.vtu",phi);
	//OutputFile("VertexField.vtu",intphi);

	//	---- The End -------
	return 0;

}
