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
#ifndef _STATTHERMALSOL_H_
#define _STATTHERMALSOL_H_

#include "Model.h"
namespace FluxSol{

class StatThermalModel
:public Model	//To Avoid Multiple Inheritance
{

	protected:
	_CC_Fv_Field <Scalar>  h;	//Enthalpy, if ICO is cp T
	_CC_Fv_Field <Scalar>  T;	//Temperature
	
	public:
	StatThermalModel(){}
	virtual void SolveIter();


};

}

#endif
