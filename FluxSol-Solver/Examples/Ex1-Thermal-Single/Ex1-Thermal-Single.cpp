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
#include <list>
#include <vector>

using namespace std;
using namespace FluxSol;
template <typename T,unsigned S>
unsigned arraysizebis(const T (&v)[S]) { return S; }

// EXAMPLE 1
// THERMAL EXAMPLE
// 2 x 2 SQUARE MESH
//

int main()
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
	T.Boundaryfield()._PatchField(0).AssignValue(wallvalue);
	T.Boundaryfield()._PatchField(1).AssignValue(topvalue);

	// Materiales
	vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	//Scalar k(1.);	//Difusion, puede ser un escalar
	Scalar kdiff=material[0].k;


	//Different ways
	//1) EqnSystem <Scalar> TEqn(FvImp::Laplacian(k,T))
	//TEqn=FvImp::Laplacian(k,T);

    cout << "Creating Eqn System"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);
	cout << "Solving system..."<<endl;
	Solve(TEqn);
	cout << "Writing log..."<<endl;
	TEqn.Log("EqLog.txt");
	//solve(Laplacian(k,T)==0);

	malla.Log("LogMalla.txt");
    T=TEqn.Field();
	/////////
	//Div test

	EqnSystem <Scalar> PruEqn;
	_Surf_Fv_Field<Scalar> fip;
	_CC_Fv_Field <Vec3D> Tpru(malla);
	//PruEqn=FvImp::Div(fip,Tpru);

    OutputFile("CellField-Temp.vtu",T);
	//Prueba de lectura de malla
	//Mesh readers test
	//Fv_CC_Grid malla2;

	//malla2.Read_CGNS();

	return 0;
}

