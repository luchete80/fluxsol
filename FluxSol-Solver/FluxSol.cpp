/************************************************************************

	Copyright 2012-2013 Luciano Buglioni

	Contact: luciano.buglioni@gmail.com

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

#include <iostream>
#include "FluxSol.h"

using namespace std;
using namespace FluxSol;


// FLUXSOL MAIN
int main (int argc, char *argv[])
{

    // CHECK WHICH TIPE OF MODEL
    //cout <<"arg number: "<<argc<<", "<< argv[1]<<endl;
   if (argc > 1)
   {
       string inputFileName=argv[1];
       InputFile input(inputFileName);
       string solscheme=input.ReadEqnType();   //To modify, this must be per grid

       if (solscheme=="navier-stokes") //if this is empty
       {
           CFDModel model(inputFileName);

           model.Solve();
       }
       else if (solscheme=="diffusion")
       {

       }
   }
   else
   {
       cout << "ERROR: Input file not found. FluxSol syntax: "<<endl<<endl;
       cout << "fluxsol.exe <InputFile.in>"<<endl;
   }
	return 0;

}


