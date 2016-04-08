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
#ifndef _FIELDDEF_H_
#define _FIELDDEF_H_

#include "FvField.h"


using namespace std;
namespace FluxSol {


//template <typename T>
//_Field<T>::Log ()
//{
	//for ()

//}

template <typename T>
void _Field<T>::Resize(const int &i)
{

    //if (value.size()!=0)value.clear();
    cout << "Inside Resizing "<<endl;
    vector <T> temp(i,0.);
    T t(0.);
    value=temp;
    prev_value=temp;

    //value.push_back(t);
    numberofvals=i;
}

template <typename T>
Vertex_Fv_Field<T>::Vertex_Fv_Field(const Fv_CC_Grid &grid)
{
	GridPtr=&grid;

	T val;
	int nv= grid.Num_Verts();
	this->value.assign(nv,val);
	this->prev_value.assign(nv,val);
}

template <typename T>
_Field<T> & _Field<T>::operator=(const T &val)
{
	for (int v=0;v<this->value.size();v++)
	{
		this->value[v]=val;
	}
	return *this;
}

template <typename T>
_Field<T> & _Field<T>::operator=(T val)
{
	for (int v=0;v<this->value.size();v++)
	{
		this->value[v]=val;
	}
	return *this;
}

//template <typename T>
//void _Field<T>::operator=(const double &val)
//{
//	for (int v=0;v<value.size();v++)
//	{
//		value[v]=val;
//	}
//}

template <typename T>
_Field<T> _Field<T>::operator=(const double &val)
{
	for (int v=0;v<value.size();v++)
	{
		value[v]=val;
	}
	return *this;
}


template <typename T>
const Scalar MaxDiff(const _Field<T> &field1, const _Field<T> &field2)
{
	Scalar r;
	//Check for
	if (field1.Numberofvals() != field2.Numberofvals() )
		return r;

	Scalar maxdiff(1.0e10);
	for (int c=0;c<field1.Numberofvals();c++)
	{
		//Can be used the general Norm function
		Scalar diff=(field1[c].Norm()-field2[c].Norm())/field1[c].Norm();
		//Can define an operator
		if (diff<maxdiff)
			maxdiff=diff.Norm();
	}
}



// INITIALLY NO VIRTUAL FUNCTION
//template <typename T>
//_Field<T> & _Field<T>::operator&(const _Field<T> &right) const
//{


//}





}//FluxSol

#include "Field.inst"

#endif
