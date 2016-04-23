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
#include "StatThermoFluidModel.h"

using namespace std;
using namespace FluxSol;


#include <iostream>
#include "FluxSol.h"
#include <time.h>

using namespace std;
using namespace FluxSol;

///////////////////////////
//// FLUXSOL EXAMPLE 6 ////
///////////////////////////
// RUNS SIMPLE PRESSURE - VELOCITY COUPLING ALGORITHM FROM AN INPUT FILE

stringstream reslog;

int main(int argc,char **args)
{

    string inputFileName="Inputbdf-10x10-Struct.in";
	//if (argc>1)
	//{
	//string inputFileName=args[1];
	//cout << "Opening "<< inputFileName <<endl;
	InputFile input(inputFileName);

    string meshfname=input.section("grid",0).get_string("file");
	Fv_CC_Grid mesh(meshfname);
	mesh.Log("Log.txt");

    for (int c=0;c<mesh.Num_Cells();c++)
        cout << mesh.Cell(c).Vp().outstr()<<endl;

	//Fields
	_CC_Fv_Field <Scalar> p(mesh);
	_CC_Fv_Field <Vec3D>  U(mesh);

	//
	//_CC_Fv_Field <Scalar>  T(mesh);
	_CC_Fv_Field <Scalar>  h(mesh);

    //TO MODIFY, MUST BE READED BOTH h AND T
	ReadVelocityFieldFromInput(input,U,mesh);

	_Surf_Fv_Field <Scalar>  phi(mesh); //Mass Flux

	//_CC_Fv_Field <Vec3D> UDiff,pDiff;

	//To be Passed to input reader

	p.AssignPatchFieldTypes(FIXEDGRADIENT);
	U.AssignPatchFieldTypes(FIXEDVALUE);

    //TEMPERATURE IS A MIXING FIELD
    h.Boundaryfield().PatchField(0).AssignType(FIXEDVALUE);
    h.Boundaryfield().PatchField(2).AssignType(FIXEDVALUE);
    h.Boundaryfield().PatchField(1).AssignType(FIXEDGRADIENT);
    h.Boundaryfield().PatchField(3).AssignType(FIXEDGRADIENT);



	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;



	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar
	Scalar rho(1.0);
	Scalar alpha_p=0.3;
	Scalar alpha_u=0.7;
	Scalar cp=1.;

    //U=Vec3D(1.0,0.,1.0);
    //U=Vec3D(0.01,0.,0.01);
    U=Vec3D(0.0,0.,0.0);
    h=0;

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

//    cout << "Face Patches" <<endl;
//    for (int p=0;p<mesh.vBoundary().Num_Patches();p++)
//    {
//        cout << "Patch " <<p<<endl;
//        for (int f=0;f<mesh.vBoundary().vPatch(p).Num_Faces();f++)
//        {
//            cout <<mesh.vBoundary().vPatch(p).Id_Face(f)<<endl;
//        }
//    }


    EqnSystem <Scalar> pEqn;
    EqnSystem <Vec3D> UEqn;     //To modify: associate Eqn System with field
    EqnSystem <Scalar> TEqn;

    UEqn.SetRelaxCoeff(alpha_u);
    pEqn.SetRelaxCoeff(alpha_p);


	//ITERATION BEGINS
	clock_t starttime,endtime;
	time_t starttimec,endtimec;
	int it=0;

    _BoundaryField<Vec3D>  bf =U.Boundaryfield();   //Temp pbf
    _BoundaryField<Scalar>pbf =p.Boundaryfield();   //Temp bf

	vector <double> ures;
	while (it <100)
	{

	    cout << "-----------------------------------------------------------------------------------------------"<<endl;
	    cout << "Iteration: "<<it+1<< endl;
		//1.Restore Iteration

      //Boundary Conditions
//        Pressure gradient is null at all walls
        for (int pf=0;pf<4;pf++) p.Boundaryfield().PatchField(pf).AssignValue(0.0);
        p.Val(0,0.);    //Reference Pressure

//        h.Boundaryfield().PatchField(1).AssignValue(0.0);
//        h.Boundaryfield().PatchField(3).AssignValue(0.0);

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
        U.Boundaryfield().PatchField(2).AssignValue(Vec3D(1.,0.,0.));

        for (int pf=0;pf<4;pf++) h.Boundaryfield().PatchField(pf).AssignValue(Scalar(0.));
            h.Boundaryfield().PatchField(2).AssignValue(Scalar(1.));

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

        gradpV=-FvExp::GradV(p);

		//Correct boundary conditions, by imposing zero pressure gradient at wall

		//From sezai courses
//		An iterative process is required to calculate gradients:
//        Step 1: Calculate gradient from Eq. (11.31)
//        Step 2: Calculate φf from Eq. (11.32)
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

        //till down here this is like examples 6 and 15
        /////////////////////////////////
        //TEMPERATURE
        /////////////////////////////////
        //Flux for transport eqn is cT=h
        //d(rho c T)/dt + Div(rho c T U) - Div . (k Grad (cT) ) = ScT
        //tauij=mu(dui/dxj+duj/dxi)

        cout << "Phi values: "<<endl;
        cout << phi.outstr()<<endl;
        //h=cp*T;
        TEqn=( FvImp::Div(phi, h)-FvImp::Laplacian(k,h) );
        TEqn==0.;
        Solve(TEqn);
        h=TEqn.Field();
        cout << "Enthalpy value: "<<endl;
        for (int e=0;e<TEqn.Num_Eqn();e++)cout <<TEqn.Eqn(e).Source().outstr()<<endl;
        cout << TEqn.outstr()<<endl;
        //T=h;

        //HERE MUSt BE T=h/cp,


//        {
//    solve
//    (
//        fvm::ddt(rho, h)
//      + fvm::div(phi, h)
//      - fvm::laplacian(turbulence->alphaEff(), h)
//     ==
//        DpDt
//    );
//
//    thermo->correct();
//}
    //http://cfd.direct/openfoam/energy-equation/


    // volTensorField gradU = fvc::grad(U);
    // volTensorField tau = mu * (gradU + gradU.T());
    // rho * cp * (fvm::ddt(T) + fvm::div(phi,T)) - fvm::laplacian(kT,T)+ ( tau && gradU )


        /////////////////////////////////
        /// END TEMPERATURE
        /////////////////////////////////

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
            U.AssignBoundaryField(bf);
            p.AssignBoundaryField(pbf);

        it++;
//        _CC_Fv_Field <Vec3D> test(mesh);
//        test=FvExp::Grad(p);
//        OutputFile("CellField-gradpx.vtu",test,0);
//        OutputFile("CellField-gradpy.vtu",test,2);
	} //while

    cout << "Writing results ..." <<endl;

    cout << "Writing cell fielda..."<<endl;
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

    h.Boundaryfield().PatchField(2).AssignValue(1.);
    vF=interp.Interpolate(h);
    OutputFile("VertexField-h.vtu",vF);

    U.Boundaryfield().PatchField(1).AssignValue(Vec3D(1.,0.,0.));
    vv=interv.Interpolate(U);
    OutputFile("VertexField-U.vtu",vv);
    OutputFile("VertexField-Uz.vtu",vv,2);
//	}//If argc>1
//	else
//    {
//        cout << "Input file not specified. Usage Ex6.. <Input file name.in>"<<endl;
//    }
	//	---- The End -------
	return 0;
}
