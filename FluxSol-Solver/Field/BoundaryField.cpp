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

#include "GeometricField.h"

using namespace FluxSol;

template <typename T>
void _BoundaryField<T>:: Add_UDOAddr(UDO *udo)
{
		    //_PatchField <T> *pf=new _PatchField<T>;
		    //pf=(_PatchField <T>*) &udo;
		    //patchfield.push_back(pf);
		    patchfield.push_back((_PatchField <T>*)udo);
}

#include "BoundaryField.inst"
