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
	//ierr = KSPSetOperators(ksp,A,A);CHKERRQ(ierr);
	//KSPSetFromOptions(ksp);

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
	//ierr = PCSetType(pc,PCJACOBI);CHKERRQ(ierr);
	//ierr = PCSetType(pc,PCGAMG);CHKERRQ(ierr);
	//ierr = PCSetType(pc,PCICC);CHKERRQ(ierr);
	//ierr = PCSetType(pc,PCICC);CHKERRQ(ierr);
	ierr = PCSetType(pc,PCILU);CHKERRQ(ierr);
	ierr = KSPSetTolerances(ksp,1.e-3,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT);CHKERRQ(ierr);
	//PetscErrorCode  KSPSetTolerances(KSP ksp,PetscReal rtol,PetscReal abstol,PetscReal dtol,PetscInt maxits)

    //ksp	- the Krylov subspace context
    //rtol	- the relative convergence tolerance, relative decrease in the (possibly preconditioned) residual norm
    //abstol	- the absolute convergence tolerance absolute size of the (possibly preconditioned) residual norm
    //dtol	- the divergence tolerance, amount (possibly preconditioned) residual norm can increase before KSPConvergedDefault() concludes that the method is diverging
    //maxits	- maximum number of iterations to use

	KSPSetInitialGuessKnoll(ksp,PETSC_TRUE);
	//KSPSetType(ksp,KSPGMRES);
	KSPSetType(ksp,KSPBCGS);    //BiCGSTAB

	/*
	Set runtime options, e.g.,
	-ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
	These options will override those specified above as long as
	KSPSetFromOptions() is called _after_ any other customization
	routines.
	*/
	//ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr); THIS IS PREFERRED BEFORE PREVIOUS SOLVER
	//AND MUST BE CALLED SETUP,
	if (nonzeroguess)
	{
		PetscScalar p = .5;
		ierr = VecSet(x,p);CHKERRQ(ierr);
		ierr = KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);CHKERRQ(ierr);
	}


	ierr=VecSet(this->b,0.);
	ierr=VecSet(this->x,0.);

	//REORDErING VARS
    permute=PETSC_FALSE;     //Matrix reordering
    mat_ord_type=MATORDERINGRCM;
    rowperm = NULL,colperm = NULL;

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
    //PetscErrorCode  MatSeqAIJSetPreallocation(Mat B,PetscInt nz,const PetscInt nnz[])
    //
    //B	- The matrix
    //nz	- number of nonzeros per row (same for all rows)
    //nnz	- array containing the number of nonzeros in the various rows (possibly different for each row) or NULL

}

template <typename number>
void PETSC_KSP_Solver<number>::Solve()
{

	ierr = MatAssemblyBegin(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(this->A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);


    if (permute)
    {
        Mat Aperm;
        MatGetOrdering(this->A,mat_ord_type,&rowperm,&colperm);
        MatPermute(this->A,rowperm,colperm,&Aperm);
        VecPermute(b,colperm,PETSC_FALSE);
        MatDestroy(&A);
        this->A    = Aperm;               /* Replace original operator with permuted version */
        //MatDestroy(&Aperm);
    }

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

	if (permute) {VecPermute(x,rowperm,PETSC_TRUE);}

	/*
	Free work space. All PETSc objects should be destroyed when they
	are no longer needed.
	*/

	ISDestroy(&rowperm);  ISDestroy(&colperm);
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

//
//template <typename T>
//void Solve(EqnSystem <T> &TEqn)
//{
//
//    ///// PETSC SOLVE MANUALLY ///////
//
//	int numberofcomp=pow(3.,TEqn.Dim());
//	int totrows=numberofcomp*TEqn.Num_Eqn();
//
//    cout << "Creating Solver"<<endl;
//	PETSC_KSP_Solver<double> Solver(totrows);
//
//    cout << "Number of comps" << numberofcomp<<endl;
//    cout << "Number of rows" << totrows<<endl;
//
//    vector <int> nonzerosperrow;
//	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
//    {
//        //TO MODIFY
//        for (int dim=0;dim<numberofcomp;dim++)  nonzerosperrow.push_back(5);
//    }
//
//    cout << "Allocating Rows..."<<endl;
//    Solver.PreAllocateRows(nonzerosperrow);
//    cout << "Assembying Eqns"<<endl;
//	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
//	{
//	    //Width Assign
//
//        //cout << "Assemblying Eqn "<<e<<endl;
//		vector <double> ap=TEqn.Eqn(e).Ap().Comp();
//		Scalar ap_sc=TEqn.Eqn(e).Ap();
//		Scalar value;
//		int width=(TEqn.Eqn(e).Width()-1)*numberofcomp+1;
////		//int width=TEqn.Eqn(e).Width()*numberofcomp;   // THIS IS WRONG
////
//		int sparsecol=0;
//		int row=e*numberofcomp;
//
//		vector <double> nullval;
//		nullval.assign(numberofcomp,0.);
//
//		//This id is relative to the cell, not the column
//		int fisrt_nonzero_col=TEqn.Eqn(e).MinNeigbourId();  //In a single component matrix
//		int realcellid;
//
//
//
//		//Look trough entire width for neighbours id
//		//The main idea is to look through eqn width
//		//Real cell id is taken, and then are watched all neighbours to check if each real cell id belongs to neighbours vector
//		for (int width_cells=0;width_cells<TEqn.Eqn(e).Width();width_cells++)
//		{
//			realcellid=width_cells+fisrt_nonzero_col;   //Wich cell
//
//			vector <double> col;
//			int columnid;
//			//Found central
//			bool foundcell=false;
//			int localneighbourfound=FluxSol::SearchVal(realcellid,TEqn.Eqn(e).NeighboursIds());
//			//vector <int> neigh=TEqn.Eqn(e).NeighboursIds();
//			//std::vector<int>::iterator it;
//			if (TEqn.Eqn(e).Id()==realcellid)
//			{col=ap;columnid=row;foundcell=true;}//row is equal to
//			//Neighbours ids are not neccesarily ordered, then must search for cellid in all neighbours
//			else if(localneighbourfound>-1)
//			{col=TEqn.Eqn(e).An(localneighbourfound).Comp();columnid=numberofcomp*realcellid;foundcell=true;}
//			else //column index is not a neighbour neither central cell
//			{columnid=numberofcomp*realcellid;}
//
//			//Write Matrix
//			if (foundcell)
//			{
//                //cout << "Found Cell " <<endl;
//				for (int dim=0;dim<numberofcomp;dim++)
//				{
//                    //INFO
//                    //cout << "(Indexes From 1)  K(" <<  row+dim+1<<","<<columnid+dim+1<<")"<<"=" << 0.0<<endl;
//                    //cout << "(From zero) Sparse col: " << numberofcomp*width_cells <<endl;
//
//					//Q_SetEntry(&K,row+dim+1,numberofcomp*width_cells,columnid+dim+1,col[0]);
//					Solver.SetMatVal(row+dim, columnid+dim, col[0]);
//
//				}
//
//			}
//
//		}//En of width
//
//
//	}//End of cells
//
//    //cout << "R vector (from zero)"<<endl;
//	//V_SetAllCmp(&R,0.0);
//	for (int e=0;e<TEqn.Num_Eqn();e++)
//	{
//	    //cout << "Eqn " << e<<endl;
//	    //cout << "[" <<e<<"]: "  ;
//		vector <double> source=TEqn.Eqn(e).Source().Comp();
//		for (int dim=0;dim<numberofcomp;dim++)
//        {
//            Solver.SetbValues(e*numberofcomp+dim, source[dim]);
//        }
//        //cout << endl;
//
//	}
//	//cout << "tot rows" << totrows<<endl;
//	//std::vector <double> Ui,Ri;
//	//Ui.assign(totrows,0.);
//	//Ri.assign(totrows,0.);
//	//V_SetAllCmp(&U,0.0);
//	//SetRTCAccuracy(1e-5);
//
//    clock_t ittime_begin, ittime_end;
//    double ittime_spent;
//
//    ittime_begin = clock();
//
//    Solver.Solve();
//
//    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;
//
//    cout << "PETSC Solving elapsed time: "<<ittime_spent<<endl;
//
//    cout <<"Solver Results "<<endl;
//	 for (int e=0;e<TEqn.Num_Eqn();e++)
//	 {
//	     //cout << "e= "<<e<<endl;
//		 vector <double> r;
//		 for (int dim=0;dim<numberofcomp;dim++)
//         {
// 			 r.push_back(Solver.X(numberofcomp*e+dim));
// 			 //cout << "xi= "<<numberofcomp*e+dim<<", ";
//             //cout <<U.Cmp[numberofcomp*e+dim+1]<<" ";
//         }
////         //cout <<endl;
//
//        TEqn.Eqn(e).X()=r;
//		 //cout << " X Copied Vector: "<<eq.Eqn(e).X().outstr()<<endl;
//		 //cout << endl;
//     }
//
//     cout << "Destroying "<<endl;
//
//    Solver.Destroy();
//
//    cout << "Destroyed"<<endl;
//
//}

//
///////////// FAST SOLVER /////////////
/////////////  FIXED ALLOCATING NEIGHBOURS SECTION
//
template <typename T>
void Solve(EqnSystem <T> &TEqn)
{


    clock_t ittime_begin, ittime_end,ittime_start;
    double ittime_spent;

    ittime_begin = clock();
    ittime_start = clock();

    ///// PETSC SOLVE MANUALLY ///////

	int numberofcomp=pow(3.,TEqn.Dim());
	int totrows=numberofcomp*TEqn.Num_Eqn();

    cout << "Creating Solver"<<endl;
	PETSC_KSP_Solver<double> Solver(totrows);

    cout << "Number of comps" << numberofcomp<<endl;
    cout << "Number of rows" << totrows<<endl;

    vector <int> nonzerosperrow;
	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
    {
        //TO MODIFY
        for (int dim=0;dim<numberofcomp;dim++)  nonzerosperrow.push_back(5);
    }

    cout << "Allocating Rows..."<<endl;
    Solver.PreAllocateRows(nonzerosperrow);

    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;

    cout << "PETSC Creating Solver and Preallocating time: "<<ittime_spent<<endl;

    ittime_begin = clock();

    vector <double> col;    //CREATING Col IS FasTER

    cout << "Assembying Eqns"<<endl;
    PetscInt rowi,coli;

    //Max Neighbours
    PetscScalar vals [1][10];      //block
    PetscInt idxm[3],idxn[3][10];   //rows and cols index


	for (int e=0;e<TEqn.Num_Eqn();e++)	//Aca voy con las filas de a 2
	{
	    //Width Assign

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
            MatSetValues(Solver.Matrix(),1,&rowi,1,&rowi,&col[0],INSERT_VALUES);
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
            MatSetValues(Solver.Matrix(),1,&idxm[dim],TEqn.Eqn(e).Num_Neighbours(),idxn[dim],vals[0],INSERT_VALUES);


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
            Solver.SetbValues(e*numberofcomp+dim, source[dim]);
        }
        //cout << endl;
	}


    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;
	cout << "Assemblying elapsed time: "<<ittime_spent<<endl;

    ittime_begin = clock();

    Solver.Solve();

    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;

    cout << "PETSC Solving elapsed time: "<<ittime_spent<<endl;

    ittime_begin = clock();
    cout <<"Solver Results "<<endl;
    vector <double> r(numberofcomp);
	 for (int e=0;e<TEqn.Num_Eqn();e++)
	 {
         //cout << "e= "<<e<<endl;
         for (int dim=0;dim<numberofcomp;dim++)
         {
             r[dim]=Solver.X(numberofcomp*e+dim);
             //cout << "xi= "<<numberofcomp*e+dim<<", ";
             //cout <<U.Cmp[numberofcomp*e+dim+1]<<" ";
         }
        TEqn.Eqn(e).X()=r;

     }

    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;
	cout << "PETSC Allocating results time: "<<ittime_spent<<endl;

    ittime_begin = clock();

    //Solver.ShowInfo();


     cout << "Destroying "<<endl;

    Solver.Destroy();

    ittime_spent = (double)(clock() - ittime_begin) / CLOCKS_PER_SEC;
	cout << "PETSC Destroy time: "<<ittime_spent<<endl;

    cout << "Destroyed"<<endl;


    ittime_spent = (double)(clock() - ittime_start) / CLOCKS_PER_SEC;
	cout << "PETSC Total time: "<<ittime_spent<<endl;

}

} //FluxSol

#include "PETSC_Solver.inst"
