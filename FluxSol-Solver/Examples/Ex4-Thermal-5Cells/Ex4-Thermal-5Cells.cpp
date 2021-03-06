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
// THERMAL 5 CELLS EXAMPLE //

#include <iostream>
#include "FluxSol.h"

using namespace std;
using namespace FluxSol;

int main()
{

    Fv_CC_Grid mesh(5,1,0.5,0.01);
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
	_CC_Fv_Field <Scalar> T(mesh);

	//Boundary conditions
	Scalar left=100.;
	Scalar right=500.;
	T.Boundaryfield().PatchField(0).AssignValue(left);
	T.Boundaryfield().PatchField(1).AssignValue(right);
	
	// Materiales
	vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;

	//Scalar k(1000.0);	//Diffusion
	Scalar kdiff=material[3].k;

	cout<<"Generating Eqn system"<<endl;


	TEqn = (FvImp::Laplacian(kdiff, T) == 0);

	cout << "Writing log.."<<endl;
	TEqn.Log("CDEqLog.txt");

	cout<<"Solving system"<<endl;
	Solve(TEqn);
    T=TEqn.Field();

    cout << "Results"<<endl;
	for (int e=0;e<TEqn.Num_Eqn();e++)
    {
        cout << TEqn.Eqn(e).X().Val()<<endl;
        cout << T.Val(e).Val()<<endl;
    }

	//CDEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp(T);

	Vertex_Fv_Field<Scalar> intT;    //interpolated phi


	//phi.ToCellCenters(CDEqn);


	cout<<"Interpolating to vertices"<<endl;
	//intphi=interp.Interpolate(phi);

	cout<<"Writing files"<<endl;
	OutputFile("CellFieldCD.vtu",T);
	//OutputFile("VertexField.vtu",intphi);

	//	---- The End -------
	return 0;

}
