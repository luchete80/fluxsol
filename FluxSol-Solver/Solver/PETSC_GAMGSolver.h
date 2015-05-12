/************************************************************************

	Copyright 2012-2013 Luciano Buglioni

	Contact:
        luciano.buglioni@gmail.com
        pzitelli@gmail.com

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
#ifndef _PETSC_AMG_SOLVER_H
#define _PETSC_AMG_SOLVER_H

#include "PETSC_Solver.h"
#include <time.h>

namespace FluxSol
{
    //MUST BE INHERITED FROM PETSC MPI SOLVER
    template <typename number>
    class PETSC_GAMGSolver:
            public PETSC_KSP_Solver<number>
    {

        protected:

            Mat            Pmat;    //Mat is inherited
          PetscInt       i,m,M,its,Istart,Iend,j,Ii,bs,ix;
          // PetscReal      x,y,h;
          // PetscReal      soft_alpha = 1.e-3;
          MPI_Comm       comm;
          PetscMPIInt    npe,rank;
          PC             pc;

        public:
        PETSC_GAMGSolver(){}

        template <typename T>
        void Solve(EqnSystem<T> &);

    };
}


#endif
