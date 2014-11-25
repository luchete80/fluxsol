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
#include "FvGrid.h"

//To utils
//Redundance with vector c++ lib
template <class T> const T& max_int(const T& a, const T& b) {
	return (a<b) ? b : a;     // or: return comp(a,b)?b:a; for version (2)
}

using namespace std;

namespace FluxSol {

Fv_CC_Grid::Fv_CC_Grid(const int &nex,const int &ney,const double &lx,const double &ly):
_Grid(nex,ney)
{
	double elx,ely;
	elx=lx/nex; ely=ly/ney;
	//Hasta aca genero los vertices
	GenerarMallaRectangular(elx,ely,(*this));

	//Ahora debo generar los nodos
	cout << "Creating Nodes..."<<endl;
	CreateNodesFromCellVerts();
	//Inicio las caras
	cout << "Initializing faces..."<<endl;
	Iniciar_Caras();
	cout << "Assigning Neighbours..."<<endl;
	AssignNeigboursCells();
}

//Inserto una celda
void FluxSol::Fv_CC_Grid::PushCell(const std::vector <int> &vc)
{
    Cell_CC c;
    c.Conect(vc);
    PushCell(c);
}

void FluxSol::Fv_CC_Grid::Log(string s)
{
    //ESTO PUEDO COLOCARLO EN UN LOG APARTE PARA CLASE BASE
    Log_Abrir(s.c_str());       //Abro archivo de log
    meshlog << "Vertices Coordinates" << endl;
    meshlog << endl;
	Log_Verts();                //El Log de los vertices es el mismo
    std::vector < Cell_CC >::iterator ccellit;
    meshlog << endl;
	meshlog<<"//////////////////////////////"<<endl;
	meshlog << endl;
	meshlog<<"Mesh Cells"<<endl;
	meshlog << endl;
	int i=0;
    for (ccellit=cell.begin();ccellit!=cell.end();ccellit++)
	{
		meshlog << "Cell " << i << endl;
		meshlog<<"Vertices"<<endl;
        meshlog<<(*ccellit).Imprimir_Conect()<<endl;
		meshlog<<"Faces"<<endl;
		for (int f=0;f<(*ccellit).Num_Faces();f++)
			meshlog<<(*ccellit).Id_Face(f)<< "  ";
		meshlog <<endl;
		meshlog << "Neighbours" << endl;
		for (int n=0; n<(*ccellit).Num_Neighbours(); n++)
            meshlog << (*ccellit).Neighbour(n) << endl;
        meshlog << endl;
        i++;

	}
    //Ahora viene la diferencia
	//Busco las coordenadas de los nodos
	meshlog<<"//////////////////////////////"<<endl;
	meshlog << endl;
	meshlog << "Nodes Coordinates" << endl;
	meshlog << endl;
	for (nodeit=node.begin();nodeit!=node.end();nodeit++)
		meshlog<<nodeit->Imprimir_Coord()<<endl;

	//Informacion de faces
	int f=0;
    meshlog << endl;
	meshlog<<"//////////////////////////////"<<endl;
	meshlog << endl;
	meshlog<<"Face Conectivity"<<endl;
	meshlog << endl;
	for (faceit=this->face.begin();faceit!=face.end();faceit++)
	{
		meshlog<<"Face "<< f<<" cells"<<endl;
		for (int i=0;i<faceit->NumCells();i++)
			meshlog<<faceit->Cell(i)<<"  ";
		meshlog<<endl;

		meshlog<<"Face "<< f<<" vertices"<<endl;
		for (int i=0;i<faceit->NumVerts();i++)
			meshlog<<faceit->Vert(i)<<"  ";
		meshlog<<endl;

		meshlog<<"Face "<< f<<" fp"<<endl;
        meshlog<<faceit->Fp().outstr()<<"  ";
		meshlog<<endl;

		f++;
	}
	f=0;
	meshlog<<"//////////////////////////////"<<endl;
	meshlog << endl;
	meshlog<<"Face Geometry"<<endl;
	meshlog << endl;
	for (faceit=this->face.begin();faceit!=face.end();faceit++)
	{

		meshlog << "Face "<<f<<"Ad:"<<endl;
		faceit->Ad().Log(meshlog);
		meshlog<<endl;
		meshlog << endl;

		f++;
	}
    //ESTO ES NUEVO PARA LA CLASE DERIVADA

}

// VARIABLES DE NODOS
// Creo los nodos centrales
//La llamo muchas veces
Node & FluxSol::Fv_CC_Grid::CreateNodeFromCellVerts(const int &cellid)
{
	vector <Vec3D> verts;

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

	return nod;
}

//Esto no llama mil veces la funcion
void FluxSol::Fv_CC_Grid::CreateNodesFromCellVerts()
{
	vector <Vec3D> verts;

	//El nodo tiene el mismo indice que el cell
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
		//cout <<cellid<<endl;
		//Ahora divido por la cantidad de vertices
		nod/=(double)cell.Num_Vertex();
		nod.Id(cellid);
		node.push_back(nod);
	}

	inicie_nodes=true;

}

// Constructor que sirva para construir por ejemplo desde un archivo
Fv_CC_Grid::Fv_CC_Grid(vector <_Vertex> &vvert,vector <Cell_CC> &vcell, Boundary &bound)
{
	num_verts=vvert.size();
	num_cells=vcell.size();
	//Agrego los vertices
	for (int nv=0;nv<num_verts;nv++)	vert.push_back(vvert[nv]);
	for (int nc=0;nc<num_cells;nc++)	cell.push_back(vcell[nc]);
	for (int nc=0;nc<num_cells;nc++)	CreateNodeFromCellVerts(nc);

	boundary=bound;

	inicie_cells=true;
	//Creo los nodos
	inicie_nodes=true;

	//Chequeo si es 2D y agrego los vertices que corresponden para convertirlo en 3D
	//(Recordar que para ensamblar se hace como 2D)
	//Chequeo la cantidad de vertices
	_Vertex vp;
	for (cellit=cell.begin();cellit!=cell.end();cellit++)
	{
		if(cellit->Num_Vertex()<=4)	//Es bidimensional
		{
			//Chequeo cual es la coordenada normal al plano bidimensional
			//_Vertex vd1(vvert[cellit->Vert(1)-vvert[cellit->Vert(0)]);
			//_Vertex vd2(vvert[cellit->Vert(1)-vvert[cellit->Vert(0)]);//prod=;

		//vv[cellit->Vert(1)]
		}

		//Construyo las caras internas
		//Recorro los vertices y hago grupos de 2 caras, en caso que no esten las agrego
		this->Iniciar_Caras();

	}



	//Chequeo si no queda una parte libre del boundary
}

void Fv_CC_Grid::AssignNeigboursCells()
{

	int c=0;
//	cout <<"neigh assign" <<endl;
//    cout << "Grid Num Faces" <<this->Num_Faces()<<endl;
//    cout << "Grid Face Size" <<this->face.size()<<endl;
//    cout << "Grid Cell Size" <<this->cell.size()<<endl;
    bool end=false;
    if (!this->Num_Faces() || this->face.size()==0 || this->cell.size()==0)
        end=true;
    if (!end)
    {

        for (cellit=cell.begin();cellit!=cell.end();cellit++)
        {
//            cout << "neigh assign cell "<<c<<endl;
//            cout << "Num Faces" <<cellit->Num_Faces()<<endl;
            for (int intface=0;intface<cellit->Num_Faces();intface++)
            {
//                cout << "Int face" << intface<<endl;
//                cout << "Id Face"<<cellit->Id_Face(intface);
                int idface=cellit->Id_Face(intface);
                _FvFace fvface=this->Face(idface);
                if (!fvface.Is_Null_Flux_Face() && !fvface.Boundaryface())
                {
//                    cout << "Flux Internal Face"<<endl;
                    if (Face(idface).Cell(0)==c)
                        cellit->AddNeighbour(Face(idface).Cell(1));
                    else
                        cellit->AddNeighbour(Face(idface).Cell(0));
                }
            }
            c++;
        }
    }
    else
    {
        cout <<"ERROR: Grid does not have any face..." <<endl;
    }
}


//Esto supone que el tipo de elementos ya esta definido
//Virtual function
void Fv_CC_Grid::Iniciar_Caras()
{
	//Numero de faces dependiendo de los numeros de vertices
	int numcellfaces [9]={0,0,0,0,4,5,5,0,6};

	int v1,v2;
//
	int numfaces=0;
	int nfi=0;	//Numero de caras interiores
	int nfb=0;	//Numero de caras en el borde
	bool intfacefound;
	int dc=0;		//dummy cells
	int c2;


//    //Declaro otro iterador que no esta aca
//
//    //Inicio las celdas
    //cout << "Cell initialized "<<inicie_cells<<endl;
    //cout << "Node initialized "<<inicie_nodes<<endl;
    if (!inicie_cells || !inicie_nodes)
    {
        cout << "Nodes or cells not initiated. Grid initiation failed..."<<endl;
        return;

    }

	vector <int> v(0);
	//vector<vector<int> > vec(4, vector<int>(4))


	//Para borde
	int numdummycells=0;

	vector < vector <bool> > cellfacefound;
	//cout << "cell loop"<<endl;
	//Control for
	//Init cell face containers
	for (cellit=cell.begin();cellit!=cell.end();cellit++)
	{
	    //cout <<"cell loop"<<endl;
		cellit->Init_Idface(numcellfaces[cellit->Num_Vertex()]);
		cellit->Init_NumberofFaces(numcellfaces[cellit->Num_Vertex()]);
		vector <bool> cellfaces;
		cellfaces.assign(numcellfaces[cellit->Num_Vertex()],false);
		cellfacefound.push_back(cellfaces);
	}
    //Esto antes era sin iteradores
    //Ahora recorro asi las celdas
	int c1=0;	//indice de la celda 1
	//Celda primera
    for (cellit=cell.begin();cellit!=cell.end();cellit++)
    {
        //cout <<"cell loop"<<endl;
		//cout <<"Rcorriendo celda"<<c1<<endl;
		vector <int> intverts;				//Los vertices interiores que voy encontrando

		vector <Cell_CC>::iterator cellit2;		//cellit 2 es a la segunda cara del elemento
		int c2=c1+1;
		//Recorro las caras
		int numfaceverts;
		vector <int> tempNodes;							//Nodos de cada cara

		for (int nf=0;nf<numcellfaces[cellit->Num_Vertex()];nf++)
		{
			tempNodes=cellit->GlobalVertFace(nf);
			numfaceverts=tempNodes.size();

			//Comienzo del 1 aumentado una posicion
			//Busco la otra celda que comparta los vertices de la cara de cellit
			cellit2=cellit+1;
			c2=c1+1;
			intfacefound=false;								//enc refiere a que encontro una cara interna (sino es del borde)
			//Si es la ultima celda solo quedan dummy cells
			int nfenc2;
			while (cellit2!=cell.end() && !intfacefound && !cellfacefound[c1][nf])//Recorrida por todos los elementos para ver quien comparte lado
			{
				vector <int> tempNodes2;	//cell2 verts
				int numfaceverts2;
				for (int nf2=0;nf2<numcellfaces[cellit2->Num_Vertex()];nf2++)
				{
					tempNodes2=cellit2->GlobalVertFace(nf2);
					numfaceverts2=tempNodes2.size();
					//Compare tempNodes (vertices of cell1 face) with
					int nvenc=NumVecElemFound(tempNodes,tempNodes2);
					//if (NumVecElemFound(tempNodes,cellit2->Conect())==tempNodes.size())
					//Si no lo coloco asi no anda
					int maxfaces=max_int(numfaceverts,numfaceverts2);
					//Found an internal face
					//cout << "nf1 nf2 " << nf << " "<<nf2<<endl;
					//cout << "c1 c2" << c1 << " " << c2<<endl;
					//cout << "TempNodes"<<endl;
                    //for (int nn=0;nn<tempNodes.size();nn++)cout << tempNodes[nn]<<endl;
                    //cout << "TempNodes2"<<endl;
                    //for (int nn=0;nn<tempNodes2.size();nn++)cout << tempNodes2[nn]<<endl;
                    //cout << "nvenc maxfaces" <<nvenc<< maxfaces<<endl;
					if (nvenc==maxfaces)
					{
						intfacefound=true;
						//Los numeros de vertices encontrados tienen que ser mayores o iguales a 3
						// ENCONTRE LA CARA INTERNA
						//Ya tengo que tener iniciado el numero de cara
						//Recorro las caras
						//////Aca cambio esto y construyo la cara

						vector <int>	 facecells(c1,c2);	//aca el primer cell es el c
						vector <_Vertex> facevertex;
						vector <Node>	 facenodes;
						facenodes.push_back(node[c1]);facenodes.push_back(node[c2]);
						for (int fn=0;fn<numfaceverts;++fn) facevertex.push_back(this->vert[tempNodes[fn]]);
						nfenc2=nf2;

						//Busco que vertices encontro
						//Falta ubicar los Cells, pero se corresponden con los nodos en las FvGrids
						//Ojo adentro de la funcion f se ve para donde va la normal
						_FvFace f(nf,tempNodes,facevertex,facenodes,false);

						cellfacefound[c2][nf2]=true;

						//Adding global face indexes for each cell
						this->Cell(c1).Id_Face(nf,numfaces);
						this->Cell(c2).Id_Face(nfenc2,numfaces);
                        //cout << "Found internal face"<<numfaces<<endl;
                        //cout << "Internal faces"<<nfi<<endl;
						face.push_back(f);
						nfi++;
						numfaces++;

					} //Fin del if	found internal face


				}//End of search faces
				c2++;
				cellit2++;
			}//fin del while

			//Tengo que agrupar los vertices del cell1  que no fueron encontrados
			//Puede ser que haya mas de 2 faces, para esto puedo hacer lo siguiente
			//1)tengo que ver las normales
			//2) Puedo asociar las conectividades de acuerdo a Nastran
			//Busco que
			//De todos los vertices del cell1 no encontre 3 o mas comunes con otros cells
			//Boundary faces
			if (!intfacefound && !cellfacefound[c1][nf])	//Si tengo vertices que no son interiores
			{
				//Create Dummy Cells

				vector <int>	 facecells(c1,this->Num_Cells()+numdummycells);	//aca el primer cell es el c, el c2 es el dummy
				vector <_Vertex> facevertex;
				vector <Node>	 facenodes;
				facenodes.push_back(node[c1]);		//El otro nodo es ficticio, puedo ver que hago con esto
													//Lo necesito para la distancia PN
				for (int fn=0;fn<numfaceverts;++fn) facevertex.push_back(this->vert[tempNodes[fn]]);

				this->Cell(c1).Id_Face(nf,numfaces);

				//Busco que vertices encontro
				//Falta ubicar los Cells, pero se corresponden con los nodos en las FvGrids
				//Ojo adentro de la funcion f se ve para donde va la normal
				//Si en esta funcion entra un vertice ya se que es una BoundaryFace
				_FvFace f(nf,tempNodes,facevertex,facenodes,true);
				face.push_back(f);
				nfb++;
				numdummycells++;
				numfaces++;
			}

		}//Fin del for de faces para cell c2
		c1++;
	}// Fin de for de cell c1

	//Bidimensional case
	if (this->bidim_mesh)
	{
		for(cellit=cell.begin();cellit!=cell.end();cellit++)
		{
			//In bidimensional cases face 0 and 1 are null_flux_faces
			this->Face(cellit->Id_Face(0)).Null_Flux_Face(true);
			this->Face(cellit->Id_Face(1)).Null_Flux_Face(true);
		}
	}

	num_faces=numfaces;
	num_boundary_faces=nfb;

	cout << "[I] Faces: " <<numfaces<<endl;
	cout << "[I] Boundary Faces: " <<num_boundary_faces<<endl;


}


void Fv_CC_Grid::CalcCellVolumes()
{
	for (int c=0;c<Num_Cells();c++)
	{
		Scalar vol=0.;
		for (int cellface=0;cellface<this->cell[c].Num_Faces();cellface++)
		{
			_FvFace face = this->Face(Cell(c).Id_Face(cellface));
			vol+=face.Af()&face.Center();
			this->Cell(c).Vp()=vol;
		}

	}

}

//TO MODIFY, MUST RETURN A REFERENCE
const GeomSurfaceField<Vec3D> Fv_CC_Grid::Sf() const
{
	GeomSurfaceField<Vec3D> ret(this->num_faces);
    ret.AssignGrid(this);

	for (int f = 0; f < this->num_faces; f++)
	{
		ret.Val(f, this->face[f].Af().VecNorm());
	}

	return ret;
}

}
