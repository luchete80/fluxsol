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
#ifndef _FIELD_H
#define _FIELD_H

// Parado en directorio Field //
#include "../Type/Scalar.h"
#include "../Type/Vec3d.h"

#include <string>
#include <fstream>
#include "UDO.h"


//#include "../Input/Input.h"

#include <list>

namespace FluxSol{
//Los campos no dependen de la discretizacion
//

//Campo generico
//De este derivan todos los campos
//Este campo no tiene asociada una malla
//Siempre en los campos internos
template<typename T>
class _Field:
    public UDO
    {


    protected:
    std::vector <T> value;
	int order;
	int numberofvals;
	//This must not be able for _PatchFields since is confusing
	std::vector <int> idgrid_domain;	//Ya sea vertex, surface o volume
										//Son los id de los vertices que forman parte del dominio


    std::vector <T> prev_value;

	std::string name;

    public:
    _Field(){};
    void Resize(const int &i);

    const int Order(){return order;};
	_Field(const int &numval, const double &val =0.)
    {
 	    this->value=vector<T>(numval);
 	    this->prev_value=vector<T>(numval);

        this->numberofvals=numval;
        for (int v=0;v<numval;v++)
        {
            //T num(number);
            //value.push_back(num);
            this->value[v]=val;
        }
    }

	_Field(const vector<T> &val)
	{
	    this->value=vector<T>(val.size());
	    this->prev_value=vector<T>(val.size());
	    for (int v=0;v<val.size();v++)
            //this->value.push_back(val[v]);
            this->value[v]=val[v];

        this->numberofvals=val.size();
	}
	const T & Val(const int &i)const{return value[i];};
	const T & PrevVal() const {return this->old_value;}
	inline void Val (const int &i,const T &v){this->value[i]=v;}
	const int & Numberofvals()const{return numberofvals;}
	void  Insert_Idgrid_domain(const int &i){idgrid_domain.push_back(i);}
	int & Idgrid_domain(const int &i){return idgrid_domain[i];}
	void Log();

    void AssignVecVal(const vector <T> &val)
    {
        for (int c=0;c<this->numberofvals;c++)
            this->value[c]=val[c];

    }

	//Operations

	_Field<T> & operator=(const T &val);
	_Field<T> & operator=(T val);
	//It is not const since it can be changed
	//Declare both operators to modify or not field
	T& operator[](const int &x){return value[x];}
	const T& operator[](const int &x)const {return value[x];}
	//void operator=(const double &val);

	_Field<T> operator=(const double &val);

	const string & Name()const {return name;}

	const std::string outstr() const
	{
        std::string cad;
        //meshlog.open(nombre.c_str());
        for (int v=0;v<this->numberofvals;v++)
        {
            std::ostringstream strs;
            strs << "[" <<v << "]: ";
            cad+=strs.str();
            cad+=this->value[v].outstr();
            //cad+="\n";
        }

        return cad;
	}

    virtual void Calculate(){}   //For User Defined Objects

    //~_Field(){}

    //THIS MUST BE DONE ONCE
    //Inner product
//    virtual const _Field<typename innerProduct < T, T> ::type>
//    operator &(const _Field<T> &right) const
//    {
//        _Field<typename innerProduct < T, T> ::type>  ret(this->Numberofvals());
//        //Sizes must be equal and rank must be large than zero?
//        for (int c = 0; c < this->numberofvals; c++)
//        {
//            ret.value[c] = this->value[c]&right.value[c];
//            cout << "Interpolate value "<<ret[c].outstr()<<endl;
//        }
//
//        return ret;
//    }


};//Field


}//FluxSol



#endif
