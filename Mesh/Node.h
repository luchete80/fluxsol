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
#ifndef _NODE_H
#define _NODE_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include "./Variables/Variable.h"
#include "../Type/Vec3d.h"

namespace FluxSol{
// ATENCION: ESTOS SON LOS NODOS DEL BARICENTRO DONDE SE GUARDA LA INFORMACION COLOCALIZADA
//En lo que es posiciones el nodo puede perteneces a una cara, a una celda o a un vertice
class Node:public Vec3D{


    //Miembros privados
	Vec3D coords;				//Coordenadas

	bool es_ghost;			    //Si esta afuera, AFUERA DE DONDE??
    //Acatengo que ver si es perteneciente a un vectice

	//Variables del nodo
	_Variable <Vec3D> u;    //
	_Variable <double> p;   //No es mejor Escalar?? Si
	int id;	                //Id de Nastran
	std::vector <double> Coord;
	std::vector <double> Coord_cart;
	int SistCoord;
	int SistCoord_int;
	int pos_nastran;	//Ubicacion en el NASTRAN

	public:

	//Constructores
   
	Node(const int &i, const int &sc, vector<double> &coord);
	Node(const double &d):Vec3D(d){}

	void Id(const int &Id){id=Id;}
	int & Id(){return id;}

	//NASTRAN
	int VerId_Nastran();
	Vec3D Coords(){};
	const int Sc();
	const int Sc_int();
	const int Pos_Nastran();
	void Node_Cargar_Ubic(const int u);
	void Cargar_IdSc_Int(const int &val);
	void Iniciar_Coord_cart(vector<double> &coord);
	vector <double> Coord_carts();

	//

	};

}
#endif
