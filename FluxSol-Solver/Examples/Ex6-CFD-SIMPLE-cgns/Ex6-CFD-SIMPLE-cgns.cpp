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
#include <time.h>

using namespace std;
using namespace FluxSol;

///////////////////////////
//// FLUXSOL EXAMPLE 6 ////
///////////////////////////     template<class T>

stringstream reslog;

int main()
{

    bool orth_mesh=false;

    //string inputFileName=argv[1];
	string inputFileName="InputEx.in";
	cout << "Opening InputEx.in ..."<<endl;
	InputFile input(inputFileName);

    string meshfname=input.section("grid",0).get_string("file");
	Fv_CC_Grid mesh(meshfname);
	mesh.Log("Log.txt");

	cout << "Is your mesh Non-Orthogonal?"<<endl;

    for (int c=0;c<mesh.Num_Cells();c++)
        cout << mesh.Cell(c).Vp().outstr()<<endl;

	//Fields
	_CC_Fv_Field <Scalar> p(mesh);
	_CC_Fv_Field <Vec3D>  U(mesh);


	ReadVelocityFieldFromInput(input,U,mesh);

	_Surf_Fv_Field <Scalar>  phi(mesh); //Mass Flux

	//_CC_Fv_Field <Vec3D> UDiff,pDiff;

	//To be Passed to input reader

	p.AssignPatchFieldTypes(FIXEDGRADIENT);
	U.AssignPatchFieldTypes(FIXEDVALUE);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;



	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar
	Scalar rho(1.0);
	Scalar alpha_p=0.3;
	Scalar alpha_u=0.7;

    //U=Vec3D(1.0,0.,1.0);
    //U=Vec3D(0.01,0.,0.01);
    U=Vec3D(0.0,0.,0.0);

    phi=mesh.Sf() & FvExp::Interpolate(U);

    cout << "Initial Flux Info"<<endl;
    cout << phi.outstr()<<endl;

    p=0.;
	bool conv=true;


    _Surf_Fv_Field <Vec3D> Gradpf_;


    //EXAMPLE PENDING TASKS
    //TO ADD BOUNDARY FIELD IN OPERATORS= FROM SURF AND VOLFIELDS
    //TO MODIFY: ASSIGN A FIELD
    vector<Vec3D> uant;
    uant.assign(mesh.Num_Cells(),Vec3D(0.,0.,0.));

    vector<Scalar> pant;
    pant.assign(mesh.Num_Cells(),Scalar(0.));

    cout << "Face Patches" <<endl;
    for (int p=0;p<mesh.vBoundary().Num_Patches();p++)
    {
        cout << "Patch " <<p<<endl;
        for (int f=0;f<mesh.vBoundary().vPatch(p).Num_Faces();f++)
        {
            cout <<mesh.vBoundary().vPatch(p).Id_Face(f)<<endl;
        }
    }


    EqnSystem <Scalar> pEqn;
    EqnSystem <Vec3D> UEqn;     //To modify: associate Eqn System with field

    UEqn.SetRelaxCoeff(alpha_u);
    pEqn.SetRelaxCoeff(alpha_p);


	//ITERATION BEGINS
	clock_t starttime,endtime;
	time_t starttimec,endtimec;
	int it=0;

	vector <double> ures;
	while (it <100)
	{

	    cout << "-----------------------------------------------------------------------------------------------"<<endl;
	    cout << "Iteration: "<<it+1<< endl;
		//1.Restore Iteration

      //Boundary Conditions
//        Pressure gradient is null at all walls
        for (int pf=0;pf<4;pf++) p.Boundaryfield().PatchField(pf).AssignValue(0.0);
        //p.Val(36,0.);    //Reference Pressure

        cout << "Assigning boundary value"<<endl;
        for (int f=0;f<mesh.Num_Faces();f++)
        {
            if (mesh.Face(f).Boundaryface())
                //cout << "Face "<<f << "is boundary"<<endl;
                phi.Val(f,0.);
        }
//
        //To modify, correct in all faces
        //Surface fields have until now redundant information
        //It is crucial to correct phi values to zero. If these are corrected and the
        //Pressure correction p´ has Newmann conditions, then corrected flux will be against
        //null at walls
        //TEMPORARYLLY SURFACE FIELDS HAVE NOT ACTIVE PATCHES
        //TO MODIFY
        //for (int wf=0;wf<6;wf++)    phi.Val(wallfaces[wf],0.0);
        //phi.Val(15,0.0);phi.Val(19,0.0);

        //TO Modify (Simply correct an internal field constant value)
        //Like Update field Boundary Values
        for (int pf=0;pf<4;pf++) U.Boundaryfield().PatchField(pf).AssignValue(Vec3D(0.,0.,0.));
        U.Boundaryfield().PatchField(1).AssignValue(Vec3D(1.,0.,0.));

		//2. U Calculation
		//UEqn=FvImp::Div_CDS(phi, U)-FvImp::Laplacian(k,U);//TO MODIFY WITH CONVECTION SCHEME

		UEqn=FvImp::Div(phi, U)-FvImp::Laplacian(k,U);
//
//		//4. Solve Momentum predictor (UEqn)
		//Solve(UEqn==-FvExp::Grad(p));
		//_CC_Fv_Field<Vec3D> pru2(-FvExp::Grad(p));
        //TO MODIFY: IF MESH IS NOT ASSIGNED PREVIOUSLY TO EQUAL, ERROR
		_CC_Fv_Field <Vec3D> gradpV(mesh);
//		-FvExp::Grad(p);


        if (orth_mesh)
            gradpV=-FvExp::GradV(p);
		else
		gradpV=-FvExp::NonOrthGrad(p);

		//Correct boundary conditions, by imposing zero pressure gradient at wall

		//From sezai courses
//		An iterative process is required to calculate gradients:
//        Step 1: Calculate gradient from Eq. (11.31)
//        Step 2: Calculate φf from Eq. (11.32) [fiface=gradfi_fo+gradfi_fo]
//        Step 3: Repeat steps 1 and 2 until convergence. (4-5 repetitions required )


		//UEqn==gradpV;
		UEqn==(1.-alpha_u)/alpha_u*(UEqn.A()*U)+gradpV;


                        //TO MODIFY
		UEqn.Relax();   //This MUST INCLUDE R VECTOR

        starttimec= time(0);
		Solve(UEqn);
        endtimec= time(0);
		//double timec=(double) (endtime-starttime) / CLOCKS_PER_SEC * 1000.0;
		double time=(double) difftime(endtimec, starttimec);
		cout    << "Solving U time: "<<
                std::setprecision(3)<<time <<"mseconds (time)"<<endl;
//
//
		U=UEqn.Field();

        _CC_Fv_Field <Scalar> AUr(mesh);

        //THIS CRASHES!!!!
        //AUr=0.001/UEqn.A();       // In OpenFoam these are scalar

        AUr=mesh.Vp()/UEqn.A();
        cout << "AUr"<<AUr.outstr()<<endl;

//        //Assign to U Eqn Solved values
        _Surf_Fv_Field <Vec3D> Uf_;

        //What happens in Uf_ boundary?
        Uf_=FvExp::Interpolate(U);  //Uf Overbar
        //cout << "Uf_ "<< Uf_.outstr()<<endl;
        //cout << "UEqn Ap"<< UEqn.A.outstr()<<endl;

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
        //phi=phi - AUrf_*( FvExp::SnGrad(p) - ( Gradpf_ & mesh.Sf()) );
        phi=Uf_ & mesh.Sf();
        phi= phi - alpha_u*AUrf_*( FvExp::SnGrad(p) - ( Gradpf_ & mesh.Sf()) );

        //To modify FvExp::Interpolate
        for (int f=0;f<mesh.Num_Faces();f++)
        {
            if (mesh.Face(f).Boundaryface())
                //cout << "Face "<<f << "is boundary"<<endl;
                phi.Val(f,0.);
        }

        //cout << "Corrected phi"<<phi.outstr()<< endl;

		//8. Define and Solve Pressure Correction And Repeat
		//Div(mf)=Div(m´f+m*f)=0 ==> Div(m*f)+Div(-rho(DfGrad(p´f)Af)=0
        //We solve pressure correction in cell centers but eqn is indeed for cell faces
		//THIS IS INSIDE DIV ALGORITHM Sum(-rhof (Df) Grad(p´f)Af + Sum (m*f) = 0
		//for the prescribed for the non orth steps


        pEqn=FvImp::Laplacian(rho*AUr,p);   //Solve Laplacian for p (by the way, is p´)
        pEqn==FvExp::Div(phi);
        //pEqn.Eqn(36).SetValueCondition(0.);
        //Solve(pEqn==FvExp::Div(phi)); //Simply sum fluxes through faces
        Solve(pEqn);
        //Important:
        //Since Correction is in flux we have yet the faces areas includes, then
        //we must not to compute inner product another time
        //BEING BUILT
        //Nodal are corrected with Gauss grad and central coeffs
        U=U-alpha_u*(AUr*FvExp::Grad(pEqn.Field()));                  //up=up*-Dp*Grad(p´_p), GAUSS GRADIENT
        p=p+alpha_p*pEqn.Field();



        //Correct Flux: m = m* + m´
        //phi=phi-FvExp::SnGrad(AUr*p);   //Add deferred correction to this gradient
        //Correct WITH P CORRECTION
        _CC_Fv_Field<Scalar> pcorr(mesh);   //TO MODIFY; ASSIGN MESH AUTOMATICALLY
        pcorr=pEqn.Field();
        //phi= phi - alpha_u*FvExp::SnGrad(pcorr);
        phi= phi - alpha_u*AUrf_*FvExp::SnGrad(pcorr);

        //TO MODIFY, CORRECT THIS
        //phi=phi-alpha_u*(AUrf_*FvExp::SnGrad(prod));

        //cout << "grad p" <<FvExp::Grad(p).Val(0).outstr()<<endl;
        //cout << "AU Val(0)" <<AU.Val(0).outstr()<<endl;
        //cout << "AU*FvExp::Grad(p)"<<(AU*FvExp::Grad(p)).Val(0).outstr()<<endl;
        //cout << "U(0) Val: "<<U.Val(0).outstr()<<endl;

        Scalar sum;
        for (int c=0;c<p.Numberofvals();c++)
        {
            sum+=pEqn.Eqn(c).Source().Norm();
        }

        ures=UEqn.GlobalRes();

        reslog.str("");

        reslog << "[I] Iter - Residuals u v w p - Time || " << it << " - " <<ures[0] << " " << ures[1] << " "<< ures[2] << " " <<sum.outstr()<< " - " << /*time_spent<<*/endl;
        // TO MODIFY: ADD PERSONALIZED RESIDUALS
        cout <<reslog.str()<<endl;



        Vec3D maxudiff=0.;
        Scalar maxpdiff=0.;

        for (int nu=0;nu<mesh.Num_Cells();nu++)
        {
            Vec3D diff=(U.Val(nu)-uant[nu])/U.Val(nu).Norm();
            for (int dim=0;dim<3;dim++)
                if (diff[dim]>maxudiff[dim])maxudiff[dim]=diff[dim];
            Scalar pdiff=(p.Val(nu)-pant[nu])/p.Val(nu).Norm();
            if (pdiff.Val()>maxpdiff.Val())maxpdiff=pdiff;
        }

        cout << "Max U Residuals"<<endl;
        cout << maxudiff.outstr()<<endl;

        cout << "Max P Residuals"<<endl;
        cout << maxpdiff.outstr()<<endl;

        for (int nu=0;nu<mesh.Num_Cells();nu++)
        {
            uant[nu]=U.Val(nu);
            pant[nu]=p.Val(nu);
        }

        it++;
//        _CC_Fv_Field <Vec3D> test(mesh);
//        test=FvExp::Grad(p);
//        OutputFile("CellField-gradpx.vtu",test,0);
//        OutputFile("CellField-gradpy.vtu",test,2);
	} //while

    cout << "Writing results ..." <<endl;

	OutputFile("CellField-U.vtu",U);
	OutputFile("CellField-Uy.vtu",U,1);
    OutputFile("CellField-Uz.vtu",U,2);
	OutputFile("CellField-p.vtu",p);

    CenterToVertexInterpolation <Scalar> interp(mesh);
    CenterToVertexInterpolation <Vec3D> interv(mesh);
    Vertex_Fv_Field<Scalar> vF;
    Vertex_Fv_Field<Vec3D> vv;


	vF=interp.Interpolate(p);
    OutputFile("VertexField-p.vtu",vF);

    U.Boundaryfield().PatchField(1).AssignValue(Vec3D(1.,0.,0.));
    vv=interv.Interpolate(U);
    OutputFile("VertexField-U.vtu",vv);
    OutputFile("VertexField-Uz.vtu",vv,2);

	//	---- The End -------
	return 0;
}


// EXAMPLE OF ITER SOLVED BY SOLVEITER

//
//void CFDModel::SolveIter()
//{
//
//           U.Boundaryfield().ApplyBC();
//           p.Boundaryfield().ApplyBC();
//
//           UEqn=FvImp::Div(phi, U)-FvImp::Laplacian(k,U);
//
//		   _CC_Fv_Field <Vec3D> gradpV(mesh);
//		   gradpV=-FvExp::GradV(p);
//
//		   UEqn==(1.-alpha_u)/alpha_u*(UEqn.A()*U)+gradpV;
//
//           UEqn.Relax();   //This MUST INCLUDE R VECTOR
//
//           FluxSol::Solve(UEqn);
//
//		   U=UEqn.Field(); //GetNShowTimeSpent("phi_calc: U Creation");
//
//           AUr=mesh.Vp()/UEqn.A();       //GetNShowTimeSpent("phi_calc: Aur=V/A");// In OpenFoam these are scalar
//
//           AUrf_=FvExp::Interpolate(AUr);//GetNShowTimeSpent("phi_calc: AUr interpolation");
//
//           Gradpf_=FvExp::Interpolate(FvExp::Grad(p));//GetNShowTimeSpent("phi_calc: Gradp Interpolate");
//
//           phi= (FvExp::Interpolate(U) & meshSf) - alpha_u*AUrf_*( FvExp::SnGrad(p) - ( Gradpf_ & meshSf ) );
//
//           for (int pa=0;pa<mesh.vBoundary().Num_Patches();pa++)
//           {
//               //cout << "Velocity val"<<bf.PatchField(pa).ConstValue().outstr()<<endl;
//               for (int f=0;f<mesh.vBoundary().vPatch(pa).Num_Faces();f++)
//               {
//                   int idface=mesh.vBoundary().vPatch(pa).Id_Face(f);
//                   //_FvFace face=mesh.Face(idface);  //TO MODIFY idface or face pos??
//
//                   Scalar val;
//                   //if (!face.Is_Null_Flux_Face())
//                   if (bf.PatchField(pa).Type()==FIXEDVALUE)
//                   {
//                       //If constant value
//                       val=bf.PatchField(pa).ConstValue() & mesh.Face(idface).Af();
//                       //cout << "boundary val"<<val.outstr()<<endl;
//                       phi.Val(idface,val);
//                   }
//                   else //fixedgradient
//                   {
//
//                   }
//               }
//           }
//
//           pEqn=FvImp::Laplacian(rho*AUr,p);   //Solve Laplacian for p (by the way, is p´)
//           //GetNShowTimeSpent("peqn Laplacian (LHS)==");
//
//           //FvExp::Div(phi);GetNShowTimeSpent("Temp Ext div");
//           pEqn==FvExp::Div(phi);GetNShowTimeSpent("Ext div + operator==");
//
//
//           Scalar sum;
//           for (int c=0;c<p.Numberofvals();c++)
//           {
//               sum+=pEqn.Eqn(c).Source().Norm();
//           }
//
//           //pEqn.Eqn(36).SetValueCondition(0.);
//           //Solve(pEqn==FvExp::Div(phi)); //Simply sum fluxes through faces
//
//           //FluxSol::Solve(pEqn);
//           PETSC_GAMGSolver <double>pSolver;
//           pSolver.Solve(pEqn);
//
//
//           pcorr=pEqn.Field();
//           //U=U-alpha_u*(AUr*FvExp::Grad(pEqn.Field()));                  //up=up*-Dp*Grad(p´_p), GAUSS GRADIENT
//           //p=p+alpha_p*pEqn.Field();
//           U=U-alpha_u*( AUr*FvExp::Grad(pcorr) );
//           p=p+alpha_p*pcorr;
//
//           phi= phi - alpha_u*AUrf_*FvExp::SnGrad(pcorr);
//           //phi= phi - alpha_u*AUrf_*FvExp::SnGrad(pEqn.Field());
//
//           Vec3D maxudiff=0.;
//           Scalar maxpdiff=0.;
//           Scalar maxphidiff=0.;
//
//
//           // THESE ASSIGS MUST BE INSIDE ITERATIONS!!!! THESE COPIES CVALUES
//           U.AssignBoundaryField(bf);
//           p.AssignBoundaryField(pbf);
//
//           ures=UEqn.GlobalRes();
//
//           reslog.str("");
//
//           reslog << "[I] Iter - Residuals u v w p - Time || " << iternumber << " - " <<ures[0] << " " << ures[1] << " "<< ures[2] << " " <<sum.outstr()<< " - " << time_spent<<endl;
//           // TO MODIFY: ADD PERSONALIZED RESIDUALS
//
//           cout << reslog.str()<<endl;
//
//           itlog << ittime_spent <<" "<<endl;
//           iternumber++;

