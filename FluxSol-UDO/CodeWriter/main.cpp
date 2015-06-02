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

#include "FileSearch.h"
#include "PatchField.inst"

// GIVEN A .CPP CONTAINING User Definitions
// this program writes an "UDOCreator.cpp"
// which must be then linked to libgen library to build UDOLibrary
// Then UDOLib must be created with the user .cpp file
// UDOLib("userfile.cpp")
// Each model has an UDOLib

int main()
{

	vector <UDO *> vudo;
	map <UDOIds, UDO*> test;


    FileSearcher fs(".");
    //test = fs.GetUDOMapsFromFile("boundary.cpp");


    fs.WriteUDOLibFile();
    //UDO* udo=new _PatchField<Scalar>();
	//vudo.push_back(udo);



}
