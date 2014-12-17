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
// THERMAL 5 CELLS EXAMPLE //

#include <iostream>
#include "FluxSol.h"

using namespace std;
using namespace FluxSol;



int main()
{
	//Mesh Generation
	Fv_CC_Grid mesh(2,2,1.0,1.0);
	mesh.Log("Log.txt");
//	MyList<int>wallfaces = { 1, 2};
//	MyVector<int> hola={1,2};
	int wallfaces[]={2,5,8,10,13,18};
	int movwallfaces[]={15,19};
	list <int> lmovwallfaces;	for (int i=0;i<2;i++)	lmovwallfaces.push_back(movwallfaces[i]);
	list <int> lwallfaces;	for (int i=0;i<6;i++)	    lwallfaces.push_back(wallfaces[i]);
	Patch fwall(lwallfaces);
	Patch mwall(lmovwallfaces);
	vector<Patch> vpatch;
	//Must be a verification - Debe haber una verificacion
	vpatch.push_back(lwallfaces); 	vpatch.push_back(lmovwallfaces);
	//(fwall,mwall);

	Boundary bound(vpatch);
	mesh.AddBoundary(bound);

	//Fields
	_CC_Fv_Field <Scalar> p(mesh);
	_CC_Fv_Field <Vec3D>  U(mesh);
	_Surf_Fv_Field <Scalar>  phi(mesh); //Mass Flux

	//_CC_Fv_Field <Vec3D> UDiff,pDiff;

	p.AssignPatchFieldTypes(FIXEDGRADIENT);
	U.AssignPatchFieldTypes(FIXEDVALUE);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;



	//Construir aca con la malla
	Scalar k(0.01);	//Difusion, puede ser un escalar
	Scalar rho(1.0);
	Scalar alpha_p=0.6;

    U=Vec3D(0.01,0.01,0.);

    phi=mesh.Sf() & FvExp::Interpolate(U);

    cout << "Initial Flux Info"<<endl;
    cout << phi.outstr()<<endl;

    p=0.;
	bool conv=true;


    _Surf_Fv_Field <Vec3D> Gradpf_;


    //EXAMPLE PENDING TASKS
    //TO ADD BOUNDARY FIELD IN OPERATORS= FROM SURF AND VOLFIELDS
    p.Val(0,0.);    //Reference Pressure

	//ITERATION BEGINS
	int it=0;
	while (it <10)
	{
        EqnSystem <Scalar> pEqn;
        EqnSystem <Vec3D> UEqn;

	    cout << "-----------------------------------------------------------------------------------------------"<<endl;
	    cout << "Iteration: "<<it+1<< endl;
		//1.Restore Iteration
//
//      //Boundary Conditions
        //Pressure gradient is null at all walls
        p.Boundaryfield().PatchField(0).AssignValue(0.0);
        p.Boundaryfield().PatchField(1).AssignValue(0.0);

        //To modify, correct in all faces
        //Surface fields have until now redundant information
        //It is crucial to correct phi values to zero. If these are corrected and the
        //Pressure correction p´ has Newmann conditions, then corrected flux will be against
        //null at walls
        for (int wf=0;wf<6;wf++)    phi.Val(wallfaces[wf],0.0);
        phi.Val(15,0.0);phi.Val(19,0.0);

        //TO Modify (Simply correct an internal field constant value)
        //Like Update field Boundary Values
        U.Boundaryfield().PatchField(0).AssignValue(Vec3D(0.,0.,0.));
        U.Boundaryfield().PatchField(1).AssignValue(Vec3D(1.,0.,0.));

        cout <<"Original Flux field" << phi.outstr()<<endl;

		//2. U Calculation
		UEqn=FvImp::Div(phi, U)-FvImp::Laplacian(k,U);

		FvImp::Laplacian(k,U).Log("Diffusion.txt");
		FvImp::Div(phi,U).Log("Conv.txt");

		(FvImp::Div(phi, U)-FvImp::Laplacian(k,U)).Log("UEqn.txt");
//
//		//4. Solve Momentum predictor (UEqn)
		//Solve(UEqn==-FvExp::Grad(p));
		//_CC_Fv_Field<Vec3D> pru2(-FvExp::Grad(p));
        //TO MODIFY: IF MESH IS NOT ASSIGNED PREVIOUSLY TO EQUAL, ERROR
		_CC_Fv_Field <Vec3D> gradp(mesh);
//		-FvExp::Grad(p);
		gradp=-FvExp::Grad(p);
		//Correct boundary conditions, by imposing zero pressure gradient at wall

		cout <<"P Field Info: "<<endl;
		cout << p.outstr()<<endl;

		cout <<"GradP Info: "<<endl;
		cout << gradp.outstr()<<endl;

		UEqn==gradp;
		UEqn.Log("UEqn.txt");
		Solve(UEqn);
//
//
		U=UEqn.Field();
        cout << "Solved U Momentum Eq" <<UEqn.Field().outstr()<<endl;

//        //Assign to U Eqn Solved values
        _Surf_Fv_Field <Vec3D> Uf_;

        Uf_=FvExp::Interpolate(U);  //Uf Overbar
        _CC_Fv_Field <Scalar> AUr(mesh);

        AUr=1./UEqn.A();       // In OpenFoam these are scalar

        cout << "Uf_ "<< Uf_.outstr()<<endl;
        cout << "UEqn Ap"<< AUr.outstr()<<endl;

        //TO FIX: MAKE THIS WORK
        _Surf_Fv_Field <Scalar> AUrf_;
        AUrf_=FvExp::Interpolate(AUr);

        //INSTEAD OF
        Gradpf_=FvExp::Interpolate(FvExp::Grad(p));

//
//        //Rhie-Chow Correction
//        //vf=vf_ - Df (Grad(p)-Grad_(p))
//        //Where Grad(p)=(pn-pp)/.. + Orth Correction
//        //Is more simple to directly calculate fluxes
        //Obtaining m*, RhieChow Interpolated Flux
        phi=phi - AUrf_*( FvExp::SnGrad(p) - ( Gradpf_ & mesh.Sf()) );

//        cout << "Corrected phi"<<phi.outstr()<< endl;
//
//		//8. Define and Solve Pressure Correction And Repeat
//		//Div(mf)=Div(m´f+m*f)=0 ==> Div(m*f)+Div(-rho(DfGrad(p´f)Af)=0
//        //We solve pressure correction in cell centers but eqn is indeed for cell faces
//		//THIS IS INSIDE DIV ALGORITHM Sum(-rhof (Df) Grad(p´f)Af + Sum (m*f) = 0
//		//for the prescribed for the non orth steps
        cout << "AUr " << AUr.outstr()<<endl;
        pEqn=FvImp::Laplacian(rho*AUr,p);   //Solve Laplacian for p (by the way, is p´)
        pEqn==FvExp::Div(phi);
        pEqn.Eqn(0).SetValueCondition(0.);
        Solve(pEqn); //Simply sum fluxes through faces

        cout << "Flux Divergence"<<FvExp::Div(phi).outstr()<<endl;
        pEqn.Log("PEqn.txt");
        //Important:
        //Since Correction is in flux we have yet the faces areas includes, then
        //we must not to compute inner product another time
        cout << "Solved p coorection" <<pEqn.Field().outstr()<<endl;
        //BEING BUILT
        //Nodal are corrected with Gauss grad and central coeffs
        U=U-(AUr*FvExp::Grad(p));                  //up=up*-Dp*Grad(p´_p), GAUSS GRADIENT
        p=p+alpha_p*pEqn.Field();

        //Correct Flux: m = m* + m´
        //phi=phi-FvExp::SnGrad(AUr*p);   //Add deferred correction to this gradient
        _CC_Fv_Field<Scalar> prod=AUr*p;
        phi=phi-FvExp::SnGrad(prod);
        cout << "Corrected U"   << U.outstr()<<endl;
        cout << "Corrected p"   << p.outstr()<<endl;
        cout << "Corrected phi" << phi.outstr()<<endl;

        //cout << "grad p" <<FvExp::Grad(p).Val(0).outstr()<<endl;
        //cout << "AU Val(0)" <<AU.Val(0).outstr()<<endl;
        //cout << "AU*FvExp::Grad(p)"<<(AU*FvExp::Grad(p)).Val(0).outstr()<<endl;
        //cout << "U(0) Val: "<<U.Val(0).outstr()<<endl;



        it++;
	}
	//	---- The End -------
	return 0;
}


