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
#include "UDOLib.h"
#include "FileSearch.h"

using namespace std;
namespace FluxSol
{

UDOLib::UDOLib()
{
    cout << "[I] Creating UDOLib 2..."<<endl;

    FileSearcher fs(".");
    std::vector<UDOIds>::iterator it;
    cout << "UDO number: " << fs.UDO_Ids().size()<<endl;
    int count =0;
    for (it=fs.UDO_Ids().begin();it!=fs.UDO_Ids().end();it++)
    {
        //UDOLib::AddFile(*sit);
        std::string classname=it->ClassName();
        //cout << "Inserting classname: " << classname<<endl;
        //UDO* udo=CreateUDO(classname);
        udomap.insert(std::pair<string, UDO*>(classname,CreateUDO(classname)) );
        cout << "inserted "<<endl;
        udomap[classname]->Calculate();
        //udo->Calculate();
        //delete udo;
        count ++;

    }
    cout << "Created map"<<endl;
}

UDOLib::UDOLib(set <string> &files) //
{

}
UDOLib::UDOLib(string &file) //
{

}

//Ads maps from file
void UDOLib::AddFile(string file)
{

}

}

