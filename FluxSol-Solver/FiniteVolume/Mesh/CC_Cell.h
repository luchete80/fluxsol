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
#ifndef _CC_CELL_H
#define _CC_CELL_H

#include "../../Mesh/Cell.h"
#include "../../Mesh/Node.h"

namespace FluxSol {
//Celda con nodo centrado en el Cuerpo
//Cell Centered
class Cell_CC:public _Cell{

public:
	Cell_CC(){};
	//Recordar que los constructores no se heredan explicitamente
	Cell_CC(const int &Id, std::vector<int> conect):_Cell(Id,conect){}

    ~Cell_CC(){};


	//Devuelvo un nodo a partir de las coordenadas del cell

};

}//FluxSol

#endif
