/************************************************************************

Copyright 2012-2013 Luciano Buglioni
                    Pablo Zitelli

Contact:
    luciano.buglioni@gmail.com
    pzitelli@gmail.com

This file is a part of FluxSol

FluxSol is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

Free CFD is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

For a copy of the GNU General Public License,
see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "PETSC_Solver.h"

namespace FluxSol
{

template <typename number>
void PETSC_KSP_Solver<number>::PETSC_Init()
{
	int n=this->matdim;

	cout << "Initializing Solver..."<<endl;

	//If no values passed
	int argc;
	char **args;

	char help[100];

	argc=0;
	args=NULL;

	PetscInitialize(&argc,&args,(char *)0,help);

    PetscMPIInt size,rank;
	ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
	ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);CHKERRQ(ierr);
	if (size != 1) SETERRQ(PETSC_COMM_WORLD,1,"This is a uniprocessor example only!");
	//MPI_Init()

	ierr = PetscOptionsGetInt(PETSC_NULL,"-n",&n,PETSC_NULL);CHKERRQ(ierr);
	ierr = PetscOptionsGetBool(PETSC_NULL,"-nonzero_guess",&nonzeroguess,PETSC_NULL);CHKERRQ(ierr);

	/*
	Create vectors. Note that we form 1 vector from scratch and
	then duplicate as needed.
	*/
	cout <<"Creating matrix and vectors"<<endl;
	ierr = VecCreate(PETSC_COMM_WORLD,&x);CHKERRQ(ierr);
	ierr = PetscObjectSetName((PetscObject) x, "Solution");CHKERRQ(ierr);
	ierr = VecSetSizes(x,PETSC_DECIDE,n);CHKERRQ(ierr);
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);
	ierr = VecDuplicate(x,&b);CHKERRQ(ierr);
	//ierr = VecDuplicate(x,&u);CHKERRQ(ierr);

	/*
	Create matrix. When using MatCreate(), the matrix format can
	be specified at runtime.
	Performance tuning note: For problems of substantial size,
	preallocation of matrix memory is crucial for attaining good
	performance. See the matrix chapter of the users manual for details.
	*/

    //THIS FUNCTIONS MUST NOT BE CALLED (SLOW ACCORDING TO PETSC MANUAL)
	ierr = MatCreate(PETSC_COMM_WORLD,&A);CHKERRQ(ierr);	//Instead of create Sij
	ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);




	ierr = MatSetFromOptions(A);CHKERRQ(ierr);
	//TEMPORARY
	//THIS IS NOT RECOMMENDED DIRECTLY
	//ierr = MatCreateSeqBAIJ(PETSC_COMM_WORLD,PetscInt bs,PetscInt m,PetscInt n,PetscInt nz,const PetscInt nnz[],Mat *A)
	//SEQSBAIJ

	//Symmetric, THIS IS TEMP, TO MODIFY
	//ierr = MatSetOption(A,MAT_SYMMETRIC,PETSC_TRUE);


    vector<int> diagonal_nonzeros, off_diagonal_nonzeros;
	int nextCellCount;

	int nVars=3;


//	// Calculate space necessary for matrix memory allocation
//	for (int cellit=grid[gid].cell.begin();cit!=grid[gid].cell.end();cit++) {
//		nextCellCount=0;
//		for (it=(*cit).faces.begin();it!=(*cit).faces.end();it++) {
//			if (grid[gid].face[*it].bc==INTERNAL_FACE) {
//				nextCellCount++;
//			}
//		}
//		for (int i=0;i<nVars;++i) {
//			diagonal_nonzeros.push_back( (nextCellCount+1)*nVars);
//			off_diagonal_nonzeros.push_back( ((*cit).ghosts.size())*nVars);
//		}
//	}
//
//	MatCreateMPIAIJ(
//					PETSC_COMM_WORLD,
//					grid[gid].cellCount*nVars,
//					grid[gid].cellCount*nVars,
//					grid[gid].globalCellCount*nVars,
//					grid[gid].globalCellCount*nVars,
//					0,&diagonal_nonzeros[0],
//					0,&off_diagonal_nonzeros[0],
//					&impOP);

    //  MatCreateSeqAIJ(MPI_Comm comm,PetscInt m,PetscInt n,PetscInt nz,const PetscInt nnz[],Mat *A)
//    	comm 	- MPI communicator, set to PETSC_COMM_SELF
//	m 	- number of rows
//	n 	- number of columns
//	nz 	- number of nonzeros per row (same for all rows)
//	nnz 	- array containing the number of nonzeros in the various rows (possibly different for each row) or NULL

	//Must call MatXXXSetPreallocation() or MatSetUp() on argument 1 "mat" before MatSetValues()!

	ierr = MatSetUp(A);
	//
	//ierr= MatSeqAIJSetPreallocation(Mat B,PetscInt nz,const PetscInt nnz[])
	//where
	//B	- The matrix-free
	//nz	- number of nonzeros per row (same for all rows)
	//nnz	- array containing the number of nonzeros in the various rows (possibly different for each row) or NULL



	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	Create the linear solver and set various options
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*
	Create linear solver context
	*/
	ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
	/*
	Set operators. Here the matrix that defines the linear system
	also serves as the preconditioning matrix.
	*/

	ierr = KSPSetOperators(ksp,A,A,DIFFERENT_NONZERO_PATTERN);CHKERRQ(ierr);
	/*
	Set linear solver defaults for this problem (optional).
	- By extracting the KSP and PC contexts from the KSP context,
	we can then directly call any KSP and PC routines to set
	various options.
	- The following four statements are optional; all of these
	parameters could alternatively be specified at runtime via
	KSPSetFromOptions();
	*/
	ierr = KSPGetPC(ksp,&pc);CHKERRQ(ierr);
	ierr = PCSetType(pc,PCJACOBI);CHKERRQ(ierr);
	ierr = KSPSetTolerances(ksp,1.e-5,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(ierr);
	/*
	Set runtime options, e.g.,
	-ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
	These options will override those specified above as long as
	KSPSetFromOptions() is called _after_ any other customization
	routines.
	*/
	ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);
	if (nonzeroguess)
	{
		PetscScalar p = .5;
		ierr = VecSet(x,p);CHKERRQ(ierr);
		ierr = KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);CHKERRQ(ierr);
	}

	ierr=VecSet(this->b,0.);
	ierr=VecSet(this->x,0.);
	cout << "Solver Initialized." <<endl;
}
//
template <typename number>
PETSC_KSP_Solver<number>::
PETSC_KSP_Solver(const int &d):
Solver<number>(d)
{
	PETSC_Init();
}


template <typename number>
void PETSC_KSP_Solver<number>::PreAllocateRows(const vector <int> &nnz)
{
	MatSeqAIJSetPreallocation(this->A,PETSC_NULL,&nnz[0]);
}

template <typename number>
void PETSC_KSP_Solver<number>::PreAllocateRows(const PetscInt &cols)
{
	MatSeqAIJSetPreallocation(this->A,cols,PETSC_NULL);
}

template <typename number>
void PETSC_KSP_Solver<number>::Solve()
{
	ierr = MatAssemblyBegin(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	//ierr = MatSetOption(this->A,MAT_SYMMETRIC,PETSC_TRUE);


	ierr = KSPSolve(ksp,b,x);CHKERRQ(ierr);
	/*
	View solver info; we could instead use the option -ksp_view to
	print this info to the screen at the coknclusion of KSPSolve().
	*/
	ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	Check solution and clean up
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*
	Check the error
	*/
	PetscReal norm;
	PetscInt its;

	ierr = VecNorm(x,NORM_2,&norm);CHKERRQ(ierr);
	ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of error %A, Iterations %D\n",
	norm,its);CHKERRQ(ierr);
	/*
	Free work space. All PETSc objects should be destroyed when they
	are no longer needed.
	*/

}

template <typename number>
void PETSC_KSP_Solver<number>::InsertRow(const int &row, const std::vector<int> &cols, const std::vector <double> &vals)
{
	//ierr = MatSetValues(this->A,1,row,cols.size(),cols,&vals[0],INSERT_VALUES);CHKERRQ(ierr);
}

template <typename number>
void PETSC_KSP_Solver<number>::SetMatVal(const PetscInt &row, const PetscInt &col, const PetscScalar &value)
{
	ierr=MatSetValues(this->A,1,&row,1,&col,&value,INSERT_VALUES);
}

template <typename number>
void PETSC_KSP_Solver<number>::AddMatVal(const PetscInt &row, const PetscInt &col, const PetscScalar &value)
{
	ierr=MatSetValues(this->A,1,&row,1,&col,&value,ADD_VALUES);
}


template <typename number>
void PETSC_KSP_Solver<number>::ViewInfo()
{

	ierr = MatAssemblyBegin(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	cout << "Matrix A: "<<endl;
	MatView(this->A,PETSC_VIEWER_STDOUT_SELF);
	cout << "RHS: "<<endl;
	VecView(b,PETSC_VIEWER_STDOUT_SELF);
	cout << "Solution: "<<endl;
	VecView(x,PETSC_VIEWER_STDOUT_SELF);
}

template <typename number>
void PETSC_KSP_Solver<number>::SetbValues(const PetscInt &row, const PetscScalar &value)
{
	ierr=VecSetValues(this->b,1,&row,&value,INSERT_VALUES);
}

//template <typename number,int dim>
//void PETSC_KSP_Solver<number,dim>::AddbValues(const PetscInt &row, const PetscScalar &value)
//{
//	ierr=VecSetValues(this->b,1,&row,&value,ADD_VALUES);
//}
//
//template <typename number,int dim>
//void PETSC_KSP_Solver<number,dim>::SetbValues(const PetscScalar &value)
//{
//	ierr=VecSet(this->b,value);
//}
//



template <typename number>
const std::vector <number>
PETSC_KSP_Solver<number>::X() const
{
	std::vector <number> v(this->matdim);

	int ix[1];
	double y[1];

	for (int i=0;i<this->matdim;i++) //TO EVALUATE SPEED
	{
		ix[0]=i;
		VecGetValues(x,1,ix,y);
		number val;
		val=y[0];
		v[i]=val;
	}

	//PetscErrorCode  VecGetValues(Vec x,PetscInt ni,const PetscInt ix[],PetscScalar y[])

	return v;
}

template <typename number>
void PETSC_KSP_Solver<number>::ClearMat()
{
	//TO MODIFY
	int row,col;
	//for (int i=0;i<)
	//ierr=MatSetValue(this->A,1,&row,1,&col,&value,INSERT_VALUES);

}

template <typename number>
const number
PETSC_KSP_Solver<number>::B(const int &i)const
{
	double y[1];
	int ix[1];
	ix[0]=i;

	VecGetValues(this->b,1,ix,y);
	//cout << "value get" << y[0] <<endl;
	number n=y[0];
	return n;
}

template <typename number>
const number
PETSC_KSP_Solver<number>::X(const int &i)const
{
	double y[1];
	int ix[1];
	ix[0]=i;

	VecGetValues(this->x,1,ix,y);
	//cout << "value get" << y[0] <<endl;
	number n=y[0];
	return n;
}

template <typename number>
const std::vector <number>
PETSC_KSP_Solver<number>::B() const
{
	std::vector <number> v(this->matdim);

	int ix[1];
	double y[1];

	for (int i=0;i<this->matdim;i++) //TO EVALUATE SPEED
	{
		ix[0]=i;
		VecGetValues(this->b,1,ix,y);
		number val;
		val=y[0];
		v[i]=val;
	}

	//PetscErrorCode  VecGetValues(Vec x,PetscInt ni,const PetscInt ix[],PetscScalar y[])

	return v;
}

} //FluxSol

#include "PETSC_Solver.inst"
