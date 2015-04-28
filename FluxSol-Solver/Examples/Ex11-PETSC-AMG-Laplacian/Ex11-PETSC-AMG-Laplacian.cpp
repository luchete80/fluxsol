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
int main(int argc,char **args)
{

    cout << "Reading cgns file..."<<endl;
	Fv_CC_Grid mesh(argv[1]);
	//malla.ReadCGNS();

	mesh.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(mesh);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
        T.Boundaryfield().PatchField(p).AssignValue(wallvalue);

	T.Boundaryfield().PatchField(3).AssignValue(topvalue);



// PETSC VALUES //
  Mat            Amat,Pmat;
  PetscErrorCode ierr;
  PetscInt       i,m,M,its,Istart,Iend,j,Ii,bs,ix,ne=4;
  PetscReal      x,y,h;
  Vec            xx,bb;
  KSP            ksp;
  PetscReal      soft_alpha = 1.e-3;
  MPI_Comm       comm;
  PetscMPIInt    npe,mype;
  PC             pc;
  PetscScalar    DD[4][4],DD2[4][4];
#if defined(PETSC_USE_LOG)
  PetscLogStage stage;
#endif
#define DIAG_S 0.0
  PetscScalar DD1[4][4] = { {5.0+DIAG_S, -2.0, -1.0, -2.0},
                            {-2.0, 5.0+DIAG_S, -2.0, -1.0},
                            {-1.0, -2.0, 5.0+DIAG_S, -2.0},
                            {-2.0, -1.0, -2.0, 5.0+DIAG_S} };

  PetscInitialize(&argc,&args,(char*)0,help);
  comm = PETSC_COMM_WORLD;
  ierr  = MPI_Comm_rank(comm, &mype);CHKERRQ(ierr);
  ierr  = MPI_Comm_size(comm, &npe);CHKERRQ(ierr);
  ierr  = PetscOptionsGetInt(NULL,"-ne",&ne,NULL);CHKERRQ(ierr);
  h     = 1./ne;
  /* ne*ne; number of global elements */
  ierr = PetscOptionsGetReal(NULL,"-alpha",&soft_alpha,NULL);CHKERRQ(ierr);
  M    = (ne+1)*(ne+1); /* global number of nodes */
  /* create stiffness matrix */
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


    //Setting MAT values


	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
	{
	    //Width Assign

        //cout << "Assemblying Eqn "<<e<<endl;
		vector <double> ap=TEqn.Eqn(e).Ap().Comp();
		Scalar ap_sc=TEqn.Eqn(e).Ap();
		Scalar value;
		int width=(TEqn.Eqn(e).Width()-1)*numberofcomp+1;
//		//int width=TEqn.Eqn(e).Width()*numberofcomp;   // THIS IS WRONG
//
		int sparsecol=0;
		int row=e*numberofcomp;

//        cout << "Eqn " <<e << "Width: "<<TEqn.Eqn(e).Width()<<endl;

		for (int dim=0;dim<numberofcomp;dim++)
        {
            //cout <<"Row "<< row+dim+1<<" length: "<<width<<endl;
            //Q_SetLen(&K,row+dim+1,width);
        }

		vector <double> nullval;
		nullval.assign(numberofcomp,0.);

		//This id is relative to the cell, not the column
		int fisrt_nonzero_col=TEqn.Eqn(e).MinNeigbourId();
		int realcellid;



		//Look trough entire width for neighbours id
		for (int width_cells=0;width_cells<TEqn.Eqn(e).Width();width_cells++)
		{
			realcellid=width_cells+fisrt_nonzero_col;

			vector <double> col;
			int columnid;
			//Found central
			bool foundcell=false;
			int localneighbourfound=FluxSol::SearchVal(realcellid,TEqn.Eqn(e).NeighboursIds());
			if (TEqn.Eqn(e).Id()==realcellid)
			{col=ap;columnid=row;foundcell=true;}//row is equal to
			//Neighbours ids are not neccesarily ordered, then must search for cellid in all neighbours
			//else if (TEqn.Eqn(e).Neighbour(neighb)==realcellid) //Found an
			else if(localneighbourfound>-1)
			{col=TEqn.Eqn(e).An(localneighbourfound).Comp();columnid=numberofcomp*realcellid;foundcell=true;}
			else //column index is not a neighbour neither central cell
			{columnid=numberofcomp*realcellid;}

			//Write Matrix
			if (foundcell)
			{
                //cout << "Found Cell " <<endl;
				for (int dim=0;dim<numberofcomp;dim++)
				{
                    //INFO
                    //cout << "(Indexes From 1)  K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << 0.0<<endl;
                    //cout << "(From zero) Sparse col: " << numberofcomp*width_cells <<endl;

					//Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,col[0]);
					Solver.SetMatVal(row+dim, columnid+dim, col[0]);

				}

			}
			else
			{
                //cout << "Cell Not Found" <<endl;
				for (int dim=0;dim<numberofcomp;dim++)
                {
                    Solver.SetMatVal(row+dim, columnid+dim, 0.);
					//Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,0.0);
                    //INFO
                    //cout << "(Indexes From 1)  K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << 0.0<<endl;
                    //cout << "(From zero) Sparse col: " << numberofcomp*width_cells <<endl;

                }

			}


		}//En of width


	}//End of cells






  /* generate element matrices */
  {
    FILE *file;
    char fname[] = "data/elem_2d_therm.txt";
    file = fopen(fname, "r");
    if (file == 0) {
      DD1[0][0] =  0.66666666666666663;
      DD1[0][1] = -0.16666666666666669;
      DD1[0][2] = -0.33333333333333343;
      DD1[0][3] = -0.16666666666666666;
      DD1[1][0] = -0.16666666666666669;
      DD1[1][1] =  0.66666666666666663;
      DD1[1][2] = -0.16666666666666666;
      DD1[1][3] = -0.33333333333333343;
      DD1[2][0] = -0.33333333333333343;
      DD1[2][1] = -0.16666666666666666;
      DD1[2][2] =  0.66666666666666663;
      DD1[2][3] = -0.16666666666666663;
      DD1[3][0] = -0.16666666666666666;
      DD1[3][1] = -0.33333333333333343;
      DD1[3][2] = -0.16666666666666663;
      DD1[3][3] =  0.66666666666666663;
    } else {
      for (i=0;i<4;i++) {
        for (j=0;j<4;j++) {
          ierr = fscanf(file, "%le", &DD1[i][j]);
        }
      }
    }
    /* BC version of element */
    for (i=0;i<4;i++) {
      for (j=0;j<4;j++) {
        if (i<2 || j < 2) {
          if (i==j) DD2[i][j] = .1*DD1[i][j];
          else DD2[i][j] = 0.0;
        } else DD2[i][j] = DD1[i][j];
      }
    }
  }
  {
    PetscReal coords[2*m];
    /* forms the element stiffness for the Laplacian and coordinates */
    for (Ii=Istart,ix=0; Ii<Iend; Ii++,ix++) {
      j = Ii/(ne+1); i = Ii%(ne+1);
      /* coords */
      x            = h*(Ii % (ne+1)); y = h*(Ii/(ne+1));
      coords[2*ix] = x; coords[2*ix+1] = y;
      if (i<ne && j<ne) {
        PetscInt jj,ii,idx[4] = {Ii, Ii+1, Ii + (ne+1) + 1, Ii + (ne+1)};
        /* radius */
        PetscReal radius = PetscSqrtScalar((x-.5+h/2)*(x-.5+h/2) + (y-.5+h/2)*(y-.5+h/2));
        PetscReal alpha  = 1.0;
        if (radius < 0.25) alpha = soft_alpha;


        for (ii=0; ii<4; ii++) {
          for (jj=0; jj<4; jj++) DD[ii][jj] = alpha*DD1[ii][jj];
        }
        ierr = MatSetValues(Pmat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
        if (j>0) {
          ierr = MatSetValues(Amat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
        } else {
          /* a BC */
          for (ii=0;ii<4;ii++) {
            for (jj=0;jj<4;jj++) DD[ii][jj] = alpha*DD2[ii][jj];
          }
          ierr = MatSetValues(Amat,4,idx,4,idx,(const PetscScalar*)DD,ADD_VALUES);CHKERRQ(ierr);
        }
      }
      if (j>0) {
        PetscScalar v  = h*h;
        PetscInt    jj = Ii;
        ierr = VecSetValues(bb,1,&jj,&v,INSERT_VALUES);CHKERRQ(ierr);
      }
    }
    ierr = MatAssemblyBegin(Amat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(Amat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyBegin(Pmat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(Pmat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = VecAssemblyBegin(bb);CHKERRQ(ierr);
    ierr = VecAssemblyEnd(bb);CHKERRQ(ierr);

    /* Setup solver */
    ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
    ierr = KSPSetType(ksp, KSPCG);CHKERRQ(ierr);
    ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
    ierr = PCSetType(pc,PCGAMG);CHKERRQ(ierr);
    ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

    /* ierr = PCGAMGSetType(pc,"agg");CHKERRQ(ierr); */

    /* finish KSP/PC setup */
    ierr = KSPSetOperators(ksp, Amat, Amat, SAME_NONZERO_PATTERN);CHKERRQ(ierr);
    ierr = PCSetCoordinates(pc, 2, m, coords);CHKERRQ(ierr);
  }

  if (!PETSC_TRUE) {
    PetscViewer viewer;
    ierr = PetscViewerASCIIOpen(comm, "Amat.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = MatView(Amat,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);
  }

  /* solve */
#if defined(PETSC_USE_LOG)
  ierr = PetscLogStageRegister("Solve", &stage);CHKERRQ(ierr);
  ierr = PetscLogStagePush(stage);CHKERRQ(ierr);
#endif
  ierr = VecSet(xx,.0);CHKERRQ(ierr);

  ierr = KSPSolve(ksp,bb,xx);CHKERRQ(ierr);

#if defined(PETSC_USE_LOG)
  ierr = PetscLogStagePop();CHKERRQ(ierr);
#endif

  ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);

  if (!PETSC_TRUE) {
    PetscReal   norm,norm2;
    PetscViewer viewer;
    Vec         res;
    ierr = PetscViewerASCIIOpen(comm, "rhs.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = VecView(bb,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);
    ierr = VecNorm(bb, NORM_2, &norm2);CHKERRQ(ierr);

    ierr = PetscViewerASCIIOpen(comm, "solution.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = VecView(xx,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);

    ierr = VecDuplicate(xx, &res);CHKERRQ(ierr);
    ierr = MatMult(Amat, xx, res);CHKERRQ(ierr);
    ierr = VecAXPY(bb, -1.0, res);CHKERRQ(ierr);
    ierr = VecDestroy(&res);CHKERRQ(ierr);
    ierr = VecNorm(bb,NORM_2,&norm);CHKERRQ(ierr);
    PetscPrintf(PETSC_COMM_WORLD,"[%d]%s |b-Ax|/|b|=%e, |b|=%e\n",0,__FUNCT__,norm/norm2,norm2);

    ierr = PetscViewerASCIIOpen(comm, "residual.m", &viewer);CHKERRQ(ierr);
    ierr = PetscViewerSetFormat(viewer, PETSC_VIEWER_ASCII_MATLAB);CHKERRQ(ierr);
    ierr = VecView(bb,viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);
  }

  /* Free work space */
  ierr = KSPDestroy(&ksp);CHKERRQ(ierr);
  ierr = VecDestroy(&xx);CHKERRQ(ierr);
  ierr = VecDestroy(&bb);CHKERRQ(ierr);
  ierr = MatDestroy(&Amat);CHKERRQ(ierr);
  ierr = MatDestroy(&Pmat);CHKERRQ(ierr);

  ierr = PetscFinalize();
  return 0;
}

