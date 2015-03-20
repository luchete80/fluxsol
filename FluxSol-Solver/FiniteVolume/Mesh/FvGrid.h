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

	virtual void Read_CGNS()
	{

        //raw data - vertices
        //base class function
        Read_InitialCGNS();

        //Read boundary cells - Assuming boundary cell entry
        vector<vector<int> > bpnode;

        int boundnode =0;

        //--------------------------------
        //IF INPUT IS WITH BOUNDARY CELLS
        //--------------------------------
                //raw->Cells
        vector <Cell_CC> vboundcell;    //For bidimensional cells
        int boundelem =0;
        //Read boundary cells - Assuming boundary cell entry
        vector<vector<int> > bpelem;
        //Boundary Patches bc
        for (int bp=0;bp<this->raw.bc_elem_list.size();bp++)
        {
            //Associate face <-> 2d cell coinc
            //bcfaces.push_back()
            vector <int> temp;

            //Search bc vertex in each 2D Cell
            set <int> sbc = this->raw.bc_elem_list[bp];
            set <int>::iterator sit=sbc.begin();


            for (; sit!=sbc.end();sit++)
            {
                boundelem++;
                temp.push_back(*sit);
            }
            bpelem.push_back(temp);
        }

        //vector<vector<int>> idbcellasoc(boundelem,vector<int>(2,-1)); //Id cells
        vector<int> idbcell(boundelem,-1);
        int bcell=0;

        for (int idcell =0 ; idcell<raw.cellConnIndex.size();idcell++)
        {
            int cellvertnum;
            if (idcell<(raw.cellConnIndex.size()-1))
                cellvertnum = raw.cellConnIndex [idcell+1] - raw.cellConnIndex [idcell];
            else cellvertnum = raw.cellConnectivity.size()- raw.cellConnIndex [idcell] ;

            vector<int> connect;
            for (int cv =0 ; cv<cellvertnum;cv++)
                connect.push_back(raw.cellConnectivity[ raw.cellConnIndex [idcell] + cv]);

            Cell_CC scell(idcell,connect);

                //Check if this is a boundary element
                //TO MODIFY, MUST CONSIDER TETRA CELLS
                if (connect.size()<=4)   //All connectivity numbers are 3d
                    for (int bp=0;bp<bpelem.size();bp++)
                        for(int el=0;el<bpelem[bp].size();el++)
                            if (idcell==(bpelem[bp][el]-1))     //Index of rawdata is counting from 1
                            {
                                vboundcell.push_back(scell);
                                idbcell[bcell]=idcell;
                                bcell++;
                            }

                //}

        }

        //----------------------------
        // BOUNDARY NODES
        //----------------------------
        //Boundary Patches bc
        for (int bp=0;bp<this->raw.bc_elem_list.size();bp++)
        {
            vector <int> temp;

            //Search bc vertex in each 2D Cell
            set <int> sbc =this->raw.bocoNodes[bp];
            set <int>::iterator sit=sbc.begin();


            for (; sit!=sbc.end();sit++)
            {
                boundnode++;
                temp.push_back(*sit);
            }
            cout << "[I] BC "<< bp << "has " <<temp.size()<< " nodes" <<endl;
            bpnode.push_back(temp);
        }

        //vector<vector<int>> idbcellasoc(boundelem,vector<int>(2,-1));	//Id cells
        bcell=0;

        for (int idcell =0 ; idcell<raw.cellConnIndex.size();idcell++)
        {
            int cellvertnum;
            if (idcell<(raw.cellConnIndex.size()-1))
                cellvertnum = raw.cellConnIndex [idcell+1] - raw.cellConnIndex [idcell];
            else cellvertnum = raw.cellConnectivity.size()- raw.cellConnIndex [idcell] ;

            vector<int> connect;
            for (int cv =0 ; cv<cellvertnum;cv++)
                connect.push_back(raw.cellConnectivity[ raw.cellConnIndex [idcell] + cv]);

            Cell_CC scell(idcell,connect);

                //Check if this is a boundary element
                //TO MODIFY
                if (connect.size()>4)	//All connectivity numbers are 3d, BUT TETRA?
                    this->cell.push_back(scell);

        }

        //Updating cell number
        this->num_cells=cell.size();

        // Nodes
        CreateNodesFromCellVerts();

        this->inicie_nodes=true;
        this->inicie_cells=true;
        Iniciar_Caras();
        AssignNeigboursCells();
        CalcCellVolumes();
        CreateNodesFromCellVerts();

        vector <Patch> vpatch;

        cout << "boundary faces size" <<temp_boundfaces.size()<<endl;
        std::vector<std::list <int> >bpfaces;

//        cout << "Boundary zone nodes" << bpnode[0][node]<<endl;
        cout << "[I] Number of Patches: " << this->raw.bc_elem_list.size() <<endl;
        int nBocos=raw.bocoNameMap.size();  //Extrcted from freecfd
        cout << "[I] Creating Patches ..."<<endl;

        string pname;
        int totalboundfaces=temp_boundfaces.size();
        for (int bp=0;bp<this->raw.bc_elem_list.size();bp++)    //Look Through patch
        {
            cout << "[I] Searching patch " << bp << ", remaining boundary faces count: "<<temp_boundfaces.size()<<endl;
            pname=this->imported_patchnames[bp];
            list <int> temp;

            int nfoundfaces=0;
            if (raw.bocoNodes[bp].size()>0)
            {
                for (int bf=0;bf<temp_boundfaces.size();bf++)   //Look through all boundary faces, coincidence with each boconodes patch
                {
                    int idf=temp_boundfaces[bf];
                    //bool enc=FindAllVals(this->Face(idf).Vert(), bpnode[bp]);
                    bool enc=true;
                    for (int n=0;n<this->Face(idf).Vert().size();n++)
                        if (!(this->raw.bocoNodes[bp].find(this->Face(idf).Vert()[n])!=this->raw.bocoNodes[bp].end())) enc=false;

                    if (enc)
                    {
                        temp.push_back(idf);
                        temp_boundfaces.erase(temp_boundfaces.begin()+bf);
                        bf--;
                    }
                }
            }
            else if (bpelem[bp].size()>0) //If bc are defined via
            {
               //Looking through raw elements (faces in Grid)
                for (int el=0;el<bpelem[bp].size();el++)
                {
                    vector<int> faceverts;
                    //Adding element vertices
                    //for (int iv=0;iv<this->Cell(bpelem[bp][el]).Num_Vertex();iv++)
                    for (int iv=0;iv<vboundcell[bcell].Num_Vertex();iv++)
                        faceverts.push_back(vboundcell[bcell].Vert(iv));

                    for (int idf=0;idf<this->Num_Faces();idf++)
                    {
                        bool enc=FindAllVals(faceverts,this->Face(idf).Vert());
                        if (enc)
                        {
                            //Add face idf to Boundary patch - Agrego idf al Patch
                            temp.push_back(idf);
                        }
                    }
                    bcell++;
                }//End element
            }

//
//            bool badpatch=false;
//            if (  ( bp==(this->raw.bc_elem_list.size()-1) && (temp.size()==totalboundfaces) )  )  //If it is last pacth and has all faces
//                badpatch=true;


            bpfaces.push_back(temp);
            cout << "[I] Created Patch "<<pname<<", Face Count: " <<temp.size()<<endl;
            Patch p(pname,bpfaces[bp]);
            vpatch.push_back(p);

//            else
//            {
//                cout << "[E] Wrong Last Patch found. Created Patch_1"<<endl;
//                temp.clear();
//                for (int i=0;i<temp_boundfaces.size();i++)temp.push_back(temp_boundfaces[i]);
//
//                bpfaces.push_back(temp);
//                Patch p(pname,bpfaces[bp]);
//                vpatch.push_back(p);
//            }
        }



        Boundary bound(vpatch);
        this->AddBoundary(bound);

        cout << "[I] Mesh created ..." << endl;

	}
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
};

} //Fin FluxSol

#endif
