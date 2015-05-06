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

#include "FluxSol.h"

static char help[] = "Creates a matrix from quadrilateral finite elements in 2D, Laplacian \n\
  -ne <size>       : problem size in number of elements (eg, -ne 31 gives 32^2 grid)\n\
  -alpha <v>      : scaling of material coeficient in embedded circle\n\n";

#include <petscksp.h>

#undef __FUNCT__
#define __FUNCT__ "main"

using namespace FluxSol;

int main(int argc,char **args)
{

    cout << "Reading cgns file..."<<endl;
	Fv_CC_Grid mesh(args[1]);
	//malla.ReadCGNS();

	mesh.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(mesh);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
        T.Boundaryfield().PatchField(p).AssignValue(wallvalue);

	T.Boundaryfield().PatchField(3).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	//Scalar k(1.);	//Difusion, puede ser un escalar
	//Scalar kdiff=material[0].k;
	Scalar kdiff=1.;
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(kdiff,T)==0.);
	cout<<"Solving system"<<endl;

    //Setting MAT values
	int numberofcomp=pow(3.,TEqn.Dim());
	int totrows=numberofcomp*TEqn.Num_Eqn();

    cout << "Number of comps: "<<numberofcomp<< ", Rows: "<< totrows<<endl;

    // PETSC VALUES //
      Mat            Amat,Pmat;
      PetscErrorCode ierr;
      PetscInt       i,m,M,its,Istart,Iend,j,Ii,bs,ix,ne=4;
      PetscReal      x,y,h;
      Vec            xx,bb;
      KSP            ksp;
      PetscReal      soft_alpha = 1.e-3;
    #if defined(PETSC_USE_LOG)
      PetscLogStage stage;
    #endif
    #define DIAG_S 0.0   //Softening
      MPI_Comm       comm;
      PetscMPIInt    npe,mype;
      PC             pc;


      PetscInitialize(&argc,&args,(char*)0,help);
      comm = PETSC_COMM_WORLD;
      ierr  = MPI_Comm_rank(comm, &mype);CHKERRQ(ierr);
      ierr  = MPI_Comm_size(comm, &npe);CHKERRQ(ierr);
      ierr  = PetscOptionsGetInt(NULL,"-ne",&ne,NULL);CHKERRQ(ierr);
      h     = 1./ne;
      /* ne*ne; number of global elements */
      ierr = PetscOptionsGetReal(NULL,"-alpha",&soft_alpha,NULL);CHKERRQ(ierr);
      M    = totrows; /* global number of nodes */
      /* create stiffness matrix */
      //PetscErrorCode  MatCreateAIJ(MPI_Comm comm,PetscInt m,PetscInt n,PetscInt M,PetscInt N,
                                     //PetscInt d_nz,const PetscInt d_nnz[],PetscInt o_nz,const PetscInt o_nnz[],Mat *A)
    //M	- number of global rows (or PETSC_DETERMINE to have calculated if m is given)
    //N	- number of global columns (or PETSC_DETERMINE to have calculated if n is given)
    //d_nz	- number of nonzeros per row in DIAGONAL portion of local submatrix (same value is used for all local rows)
    //d_nnz	- array containing the number of nonzeros in the various rows of the DIAGONAL portion of the local submatrix (possibly different for each row) or NULL, if d_nz is used to specify the nonzero structure. The size of this array is equal to the number of local rows, i.e 'm'.
    //o_nz	- number of nonzeros per row in the OFF-DIAGONAL portion of local submatrix (same value is used for all local rows).
    //o_nnz	- array containing the number of nonzeros in the various rows of the OFF-DIAGONAL portion of the local submatrix (possibly different for each row) or NULL, if o_nz is used to specify the nonzero structure. The size of this array is equal to the number of local rows, i.e 'm'.


    //TO MODIFY
    vector <int> nonzerosperrow;
	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
    {
        int width=(TEqn.Eqn(e).Width()-1)*numberofcomp+1;
        //int nz=(TEqn.Eqn(e).An().size()-1)*numberofcomp+1;
        for (int dim=0;dim<numberofcomp;dim++)  nonzerosperrow.push_back(5);
    }

  ierr = MatCreateAIJ(comm,PETSC_DECIDE,PETSC_DECIDE,M,M,
                      18,NULL,6,NULL,&Amat);CHKERRQ(ierr);
  ierr = MatCreateAIJ(comm,PETSC_DECIDE,PETSC_DECIDE,M,M,
                      18,NULL,6,NULL,&Pmat);CHKERRQ(ierr);
  ierr = MatGetOwnershipRange(Amat,&Istart,&Iend);CHKERRQ(ierr);
  m    = Iend-Istart;
  bs   = 1;
  /* Generate vectors */
  ierr = VecCreate(comm,&xx);CHKERRQ(ierr);
  ierr = VecSetSizes(xx,m,M);CHKERRQ(ierr);
  ierr = VecSetFromOptions(xx);CHKERRQ(ierr);
  ierr = VecDuplicate(xx,&bb);CHKERRQ(ierr);
  ierr = VecSet(bb,.0);CHKERRQ(ierr);

    double value=1.;
    PetscInt index=0;

    //cout << "Setting Initial Mat values..."<<endl;
    //ierr=MatSetValues(this->A,1,&row,1,&col,&value,INSERT_VALUES);
    MatSetValues(Amat,1,&index,1,&index,&value,INSERT_VALUES);

    cout << "Inserting Matrix Values"<<endl;

    cout << "Assembying Eqns"<<endl;
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
                MatSetValues(Amat,1,&r,1, &r, &col[0],INSERT_VALUES);    //An is scalar
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
                MatSetValues(Amat,1,&r,1, &c, &col[0],INSERT_VALUES);    //An is scalar
                //Solver.SetMatVal(row+dim, columnid+dim, col[0]);    //An is scalar
            }

		}//En of neighbours


	}//End of cells	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2

//
//
//    PetscReal coords[3*m];
//    /* forms the element stiffness for the Laplacian and coordinates */
//    for (Ii=Istart,ix=0; Ii<Iend; Ii++,ix++) {
//      j = Ii/(ne+1); i = Ii%(ne+1);
//      /* coords */
//      x            = h*(Ii % (ne+1)); y = h*(Ii/(ne+1));
//      coords[2*ix] = x; coords[2*ix+1] = y;
////      if (i<ne && j<ne) {
////        PetscInt jj,ii,idx[4] = {Ii, Ii+1, Ii + (ne+1) + 1, Ii + (ne+1)};
////        /* radius */
////        PetscReal radius = PetscSqrtScalar((x-.5+h/2)*(x-.5+h/2) + (y-.5+h/2)*(y-.5+h/2));
////        PetscReal alpha  = 1.0;
////        if (radius < 0.25) alpha = soft_alpha;
////
////
////        for (ii=0; ii<4; ii++) {
////          for (jj=0; jj<4; jj++) DD[ii][jj] = alpha*DD1[ii][jj];
////        }
////        ierr = MatSetValues(Pmat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
////        if (j>0) {
////          ierr = MatSetValues(Amat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
////        } else {
////          /* a BC */
////          for (ii=0;ii<4;ii++) {
////            for (jj=0;jj<4;jj++) DD[ii][jj] = alpha*DD2[ii][jj];
////          }
////          ierr = MatSetValues(Amat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
////        }
////      }
////      if (j>0) {
////        PetscScalar v  = h*h;
////        PetscInt    jj = Ii;
////        ierr = VecSetValues(bb,1,&jj,&v,INSERT_VALUES);CHKERRQ(ierr);
////      }
////    }
//
//    //for (Ii=Istart,ix=0; Ii<Iend; Ii++,ix++)
//    for (int n=0;n<mesh.Num_Cells();n++)
//    {
//
//        for (int dim=0;dim<3;dim++) coords[3*n+dim]=mesh.Node_(n).Coords()[dim];
//         //coords[2*ix] = x; coords[2*ix+1] = y;
//    }
//
//    ierr = MatAssemblyBegin(Amat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
//    ierr = MatAssemblyEnd(Amat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
//    ierr = MatAssemblyBegin(Pmat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
//    ierr = MatAssemblyEnd(Pmat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
//    ierr = VecAssemblyBegin(bb);CHKERRQ(ierr);
//    ierr = VecAssemblyEnd(bb);CHKERRQ(ierr);
//
//    /* Setup solver */
//    ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
//    ierr = KSPSetType(ksp, KSPCG);CHKERRQ(ierr);
//    ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
//    ierr = PCSetType(pc,PCGAMG);CHKERRQ(ierr);
//    ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);
//
//    /* ierr = PCGAMGSetType(pc,"agg");CHKERRQ(ierr); */
//
//    /* finish KSP/PC setup */
//    ierr = KSPSetOperators(ksp, Amat, Amat, SAME_NONZERO_PATTERN);CHKERRQ(ierr);
//    ierr = PCSetCoordinates(pc, 2, m, coords);CHKERRQ(ierr);
//  }
//
//  if (!PETSC_TRUE) {
//    PetscViewer viewer;
//    ierr = PetscViewerASCIIOpen(comm, "Amat.m", &viewer);CHKERRQ(ierr);
//    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
//    ierr = MatView(Amat,viewer);CHKERRQ(ierr);
//    ierr = PetscViewerDestroy(&viewer);
//  }
//
//  /* solve */
//#if defined(PETSC_USE_LOG)
//  ierr = PetscLogStageRegister("Solve", &stage);CHKERRQ(ierr);
//  ierr = PetscLogStagePush(stage);CHKERRQ(ierr);
//#endif
//  ierr = VecSet(xx,.0);CHKERRQ(ierr);
//
//  ierr = KSPSolve(ksp,bb,xx);CHKERRQ(ierr);
//
//#if defined(PETSC_USE_LOG)
//  ierr = PetscLogStagePop();CHKERRQ(ierr);
//#endif
//
//  ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
//
//  if (!PETSC_TRUE) {
//    PetscReal   norm,norm2;
//    PetscViewer viewer;
//    Vec         res;
//    ierr = PetscViewerASCIIOpen(comm, "rhs.m", &viewer);CHKERRQ(ierr);
//    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
//    ierr = VecView(bb,viewer);CHKERRQ(ierr);
//    ierr = PetscViewerDestroy(&viewer);
//    ierr = VecNorm(bb, NORM_2, &norm2);CHKERRQ(ierr);
//
//    ierr = PetscViewerASCIIOpen(comm, "solution.m", &viewer);CHKERRQ(ierr);
//    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
//    ierr = VecView(xx,viewer);CHKERRQ(ierr);
//    ierr = PetscViewerDestroy(&viewer);
//
//    ierr = VecDuplicate(xx, &res);CHKERRQ(ierr);
//    ierr = MatMult(Amat, xx, res);CHKERRQ(ierr);
//    ierr = VecAXPY(bb, -1.0, res);CHKERRQ(ierr);
//    ierr = VecDestroy(&res);CHKERRQ(ierr);
//    ierr = VecNorm(bb,NORM_2,&norm);CHKERRQ(ierr);
//    PetscPrintf(PETSC_COMM_WORLD,"[%d]%s |b-Ax|/|b|=%e, |b|=%e\n",0,__FUNCT__,norm/norm2,norm2);
//
//    ierr = PetscViewerASCIIOpen(comm, "residual.m", &viewer);CHKERRQ(ierr);
//    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
//    ierr = VecView(bb,viewer);CHKERRQ(ierr);
//    ierr = PetscViewerDestroy(&viewer);
//  }
//
//  /* Free work space */
//  ierr = KSPDestroy(&ksp);CHKERRQ(ierr);
//  ierr = VecDestroy(&xx);CHKERRQ(ierr);
//  ierr = VecDestroy(&bb);CHKERRQ(ierr);
//  ierr = MatDestroy(&Amat);CHKERRQ(ierr);
//  ierr = MatDestroy(&Pmat);CHKERRQ(ierr);
//
//  ierr = PetscFinalize();
  return 0;
}

