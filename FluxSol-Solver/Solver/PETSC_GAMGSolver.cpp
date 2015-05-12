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
// THIS IS LIKE EXAMPLE 54 FROM PETSC
// But sequential

#include "PETSC_GAMGSolver.h"
#include "SistEcuac.h"

using namespace FluxSol;

template <typename number>
template <typename T>
void PETSC_GAMGSolver<number>::Solve(EqnSystem < T > &TEqn)
{

    clock_t ittime_begin, ittime_end, ittime_temp;
    double ittime_spent;

    ittime_end = clock();


    // //Setting MAT values
	 int numberofcomp=pow(3.,TEqn.Dim());
	 int totrows=numberofcomp*TEqn.Num_Eqn();
    cout << "GAMG Solver..."<<endl;
     cout << "Number of comps: "<<numberofcomp<< ", Rows: "<< totrows<<endl;


//     #if defined(PETSC_USE_LOG)
//       PetscLogStage stage;
//     #endif
//     #define DIAG_S 0.0   //Softening


      // Petsc  (&argc,&args,(char*)0,help);
      comm = PETSC_COMM_WORLD;
      this->ierr  = MPI_Comm_rank(comm, &rank);CHKERRQ(this->ierr);     //ATENTION!!!: Is like mype in example 54
      this->ierr  = MPI_Comm_size(comm, &npe);CHKERRQ(this->ierr);

      // ierr  = PetscOptionsGetInt(NULL,"-ne",&ne,NULL);CHKERRQ(ierr);
      //h     = 1./ne;
      /* ne*ne; number of global elements */
      // ierr = PetscOptionsGetReal(NULL,"-alpha",&soft_alpha,NULL);CHKERRQ(ierr);
      M    = totrows; /* global number of nodes */
      // /* create stiffness matrix */
      // //PetscErrorCode  MatCreateAIJ(MPI_Comm comm,PetscInt m,PetscInt n,PetscInt M,PetscInt N,
                                     // //PetscInt d_nz,const PetscInt d_nnz[],PetscInt o_nz,const PetscInt o_nnz[],Mat *A)
    // //M	- number of global rows (or PETSC_DETERMINE to have calculated if m is given)
    // //N	- number of global columns (or PETSC_DETERMINE to have calculated if n is given)
    // //d_nz	- number of nonzeros per row in DIAGONAL portion of local submatrix (same value is used for all local rows)
    // //d_nnz	- array containing the number of nonzeros in the various rows of the DIAGONAL portion of the local submatrix (possibly different for each row) or NULL, if d_nz is used to specify the nonzero structure. The size of this array is equal to the number of local rows, i.e 'm'.
    // //o_nz	- number of nonzeros per row in the OFF-DIAGONAL portion of local submatrix (same value is used for all local rows).
    // //o_nnz	- array containing the number of nonzeros in the various rows of the OFF-DIAGONAL portion of the local submatrix (possibly different for each row) or NULL, if o_nz is used to specify the nonzero structure. The size of this array is equal to the number of local rows, i.e 'm'.


    // //TO MODIFY
    // vector <int> nonzerosperrow;
	// for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
    // {
        // int width=(TEqn.Eqn(e).Width()-1)*numberofcomp+1;
        // //int nz=(TEqn.Eqn(e).An().size()-1)*numberofcomp+1;
        // for (int dim=0;dim<numberofcomp;dim++)  nonzerosperrow.push_back(5);
    // }

   this->ierr = MatCreateAIJ(comm,PETSC_DECIDE,PETSC_DECIDE,M,M,
                       18,NULL,6,NULL,&this->A);CHKERRQ(this->ierr);
   this->ierr = MatCreateAIJ(comm,PETSC_DECIDE,PETSC_DECIDE,M,M,
                       18,NULL,6,NULL,&this->Pmat);CHKERRQ(this->ierr);
   this->ierr = MatGetOwnershipRange(this->A,&Istart,&Iend);CHKERRQ(this->ierr);
   m    = Iend-Istart;
   bs   = 1;
   /* Generate vectors */
   this->ierr = VecCreate(comm,&this->x);CHKERRQ(this->ierr);
   this->ierr = VecSetSizes(this->x,m,M);CHKERRQ(this->ierr);
   this->ierr = VecSetFromOptions(this->x);CHKERRQ(this->ierr);
   this->ierr = VecDuplicate(this->x,&this->b);CHKERRQ(this->ierr);
   this->ierr = VecSet(this->b,.0);CHKERRQ(this->ierr);

    // double value=1.;
    // PetscInt index=0;

     cout << "Istart - Iend = m: " << m <<endl;
     cout << "Istart: "<<Istart<<"; IEnd: "<<Iend<<endl;

    // cout << "Inserting Matrix Values"<<endl;

    // cout << "Assembying Eqns"<<endl;
    // TO MODIFY FOR MPI
	 for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
	 {
	     //Width Assign

         //cout << "Assemblying Eqn "<<e<<endl;
		 vector <double> ap=TEqn.Eqn(e).Ap().Comp();
		 Scalar ap_sc=TEqn.Eqn(e).Ap();
		 Scalar value;

		 int row=e*numberofcomp;

         vector <double> col;
         //CENTRAL COEFFS
         col=ap;
         for (int dim=0;dim<numberofcomp;dim++)
             {
                 PetscInt r,c;
                 r=row+dim;
                 MatSetValues(this->A,1,&r,1, &r, &col[0],INSERT_VALUES);    //An is scalar
             }
             //Solver.SetMatVal(&Amat,row+dim, row+dim, col[0],INSERT_VALUES);    //An is scalar
             //MatSetValues(this->A,1,&row,1,&col,&value,INSERT_VALUES);


		 //Look trough entire width for neighbours id
		 //The main idea is to look through eqn width
		 //Real cell id is taken, and then are watched all neighbours to check if each real cell id belongs to neighbours vector
		 for (int nc=0;nc<TEqn.Eqn(e).Num_Neighbours();nc++)
		 {
			 int realcellid=TEqn.Eqn(e).Neighbour(nc);   //Wich cell

			 col=TEqn.Eqn(e).An(nc).Comp();
			 int columnid=numberofcomp*realcellid;

             //cout << "Found Cell " <<endl;
             for (int dim=0;dim<numberofcomp;dim++)
             {
                 PetscInt r,c;
                 r=row+dim;c=columnid+dim;
                 MatSetValues(this->A,1,&r,1, &c, &col[0],INSERT_VALUES);    //An is scalar
                 //Solver.SetMatVal(row+dim, columnid+dim, col[0]);    //An is scalar
             }

		 }//En of neighbours


	 }//End of cells

	     //cout << "R vector (from zero)"<<endl;
	 //V_SetAllCmp(&R,0.0);
	 for (int e=0;e<TEqn.Num_Eqn();e++)
	 {
	     //cout << "Eqn " << e<<endl;
	     //cout << "[" <<e<<"]: "  ;
		 vector <double> source=TEqn.Eqn(e).Source().Comp();
		 for (int dim=0;dim<numberofcomp;dim++)
         {
             int row=e*numberofcomp+dim;
             //Solver.SetbValues(e*numberofcomp+dim, source[dim]);

             //if (source[dim]>0) cout << "<0 Found!!"<<endl;
             VecSetValues(this->b,1,&row,&source[dim],INSERT_VALUES);
         }
         //cout << endl;
	 }


// //
// //
      PetscReal coords[3*m];
// //    /* forms the element stiffness for the Laplacian and coordinates */
// //    for (Ii=Istart,ix=0; Ii<Iend; Ii++,ix++) {
// //      j = Ii/(ne+1); i = Ii%(ne+1);
// //      /* coords */
// //      x            = h*(Ii % (ne+1)); y = h*(Ii/(ne+1));
// //      coords[2*ix] = x; coords[2*ix+1] = y;
// ////      if (i<ne && j<ne) {
// ////        PetscInt jj,ii,idx[4] = {Ii, Ii+1, Ii + (ne+1) + 1, Ii + (ne+1)};
// ////        /* radius */
// ////        PetscReal radius = PetscSqrtScalar((x-.5+h/2)*(x-.5+h/2) + (y-.5+h/2)*(y-.5+h/2));
// ////        PetscReal alpha  = 1.0;
// ////        if (radius < 0.25) alpha = soft_alpha;
// ////
// ////
// ////        for (ii=0; ii<4; ii++) {
// ////          for (jj=0; jj<4; jj++) DD[ii][jj] = alpha*DD1[ii][jj];
// ////        }
// ////        ierr = MatSetValues(Pmat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
// ////        if (j>0) {
// ////          ierr = MatSetValues(Amat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
// ////        } else {
// ////          /* a BC */
// ////          for (ii=0;ii<4;ii++) {
// ////            for (jj=0;jj<4;jj++) DD[ii][jj] = alpha*DD2[ii][jj];
// ////          }
// ////          ierr = MatSetValues(Amat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
// ////        }
// ////      }
// ////      if (j>0) {
// ////        PetscScalar v  = h*h;
// ////        PetscInt    jj = Ii;
// ////        ierr = VecSetValues(bb,1,&jj,&v,INSERT_VALUES);CHKERRQ(ierr);
// ////      }
// ////    }
// //
    // //for (Ii=Istart,ix=0; Ii<Iend; Ii++,ix++)
    // cout << "Creating coords ..." <<endl;
    cout << "Num of Cells" << TEqn.Grid().Num_Cells()<<endl;
     for (int n=0;n<TEqn.Grid().Num_Cells();n++)
     {

         for (int dim=0;dim<3;dim++) coords[3*n+dim]=TEqn.Grid().Node_(n).Coords()[dim];

     }

    // cout << "Coordinates created. "<<endl;

     this->ierr = MatAssemblyBegin(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(this->ierr);
     this->ierr = MatAssemblyEnd(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(this->ierr);
     this->ierr = MatAssemblyBegin(Pmat,MAT_FINAL_ASSEMBLY);CHKERRQ(this->ierr);
     this->ierr = MatAssemblyEnd(Pmat,MAT_FINAL_ASSEMBLY);CHKERRQ(this->ierr);
     this->ierr = VecAssemblyBegin(this->b);CHKERRQ(this->ierr);
     this->ierr = VecAssemblyEnd(this->b);CHKERRQ(this->ierr);

     /* Setup solver */
     this->ierr = KSPCreate(PETSC_COMM_WORLD,&this->ksp);CHKERRQ(this->ierr);
     this->ierr = KSPSetType(this->ksp, KSPCG);CHKERRQ(this->ierr);
     this->ierr = KSPGetPC(this->ksp,&pc);CHKERRQ(this->ierr);
     this->ierr = PCSetType(pc,PCGAMG);CHKERRQ(this->ierr);
     this->ierr = KSPSetFromOptions(this->ksp);CHKERRQ(this->ierr);

     //this->ierr = KSPSetTolerances(this->ksp,1.e-5,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(this->ierr);

     /* ierr = PCGAMGSetType(pc,"agg");CHKERRQ(ierr); */

     /* finish KSP/PC setup */
     this->ierr = KSPSetOperators(this->ksp, this->A, this->A, SAME_NONZERO_PATTERN);CHKERRQ(this->ierr);
     this->ierr = PCSetCoordinates(pc, 3, m, coords);CHKERRQ(this->ierr);


   if (PETSC_TRUE)
     {
     PetscViewer viewer;
     this->ierr = PetscViewerASCIIOpen(comm, "Amat.m", &viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(this->ierr);
     this->ierr = MatView(this->A,viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerDestroy(&viewer);
   }

  // /* solve */
// #if defined(PETSC_USE_LOG)
//   this->ierr = PetscLogStageRegister("Solve", &stage);CHKERRQ(this->ierr);
//   this->ierr = PetscLogStagePush(stage);CHKERRQ(this->ierr);
// #endif
   this->ierr = VecSet(this->x,.0);CHKERRQ(this->ierr);

     ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;

     cout << "PETSC Assemblying elapsed time: "<<ittime_spent<<endl;
     ittime_end = clock();

     this->ierr = KSPSolve(this->ksp,this->b,this->x);CHKERRQ(this->ierr);


     ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;

     cout << "PETSC Solving elapsed time: "<<ittime_spent<<endl;


    this->ierr = KSPView(this->ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(this->ierr);

// #if defined(PETSC_USE_LOG)
  // ierr = PetscLogStagePop();CHKERRQ(ierr);
// #endif

// cout << "Getting Solver Vals..."<<endl;


    // ofstream file;     //Previously this was inside each function
    // file.open("Out.3D");
    // file << " x y z val"<<endl;

    // for (int i=0;i<TEqn.Num_Eqn();i++)
    // {
        // PetscInt row=numberofcomp*i;
        // double val[1];
        // VecGetValues(xx,1,&row,&val[0]);
        // for (int c=0;c<3;c++)   file << mesh.Node_(i).comp[c]<<" ";
        // file << val[0] <<endl;
    // }


    // file.close();

  // ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);

   if (PETSC_TRUE) {
     PetscReal   norm,norm2;
     PetscViewer viewer;
     Vec         res;
     this->ierr = PetscViewerASCIIOpen(comm, "rhs.m", &viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(this->ierr);
     this->ierr = VecView(this->b,viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerDestroy(&viewer);
     this->ierr = VecNorm(this->b, NORM_2, &norm2);CHKERRQ(this->ierr);

     this->ierr = PetscViewerASCIIOpen(comm, "solution.m", &viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(this->ierr);
     this->ierr = VecView(this->x,viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerDestroy(&viewer);

     this->ierr = VecDuplicate(this->x, &res);CHKERRQ(this->ierr);
     this->ierr = MatMult(this->A, this->x, res);CHKERRQ(this->ierr);
     this->ierr = VecAXPY(this->b, -1.0, res);CHKERRQ(this->ierr);
     this->ierr = VecDestroy(&res);CHKERRQ(this->ierr);
     this->ierr = VecNorm(this->b,NORM_2,&norm);CHKERRQ(this->ierr);
     PetscPrintf(PETSC_COMM_WORLD,"[%d]%s |b-Ax|/|b|=%e, |b|=%e\n",0,__FUNCT__,norm/norm2,norm2);

     this->ierr = PetscViewerASCIIOpen(comm, "residual.m", &viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(this->ierr);
     this->ierr = VecView(this->b,viewer);CHKERRQ(this->ierr);
     this->ierr = PetscViewerDestroy(&viewer);
   }

   /* Free work space */
   this->ierr = KSPDestroy(&this->ksp);CHKERRQ(this->ierr);
   this->ierr = VecDestroy(&this->x);CHKERRQ(this->ierr);
   this->ierr = VecDestroy(&this->b);CHKERRQ(this->ierr);
   this->ierr = MatDestroy(&this->A);CHKERRQ(this->ierr);
   this->ierr = MatDestroy(&Pmat);CHKERRQ(this->ierr);

   //this->ierr = PetscFinalize();

} // Solve


#include "PETSC_GAMGSolver.inst"
