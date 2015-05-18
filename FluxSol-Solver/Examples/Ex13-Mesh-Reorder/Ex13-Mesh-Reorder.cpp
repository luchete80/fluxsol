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

// LAPLACIAN MESH REORDER FOR



int main(int argc,char **args)
{

       Vec            x,b,u;  /* approx solution, RHS, exact solution */
   Mat            A;        /* linear system matrix */
   KSP            ksp;     /* linear solver context */
   PetscRandom    rctx;     /* random number generator context */
   PetscReal      norm;     /* norm of solution error */
   PetscInt       i,j,Ii,J,Istart,Iend,m,n,its;
   PetscBool      random_exact_sol,view_exact_sol,permute;
   char           ordering[256] = MATORDERINGRCM;
   IS             rowperm       = NULL,colperm = NULL;
   PetscScalar    v;
     #if defined(PETSC_USE_LOG)
       PetscLogStage stage;
     #endif

    permute=PETSC_TRUE;

    cout << "Input file: "<< args[1]<<endl;

    cout << "Reading square.cgns ..."<<endl;
	Fv_CC_Grid malla(args[1]);
	//malla.ReadCGNS();

	//malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;

	T.Boundaryfield().PatchField(1).AssignValue(topvalue);


	// Materiales
	//vector<Materials> material=SetMaterials();

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	//Scalar k(1.);	//Difusion, puede ser un escalar
	//Scalar kdiff=material[0].k;
	Scalar kdiff=1.;
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);


    int numberofcomp=pow(3.,TEqn.Dim());

    /////////////////////////////////// SOLVER CREATION ///////////////////////////////////////////

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          Compute the matrix and right-hand-side vector that define
          the linear system, Ax = b.
      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
   /*
      Create parallel matrix, specifying only its global dimensions.
      When using MatCreate(), the matrix format can be specified at
      runtime. Also, the parallel partitioning of the matrix is
      determined by PETSc at runtime.

      Performance tuning note:  For problems of substantial size,
      preallocation of matrix memory is crucial for attaining good
      performance. See the matrix chapter of the users manual for details.
   */

   char help[100];
   PetscInitialize(&argc,&args,(char *)0,help);

   int totrows=TEqn.Num_Eqn()*TEqn.Num_Eqn();
   MatCreate(PETSC_COMM_WORLD,&A);
   MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,totrows,totrows);
   MatSetFromOptions(A);
   MatMPIAIJSetPreallocation(A,5,NULL,5,NULL);
   MatSeqAIJSetPreallocation(A,5,NULL);
   MatSetUp(A);

   /*
      Currently, all PETSc parallel matrix formats are partitioned by
      contiguous chunks of rows across the processors.  Determine which
      rows of the matrix are locally owned.
   */
   MatGetOwnershipRange(A,&Istart,&Iend);

   /*
      Set matrix elements for the 2-D, five-point stencil in parallel.
       - Each processor needs to insert only elements that it owns
         locally (but any non-local elements will be sent to the
         appropriate processor during matrix assembly).
       - Always specify global rows and columns of matrix entries.

      Note: this uses the less common natural ordering that orders first
      all the unknowns for x = h then for x = 2h etc; Hence you see J = Ii +- n
      instead of J = I +- m as you might expect. The more standard ordering
      would first do all variables for y = h, then y = 2h etc.

    */
   PetscLogStageRegister("Assembly", &stage);
   PetscLogStagePush(stage);

   ///// MATRIX ASSEMBLY

    //MatSetOption(Solver.Matrix(),MAT_SYMMETRIC,PETSC_TRUE);

    vector <double> col;    //CREATING Col IS FasTER

    cout << "Assembying Eqns"<<endl;
    PetscInt rowi,coli;

    //Max Neighbours
    PetscScalar vals [1][10];      //block
    PetscInt idxm[3],idxn[3][10];   //rows and cols index

	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
	{
	    //Width Assign
        cout << "Eqn "<<e<<endl;
        //cout << "Assemblying Eqn "<<e<<endl;
		//vector <double> ap=TEqn.Eqn(e).Ap().Comp();
		Scalar value;

        col=TEqn.Eqn(e).Ap().Comp();
		int row=e*numberofcomp;

        //vector <double> col;
        //CENTRAL COEFFS
        //col=ap;


        for (int dim=0;dim<numberofcomp;dim++)
        {
            //SetMatVal(const PetscInt &row, const PetscInt &col, const PetscScalar &value)
            rowi=row+dim;
            MatSetValues(A,1,&rowi,1,&rowi,&col[0],INSERT_VALUES);
            //Solver.SetMatVal(row+dim, row+dim, col[0]);    //An is scalar
        }


        //vals=vector<PetscScalar>(TEqn.Eqn(e).Neighbour(nc));
        //vector < vector <> >
        PetscScalar vals [TEqn.Eqn(e).Num_Neighbours()][TEqn.Eqn(e).Num_Neighbours()];
		//Look trough entire width for neighbours id
		//The main idea is to look through eqn width
		//Real cell id is taken, and then are watched all neighbours to check if each real cell id belongs to neighbours vector
		for (int nc=0;nc<TEqn.Eqn(e).Num_Neighbours();nc++)
		{
			int realcellid=TEqn.Eqn(e).Neighbour(nc);   //Wich cell

			//col=TEqn.Eqn(e).An(nc).Comp();
			int columnid=numberofcomp*realcellid;
            col=TEqn.Eqn(e).An(nc).Comp();
            //cout << "Found Cell " <<endl;
            vals[0][nc]=TEqn.Eqn(e).An(nc).Comp()[0];


            for (int dim=0;dim<numberofcomp;dim++)
            {
                rowi=row+dim;
                coli=columnid+dim;
                //MatSetValues(Solver.Matrix(),1,&rowi,1,&coli,&col[0],INSERT_VALUES);

                idxm[dim]=row+dim;
                idxn[dim][nc]=columnid+dim;
                //Original
                //Solver.SetMatVal(row+dim, columnid+dim, col[0]);    //An is scalar
            }

		}//En of neighbours

        //New, block
        for (int dim=0;dim<numberofcomp;dim++)
            MatSetValues(A,1,&idxm[dim],TEqn.Eqn(e).Num_Neighbours(),idxn[dim],vals[0],INSERT_VALUES);


	}//End of cells	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2




    //cout << "R vector (from zero)"<<endl;
	//V_SetAllCmp(&R,0.0);
	for (int e=0;e<TEqn.Num_Eqn();e++)
	{
	    //cout << "Eqn " << e<<endl;
	    //cout << "[" <<e<<"]: "  ;
		vector <double> source=TEqn.Eqn(e).Source().Comp();
		for (int dim=0;dim<numberofcomp;dim++)
        {
            PetscInt row=e*numberofcomp+dim;
            PetscScalar val=source[dim];
            //Solver.SetbValues(e*numberofcomp+dim, source[dim]);
            VecSetValues(b,1,&row,&val,INSERT_VALUES);
        }
        //cout << endl;
	}



   /*
      Assemble matrix, using the 2-step process:
        MatAssemblyBegin(), MatAssemblyEnd()
      Computations can be done while messages are in transition
      by placing code between these two statements.
   */
   MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
   MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
   PetscLogStagePop();

   /* A is symmetric. Set symmetric flag to enable ICC/Cholesky preconditioner */
   MatSetOption(A,MAT_SYMMETRIC,PETSC_TRUE);

   /*
      Create parallel vectors.
       - We form 1 vector from scratch and then duplicate as needed.
       - When using VecCreate(), VecSetSizes and VecSetFromOptions()
         in this example, we specify only the
         vector's global dimension; the parallel partitioning is determined
         at runtime.
       - When solving a linear system, the vectors and matrices MUST
         be partitioned accordingly.  PETSc automatically generates
         appropriately partitioned matrices and vectors when MatCreate()
         and VecCreate() are used with the same communicator.
       - The user can alternatively specify the local vector and matrix
         dimensions when more sophisticated partitioning is needed
         (replacing the PETSC_DECIDE argument in the VecSetSizes() statement
         below).
   */
   VecCreate(PETSC_COMM_WORLD,&u);
   VecSetSizes(u,PETSC_DECIDE,m*n);
   VecSetFromOptions(u);
   VecDuplicate(u,&b);
   VecDuplicate(b,&x);

   /*
      Set exact solution; then compute right-hand-side vector.
      By default we use an exact solution of a vector with all
      elements of 1.0;  Alternatively, using the runtime option
      -random_sol forms a solution vector with random components.
   */
//   if (random_exact_sol) {
//     PetscRandomCreate(PETSC_COMM_WORLD,&rctx);
//     PetscRandomSetFromOptions(rctx);
//     VecSetRandom(u,rctx);
//     PetscRandomDestroy(&rctx);
//   } else {
//     VecSet(u,1.0);
//   }
//   MatMult(A,u,b);
//
//   /*
//      View the exact solution vector if desired
//   */
//   if (view_exact_sol) {VecView(u,PETSC_VIEWER_STDOUT_WORLD);}
//
//   if (permute) {
//     Mat Aperm;
//     MatGetOrdering(A,ordering,&rowperm,&colperm);
//     MatPermute(A,rowperm,colperm,&Aperm);
//     VecPermute(b,colperm,PETSC_FALSE);
//     MatDestroy(&A);
//     A    = Aperm;               /* Replace original operator with permuted version */
//   }

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                 Create the linear solver and set various options
      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /*
      Create linear solver context
   */
   KSPCreate(PETSC_COMM_WORLD,&ksp);

   /*
      Set operators. Here the matrix that defines the linear system
      also serves as the preconditioning matrix.
   */
   KSPSetOperators(ksp,A,A,DIFFERENT_NONZERO_PATTERN);

   /*
      Set linear solver defaults for this problem (optional).
      - By extracting the KSP and PC contexts from the KSP context,
        we can then directly call any KSP and PC routines to set
        various options.
      - The following two statements are optional; all of these
        parameters could alternatively be specified at runtime via
        KSPSetFromOptions().  All of these defaults can be
        overridden at runtime, as indicated below.
   */
   KSPSetTolerances(ksp,1.e-2/((m+1)*(n+1)),1.e-50,PETSC_DEFAULT,
                           PETSC_DEFAULT);

   /*
     Set runtime options, e.g.,
         -ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
     These options will override those specified above as long as
     KSPSetFromOptions() is called _after_ any other customization
     routines.
   */
   KSPSetFromOptions(ksp);

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                       Solve the linear system
      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   KSPSolve(ksp,b,x);

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                       Check solution and clean up
      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   if (permute) {VecPermute(x,rowperm,PETSC_TRUE);}

   /*
      Check the error
   */
   VecAXPY(x,-1.0,u);
   VecNorm(x,NORM_2,&norm);
   KSPGetIterationNumber(ksp,&its);

   /*
      Print convergence information.  PetscPrintf() produces a single
      print statement from all processes that share a communicator.
      An alternative is PetscFPrintf(), which prints to a file.
   */
   PetscPrintf(PETSC_COMM_WORLD,"Norm of error %g iterations %D\n",(double)norm,its);

   /*
      Free work space.  All PETSc objects should be destroyed when they
      are no longer needed.
   */
   KSPDestroy(&ksp);
   VecDestroy(&u);  VecDestroy(&x);
   VecDestroy(&b);  MatDestroy(&A);
   ISDestroy(&rowperm);  ISDestroy(&colperm);

   /*
      Always call PetscFinalize() before exiting a program.  This routine
        - finalizes the PETSc libraries as well as MPI
        - provides summary and diagnostic information if certain runtime
          options are chosen (e.g., -log_summary).
   */



///




//	Solver.Solve();
    ofstream file;
    file.open("Out-Permuted.3D");
    file << " x y z val"<<endl;
    //sol=Solver.X();
    double sol;
    for (int i=0;i<totrows;i++)
    {
        VecGetValues(x,1,&i,&sol);;
        for (int c=0;c<3;c++)   file << malla.Node_(i).comp[c]<<" ";
        file << sol<<endl;
    }

    file.close();


	return 0;
}

