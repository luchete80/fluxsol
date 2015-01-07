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
#ifndef _READ_FIELD_DEF_H_
#define _READ_FIELD_DEF_H_

#include "Input.h"
#include "../Field/Field.h"

namespace FluxSol
{


//Case of single field input
template <typename T>
void ReadFieldFromInput(InputFile &input, _CC_Fv_Field<T> &field, Fv_CC_Grid &grid) {

	int gid = 0;
	// Loop through each boundary condition region and apply sequentially
	//The number of boundary conditions must be coincident with those in mesh
	int count=input.section("grid",gid).subsection("BC",0).count;
	//grid[gid].bcCount=count;
	//int Rank=grid[gid].Rank;
	//int np=grid[gid].np;
	//grid[gid].maps.face2bc.resize(grid[gid].faceCount);

	vector<int> bc_counter (count,0);

	_BoundaryField <T> boundfield;

	Boundary boundary = grid.vBoundary();

	//Loop through boundary conditions
	for (int b=0;b<count;++b) {

		// Store the reference to current BC region
		Subsection &region=input.section("grid",gid).subsection("BC",b);

		//BCregion bcRegion;

		string type=region.get_string("type");
		string kind=region.get_string("kind");

		//type can be phisical or numerical

		//new
		entry<double> data=region.get_double("T");

		if (type=="fixedvalue")
		{
			_Fv_FixedValue_Patch_Field <T> _PatchField(boundary.vPatch(b));
			_PatchField.AssignValue(data.value);
			boundfield.Add_PatchField(_PatchField);
		}
		else if (type=="fixedgradient")
		{
			_Fv_FixedGradient_Patch_Field <T> _PatchField(boundary.vPatch(b));
			_PatchField.AssignValue(data.value);
			boundfield.Add_PatchField(_PatchField);
		}

	} // bc loop


	_CC_Fv_Field <T> intfield (grid, boundfield);
	field = intfield;

	return;
}

//Exactly like above
template <typename T>
void ReadFieldNameFromInput(InputFile &input, _CC_Fv_Field<T> &field, Fv_CC_Grid &grid, string &varname) {

	int gid = 0;
	// Loop through each boundary condition region and apply sequentially
	//The number of boundary conditions must be coincident with those in mesh
	int count=input.section("grid",gid).subsection("BC",0).count;
	//grid[gid].bcCount=count;
	//int Rank=grid[gid].Rank;
	//int np=grid[gid].np;
	//grid[gid].maps.face2bc.resize(grid[gid].faceCount);

	vector<int> bc_counter (count,0);

	_BoundaryField <T> boundfield;

	Boundary boundary = grid.vBoundary();

	//Loop through boundary conditions
	for (int b=0;b<count;++b) {

		// Store the reference to current BC region
		Subsection &region=input.section("grid",gid).subsection("BC",b);

		//BCregion bcRegion;

		string type=region.get_string("type");
		string kind=region.get_string("kind");

		//type can be phisical or numerical

		//new
		entry<double> data=region.get_double(varname);

		if (type=="fixedvalue")
		{
			_Fv_FixedValue_Patch_Field <T> _PatchField(boundary.vPatch(b));
			_PatchField.AssignValue(data.value);
			boundfield.Add_PatchField(_PatchField);
		}
		else if (type=="fixedgradient")
		{
			_Fv_FixedGradient_Patch_Field <T> _PatchField(boundary.vPatch(b));
			_PatchField.AssignValue(data.value);
			boundfield.Add_PatchField(_PatchField);
		}

	} // bc loop


	_CC_Fv_Field <T> intfield (grid, boundfield);
	field = intfield;

	return;
}

//NEW
//SEARCH FOR U VAL AND TYPE
template <typename T>
void ReadVelocityFieldFromInput(InputFile &input, _CC_Fv_Field<T> &field, Fv_CC_Grid &grid)
{

	int gid = 0;
	// Loop through each boundary condition region and apply sequentially
	//The number of boundary conditions must be coincident with those in mesh
	int bcount=input.section("grid",gid).subsection("BC",0).count;
	//grid[gid].bcCount=count;
	//int Rank=grid[gid].Rank;
	//int np=grid[gid].np;
	//grid[gid].maps.face2bc.resize(grid[gid].faceCount);

	cout << "[I] Reading Velocity Field from input..."<<endl;

	vector<int> bc_counter (bcount,0);

	_BoundaryField <T> boundfield;

	Boundary boundary = grid.vBoundary();

	cout << "Boundary size: " <<boundary.Num_Patches()<<endl;

	//Loop through boundary conditions
	for (int b=0;b<bcount;++b) {

		// Store the reference to current BC region
		Subsection &region=input.section("grid",gid).subsection("BC",b);

		//BCregion bcRegion;

		string type=region.get_string("type");
		string kind=region.get_string("kind");

		//type can be phisical or numerical

		//new
		entry<double> data=region.get_double("U");

		if (type=="wall")
		{
			_PatchField <T> _PatchField(boundary.vPatch(b));
			_PatchField.AssignValue(data.value);
			boundfield.Add_PatchField(_PatchField);
		}
	} // bc loop


	_CC_Fv_Field <T> intfield (grid, boundfield);
	field = intfield;

	return;
}


}//End of FluxSol

#endif
