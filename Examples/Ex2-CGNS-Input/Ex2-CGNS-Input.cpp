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
void DefTempTest();

int main()
{

    cout << "Reading square.cgns ..."<<endl;
	Fv_CC_Grid malla("square.cgns");
	//malla.ReadCGNS();

	malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
	T.Boundaryfield()._PatchField(p).AssignValue(wallvalue);
	T.Boundaryfield()._PatchField(3).AssignValue(topvalue);

	// Materiales
	vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	//Scalar k(1.);	//Difusion, puede ser un escalar
	Scalar kdiff=material[0].k;

	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);
	cout<<"Solving system"<<endl;
	Solve(TEqn);
	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
//	CenterToVertexInterpolation interp(malla);

	Vertex_Fv_Field<Scalar> vT;

    _CC_Fv_Field <Scalar> U(malla);

	T=TEqn.Field();

	cout<<"Interpolating to vertices"<<endl;
//	vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
//	OutputFile("VertexField.vtu",vT);

	cout << "End. Now Reading input.in"<<endl;
	DefTempTest();
	return 0;
}


//Last Test
void DefTempTest()
{
	string inputFileName="Input.in";
	InputFile input(inputFileName);

	vector<int> equations;

	Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));

	_CC_Fv_Field<Scalar> T;

	ReadFieldFromInput(input, T,mesh);
	mesh.Log("Log.txt");

	// Materiales
	vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;

	//Scalar k(1.);	//Diffusion
	Scalar kdiff=material[0].k;

	cout<<"Generating system"<<endl;

	TEqn=(FvImp::Laplacian(kdiff,T)==0.);


	cout<<"Solving system"<<endl;
	Solve(TEqn);


	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
//	CenterToVertexInterpolation interp(mesh);

	Vertex_Fv_Field<Scalar> vT;


	T=TEqn.Field();
	//_CC_Fv_Field<Vec3D> gradT = FvExp::Grad(T);

	//_CC_Fv_Field <Vec3D> gradT=FvExp::Grad(T);

	cout<<"Interpolating to vertices"<<endl;
	//vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField-2.vtu",T);
	//OutputFile("VertexField.vtu",vT);

	//	---- The End -------
}
