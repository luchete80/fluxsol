#include "Model.h"

using namespace std;


using namespace FluxSol;

Model::Model(const std::string filename)
{
    this->inputfile=InputFile(filename);

    //Control of inputfile
    //TO MODIFY
    this->numparts=1;


    if (this->numparts==1)
    {
        //TO MODIFY, INPUT FILE MUST RETURN A MESH
        cout << "[I] Reading Mesh ..."<<endl;
        string meshfname=inputfile.section("grid",0).get_string("file");
        Fv_CC_Grid mesht(meshfname);
        this->mesh=mesht;
        inputfile.AssignGridPtr(this->mesh);                               //Assuming CGNS file
        //TO MODIFY, READED BY INPUT
        this->maxiter=100;

        std::vector<int> listi=inputfile.section("grid",0).subsection("patch",0).get_intList("list");
//        std::cout << "Getting list"<<endl;
//        std::cout <<"List size"<< listi.size()<<std::endl;

    }

}

CFDModel::CFDModel(const std::string s):Model(s)
{
    cout << "[I] Initializing fields ..."<<endl;
    this->InitFields();
}



void CFDModel::InitFields()
{
     //Converts all mesh to one

	//Fields
	_CC_Fv_Field <Scalar> pt(this->mesh);
	//_CC_Fv_Field <Vec3D>  Ut(this->mesh);

	//this->p=_CC_Fv_Field<Scalar>(this->mesh);
	//this->U=_CC_Fv_Field<Vec3D>(this->mesh);
    this->U=inputfile.UField();     //Read Field Boundary Values
    this->p=inputfile.pField();

    _CC_Fv_Field <Vec3D>  Ut;

    for (int i=0;i<4;i++)
        cout << "patch cvalue Velocity" << U.Boundaryfield().PatchField(i).ConstValue()<<endl;


    for (int i=0;i<4;i++)
        cout << "patch cvalue Pressure" << p.Boundaryfield().PatchField(i).ConstValue().outstr()<<endl;


	//ReadVelocityFieldFromInput(this->inputfile,this->U,this->mesh);

	_Surf_Fv_Field <Scalar>  phi(this->mesh); //Mass Flux

	//_CC_Fv_Field <Vec3D> UDiff,pDiff;

	//To be Passed to input reader
    cout << "[I] Assigning Flux Fields ..." <<endl;
	//p.AssignPatchFieldTypes(FIXEDGRADIENT);
	//U.AssignPatchFieldTypes(FIXEDVALUE);

    cout << "[I] Reading Boundary Conditions"<<endl;
	inputfile.AssignFieldPtrs(&this->U,&this->p);
    this->bcs=inputfile.ReadBCs();

    cout << "[I] Assigning Patch Field Types"<<endl;
	for (int p=0;p<this->mesh.vBoundary().Num_Patches();p++)
    {
        //U.Boundaryfield().PatchField(i)
        cout << "[I] Patch "<<p<<endl;
        bcs[p]->AssignPatchFieldType();
    }

    cout << "[I] Field initialized"<<endl;
    //this->p=inputfile.UField();     //Read Field Boundary Values

    //BEFORE READ BCs!
}

    void CFDModel::Solve()
    {
    //Begins with SIMPLE method


    cout << "[I] Solving system ..." <<endl;
	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;

    this->mesh.Log("MeshLog.txt");

	//Construir aca con la malla
	Scalar k(1.);	//Difusion, can be an scalar
	Scalar rho(1.0);
	Scalar alpha_p=0.3;
	Scalar alpha_u=0.7;

    cout << "[I] Initializing Fields ..."<<endl;
    //U=Vec3D(0.0,0.,0.0);

    //FvExp::Interpolate(this->U);
    phi=this->mesh.Sf() & FvExp::Interpolate(this->U);
    //cout << "Initial Flux Info"<<endl;
    //cout << phi.outstr()<<endl;

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

    vector<Scalar> phiant;
    phiant.assign(mesh.Num_Faces(),Scalar(0.));


    UEqn.SetRelaxCoeff(alpha_u);
    pEqn.SetRelaxCoeff(alpha_p);

	//ITERATION BEGINS
//	clock_t starttime,endtime;
    cout << "[I] Iterations"<<endl;
    cout << "-----------------------------------------------------------------------------------------------"<<endl;
	time_t starttimec,endtimec;
	int it=0;

        //TO MODIFY
    _BoundaryField<Vec3D>  bf =U.Boundaryfield();
    _BoundaryField<Scalar>pbf =p.Boundaryfield();

    for (int i=0;i<4;i++)
        cout << "patch " << i << "cvalue phi" << phi.Boundaryfield().PatchField(i).ConstValue().outstr()<<endl;

    conv=false;
	while (!conv)
	{

//	    cout << "Iteration: "<<it+1<< endl;
		//1.Restore Iteration
//
//      //Boundary Conditions
        //Pressure gradient is null at all walls
        //for (int pf=0;pf<4;pf++) p.Boundaryfield().PatchField(pf).AssignValue(0.0);
        //p.Val(36,0.);    //Reference Pressure

        for (int f=0;f<mesh.Num_Faces();f++)
        {
            if (mesh.Face(f).Boundaryface())
                phi.Val(f,0.);
        }


        //To modify, correct in all faces
        //Surface fields have until now redundant information
        //It is crucial to correct phi values to zero. If these are corrected and the
        //Pressure correction p� has Newmann conditions, then corrected flux will be against
        //null at walls
        //TEMPORARYLLY SURFACE FIELDS HAVE NOT ACTIVE PATCHES
        //TO MODIFY
        //for (int wf=0;wf<6;wf++)    phi.Val(wallfaces[wf],0.0);
        //phi.Val(15,0.0);phi.Val(19,0.0);

        //TO Modify (Simply correct an internal field constant value)
        //Like Update field Boundary Values



        //for (int pf=0;pf<4;pf++) U.Boundaryfield().PatchField(pf).AssignValue(Vec3D(0.,0.,0.));

        //U.Boundaryfield().PatchField(1).AssignValue(Vec3D(1.,0.,0.));

        U.Boundaryfield().ApplyBC();
        p.Boundaryfield().ApplyBC();

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


		//UEqn==gradpV;
		UEqn==(1.-alpha_u)/alpha_u*(UEqn.A()*U)+gradpV;


                        //TO MODIFY
		UEqn.Relax();   //This MUST INCLUDE R VECTOR

        starttimec= time(0);
		FluxSol::Solve(UEqn);
        endtimec= time(0);
		//double timec=(double) (endtime-starttime) / CLOCKS_PER_SEC * 1000.0;
		double time=(double) difftime(endtimec, starttimec);
//		cout    << "Solving U time: "<<
//                std::setprecision(3)<<time <<"mseconds (time)"<<endl;
//
//

		U=UEqn.Field();

        _CC_Fv_Field <Scalar> AUr(mesh);

        //TO MODIFY
        //Mesh Volume
        AUr=mesh.Vp()/UEqn.A();       // In OpenFoam these are scalar

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


        //TO MODIFY
        //IF THERE ARE WALLS
        //To modify FvExp::Interpolate
//        for (int f=0;f<mesh.Num_Faces();f++)
//        {
//            if (mesh.Face(f).Boundaryface())
//            {
//
//                phi.Val(f,0.);
//            }
//                //cout << "Face "<<f << "is boundary"<<endl;
//        }

        phi.Boundaryfield().ApplyBC();


    for (int i=0;i<4;i++)
        cout << "patch " << i << "cvalue phi" << p.Boundaryfield().PatchField(i).ConstValue().outstr()<<endl;



//        cout << "Corrected phi"<<phi.outstr()<< endl;
//
//		//8. Define and Solve Pressure Correction And Repeat
//		//Div(mf)=Div(m�f+m*f)=0 ==> Div(m*f)+Div(-rho(DfGrad(p�f)Af)=0
//        //We solve pressure correction in cell centers but eqn is indeed for cell faces
//		//THIS IS INSIDE DIV ALGORITHM Sum(-rhof (Df) Grad(p�f)Af + Sum (m*f) = 0
//		//for the prescribed for the non orth steps


        pEqn=FvImp::Laplacian(rho*AUr,p);   //Solve Laplacian for p (by the way, is p�)
        pEqn==FvExp::Div(phi);
        //pEqn.Eqn(36).SetValueCondition(0.);
        //Solve(pEqn==FvExp::Div(phi)); //Simply sum fluxes through faces
        FluxSol::Solve(pEqn);
        //Important:
        //Since Correction is in flux we have yet the faces areas includes, then
        //we must not to compute inner product another time
        //BEING BUILT
        //Nodal are corrected with Gauss grad and central coeffs
        U=U-alpha_u*(AUr*FvExp::Grad(pEqn.Field()));                  //up=up*-Dp*Grad(p�_p), GAUSS GRADIENT
        p=p+alpha_p*pEqn.Field();


        //Correct Flux: m = m* + m�
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

        Vec3D maxudiff=0.;
        Scalar maxpdiff=0.;
        Scalar maxphidiff=0.;

        for (int nu=0;nu<mesh.Num_Cells();nu++)
        {
            Vec3D diff=(U.Val(nu)-uant[nu])/U.Val(nu).Norm();
            for (int dim=0;dim<3;dim++)
                if (diff[dim]>maxudiff[dim])maxudiff[dim]=diff[dim];

            Scalar pdiff=(p.Val(nu)-pant[nu])/p.Val(nu).Norm();
            if (pdiff.Val()>maxpdiff.Val())maxpdiff=pdiff;
        }


        for (int nu=0;nu<mesh.Num_Faces();nu++)
        {
            Scalar diff=(phi.Val(nu)-phiant[nu])/phi.Val(nu).Norm();

            if (diff.Val()>maxphidiff.Val())  maxphidiff=diff;
        }

        cout << "[I] Iter - Residuals u v w p - Time || " << it << " " <<maxudiff.outstr()<<maxpdiff.outstr()<<endl;


        for (int nu=0;nu<mesh.Num_Cells();nu++)
        {
            uant[nu]=U.Val(nu);
            pant[nu]=p.Val(nu);
        }

        it++;


        if (maxudiff[0]<1.e-4 && maxudiff[1]<1.e-4 && maxudiff[2]<1.e-4  && maxpdiff<1.e-4 )   conv=true;

        //TO MODIFY, Change
        //TO MODIFY, CHECKMESH
        U.AssignBoundaryField(bf);
        p.AssignBoundaryField(pbf);

	}
//	OutputFile("CellField-U.vtu",U);
//	OutputFile("CellField-Uy.vtu",U,1);
//    OutputFile("CellField-Uz.vtu",U,2);
	OutputFile("CellField-p.vtu",p);
	OutputFile("CellField-U.vtu",U,0);
//
    CenterToVertexInterpolation <Scalar> interp(mesh);
    CenterToVertexInterpolation <Vec3D> interv(mesh);
    Vertex_Fv_Field<Scalar> vF;
    Vertex_Fv_Field<Vec3D> vv;
//
//
//    cout << "INTERPOLATING"<<endl;
	vF=interp.Interpolate(p);
    OutputFile("VertexField-p.vtu",vF);
//
    //Rewrite BCs?
    //U.Boundaryfield().PatchField(1).AssignValue(Vec3D(1.,0.,0.));
    vv=interv.Interpolate(U);
//    OutputFile("VertexField-U.vtu",vv);
    OutputFile("VertexField-U.vtu",vv);
    OutputFile("VertexField-Uz.vtu",vv,2);

    //OutputFile of("Fields.vtu",this->mesh);

    //of.WriteGrid();
    //of.WriteField(vv);
    //of.WriteField(vF);
    //of.WriteFooter();   //TO QUIT

}

void CFDModel::WriteOutput()
{

}


void CFDModel::Extract_Cells_and_BoundaryFromNastran()
{
	vector <int> patchnum;	//Numero de la propiedad
	int nump=0;
	int numboundcells=0,numboundfaces=0,numintcells=0;
	vector <Cell_CC> vc;
	vector <Patch> vp;


	//Recorro todos los elementos
	//Aca tambien estan las condiciones de borde
	//for (int e=0;e<NasFile().;e++)
	//{
	//	if (bidim_model)
	//	{
	//		//Discrimino los CQUAD4 y CQUADR
	//		if (this->Elementos[e].Tipo()=="CQUAD4" || this->Elementos[e].Tipo()=="CQUADR" ||
	//			this->Elementos[e].Tipo()=="CTRIA3" || this->Elementos[e].Tipo()=="CTRIAR" )
	//		//Es un elemento del interior
	//		{
	//			//Este lo paso al vector
	//			Cell_CC cell(this->Elementos[e].VerId(),this->Elementos[e].Conect());
	//			vc.push_back(cell);
	//			numintcells++;
	//		}
	//		else //Elemento de Boundary
	//		{
	//			int currpatch;

	//			//Si las propiedades cambian son distintos patches
	//			bool enc=false;	//Una propiedad existente
	//			int p=0;
	//
	//			while (!enc && p<patchnum.size())
	//			{
	//				if(patchnum[p]==this->Elementos[e].VerPid())
	//				{
	//					enc=true;
	//					currpatch=p;
	//				}
	//				p++;
	//			}

	//			if (!enc ||nump==0)//Si ese patch no esta
	//			{
	//				//patchnum.push_back(this->Elementos[e].VerPid());
	//				currpatch=patchnum.size()-1;
	//				nump++;
	//				//Inserto el patch en el vector
	//				//Patch patch(this->Elementos[e].VerPid());
	//				//vp.push_back(patch);
	//				//bound.AddPatch(patch);
	//			}
	//
	//			//Agrego el face al patch que corresponda
	//			vp[currpatch].AddFace(numboundfaces);
	//			numboundfaces++;
	//		}

	//	}
	//	else	//Modelo tridimensional
	//	{}
	//}//Fin del numero de elementos

	//Incluyo el numero de patches al modelo

}


