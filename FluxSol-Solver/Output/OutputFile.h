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
#ifndef _OUPUT_FILE_H
#define _OUPUT_FILE_H

#include "FvGrid.h"
#include "FvField.h"

namespace FluxSol
{
class OutputFile
{
	protected:
	//To check if new fields have same mesh
	const Fv_CC_Grid &grid;

	string fileName;
    ofstream file;     //Previously this was inside each function

	public:
	//Constructors
	OutputFile(string name, Fv_CC_Grid &grid)
	{
        this->fileName=name;
        this->file.open((fileName).c_str(),ios::out);
	}
	template <typename T>
	OutputFile(string name, _CC_Fv_Field<T> &field);

    //TO MODIFY
	template <typename T>
    OutputFile(string name, _CC_Fv_Field<T> &field, const int &comp);


	template <typename T>
	OutputFile(string name, Vertex_Fv_Field<T> &field);

	template <typename T>
    OutputFile(string name, Vertex_Fv_Field<T> &field,const int &comp);


    void WriteGrid();

    template <typename T>
    void WriteField(const Vertex_Fv_Field<T> &field);


	template <typename T>
	void AddFieldValues(_CC_Fv_Field<T>);

	void WriteFooter();

	OutputFile::~OutputFile()    //Destructor
    {
        file.close();
    }


};
}




#endif
