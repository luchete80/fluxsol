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
//SIMILAR TO EXAMPLE 18


int main(int argc,char **args)
{

       Vec            x,b,u;  /* approx solution, RHS, exact solution */
   Mat            A;        /* linear system matrix */
   KSP            ksp;     /* linear solver context */
   PetscRandom    rctx;     /* random number generator context */
   PetscReal      norm;     /* norm of solution error */
   PetscInt       i,j,Ii,J,Istart,Iend,its;
   PetscBool      random_exact_sol,view_exact_sol,permute,amgsolver;
   char           ordering[256] = MATORDERINGRCM;
   IS             rowperm       = NULL,colperm = NULL;
   PetscScalar    v;
   PC pc; // preconditioner context


     #if defined(PETSC_USE_LOG)
       PetscLogStage stage;
     #endif

    MPI_Comm       comm;

    PetscErrorCode ierr;

    clock_t ittime_begin, ittime_end;
    double ittime_spent;


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
   char           opstring[256];

    PetscOptionsBegin(PETSC_COMM_WORLD,NULL,"Poisson example options","");
    {
        //PetscOptionsBool("-random_exact_sol","Choose a random exact solution","",random_exact_sol,&random_exact_sol,NULL);
        //PetscOptionsString("-test_op","Designate which MPI_Op to use","",opstring,opstring,256,NULL);
      PetscOptionsString("-file","Grid File","",opstring,opstring,256,NULL);
     permute          = PETSC_FALSE;
     //PetscOptionsBool(const char opt[],const char text[],const char man[],PetscBool deflt,PetscBool  *flg,PetscBool  *set)
     PetscOptionsBool("-permute","Ordering","",permute,&permute,NULL);
     //PetscOptionsFList("-permute","Permute matrix and vector to solving in new ordering","",MatOrderingList,ordering,ordering,sizeof(ordering),&permute);
    amgsolver=PETSC_FALSE;
     PetscOptionsBool("-amg","solver AMG","",amgsolver,&amgsolver,NULL);
}
  PetscOptionsEnd();

    //cout << "Input file: "<< args[1]<<endl;
    cout << "Input file: "<< opstring<<endl;

    if (permute)    cout << "Matrix Permutation is enabled..."<<endl;
    else            cout << "Matrix Permutation is disabled..."<<endl;

	//Fv_CC_Grid malla(args[1]);
	Fv_CC_Grid malla(opstring);
	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;

	T.Boundaryfield().PatchField(1).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	Scalar kdiff=1.;
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);

    int numberofcomp=pow(3.,TEqn.Dim());


   int totrows=TEqn.Num_Eqn();
   comm = PETSC_COMM_WORLD;
   MatCreate(PETSC_COMM_WORLD,&A);
   MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,totrows,totrows);
   MatSetFromOptions(A);
   MatMPIAIJSetPreallocation(A,5,NULL,5,NULL);
   MatSeqAIJSetPreallocation(A,5,NULL);
   MatSetUp(A);

   VecCreate(PETSC_COMM_WORLD,&u);
   VecSetSizes(u,PETSC_DECIDE,totrows);
   VecSetFromOptions(u);
   VecDuplicate(u,&b);
   VecDuplicate(b,&x);


//
//   /*
//      Currently, all PETSc parallel matrix formats are partitioned by
//      contiguous chunks of rows across the processors.  Determine which
//      rows of the matrix are locally owned.
//   */
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
        //cout << "Eqn "<<e<<endl;
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



    cout << "Assembly RHS vector"<<endl;
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


     if (PETSC_TRUE)
    {
    PetscViewer viewer;
    ierr = PetscViewerASCIIOpen(comm, "Amat.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = MatView(A,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);
  }

   /* A is symmetric. Set symmetric flag to enable ICC/Cholesky preconditioner */
   //MatSetOption(A,MAT_SYMMETRIC,PETSC_TRUE);

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
   if (permute) {
     Mat Aperm;
     MatGetOrdering(A,ordering,&rowperm,&colperm);
     MatPermute(A,rowperm,colperm,&Aperm);
     VecPermute(b,colperm,PETSC_FALSE);
     MatDestroy(&A);
     A    = Aperm;               /* Replace original operator with permuted version */
   }

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


	if (amgsolver)
    {
        PetscReal coords[3*totrows];
     for (int n=0;n<TEqn.Grid().Num_Cells();n++)
         for (int dim=0;dim<3;dim++) coords[3*totrows+dim]=TEqn.Grid().Node_(n).Coords()[dim];

     ierr = KSPSetType(ksp, KSPCG);CHKERRQ(ierr);
     ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
     ierr = PCSetType(pc,PCGAMG);CHKERRQ(ierr);


     /* ierr = PCGAMGSetType(pc,"agg");CHKERRQ(ierr); */

     /* finish KSP/PC setup */
     ierr = KSPSetOperators(ksp, A, A, SAME_NONZERO_PATTERN);CHKERRQ(ierr);
     ierr = PCSetCoordinates(pc, 3, totrows, coords);CHKERRQ(ierr);

    }
    else
    {
        ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
        //ierr = PCSetType(pc,PCJACOBI);CHKERRQ(ierr);
        //ierr = PCSetType(pc,PCGAMG);CHKERRQ(ierr);
        //ierr = PCSetType(pc,PCICC);CHKERRQ(ierr);
        //ierr = PCSetType(pc,PCICC);CHKERRQ(ierr);
        ierr = PCSetType(pc,PCILU);CHKERRQ(ierr);
    }
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
//   KSPSetTolerances(ksp,1.e-5,1.e-50,PETSC_DEFAULT,
//                       PETSC_DEFAULT);

   KSPSetTolerances(ksp,1.e-2,PETSC_DEFAULT,PETSC_DEFAULT,
                       PETSC_DEFAULT);

   /*
     Set runtime options, e.g.,
         -ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
     These options will override those specified above as long as
     KSPSetFromOptions() is called _after_ any other customization
     routines.
   */
   KSPSetFromOptions(ksp);
//   KSPSetType(ksp,KSPBCGS);    //BiCGSTAB
   KSPSetType(ksp,KSPGMRES);    //GMRES

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                       Solve the linear system
      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    ittime_begin = clock();

    KSPSolve(ksp,b,x);

    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;

    cout << "PETSC Solving elapsed time: "<<ittime_spent<<endl;

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

  if (PETSC_TRUE) {
    PetscReal   norm,norm2;
    PetscViewer viewer;
    Vec         res;
    ierr = PetscViewerASCIIOpen(comm, "rhs.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = VecView(b,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);
    ierr = VecNorm(b, NORM_2, &norm2);CHKERRQ(ierr);

    ierr = PetscViewerASCIIOpen(comm, "solution.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = VecView(x,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);

    ierr = VecDuplicate(x, &res);CHKERRQ(ierr);
    ierr = MatMult(A, x, res);CHKERRQ(ierr);
    ierr = VecAXPY(b, -1.0, res);CHKERRQ(ierr);
    ierr = VecDestroy(&res);CHKERRQ(ierr);
    ierr = VecNorm(b,NORM_2,&norm);CHKERRQ(ierr);
    PetscPrintf(PETSC_COMM_WORLD,"[%d]%s |b-Ax|/|b|=%e, |b|=%e\n",0,__FUNCT__,norm/norm2,norm2);

    ierr = PetscViewerASCIIOpen(comm, "residual.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = VecView(b,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);
  }


       ofstream file;
    file.open("Out-Permuted.3D");
    file << " x y z val"<<endl;
    //sol=Solver.X();
    double sol;
    for (int i=0;i<totrows;i++)
    {
        //cout <<i<<endl;
        VecGetValues(x,1,&i,&sol);;
        for (int c=0;c<3;c++)   file << malla.Node_(i).comp[c]<<" ";
        file << sol<<endl;
    }



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

   /*
      Always call PetscFinalize() before exiting a program.  This routine
        - finalizes the PETSc libraries as well as MPI
        - provides summary and diagnostic information if certain runtime
          options are chosen (e.g., -log_summary).
   */



    file.close();
    PetscFinalize();

	return 0;
}

