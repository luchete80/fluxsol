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
#include "Modelo.h"
#include "NastranMesh.h"

// LAPLACIAN MESH REORDER FOR
//SIMILAR TO EXAMPLE 18

using namespace FluxSol;

int main(int argc,char **args)
{

    cout << "Open Model from file..."<<endl;
//    Nastran::Modelo model("test2.bdf");

    NastranMesh nasmesh("test2.bdf");
    //Nastran::Modelo model;

	return 0;
}

