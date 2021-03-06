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
// EXAMPLE 3 - CONVECTION DIFUSSION UNIDIMENSIONAL EXAMPLE //

#include <iostream>
#include "FluxSol.h"

using namespace std;
using namespace FluxSol;

void Test();

int main()
{

    //Test();
    //cout << "Now Ex3"<<endl;
    Fv_CC_Grid mesh(5,1,1.0,1.0);
	mesh.Log("MeshLog.txt");

//	MyList<int>wallfaces = { 1, 2};
//	MyVector<int> hola={1,2};
	int abfacesl[]={2}; //left
    int abfacesr[]={23};    //right
	list <int> lbfacesl;	for (int i=0;i<1;i++)	lbfacesl.push_back(abfacesl[i]);
	list <int> lbfacesr;	for (int i=0;i<1;i++)	lbfacesr.push_back(abfacesr[i]);

    for (int f=0;f<mesh.Num_Faces();f++)
    {
        if(mesh.Face(f).Boundaryface())
            mesh.Face(f).Null_Flux_Face(true);
    }
    //TO MODIFY
    mesh.Face(2).Null_Flux_Face(false);
    mesh.Face(23).Null_Flux_Face(false);

	Patch bfpl(lbfacesl);
	Patch bfpr(lbfacesr);
	//Patch mwall(lmovwallfaces);
	vector<Patch> vpatch;
	//Must be a verification - Debe haber una verificacion
	vpatch.push_back(bfpl);
	vpatch.push_back(bfpr);
	//(fwall,mwall);

	Boundary bound(vpatch);
	mesh.AddBoundary(bound);
	//Campo de temperatura
	_CC_Fv_Field <Scalar> phi(mesh);

	//Boundary conditions
	Scalar left=0.;
	Scalar right=1.;
	phi.Boundaryfield().PatchField(1).AssignValue(left);
	phi.Boundaryfield().PatchField(0).AssignValue(right);

	// Materiales
	vector<Materials> material=SetMaterials();

	//string inputFileName="Input.in";
	//InputFile input(inputFileName);

    //cout << "Creating Mesh..."<<endl;
	//Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));

	_CC_Fv_Field<Vec3D> U(mesh);

    U=Vec3D(0.1,0.,0.);

	//Scalar k(0.1);	//Diffusion
	Scalar kdiff=material[2].k;

	cout<<"Generating Eqn system"<<endl;

    cout << "Field U Number of Values" << U.Numberofvals()<<endl;


    //LONG WAY
    CenterToFaceInterpolation <Vec3D> interp(U);
    //_Surf_Fv_Field <Scalar> FluxField(mesh);
    _Surf_Fv_Field <Scalar> FluxField(mesh.Num_Faces());
    //This equals different fields
    FluxField= mesh.Sf() & interp.Interpolate();

    //TO VISUALIZE SEPARATELY
    EqnSystem <Scalar> CDEqn;
    CDEqn=FvImp::Div(FluxField,phi);
    CDEqn.Log("Convection Eqn Log.txt");

    CDEqn=FvImp::Laplacian(kdiff, phi) ;
    CDEqn.Log("Diffusion Eqn Log.txt");

    CDEqn = (FvImp::Laplacian(kdiff, phi) == FvImp::Div(FluxField,phi));
	//ANOTHER OPTION IS
	//CDEqn = (FvImp::Laplacian(kdiff, phi) == FvImp::Div(U&mesh.Sf(),phi));
    //CDEqn = FvImp::Laplacian(k, phi) ;
    //EqnSystem <Scalar> CDEqn(FvImp::Laplacian(k, phi) == FvImp::Div(phi,U));
	cout << "Writing log.."<<endl;
	CDEqn.Log("CDEqLog.txt");

	cout<<"Solving system"<<endl;
	Solve(CDEqn);
    phi=CDEqn.Field();

    cout << "Results"<<endl;
	for (int e=0;e<CDEqn.Num_Eqn();e++)
    {
//        cout << CDEqn.Eqn(e).X().Val()<<endl;
        cout << phi.Val(e).Val()<<endl;
    }

	//CDEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp_phi(phi);

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

//void Test()
//{
//
//    cout << "Reading square.cgns ..."<<endl;
//	Fv_CC_Grid malla("square.cgns");
//	//malla.ReadCGNS();
//
//	malla.Log("Log.txt");
//
//	_CC_Fv_Field <Scalar> T(malla);
//
//	_CC_Fv_Field <Vec3D> U(malla);
//	_CC_Fv_Field <Scalar> phi(malla);
//	U=Vec3D(1.,0,0);
//
//
//	//Boundary conditions
//	Scalar wallvalue=0.;
//	Scalar topvalue=1.;
//	for (int p=0;p<3;p++)
//	T.Boundaryfield().PatchField(p).AssignValue(wallvalue);
//	T.Boundaryfield().PatchField(3).AssignValue(topvalue);
//
//	// Materiales
//	vector<Materials> material=SetMaterials();
//
//	EqnSystem <Scalar> TEqn;
//	//Construir aca con la malla
//	//Scalar k(1.);	//Difusion, puede ser un escalar
//	Scalar kdiff=material[2].k;
//
//	cout<<"Generating system"<<endl;
//	TEqn=(FvImp::Laplacian(kdiff,T)==0.);
//
//	EqnSystem <Scalar> CDEqn;
//	CDEqn=(FvImp::Laplacian(kdiff,T)==FvImp::Div(phi,U));
//	cout<<"Solving system"<<endl;
//	Solve(TEqn);
//	TEqn.Log("EqLog.txt");
//
//	cout<<"Generating field"<<endl;
////	CenterToVertexInterpolation interp(malla);
//
//	Vertex_Fv_Field<Scalar> vT;
//
//
//	T=TEqn.Field();
//
//	cout<<"Interpolating to vertices"<<endl;
////	vT=interp.Interpolate(T);
//
//	cout<<"Writing files"<<endl;
//	OutputFile("CellField.vtu",T);
////	OutputFile("VertexField.vtu",vT);
//
//	cout << "End. Now Reading input.in"<<endl;
//
//	return 0;
//
//}
