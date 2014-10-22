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
#ifndef _FV_GRID_H
#define _FV_GRID_H

//Este archivo contiene una malla de volumenes finitos centrada en el cuerpo
//que se hereda de una malla general
#include <vector>
#include <math.h> //For max function

#include "../../Mesh/Node.h"
#include "../../Mesh/Grid.h"
#include "../../FiniteVolume/Mesh/CC_Cell.h"
#include "FvFace.h"
#include "../Boundary/Boundary.h"
#include "../Mesh/Structured.h"
#include "../Mesh/Vertex.h"
#include "../Utils/Utils.h"
#include "../../Type/Operations.h"	//For mixed types
#include "GeometricField.h"


using namespace std;    //Tiene inlines, ver que pasa cuando tiene inlines

namespace FluxSol{

class Fv_CC_Grid:public _Grid{

private:
	 std::vector <_FvFace> face;
	 std::vector <Cell_CC> cell;    //Celdas con nodos centrados en el cuerpo
	 std::vector <Node>    node;    //En el centro de cada una de las cells

	 std::vector<Node>::iterator nodeit;
	 std::vector<_FvFace>::iterator faceit;

	protected:

	public:
	std::vector<Cell_CC>::iterator cellit;
	Boundary boundary;

    Fv_CC_Grid(){};
    //Primero LLAMO A LA FUNCION DE LA CLASE BASE, luego dentro de la funcion ya esta
    //generada la malla y cargo la malla rectangular
    //Por que la paso como argumento??
    Fv_CC_Grid(const int &nex,const int &ney,const double &lx,const double &ly);
   	Fv_CC_Grid(const char *name){fileName=name;this->Read_CGNS();};
	Fv_CC_Grid(string name){fileName=name;this->Read_CGNS();};
	//Constructor para leer desde NASTRAN
	//El boundary deberia salir solo, pero es bueno para colocarle indices
	//Colocando al boundary me ahorro el trabajo de buscar caras libres
	//Deberia arrojar un error si no
	Fv_CC_Grid(vector<_Vertex> &,vector <Cell_CC> &, Boundary &);
	void Iniciar_Caras();
	void AddBoundary(Boundary &bound){boundary=bound;};
	void CalcCellVolumes();



	//Manejo de Celdas
    void PushCell(const Cell_CC &c){cell.push_back(c);};  //Esto podria ser una plantilla
	void PushCell(const std::vector<int> &vc); //Inserto una celda segun conectividad
    //Ojo que puedo hacer esto, puedo tomar el puntero de la misma clase
    //para crear una funcion dentro de la misma clase
    //Esta lo hace ya habiendo iniciado
    void Malla_Rectangular(const double &lx,const double &ly){GenerarMallaRectangular(lx,ly,(*this));};

	Node & Node_(const int &i){return this->node[i];}
    Node & CreateNodeFromCellVerts(const int &cellid);
	void Fv_CC_Grid::CreateNodesFromCellVerts();

    //MODIFIED, THIS MUST A BE A SINGLE SURFACEFIELD
    const SurfaceField<Vec3D> Sf()const;
	//FUNCIONES DE LOG
    void Log(string );    //ES una funcion virtual en la clase base

    // VISUALIZACION DE VARIABLES DE MALLA
	//THE RETURN VALUE COULD BE MODIFIED OR NOT, BECAUSE IT IS NOT CONST
    const Cell_CC & Cell(const int &c)const {return cell[c];}   //devuelvo celda
	const _FvFace & Face(const int &i)const {return face[i];}

	void AssignNeigboursCells();

    std::vector<Cell_CC>::iterator BeginCell(){return cell.begin();}
    std::vector<Cell_CC>::iterator EndCell()  {return cell.end();}

	const Boundary& vBoundary()const {return boundary;}

    //////////////////////
    //      LOG         //
    //////////////////////

	virtual void Read_CGNS();
    void Log_Conect();


	// OUTPUT //
	void Write_vtk();
};

} //Fin FluxSol

#endif
