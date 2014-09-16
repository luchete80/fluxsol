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
#ifndef _FACE_H
#define _FACE_H


#include "../Type/Vec3d.h"
#include "Vertex.h"
namespace FluxSol
{

//Generica para elementos finitos y volumenes finitos
class _Face
{

    protected:
    int id;
	Vec3D vec_normal;									//Vector normal (normalizado)

	std::vector <int> vertex;                    //indice del vertice
	std::vector <int> cell;
	int num_vertex;
	//VARIABLES QUE PUEDEN ESTAR EN UNA CLASE DERIVADA
	Vec3D sm;                                 //Punto central de la cara

	//Directamente puedo colocar el puntero a cada uno de los vertices
	//Es un vector de punteros, mucho mas peque�o en memoria que los indices
	std::vector <Vec3D> *pvertex;


	Vec3D af;											//Area escalar
	double  norm_af;									//Vector de area = area x n, esto lo hago para evitar hacer siempre el producto

    //Las areas cambian

	//Datos del borde
	bool boundaryface;
	bool null_flux_face;										//The mesh is paralell to a bidimensional plane


	// FUNCIONES //


	public:
        // CONSTRUCTORES
		_Face(){null_flux_face=false;};
		_Face(const vector<int> & cells, const std::vector<int> & verts);
		//Este es el mas utilizado
        _Face(const int &Id,const std::vector <_Vertex> &verts);
        //PODRIA DEFINIR UN TEMPLATE PERO NO TIENE SENTIDO POR ESO COLOCO UN GRID
        //PARA ESO SIRVEN LAS CLASES DERIVADAS
//		template <typename T>
//		_Face(const vector<int> & cells, const vector<int> & verts, const T&);
//      _Face(const vector<int> & cells, const vector<int> & verts, _Grid *malla);
		const int & Cell(const int &i){return cell[i];};
		const int & Vert(const int &i){return vertex[i];};
		vector<int> &Vert(){return this->vertex;}
		const int & NumCells(){return cell.size();}
		const int & NumVerts(){return num_vertex;}
		Vec3D& Af(){return af;}

		Vec3D& Center(){return sm;}

		const bool & Boundaryface()const {return boundaryface;}
		void Null_Flux_Face(bool b){null_flux_face=b;}
		const bool & Is_Null_Flux_Face()const{return null_flux_face;}

		~_Face(){};



};

//Operadores aparte
Vec3D &CalcNormal(const vector <_Vertex> &verts);

}

#endif
