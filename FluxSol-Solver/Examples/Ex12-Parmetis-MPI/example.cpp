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

#include <parmetis.h>

class MPI_Grid:
public Grid
{
	
	
	
};

// ADAPTED FROM FREE CFD 

//Function declaration
int mesh2dual(Fv_CC_Grid &mesh);


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

	//mesh.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(mesh);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	//for (int p=0;p<3;p++)
     //   T.Boundaryfield().PatchField(p).AssignValue(wallvalue);

	T.Boundaryfield().PatchField(1).AssignValue(topvalue);  //TOP

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
      PetscMPIInt    np,Rank;
      PC             pc;


      PetscInitialize(&argc,&args,(char*)0,help);
      comm = PETSC_COMM_WORLD;
      ierr  = MPI_Comm_rank(comm, &Rank);CHKERRQ(ierr);
      ierr  = MPI_Comm_size(comm, &np);CHKERRQ(ierr);
      //ierr  = PetscOptionsGetInt(NULL,"-ne",&ne,NULL);CHKERRQ(ierr);
      //h     = 1./ne;
      /* ne*ne; number of global elements */
      ierr = PetscOptionsGetReal(NULL,"-alpha",&soft_alpha,NULL);CHKERRQ(ierr);
      //M    = totrows; /* global number of nodes */
      /* create stiffness matrix */
      //PetscErrorCode  MatCreateAIJ(MPI_Comm comm,PetscInt m,PetscInt n,PetscInt M,PetscInt N,
                                     //PetscInt d_nz,const PetscInt d_nnz[],PetscInt o_nz,const PetscInt o_nnz[],Mat *A)
    //M	- number of global rows (or PETSC_DETERMINE to have calculated if m is given)
    //N	- number of global columns (or PETSC_DETERMINE to have calculated if n is given)
    //d_nz	- number of nonzeros per row in DIAGONAL portion of local submatrix (same value is used for all local rows)
    //d_nnz	- array containing the number of nonzeros in the various rows of the DIAGONAL portion of the local submatrix (possibly different for each row) or NULL, if d_nz is used to specify the nonzero structure. The size of this array is equal to the number of local rows, i.e 'm'.
    //o_nz	- number of nonzeros per row in the OFF-DIAGONAL portion of local submatrix (same value is used for all local rows).
    //o_nnz	- array containing the number of nonzeros in the various rows of the OFF-DIAGONAL portion of the local submatrix (possibly different for each row) or NULL, if o_nz is used to specify the nonzero structure. The size of this array is equal to the number of local rows, i.e 'm'.

	
	// GRID PARTITION
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	
	int cellCount=floor(double(globalCellCount)/double(np));
	int baseCellCount=cellCount;
	int offset=Rank*cellCount;
	
	if (Rank==np-1) cellCount=cellCount+globalCellCount-np*cellCount;
	
	idx_t elmdist[np+1];
	idx_t *eptr;
	eptr = new idx_t[cellCount+1];
	idx_t *eind;
	
	int eindSize=0;
	
	// if ((offset+cellCount)==globalCellCount) {
		// eindSize=raw.cellConnectivity.size()-raw.cellConnIndex[offset];
	// }
	// else {
		// eindSize=raw.cellConnIndex[offset+cellCount]-raw.cellConnIndex[offset]+1;
	// }

		eind = new idxtype[eindSize];
	idxtype* elmwgt = NULL;
	int wgtflag=0; // no weights associated with elem or edges
	int numflag=0; // C-style numbering
	int ncon=1; // # of weights or constraints
	int ncommonnodes=3; // set to 3 for tetrahedra or mixed type

	float tpwgts[np];
	for (int p=0; p<np; ++p) tpwgts[p]=1./float(np);
	float ubvec=1.02;
	int options[3]; // default values for timing info set 0 -> 1
	options[0]=0; options[1]=1; options[2]=15;
	int edgecut ; // output
	idxtype* part = new idxtype[cellCount];

	for (int p=0;p<np;++p) elmdist[p]=p*floor(globalCellCount/np);
	elmdist[np]=globalCellCount;// Note this is because #elements mod(np) are all on last proc
	for (int c=0; c<cellCount;++c) {
		eptr[c]=raw.cellConnIndex[offset+c]-raw.cellConnIndex[offset];
	}
	if ((offset+cellCount)==globalCellCount) {
		eptr[cellCount]=raw.cellConnectivity.size()-raw.cellConnIndex[offset];
	} else {
		eptr[cellCount]=raw.cellConnIndex[offset+cellCount]-raw.cellConnIndex[offset];
	}
	for (int i=0; i<eindSize; ++i) {
		eind[i]=raw.cellConnectivity[raw.cellConnIndex[offset]+i];
	}

	MPI_Comm commWorld=MPI_COMM_WORLD;
	ParMETIS_V3_PartMeshKway(elmdist,eptr,eind, elmwgt,
	                         &wgtflag, &numflag, &ncon, &ncommonnodes,
	                         &np, tpwgts, &ubvec, options, &edgecut,
	                         part,&commWorld) ;
	delete[] eptr;
	delete[] eind;

	// Distribute the part list to each proc
	// Each proc has an array of length globalCellCount which says the processor number that cell belongs to [cellMap]
	int recvCounts[np];
	int displs[np];
	for (int p=0;p<np;++p) {
		recvCounts[p]=baseCellCount;
		displs[p]=p*baseCellCount;
	}
	recvCounts[np-1]=baseCellCount+globalCellCount-np*baseCellCount;
	
	maps.cellOwner.resize(globalCellCount);
	//cellMap of a cell returns which processor it is assigned to
	MPI_Allgatherv(part,cellCount,MPI_INT,&maps.cellOwner[0],recvCounts,displs,MPI_INT,MPI_COMM_WORLD);

	// Find new local cellCount after ParMetis distribution
	cellCount=0.;
	int otherCellCounts[np]; 
	for (int p=0;p<np;p++) {
		otherCellCounts[p]=0; 
		partitionOffset.push_back(0);
	}
	
	for (int c=0;c<globalCellCount;++c) {
		otherCellCounts[maps.cellOwner[c]]+=1;
		if (maps.cellOwner[c]==Rank) ++cellCount;
	}
	cout << "[I Rank=" << Rank << "] Number of Cells= " << cellCount << endl;
	
	myOffset=0;
	partitionOffset[0]=0;
	for (int p=1;p<np;++p) partitionOffset[p]=partitionOffset[p-1]+otherCellCounts[p-1];
	myOffset=partitionOffset[Rank];
	
	delete[] part;
	
	return 0;
}

// void Grid::setup(void) {
	// partition();
	// create_nodes_cells();
	// mesh2dual();
	// create_faces();
	// create_ghosts();
	// trim_memory();
	// areas_volumes();
	// mpi_handshake();
	// mpi_get_ghost_geometry();
	// return;
// }


int mesh2dual(Fv_CC_Grid &mesh) 
{

	// Find out other partition's cell counts
	int otherCellCounts[np];
	for (int i=0;i<np;++i) otherCellCounts[i]=0;
	for (int c=0;c<globalCellCount;++c) otherCellCounts[maps.cellOwner[c]]+=1;

	//Create the Mesh2Dual inputs
	idxtype elmdist[np+1];
	idxtype *eptr;
	eptr = new idxtype[cellCount+1];
	idxtype *eind;
	int eindSize=0;
	int ncommonnodes=1;
	int numflag=0; // C-style numbering
	MPI_Comm commWorld=MPI_COMM_WORLD;

	for (int c=0;c<cellCount;++c) {
		eindSize+=cell[c].nodeCount;
	}
	eind = new idxtype[eindSize];


	elmdist[0]=0;
	for (int p=1;p<=np;p++) elmdist[p]=otherCellCounts[p-1]+elmdist[p-1];
	eptr[0]=0;
	for (int c=1; c<=cellCount;++c) eptr[c]=eptr[c-1]+cell[c-1].nodeCount;
	int eindIndex=0;
	for (int c=0; c<cellCount;c++){
		for (int cn=0; cn<cell[c].nodeCount; ++cn) {
			eind[eindIndex]=cellNode(c,cn).globalId;
			++eindIndex;
		}
	}

	ParMETIS_V3_Mesh2Dual(elmdist, eptr, eind, &numflag, &ncommonnodes, &maps.adjIndex, &maps.adjacency, &commWorld);

	delete[] eptr;
	delete[] eind;
	
	return 0;
	
} // end Grid::partition

