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
#ifndef _UDO_H_
#define _UDO_H_

#include <string>
#include <set>
#include <vector>
#include <map>

#include "UDO_Decl.h"

using namespace std;

namespace FluxSol
{

class UDO
{

	protected:

	public:

	UDO(){};
};

//template <typename T>
//class UD_Field:
//    public _Field<T>,
//    public UDO
//{
//
//
//    public:
//        UD_Field(){};
//
//    };

class UDOIds
{
    protected:
        string classname,basename;      //class name is inherited class type

    public:
        UDOIds(const string &n, const string &ct)
        {classname=n;basename=ct;}
        const string & ClassName()const {return classname;}
        const string & BaseName()const{return basename;}


};


class UDOLib
{

	protected:
    map <string, UDO*> udomap;   //Relates UDO names with objects
	public:

    //These must be linked externally
    UDOLib(){};
	UDOLib(set <string> &files); //
	UDOLib(const string &str); //

	void AddFile(string file);

	//UDO* UDO (const int &i) {return udo[i]};
	UDO* UdoFromName(const string &name){return udomap[name];}
	UDO* CreateUDO(const string &);

};

//
//template <typename T>
//class UD_PatchField:
//    public _PatchField<T>,
//    public UDO
//    {
//
//        public:
//            UD_PatchField(){};
//
//    };

}
#endif
