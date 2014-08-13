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
#ifndef _MESHINFO_H
#define _MESHINFO_H

#include <iostream>
#include <fstream>
#include <string>
#include "./Mesh/Grid.h"

namespace FluxSol{

class MeshInfo
{

	private:

	protected:
		std::ofstream ios;
		std::string filename;

	public:
		MeshInfo(){};
		//No funciona un const
		MeshInfo(const std::string, _Grid &);
};


} //Fin FluxSol
#endif 