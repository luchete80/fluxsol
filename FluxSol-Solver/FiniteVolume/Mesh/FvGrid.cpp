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
#include <time.h>

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
	CalcCellVolumes();
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
    meshlog << "Cell Centers (Nodes)"<<endl;
    for (ccellit=cell.begin();ccellit!=cell.end();ccellit++)
	{
		meshlog<< "["<<i<<"]"<<this->Node_(i).outstr()<<endl;
		i++;
	}

	i=0;
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
        cout << "ERROR: Grid does not have any face..." << endl;
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
	//cout << "end cell loop"<<endl;

    //Esto antes era sin iteradores
    //Ahora recorro asi las celdas
	int c1=0;	//indice de la celda 1
	//Celda primera
    for (cellit=cell.begin();cellit!=cell.end();cellit++)
    {
        //cout <<"cell loop"<<endl;
//		cout <<"Looking through cell "<<c1<<endl;
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

				temp_boundfaces.push_back(numfaces);

				nfb++;
				numdummycells++;
				numfaces++;
			}

		}//Fin del for de faces para cell c2
		c1++;
	}// Fin de for de cell c1

	//Bidimensional case
	//Only for regular mesh case
//	if (this->bidim_mesh)
//	{
//		for(cellit=cell.begin();cellit!=cell.end();cellit++)
//		{
//			//In bidimensional cases face 0 and 1 are null_flux_faces
//			this->Face(cellit->Id_Face(0)).Null_Flux_Face(true);
//			this->Face(cellit->Id_Face(1)).Null_Flux_Face(true);
//		}
//	}

	num_faces=numfaces;
	num_boundary_faces=nfb;

	//Init Face orientation

	cout << "[I] Faces: " <<numfaces<<endl;
	cout << "[I] Boundary Faces: " <<num_boundary_faces<<endl;


}


//
//If searching for an element is important, I'd recommend std::set instead of std::vector. Using this:
//
//std::find(vec.begin(), vec.end(), x)
//
//runs in O(n) time, but std::set has its own find() member (ie. myset.find(x)) which runs in O(log n) time - much more efficient with large numbers of elements.
//
//std::set also guarantees all the added elements are unique, which saves you from having to do anything like 'if not contained then push_back()...`.
////
void Fv_CC_Grid::Init_Faces()
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
    else
    {
        set < vector <int> > faceverts;
        set < vector <int> > deffaceverts;
        vector < vector <int> > faceverts2;
        vector <int> cellsperface;  //Indicates
        set< vector <int> > ::iterator it;
        vector<vector <int> > cellglobalfaces(this->Num_Cells());                      //Index to global cell faces
        vector<vector <int> > facecells;                      //Index to global cell faces
        set <vector <int> >::iterator sit;
        //map<vector <int>, int> mymap;
        //map<vector <int>, int> ::iterator mymapit;
        map<int, vector <int> > mymap;
        map<int, vector <int> > ::iterator mymapit;

        map<vector <int> , int > sortfacemap;   //map to the sorted face vertex set
        map<vector <int> , int >::iterator sortfacemapit;


    	for (cellit=cell.begin();cellit!=cell.end();cellit++)
        {
            //cout <<"cell loop"<<endl;
            cellit->Init_Idface(numcellfaces[cellit->Num_Vertex()]);
            cellit->Init_NumberofFaces(numcellfaces[cellit->Num_Vertex()]);
        }

        //vector<vector<int> > vec(4, vector<int>(4))


        //Para borde
        int numdummycells=0;

        vector < vector <bool> > cellfacefound;
        //cout << "cell loop"<<endl;
        //Control for
        //Init cell face containers
        int c=0;
        double tenperc=this->Num_Cells()/10.0;
        int tennum=0;
        bool foundten[10];for (int i=0;i<10;i++)foundten[i]=false;
        double perc;
        int k=0;
        int numfaces=0;
        int globalfaceid=0;

        double ittime_spent;
        clock_t ittime_begin, ittime_end, ittime_temp;
        ittime_end = clock();
        cout << "[I] Creating Faces..."<<endl;

        for (cellit=cell.begin();cellit!=cell.end();cellit++)
        {
            vector<int> tempglobalfaces;
            //cout <<"cell "<<c<<endl;
 //           cellit->Init_Idface(numcellfaces[cellit->Num_Vertex()]);
 //           cellit->Init_NumberofFaces(numcellfaces[cellit->Num_Vertex()]);

            //cellfaces.assign(numcellfaces[cellit->Num_Vertex()],false);
            //cellfacefound.push_back(cellfaces);

            //cout << "Cell "<<c<<endl;
            for (int nf=0;nf<numcellfaces[cellit->Num_Vertex()];nf++)
            {
                std::pair<std::set< vector <int> >::iterator,bool> ret;    //To check if face are stored
                //Pushing cell faces to set
                int numfaceverts;
                vector <int> tempNodes;							//Nodos de cada cara
                tempNodes=cellit->GlobalVertFace(nf);
                numfaceverts=tempNodes.size();
                vector <int> tempNodesSort(tempNodes);
                std::sort (tempNodesSort.begin(), tempNodesSort.end(), myobject);
                vector <int> tempv;

                ret = faceverts.insert(tempNodesSort);

                it=ret.first;

                //if ((facepos)>cellsperface.size()) cout << "ERROR"<<endl;

                if (ret.second==true)   //Inserted, New faces
                {

                    deffaceverts.insert(tempNodes);
                    mymap[globalfaceid]=tempNodes;
                    mymap.insert(std::pair<int, vector<int> > (globalfaceid, tempNodes) );

                    sortfacemap.insert(std::pair< vector <int> , int > (tempNodesSort, globalfaceid) );
                    //mymap[tempNodes]=globalfaceid;  //this is long way faster than vector push_back
                    //mymap.insert(std::pair<vector<int>,int> (tempNodes,globalfaceid) );

                    cellglobalfaces[c].push_back(globalfaceid);
                    vector <int> vt;    vt.push_back(c);
                    facecells.push_back(vt);

                    //cout << "Global Face Id "<< globalfaceid<<endl;
                    this->Cell(c).Id_Face(nf,globalfaceid);
                    globalfaceid++;
                    //*it=tempNodes;
                    //cout << "true"<<endl;
                }
                else
                {
                    int faceid=sortfacemap[tempNodesSort];
//                    sortfacemapit=sortfacemap.find(tempNodesSort);
//                      int faceid=sortfacemapit->second;
//                    if (sortfacemapit!=sortfacemap.end())
//                    {
//                        cout << "face found " << sortfacemapit->second<<". facecells size"<< facecells.size()<<endl;
//                    }
                    //map find is equal to [] operator
                    //cout << "Face found" <<sortfacemap[tempNodesSort]<<endl;
                    //facecells[sortfacemapit->second].push_back(c);
                    cellglobalfaces[c].push_back(faceid);
                    facecells[faceid].push_back(c);
                    this->Cell(c).Id_Face(nf,faceid);
                }

            }//En cell faces

            if (!foundten[tennum] && c>tennum*tenperc)
            {
                foundten[tennum]=true;
                tennum++;
                cout << tennum*10<< " % ";
            }

            c++;
        }
        cout << "\n";


        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        cout << "[I] Face Creation Time: " <<scientific <<ittime_spent <<" " <<endl;

        //Inserting faces
        cout << "[I] Inserting Faces ..." <<endl;

        ittime_end = clock();


        int nf=0;
        int nfb=0;
        for (it=deffaceverts.begin(); it!=deffaceverts.end(); ++it)
        {
            vector <int> tempNodes;
            vector <_Vertex> facevertex;
            vector <Node>	 facenodes;

            tempNodes=mymap[nf];
            facenodes.push_back(this->node[facecells[nf][0]]);		//El otro nodo es ficticio, puedo ver que hago con esto
                                                //Lo necesito para la distancia PN
            for (int fn=0;fn<tempNodes.size();++fn) facevertex.push_back(this->vert[tempNodes[fn]]);

            bool boundaryface=false;

            //cout << "Face " << nf << "Cells "<<endl;
//            for (int fc=0;fc<facecells[nf].size();fc++) cout << facecells[nf][fc]<<" ";
//            cout <<endl;

            //Adding global face indexes for each cell
            //this->Cell(facecells[0]).Id_Face(nf,nf);

            // Looking for boundary faces
            if (facecells[nf].size()==1)
            {
                boundaryface=true;
                //this->Cell(facecells[1]).Id_Face(nfenc2,nf);    //Id_Face(nfenc2,Face Id);
                temp_boundfaces.push_back(nf);
                nfb++;

                //NEW
                vector <int> sortfacevertex(tempNodes);
                std::sort (sortfacevertex.begin(), sortfacevertex.end(), myobject);
                sortbfacemap.insert(std::pair< vector <int> , int > (sortfacevertex, nf) );
            }
            else
            {
                facenodes.push_back(this->node[facecells[nf][1]]);
            }
            //_FvFace f(FaceId, Face Vertex number, Face vertex (Vertex format), Face Nodes, )
            _FvFace f(nf,mymap[nf],facevertex,facenodes,boundaryface);

            this->face.push_back(f);
            nf++;
        }

        this->num_faces=deffaceverts.size();
        this->num_boundary_faces=nfb;

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        cout << "[I] Face Insertion Time: " <<scientific <<ittime_spent <<" " <<endl;

    }//If cells were initialized

}

const map<vector <int> , int > Fv_CC_Grid::FaceVertsMap()const
{
    map<vector <int> , int > ret;


    for (int f=0;f<this->Num_Faces();f++)
    {
        vector<int >faceverts=this->Face(f).Vert();
        std::sort (faceverts.begin(), faceverts.end(), myobject);
        ret.insert(std::pair< vector <int> , int > (faceverts, f) );
    }



    return ret;
}
////
////Adapted From freecfd
////void Fv_CC_Grid::Init_Faces()
////{
////
////	 Set face connectivity lists
////	int hexaFaces[6][4]= {
////		{0,3,2,1},
////		{4,5,6,7},
////		{1,2,6,5},
////		{0,4,7,3},
////		{1,5,4,0},
////		{2,3,7,6}
////	};
////	int prismFaces[5][4]= {
////		{0,2,1,0},
////		{3,4,5,0},
////		{0,3,5,2},
////		{1,2,5,4},
////		{0,1,4,3}
////	};
////	int pyraFaces[5][4]= {
////		{0,3,2,1},
////		{0,1,4,0},
////  		{1,2,4,0},
////  		{3,4,2,0},
////  		{0,4,3,0}
////	};
////	int tetraFaces[4][3]= {
////		{0,2,1},
////		{1,2,3},
////  		{0,3,2},
////  		{0,1,3}
////	};
////
////	// Search and construct faces
////	int faceCount=0;
////
////	// Time the face search
////	double timeRef, timeEnd;
////	if (Rank==0) timeRef=MPI_Wtime();
////	vector<int> unique_nodes;
////	set<int> repeated_node_cells;
////
////	 Loop through all the cells
////	for (int c=0;c<this->Num_Cells();++c) {
////		int degenerate_face_count=0;
////		 Loop through the faces of the current cell
////		for (int cf=0;cf<cell[c].Num_Faces();++cf) {
////			bool degenerate=false;
////			_FvFace tempFace;
////			int facenodeCount;  //Is vertex count indeed
////			int *tempNodes;
////			switch (this->cell[c].Num_Vertex()) {
////				case 4: // Tetrahedra
////					facenodeCount=3;
////					tempNodes= new int[3];
////					break;
////				case 5: // Pyramid
////					if (cf<1) {
////						facenodeCount=4;
////						tempNodes= new int[4];
////					} else {
////						facenodeCount=3;
////						tempNodes= new int[3];
////					}
////					break;
////				case 6: // Prism
////					if (cf<2) {
////						facenodeCount=3;
////						tempNodes= new int[3];
////					} else {
////						facenodeCount=4;
////						tempNodes= new int[4];
////					}
////					break;
////				case 8: // Brick
////					facenodeCount=4;
////					tempNodes= new int[4];
////					break;
////
////			}//Cell Node Count
////
////			 Face count is incremented everytime a new face is found
////			tempFace.id=faceCount;
////			// Assign current cell as the parent cell
////			tempFace.parent=c;
////			 Assign boundary type as internal by default, will be overwritten later
////			tempFace.bc=INTERNAL_FACE;
////			 Store the node local ids of the current face
////			for (int fn=0;fn<facenodeCount;++fn) {
////				switch (cell[c].Num_Vertex()) {
////					case 4: tempNodes[fn]=this->Cell(c).Vert(tetraFaces[cf][fn]); break; //Originally cellNode(c,tetraFaces[cf][fn]).id
////					case 5: tempNodes[fn]=this->Cell(c).Vert(pyraFaces[cf][fn]); break;
////					case 6: tempNodes[fn]=this->Cell(c).Vert(prismFaces[cf][fn]); break;
////					case 8: tempNodes[fn]=this->Cell(c).Vert(hexaFaces[cf][fn]); break;
////				}
////			}
////			 Check if there is a repeated node
////			unique_nodes.clear();
////			bool skip;
////			for (int fn=0;fn<facenodeCount;++fn) {
////				skip=false;
////				for (int i=0;i<fn;++i) {
////					if (tempNodes[fn]==tempNodes[i]) {
////						skip=true;
////						break;
////					}
////				}
////				if (!skip) unique_nodes.push_back(tempNodes[fn]);
////			}
////			if (unique_nodes.size()!=facenodeCount) {
////				repeated_node_cells.insert(c); // mark the owner cell (it has repeated nodes)
////				if (unique_nodes.size()==2) { // If a face only has two unique nodes, mark as degenerate
////					degenerate=true;
////					degenerate_face_count++;
////				}
////				facenodeCount=unique_nodes.size();
////				for (int fn=0;fn<facenodeCount;++fn) tempNodes[fn]=unique_nodes[fn];
////			}
////			 Find the neighbor cell
////			bool internal=false;
////			bool unique=true;
////			 Loop cells neighboring the first node of the current face
////			for (int nc=0;nc<node[tempNodes[0]].cells.size();++nc) {
////				 i is the neighbor cell index
////				int i=node[tempNodes[0]].cells[nc];
////				// If neighbor cell is not the current cell itself, and it has the same nodes as the face
////				if (i!=c && cell[i].HaveNodes(tempFace.nodeCount,tempNodes)) {
////					 If the neighbor cell index is smaller then the current cell index,
////					 it has already been processed so skip it
////					if (i>c) {
////						tempFace.neighbor=i;
////						internal=true;
////					} else {
////						unique=false;
////					}
////				}
////			}   //Node cell nc
////
////			if (unique && !degenerate) { // If a new face
////				 Insert the node list
////				for (int fn=0;fn<tempFace.nodeCount;++fn) tempFace.nodes.push_back(tempNodes[fn]);
////				if (!internal) { // If the face is either at inter-partition or boundary
////					tempFace.bc=UNASSIGNED_FACE; // yet
////					vector<int> face_matched_bcs;
////					int cell_matched_bc=-1;
////					bool match;
////					for (int nbc=0;nbc<raw.bocoNameMap.size();++nbc) { // For each boundary condition region
////						match=true;
////						for (int i=0;i<tempFace.nodeCount;++i) { // For each node of the current face
////							if (raw.bocoNodes[nbc].find(tempNodes[i])==raw.bocoNodes[nbc].end()) {
////								match=false;
////								break;
////							}
////						}
////						if (match) { // This means that all the face nodes are on the current bc node list
////							face_matched_bcs.push_back(nbc);
////						}
////						// There can be situations like back and front symmetry BC's in which
////						// face nodes will match more than one boundary condition
////						// Check if the owner cell has all its nodes on one of those bc's
////						// and eliminate those
////						if (cell_matched_bc==-1) {
////							match=true;
////							for (int i=0;i<cell[c].nodeCount;++i) {
////								if (raw.bocoNodes[nbc].find(cell[c].nodes[i])==raw.bocoNodes[nbc].end()) {
////									match=false;
////									break;
////								}
////							}
////							if (match) { // This means that all the cell nodes are on the current bc node list
////								cell_matched_bc=nbc;
////							}
////						}
////					}
////					if (face_matched_bcs.size()>1) {
////						for (int fbc=0;fbc<face_matched_bcs.size();++fbc) {
////							if(face_matched_bcs[fbc]!=cell_matched_bc) {
////								tempFace.bc=face_matched_bcs[fbc];
////								break;
////							}
////						}
////					} else if (face_matched_bcs.size()==1) {
////						tempFace.bc=face_matched_bcs[0];
////					}
////					// Some of these bc values will be overwritten later if the face is at a partition interface
////
////				} // if not internal
////				face.push_back(tempFace);
////				cell[c].faces.push_back(tempFace.id);
////				if (internal) cell[tempFace.neighbor].faces.push_back(tempFace.id);
////				++faceCount;
////			}   //New face
////			delete [] tempNodes;
////		} //for face cf
////		cell[c].faceCount-=degenerate_face_count;
////	} // for cells c
////
////	// Loop cells that has repeated nodes and fix the node list
////	set<int>::iterator sit;
////	vector<int> repeated_nodes;
////	for (sit=repeated_node_cells.begin();sit!=repeated_node_cells.end();sit++) {
////		// Find repeated nodes
////		repeated_nodes.clear();
////		for (int cn=0;cn<cell[(*sit)].nodeCount;++cn) {
////			for (int cn2=0;cn2<cn;++cn2) {
////				if (cell[(*sit)].nodes[cn]==cell[(*sit)].nodes[cn2]) repeated_nodes.push_back(cell[(*sit)].nodes[cn]);
////			}
////		}
////		if (cell[(*sit)].nodeCount==8 && repeated_nodes.size()==2) { // TODO Only Hexa to Penta mapping is handled for now
////			cell[(*sit)].nodes.clear();
////			// Loop triangular cell faces
////			int rindex=-1;
////			for (int cf=0;cf<cell[(*sit)].faceCount;++cf) {
////				if (cellFace((*sit),cf).nodeCount==3) {
////					// Loop the face nodes and see if the repeated node apears
////					int fn;
////					for (fn=0;fn<3;++fn) {
////						if (cellFace((*sit),cf).nodes[fn]==repeated_nodes[0]) { rindex=0; break; }
////						if (cellFace((*sit),cf).nodes[fn]==repeated_nodes[1]) { rindex=1; break; }
////					}
////					// Start from fn and fill the new cell node list
////					if (fn==0) {
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[0]);
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[1]);
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[2]);
////					} else if (fn==1) {
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[1]);
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[2]);
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[0]);
////					} else if (fn==2) {
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[2]);
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[0]);
////						cell[(*sit)].nodes.push_back(cellFace((*sit),cf).nodes[1]);
////					}
////
////				}
////
////			}
////			cell[(*sit)].nodeCount=6;
////		}
////	}
//// 	repeated_node_cells.clear();
////
////	for (int c=0; c<cellCount; ++c) {
////		if (Rank==0) if (cell[c].faceCount != cell[c].faces.size() ) cout << "no match" << "\t" << c << "\t" << cell[c].faceCount << "\t" << cell[c].faces.size() << "\t" << cell[c].nodeCount << endl;
////	}
////
////	if (Rank==0) {
////		timeEnd=MPI_Wtime();
////		cout << "[I Rank=" << Rank << "] Time spent on finding faces= " << timeEnd-timeRef << " sec" << endl;
////	}
////
////	for (int f=0;f<faceCount;++f) for (int n=0;n<face[f].nodes.size();++n) faceNode(f,n).faces.push_back(f);
////
////
////} // end Grid::create_faces

void Fv_CC_Grid::CalcCellVolumes()
{
	for (int c=0;c<Num_Cells();c++)
	{
		Scalar vol=0.;
		for (int cellface=0;cellface<this->cell[c].Num_Faces();cellface++)
		{
			_FvFace face = this->Face(Cell(c).Id_Face(cellface));
			//vol+=face.Af()&face.Center();
			//TO MODIFY
			vol+=CellFaceAf_Slow(c,cellface)&face.Center();
        }

		vol=vol/3.;
		this->Cell(c).Vp(vol);

	}

}

//TO MODIFY, MUST RETURN A REFERENCE
const GeomSurfaceField<Vec3D> Fv_CC_Grid::Sf() const
{
	GeomSurfaceField<Vec3D> ret(this->num_faces);
    ret.AssignGrid(this);

	for (int f = 0; f < this->num_faces; f++)
	{
	    //THIS MUST NOT RETURN THE NORMAIZED VALUE
//		ret.Val(f, this->face[f].Af().VecNorm());
		ret.Val(f, this->face[f].Af());
	}

	return ret;
}

const std::string  Fv_CC_Grid::Read_CGNS()
{

	    map<vector <int> , int >::iterator sortfacemapit;   //for boundary creation

        std::stringstream out;
        //raw data - vertices
        //base class function
        out << "[I] Reading Initial CGNS ...\n"<<endl;
        Read_InitialCGNS();

        //Read boundary cells - Assuming boundary cell entry
        vector<vector<int> > bpnode;

        int boundnode =0;

        cout << "[I] Creating boundary Elements ..."<<endl;
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
        cout << "[I] Boundary Elements Count: "<<boundelem<<endl;

        //vector<vector<int>> idbcellasoc(boundelem,vector<int>(2,-1)); //Id cells
        vector<int> idbcell(boundelem,-1);
        int bcell=0;

        cout << "[I] Creating Cells ..."<<endl;

        double ittime_spent;
        clock_t ittime_begin, ittime_end, ittime_temp;
        ittime_end = clock();

        set <int> bcellids;
        set <int>::iterator bcellidsit;
        for (int bp=0;bp<bpelem.size();bp++)
            for(int el=0;el<bpelem[bp].size();el++)
                bcellids.insert(bpelem[bp][el]-1);


//        vector<int> connect;
//        Cell_CC scell;
        for (int idcell =0 ; idcell<raw.cellConnIndex.size();idcell++)
        {
            int cellvertnum;
            if (idcell<(raw.cellConnIndex.size()-1))
                cellvertnum = raw.cellConnIndex [idcell+1] - raw.cellConnIndex [idcell];
            else cellvertnum = raw.cellConnectivity.size()- raw.cellConnIndex [idcell] ;
            bool found =false;

            vector<int> connect;
            for (int cv =0 ; cv<cellvertnum;cv++)
                connect.push_back(raw.cellConnectivity[ raw.cellConnIndex [idcell] + cv]);

            Cell_CC scell(idcell,connect);

            //Check if this is a boundary element
            //TO MODIFY, MUST CONSIDER TETRA CELLS
            //if (connect.size()<=4)   //All connectivity numbers are 3d
//            for (int bp=0;bp<bpelem.size();bp++)
//                for(int el=0;el<bpelem[bp].size();el++)
//                    if (idcell==(bpelem[bp][el]-1))     //Index of rawdata is counting from 1
//                    {
//                        vboundcell.push_back(scell);
//                        idbcell[bcell]=idcell;
//                        bcell++;
//                        found=true;
//                    }
            //New form

           bcellidsit=bcellids.find(idcell);
           if (bcellidsit!=bcellids.end())
            {
                vboundcell.push_back(scell);
                idbcell[bcell]=idcell;
                bcell++;
                found=true;
            }
            else
                this->cell.push_back(scell);


        }

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        cout << "[I] Cell Creation Time: " <<scientific <<ittime_spent <<" " <<endl;

        //vector<vector<int>> idbcellasoc(boundelem,vector<int>(2,-1));	//Id cells
        bcell=0;

        cout << "[I] Created " << this->cell.size() << " cells. "<<endl;
        //Updating cell number
        this->num_cells=cell.size();

        // Nodes
        cout << "[I] Creating Central Nodes ..."<<endl;
        CreateNodesFromCellVerts();

        this->inicie_nodes=true;
        this->inicie_cells=true;
        out << "[I] Assigning Faces ..."<<endl;
        //Iniciar_Caras();
        Init_Faces();
        cout << "[I] Assigning Neighbours ..."<<endl;
        AssignNeigboursCells();
        cout << "[I] Calculating Volumes ..."<<endl;
        CalcCellVolumes();
        //CreateNodesFromCellVerts();

        vector <Patch> vpatch;

        cout << "[I] Boundary Faces count: " <<temp_boundfaces.size()<<endl;
        std::vector<std::list <int> >bpfaces;

//        cout << "Boundary zone nodes" << bpnode[0][node]<<endl;
        out << "[I] Number of Patches: " << this->raw.bc_elem_list.size() <<endl;
        int nBocos=raw.bocoNameMap.size();  //Extrcted from freecfd
        out << "[I] Creating Patches ..."<<endl;

        cout << "[I] Creating Boundary Patches..." <<endl;

        ittime_end = clock();
        vector <int> faceverts;

        string pname;
        int totalboundfaces=temp_boundfaces.size();
        for (int bp=0;bp<this->raw.bc_elem_list.size();bp++)    //Look Through patch
        {
            pname=this->imported_patchnames[bp];
            list <int> temp;
            list <int> tempnew;

            out << "[I] Searching patch " << bp << ", named "<< pname<<", remaining boundary faces count: "<<temp_boundfaces.size()<<endl;

            int nfoundfaces=0;
            if (raw.bocoNodes[bp].size()>0)
            {
                cout << "[I] Patch defined via Nodes..."<<endl;
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
            else if (bpelem[bp].size()>0) //If bc are defined via 2D elements
            {
               //Looking through raw elements (faces in Grid)
               cout << "[I] Patch defined via Elems..."<<endl;
                for (int el=0;el<bpelem[bp].size();el++)
                {
                    //Adding element vertices
                    //for (int iv=0;iv<this->Cell(bpelem[bp][el]).Num_Vertex();iv++)
                    for (int iv=0;iv<vboundcell[bcell].Num_Vertex();iv++)   faceverts.push_back(vboundcell[bcell].Vert(iv));

                    vector <int> sortfaceverts(faceverts);
                    std::sort (sortfaceverts.begin(), sortfaceverts.end(), myobject);

                    //NEW FORM
                    //// /*COULD BE LIKE THIS:*/int faceid=sortbfacemap[sortfaceverts];

                    sortfacemapit=sortbfacemap.find(sortfaceverts);
                    int faceid=sortfacemapit->second;
                    if (sortfacemapit!=sortbfacemap.end()) //Found
                        temp.push_back(faceid);
                    faceverts.clear();
                    bcell++;

                }//End element
            }
            else
            out << "[I] WARNING: No Boundary defined. "<<endl;

            bpfaces.push_back(temp);
            cout << "[I] Created Patch "<<pname<<", Face Count: " <<temp.size()<<endl;
            //cout << "[I] Created new Patch "<<pname<<", Face Count: " <<tempnew.size()<<endl;

            Patch p(pname,bpfaces[bp]);
            vpatch.push_back(p);

        }

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        cout << "[I] Boundary Patches Creation Time: " <<scientific <<ittime_spent <<" " <<endl;

        this->SetFaceLocalCellNeighbours(); //New, 20150518

        Boundary bound(vpatch);
        this->AddBoundary(bound);
        cout << "[I] Creating internal faces..." <<endl;
        this->Create_IntFaces();

        this->boundary.AddGridPtr(*this);


        out << "[I] Mesh created ..." << endl;
        slog << out;
        return out.str();
	}


    //This function sets local ids of cell neighbours for each cell face
    void Fv_CC_Grid::SetFaceLocalCellNeighbours()
    {

        int ln[2]; ln[0]=1; ln[1]=0;
        int numnb=1;        //Boundary faces
        for (int f=0;f<this->Num_Faces();f++)
        {

            vector <int> temp;
            _FvFace face=this->Face(f);
            if (!this->Face(f).Boundaryface())    numnb=2;

            //cout << "Not boundary face"<<endl;
            //ap=-face.Norm_ad()/face.Dist_pn()*fi;
            //an=-ap;
            //nbr_eqn.push_back(VolField.Grid().Cell(c));
            //eqnsys.Eqn(face.Cell(0)).Coeffs(ap,an);

            //cout << "Look through neighbours"<<endl;
            for (int nb=0;nb<numnb;nb++)    //Face cells
            {
                //cout << "Neighbour" <<endl;
                int localnid;	//Id of local neigbour
                localnid=ln[nb];
                //p cell
                int pcellid=face.Cell(nb);
                int ncellid=face.Cell(localnid);
                //cout << "Eqn size" << eqnsys.Num_Eqn()<<endl;
                //cout << "Creating Eqn ... pcell id"<< pcellid<<endl;

                //eqnsys.Eqn(pcellid).Ap()+=ap;

                int neigbour_cell;
                int localneighbourid;	//local cell neigbour
                //Neighbours are
                //Find the global cell
                Cell_CC cell=this->Cell(face.Cell(nb));
                for (int localncell=0;localncell<cell.Num_Neighbours();localncell++)
                    if (cell.Neighbour(localncell)==ncellid)
                    {
                        localneighbourid=localncell;
                        temp.push_back(localncell);
                    }
                //eqnsys.Eqn(pcellid).An(localneighbourid)+=an;
            }//nb

            this->face_local_cell_neighbour.push_back(temp);

        }//Faces
    }//Fv_CC_Grid::SetFaceLocalCellNeighbour()


}// FluxSol
