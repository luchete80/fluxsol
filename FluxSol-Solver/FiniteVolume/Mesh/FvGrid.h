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
#include <set>
#include <map>
#include <math.h> //For max function

#include "Node.h"
#include "Grid.h"
#include "CC_Cell.h"
#include "FvFace.h"
#include "Boundary.h"
#include "Structured.h"
#include "Vertex.h"
#include "Utils.h"
#include "Operations.h"	//For mixed types
#include "GeometricField.h"

#include <algorithm>    // std::sort


using namespace std;    //Tiene inlines, ver que pasa cuando tiene inlines

namespace FluxSol{

class Fv_CC_Grid:public _Grid{

private:
	 std::vector <_FvFace> face;
	 std::vector <Node>    node;    //En el centro de cada una de las cells

	 std::vector<Node>::iterator nodeit;
	 std::vector<_FvFace>::iterator faceit;

	 std::set<int> internal_faces;      //TO MODIFY; FOR INSERTIONS

    int np;     //Number of processors

    struct myclass myobject;        //Object usedd to sort, TO MODIFY

	protected:
	    std::vector <Cell_CC> cell;    //Celdas con nodos centrados en el cuerpo

	    vector <vector <int> > face_local_cell_neighbour;  //Local cell neigbours

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
    void Init_Faces();

	Node & Node_(const int &i){return this->node[i];}
    Node & CreateNodeFromCellVerts(const int &cellid);
	void CreateNodesFromCellVerts()
	{

        vector <Vec3D> verts;

        //El nodo tiene el mismo indice que el cell
//        cout << "creating nodes"<<endl;
        for (int cellid=0;cellid<this->Num_Cells();cellid++)
        {
            Cell_CC cell=this->Cell(cellid);
            int globvert;
            Node nod(0.);
            //Recorro los vertices del cell
            for (int n=0;n<cell.Num_Vertex();n++)
            {
                nod+=this->Vertex(cell.Id_Vert(n));
            }
            //Ahora divido por la cantidad de vertices
            nod/=(double)cell.Num_Vertex();
            nod.Id(cellid);
            node.push_back(nod);
        }

        inicie_nodes=true;

    }

    //MODIFIED, THIS MUST A BE A SINGLE SURFACEFIELD
    const GeomSurfaceField<Vec3D> Sf()const;
	//FUNCIONES DE LOG
    void Log(string );    //ES una funcion virtual en la clase base

    // VISUALIZACION DE VARIABLES DE MALLA
	//THE RETURN VALUE COULD BE MODIFIED OR NOT, BECAUSE IT IS NOT CONST
    const Cell_CC & Cell(const int &c)const {return cell[c];}   //devuelvo celda
	const _FvFace & Face(const int &i)const {return face[i];}

	void AssignNeigboursCells();

	const map<vector <int> , int > FaceVertsMap()const;

    std::vector<Cell_CC>::iterator BeginCell(){return cell.begin();}
    std::vector<Cell_CC>::iterator EndCell()  {return cell.end();}

    const Vec3D CellFaceAf_Slow(const int &cell, const int &cellface) const
    {
        Vec3D af;
        int faceid=this->Cell(cell).Id_Face(cellface);
        af=this->Face(faceid).Af();
        //cout << "face id:" <<faceid<<endl;
        if (this->Face(faceid).Cell(0)!=cell)
        {
            af=-af;
            //cout << "Inverted"<<endl;
        }
        return af;
    }

    const int CellFaceSign(const int &cell, const int &cellface) const
    {
        int sign=1.;
        int faceid=this->Cell(cell).Id_Face(cellface);
        //cout << "face id:" <<faceid<<endl;
        if (this->Face(faceid).Cell(0)!=cell)
        {
            sign=-1.;
            //cout << "Inverted"<<endl;
        }
        return sign;
    }
	const Boundary& vBoundary()const {return boundary;}

    //////////////////////
    //      LOG         //
    //////////////////////

	virtual void Read_CGNS();

    void Log_Conect();



	// OUTPUT //
	void Write_vtk();

	Fv_CC_Grid & operator=(const Fv_CC_Grid & right)
	{
	    this->num_verts=right.num_verts;						//Este Numero no es constante por si modifico la malla
														//Si voy a tener vertices tipo baffle no lo voy a tener en cuenta
        this->num_cells_int=right.num_cells_int;				//Numero de celdas efectivas
        this->num_faces_int=right.num_faces_int;				//Numero de faces efectivas (INTERIORES)
        this->num_faces=right.num_faces;
        this->num_cells=right.num_cells;
        this->num_verts=right.num_verts;


        for (int c=0;c<right.num_cells;c++)
        {
            this->cell.push_back(right.Cell(c));
            this->node.push_back(right.Node_(c));
        }
        for (int v=0;v<right.num_verts;v++) this->vert.push_back(right.Vertex(v));
        for (int f=0;f<right.num_faces;f++) this->face.push_back(right.Face(f));

        this->boundary=right.vBoundary();

        this->SetFaceLocalCellNeighbours(); //New

        return *this;
	}


	const GeomField <Scalar> Vp() const
	{
        GeomField <Scalar> ret(this->Num_Cells());


        for (int c=0;c<this->Num_Cells();c++)
        {
            ret.Val(c,this->Cell(c).Vp());
        }
        return ret;
	}

	int Partition();

	void SetFaceLocalCellNeighbours();
	const vector <int> & FaceLocalCellNeighbour(const int &f)const{return this->face_local_cell_neighbour[f];}

};

} //Fin FluxSol

#endif
