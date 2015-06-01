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

#include "FileSearch.h"
using namespace FluxSol;

//Reads files from a directory
//http://www.cplusplus.com/forum/beginner/10292/

FileSearcher::FileSearcher(string dir)
{

  ifstream fin;
  string filepath;
  int num;
  DIR *dp;
  struct dirent *dirp;
  struct stat filestat;

  cout << "dir to get files of: " << flush;
  getline( cin, dir );  // gets everything the user ENTERs

  dp = opendir( dir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    //return errno;
    }


  while ((dirp = readdir( dp )))
    {
    filepath = dir + "/" + dirp->d_name;
    cout << "filepath: " <<filepath<<endl;

    // If the file is a directory (or is in some way invalid) we'll skip it
    if (stat( filepath.c_str(), &filestat )) continue;
    if (S_ISDIR( filestat.st_mode ))         continue;

    // Endeavor to read a single number from the file and display it
    fin.open( filepath.c_str() );
    GetUDOMapsFromFile(filepath);
//    if (fin >> num)
//    {
//
//    }
    cout << filepath << ": " << num << endl;
    this->filenames.insert(dirp->d_name);
    fin.close();
    }

  closedir( dp );

}

void
FileSearcher::GetUDOMapsFromFile(const string &fname)
{

    if(fname.substr(fname.find_last_of(".") + 1) == "cpp")
    {
        cout << "Reading file "<<fname<<endl;
        //extract file extension
        InputFile input(fname);
        //input.stripComments();
        //input.stripComments();

        bool classend=false;
        bool found, foundbase;
        string sname,basename;
        while(!classend)
        {
            cout << "Raw Data" << input.rawData<<endl;
            //Get UDO TYPE
            //bool extract_in_between(string &data, string begin, string end,
                                      //string &result,bool check_char_before=false, string acceptList="");
            //Class Name

            found=extract_in_between(input.rawData,"class",":",sname);
            if (found)
            {
                cout << "(Inherited) Defined Class Name: " << sname<<endl;
                //Base Class Name

                foundbase=extract_in_between(input.rawData,"public",";",basename);
                cout << "Base Class Name: " << basename<<endl;
                //UDOIds ids(sname,basename);
                udoids.push_back (UDOIds(sname,basename));


            }
            else classend=true;

        }

    }
    else
    {
        cout << "File " << fname << "must have .cpp extension." <<endl;
    }

    cout <<"Ending "<<endl;

}

void FileSearcher::WriteUDOLibFile()
{

    std::ofstream ofs ("UDOCreator.cpp", std::ofstream::out);
    ofs << "#include \"UDOLibGen.h\" "<<endl;

    ofs << "UDO* CreateUDO(const string &basename){"<<endl;

    ofs << "    UDO *udo;"<<endl;

    vector<UDOIds>::iterator it;
    cout << "UDO Ids size"<<udoids.size()<<endl;
    ofs << "// UDOs Can be built by default constructor and then assigned. <<" << endl;
    for (it=udoids.begin();it!=udoids.end();it++)
    {
        ofs << "    if (basename==\"" << it->ClassName() <<"\""<<")" <<endl;
        ofs << "        udo=new "<<it->ClassName()<<";"<<endl;
    }
    ofs << "return udo;}//UDO Creator"<<endl;
    ofs.close();


}
