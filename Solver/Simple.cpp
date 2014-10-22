/************************************************************************

	Copyright 2012-2013 Luciano Buglioni

	Contact: luciano.buglioni@gmail.com

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
#include "Solver.h"
#include "Simple.h"

FluxSol::_Solver::_Solver()
{
    maxiter=100;
    ptol=1.0e-3;
    vtol=1.e-03;    //Todos los valores iguales

}


FluxSol::Solver_SIMPLE::Solver_SIMPLE(const int &dim_cell):_Solver()
{

}
