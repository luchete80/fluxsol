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
#include <iostream>
#include "FluxSol.h"
#include <time.h>
#include "Time_.h"      //Time imtegration

using namespace std;
using namespace FluxSol;

///////////////////////////
//// FLUXSOL EXAMPLE 17 ////
///////////////////////////
// TRANSIENT CONVECTION DIFFUSION
// RUNS SIMPLE PRESSURE - VELOCITY COUPLING ALGORITHM FROM AN INPUT FILE

//d/dt (rho phi) + d/dx (rho U phi ) = d/dx (k d/dx(phi) ) + S
// 45 x 1 x 1 cells

stringstream reslog;

int main(int argc,char **args)
{

    // From Ibrahim Sezai courses
    // consider convection and diffusion in the onedimensional domain sketched in Figure 8.7. Calculate the transient
    //temperature field if the initial temperature is zero everywhere and the
    //boundary conditions are φ=0 at x=0 and ∂φ /∂x=0 at x=L. the data are
    //L 1.5m =1.5m, u 2m/s =2m/s, ρ=1.0kg/m3 and Γ=0.03kg/m/s.

    //Test();
    //cout << "Now Ex3"<<endl;
    const int cellnumber=45;
    Fv_CC_Grid mesh(cellnumber,1,1.5,1.);

    Time time_;  time_.AdjustDeltat(0.1);
    mesh.AssignTime(time_);
	mesh.Log("MeshLog.txt");

	int abfacesl[]={mesh.Cell(0).Id_Face(1)}; //left Face from left cell
    int abfacesr[]={mesh.Cell(cellnumber-1).Id_Face(3)};    //right

    cout << "[I]"  << "Boundary faces: "<< abfacesl[0] << ", " << abfacesr[0]<<endl;

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
	vector<Materials> material;
	//=SetMaterials();


	_CC_Fv_Field<Vec3D> U(mesh);

    U=Vec3D(2.,0.,0.);

	//Scalar k(0.1);	//Diffusion
	Scalar kdiff=material[2].k;

	cout<<"[I] Generating Eqn system"<<endl;
    cout << "[I] Field U Number of Values" << U.Numberofvals()<<endl;


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

	return 0;
}


