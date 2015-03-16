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
#ifndef __PatchField_H_
#define __PatchField_H_

#include "FvField.h"

namespace FluxSol
{

template <typename T>
_PatchField<T>::_PatchField(const Patch &p)
{
	Patch patch=p;

	this->numberofvals=0;

	for (int nf=0;nf<patch.Num_Faces();nf++)
	{
		T v(0.);
		this->value.push_back(v);
		this->numberofvals++;
	}
}

template <typename T>
_PatchField<T>::_PatchField(const Patch &p, const T &cval)
{
	Patch patch=p;

	this->numberofvals=0;

	for (int nf=0;nf<patch.Num_Faces();nf++)
	{
		T v(0.);
		this->value.push_back(v);
		this->numberofvals++;
	}
    //std::cout << "assign cval "<<cval.outstr()<<endl;
	this->cvalue=cval;
}


template <typename T>
void _PatchField<T>::AssignValue(const T &val)
{
    //""this->numberofvals
	for (int n=0;n<this->numberofvals;n++)
		this->value[n]=val;

		//cout << "Assign Value"<<val.outstr()<<endl;
}

}

#include "PatchField.inst"

#endif
