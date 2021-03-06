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

using namespace std;
using namespace FluxSol;

//int main(int argc, char *argv[])

int main()
{

    //cout << "Input file: "<< argv[1]<<endl;

    cout << "Reading test.msh... "<< endl;

	FluentMesh malla("test.msh");

	//cout << "Nodes: " << malla.nodes.size() << endl;

    //cout << "Cells: " << malla.connectivity.size() << endl;

	return 0;
}

