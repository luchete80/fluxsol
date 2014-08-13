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
#ifndef _CONVECTIONSCHEME_H
#define _CONVECTIONSCHEME_H

//For explicit divergence

#include "SistEcuac.h"
#include "FvField.h"
#include "../Mesh/FvGrid.h"


namespace FluxSol
{
	template <typename T>
	class Convection_Scheme{

	protected:

		const Fv_CC_Grid &grid;	//Aca es una referencia a la malla


	public:
		Convection_Scheme(){};


		virtual EqnSystem <T> Div(const _CC_Fv_Field<T> &);



	};


	template<typename T>
	class FirstOrderUpwindScheme :
		public Convection_Scheme<T>
	{

	public:
		FirstOrderUpwindScheme();
		EqnSystem <T> Div(const _CC_Fv_Field<T> &);
		//Div;


	};


}



#endif