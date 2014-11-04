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

	//Fields
	_CC_Fv_Field <Scalar> p(mesh);
	_CC_Fv_Field <Vec3D>  U(mesh);
	_Surf_Fv_Field <Scalar>  phi; //Mass Flux


	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	//p.Boundaryfield().PatchField(0).AssignValue(wallvalue);
	//p.Boundaryfield().PatchField(1).AssignValue(topvalue);

	EqnSystem <Scalar> pEqn;
	EqnSystem <Vec3D> UEqn;

	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar


	bool conv=false;
	//ITERATION BEGINS
	while (!conv)
	{
		//1.Restore Iteration
		//p.RestorePrevIter();

		//2. U Calculation
		UEqn=FvImp::Div(phi, U)-FvImp::Laplacian(k,U);

		//3. Under Relax UEqn
		//UEqn.Relax();

		//4. Solve Momentum predictor (UEqn)
		Solve(UEqn==-FvExp::Grad(p));

		//5. Update p Boundary conditions
		//p.BoundaryField().UpdateCoeffs();

		//6. Calculate ap (Central coeffs) and U
		// ------ OpenFoam Style ------
		//_CC_Fv_Field <Scalar> AU=UEqn.A();
		//U = UEqn.H()/AU;

		//Standard Style

		//7. Calculate the Flux, inner product with mash faces
		//Interpolate is like CenterToFaceInterpolation
		FvExp::Interpolate(U) & mesh.Sf();
		//phi= FvExp::Interpolate(U) & mesh.Sf();
		//AdjustPhi(phi,U,p);		//CHECK THIS

		//8. Define and Solve Pressure Correction And Repeat
		//for the prescribed for the non orth steps
		//pEqn=FvImp::Laplacian(1.0/AU,p)==FvExp::Div(phi);	//New, explicit Div
		//pEqn.SetReference(pRefCell,pRefVal); If the problem is Not prescribed in pressure
		//pEqn.Solve();

		//9. Correct the flux
		//phi-=pEqn.Flux();

		//10. Calculate Continuity errors

		//11. Under Relax pressure for the momentum corrector and apply the correction
		//p.Relax();
		//U-=FvExp::Grad(p)/AU;
		//U.CorrectBoundaryConditions();
	}
	//	---- The End -------
	return 0;
}


        //OpenFoam Style

		//1.Restore Iteration
		//p.RestorePrevIter();

		//2. U Calculation
		//UEqn=FvImp::Div(phi, U)-FvImp::Laplacian(k,U);

		//3. Under Relax UEqn
		//UEqn.Relax();

		//4. Solve Momentum predictor (UEqn)
		//Solve(UEqn==-FvExp::Grad(p));

		//5. Update p Boundary conditions
		//p.BoundaryField().UpdateCoeffs();

		//6. Calculate ap (Central coeffs) and U
		// ------ OpenFoam Style ------
		//_CC_Fv_Field <Scalar> AU=UEqn.A();
		//U = UEqn.H()/AU;

		//Standard Style

		//7. Calculate the Flux, inner product with mash faces
		//Interpolate is like CenterToFaceInterpolation
		//FvExp::Interpolate(U) & mesh.Sf();
		//phi= FvExp::Interpolate(U) & mesh.Sf();
		//AdjustPhi(phi,U,p);		//CHECK THIS

		//8. Define and Solve Pressure Correction And Repeat
		//for the prescribed for the non orth steps
		//pEqn=FvImp::Laplacian(1.0/AU,p)==FvExp::Div(phi);	//New, explicit Div
		//pEqn.SetReference(pRefCell,pRefVal); If the problem is Not prescribed in pressure
		//pEqn.Solve();

		//9. Correct the flux
		//phi-=pEqn.Flux();

		//10. Calculate Continuity errors

		//11. Under Relax pressure for the momentum corrector and apply the correction
		//p.Relax();
		//U-=FvExp::Grad(p)/AU;
		//U.CorrectBoundaryConditions();
