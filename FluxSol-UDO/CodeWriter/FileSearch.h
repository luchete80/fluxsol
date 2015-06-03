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
#ifndef _FILESEARCH_H_
#define _FILESEARCH_H_

#include "Input.h"          //InputBaseContainer
#include "UDO.h"        //UDO
#include <fstream>

#include <fstream>
#include <iostream>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <set>
#include <map>
#include <vector>
#include <string>
//#include <pair>

using namespace std;

namespace FluxSol
{
class FileSearcher:
    public InputBaseContainer
{

    protected:
    //std::map <set <string> , >classnames;
    //OR VOID OR TEMPLATE POINTER!!!!!!
    //std::map< std::vector <string, string>, void *> class_and_name;//class
    string dirpath;                 //filenames dir
    set < string > filenames;       //Without file

    vector < UDOIds> udoids;//class

    public:
    FileSearcher(string path);
    const set <string> & FileNames()const {return filenames;}
    vector <UDOIds> & UDO_Ids(){return udoids;}
    void GetUDOMapsFromFile(const string &str); //PROTECTED!
    void WriteUDOLibFile();


};

}//Namespace FluxSol

#endif // _FILESEARCH_H_

