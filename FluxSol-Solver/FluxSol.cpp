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
#include "CDTest.h"

using namespace std;
using namespace FluxSol;

// Equation options
#define NONE -1
#define NS 1
#define HEAT 2

void CDTest();
void CDTest2();
void Ex1();
void Ex2();
void Ex2Def();
void maintest();

//int argc, char *argv[]
int main ()
{
    //Ex1();
    cout << "Thermal test"<<endl;
	Ex1();
	cout << "Convection Diffusion Test" <<endl;
	CDTest2();
	//Ex2();
}


void maintest()
{
        Fv_CC_Grid malla(2,2,1.0,1.0);
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
	malla.AddBoundary(bound);
	//Campo de temperatura
	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	T.Boundaryfield().PatchField(0).AssignValue(wallvalue);
	T.Boundaryfield().PatchField(1).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar


	//Different ways
	//1) EqnSystem <Scalar> TEqn(FvImp::Laplacian(k,T))
	//TEqn=FvImp::Laplacian(k,T);
    cout << "Creating Eqn"<<endl;
	TEqn=(FvImp::Laplacian(k,T)==0.);
	Solve(TEqn);
	TEqn.Log("EqLog.txt");
	//solve(Laplacian(k,T)==0);

	malla.Log("LogMalla.txt");

	/////////
	//Div test

	EqnSystem <Scalar> PruEqn;
	_Surf_Fv_Field<Scalar> fip;
	_CC_Fv_Field <Vec3D> Tpru(malla);

}
void CDTest()
{


	string inputFileName="Input.in";
	InputFile input(inputFileName);

	vector<int> equations;

    cout << "Creating Mesh..."<<endl;
	Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));

	_CC_Fv_Field<Vec3D> U;
	_CC_Fv_Field<Scalar> phi;

    cout << "Reading Fields"<<endl;
	ReadFieldFromInput(input, U,mesh);


	mesh.Log("Log.txt");


	EqnSystem <Scalar> CDEqn;

	Scalar k(1.);	//Diffusion

	cout<<"Generating Eqn system"<<endl;

	//CDEqn = (FvImp::Laplacian(k, phi) == FvImp::Div(U));

	cout<<"Solving system"<<endl;
	//Solve(CDEqn);


	//CDEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp(phi);

	Vertex_Fv_Field<Scalar> intphi;    //interpolated phi


	//phi.ToCellCenters(CDEqn);


	cout<<"Interpolating to vertices"<<endl;
	intphi=interp.Interpolate(phi);

	cout<<"Writing files"<<endl;
	OutputFile("CellFieldCD.vtu",phi);
	OutputFile("VertexField.vtu",intphi);

	//	---- The End -------
	return 0;

}

void CDTest2()
{

    Fv_CC_Grid mesh(2,2,1.0,1.0);
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

    //Divergence is Div(U.A * phi)
    //Classical OpenFOAM Notation is Div (flux, type) = Eqn (type) = flux * sum type_f
	//CDEqn = (FvImp::Laplacian(k, phi) == FvImp::Div(U&mesh.Sf(),phi));

	cout << "Writing log.."<<endl;
	CDEqn.Log("CDEqLog.txt");

	cout<<"Solving system"<<endl;
	Solve(CDEqn);
    phi=CDEqn.Field();

    cout << "Results"<<endl;
	for (int e=0;e<CDEqn.Num_Eqn();e++)
    {
        //cout << CDEqn.Eqn(e).X().Val()<<endl;
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

void Ex1()
{
	//Creo la malla cuadrada
	//de 1,0 de lado por diez elementos
	//Fv_CC_Grid malla(10,10,1.0,1.0);
	//Las caras frontales no las considero
	Fv_CC_Grid malla(2,2,1.0,1.0);
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
	malla.AddBoundary(bound);
	//Campo de temperatura
	_CC_Fv_Field <Scalar> T(malla);

//	cout << "Grid Faces "<<T.Grid().Num_Faces()<<endl;

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	T.Boundaryfield().PatchField(0).AssignValue(wallvalue);
	T.Boundaryfield().PatchField(1).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar


	//Different ways
	//1) EqnSystem <Scalar> TEqn(FvImp::Laplacian(k,T))
	//TEqn=FvImp::Laplacian(k,T);

    cout << "Creating Eqn System"<<endl;
	TEqn=(FvImp::Laplacian(k,T)==0.);
	cout << "Solving system.."<<endl;
	Solve(TEqn);
	cout << "Writing log.."<<endl;
	TEqn.Log("CEqLog.txt");
	//solve(Laplacian(k,T)==0);

    T=TEqn.Field();
	cout << "Results"<<endl;
	for (int e=0;e<TEqn.Num_Eqn();e++)
    {
        cout << TEqn.Eqn(e).X().Val()<<endl;
        cout << T.Val(e).Val()<<endl;
    }
	malla.Log("LogMalla.txt");

	/////////
	//Div test

	EqnSystem <Scalar> PruEqn;
	_Surf_Fv_Field<Scalar> fip;
	_CC_Fv_Field <Vec3D> Tpru(malla);
	//PruEqn=FvImp::Div(fip,Tpru);

	//Prueba de lectura de malla
	//Mesh readers test
	//Fv_CC_Grid malla2;

	//malla2.Read_CGNS();

    CenterToVertexInterpolation <Scalar> interp(malla);
    Vertex_Fv_Field<Scalar> vT;
	//vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellFieldC.vtu",T);
//	OutputFile("VertexField.vtu",vT);

}

void Ex2Def()
{
	string inputFileName="Input.in";
	InputFile input(inputFileName);

	vector<int> equations;

	Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));

	_CC_Fv_Field<Scalar> T;

	ReadFieldFromInput(input, T,mesh);
	mesh.Log("Log.txt");


	EqnSystem <Scalar> TEqn;

	Scalar k(1.);	//Diffusion

	cout<<"Generating system"<<endl;

	TEqn=(FvImp::Laplacian(k,T)==0.);


	cout<<"Solving system"<<endl;
	Solve(TEqn);


	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp(mesh);

	Vertex_Fv_Field<Scalar> vT;


	//T.ToCellCenters(TEqn);
	//_CC_Fv_Field<Vec3D> gradT = FvExp::Grad(T);

	//_CC_Fv_Field <Vec3D> gradT=FvExp::Grad(T);

	cout<<"Interpolating to vertices"<<endl;
	vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
	OutputFile("VertexField.vtu",vT);

	//	---- The End -------
}


void Ex2()
{

	Fv_CC_Grid malla("square.cgns");
	//malla.ReadCGNS();

	malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
	T.Boundaryfield().PatchField(p).AssignValue(wallvalue);
	T.Boundaryfield().PatchField(3).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(k,T)==0.);
	cout<<"Solving system"<<endl;
	Solve(TEqn);
	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp(malla);

	Vertex_Fv_Field<Scalar> vT;


	//T.ToCellCenters(TEqn);

	cout<<"Interpolating to vertices"<<endl;
	//vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
	//OutputFile("VertexField.vtu",vT);
	return 0;
}


