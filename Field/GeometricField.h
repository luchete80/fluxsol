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
#ifndef _GEOMETRICFIELD_H
#define _GEOMETRICFIELD_H

#include "Field.h"
#include "Products.h"
//A general FvField can inherit fields below

namespace FluxSol
{
	template<typename T>
	class SurfaceField: public _Field<T>
	{

		protected:

		public:
			SurfaceField(){}
			SurfaceField(const int &i):_Field<T>(i, 0.0){}
			//SurfaceField<typename innerProduct < T, T> ::type>  operator &(const SurfaceField<T> &right) const;

	};


	//IF OPERATOR IN THOSE FIELDS ARE DECLARED AS MEMBER FUNCTIONS, IS AN ERROR WITH INNERPROD<TYPE>
	template <typename T>
	SurfaceField<typename innerProduct < T, T> ::type>  operator&(const SurfaceField<T> &left, const SurfaceField<T> &right);

}//FluxSol

#endif
