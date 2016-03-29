#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <stdlib.h>

#include "NastranMesh.h"
#include "FvGrid.h"

#include "Utils.h"
#include "Nastran.h"
#include "Modelo.h"

using namespace std;

using namespace FluxSol;


NastranMesh::NastranMesh(const string &file) {
    vector<string> data;
    fstream mesh_input(file.c_str());
    string line;
    int patch_qty;

    cout << "\nReading Fluent Mesh..." << endl;

    while (!mesh_input.eof()) {
        getline(mesh_input, line);
        data.push_back(line);
    }

    FluxSol::Nastran::Modelo mod(file);
    cout << "Nastran model created."<<endl;
    cout << "[I] Creating vertices ..."<<endl;
    this->vert.assign(mod.NumNodes(),_Vertex());
    //for (int nv=0;nv<mod.NumNodes();nv++)
    //    this->vert[nv]=_Vertex(0,0,);
    cout << "[I] " <<mod.NumNodes() << " vertices created. "<<endl;


//    CreateNodesFromCellVerts();


//    nodes=process_nodes(data);
//    connectivity=process_cells(data);
//
//    cout << "[I] Creating cells..." << endl;
//    for (int idcell=0; idcell<connectivity.size(); idcell++) {
//        Cell_CC scell(idcell, connectivity[idcell]);
//        this->cell.push_back(scell);
//        }
//    this->num_cells=cell.size();
//
//    // Paso del vector nodes al Vertex<Vec3D>
//    for (int i=0; i<nodes.size(); i++) {
//        _Vertex temp(0.);
//        for (int j=0; j<3; j++) {
//            temp[j]=nodes[i][j];
//            this->vert.push_back(temp);
//        }
//    }
//
//    // se crean los nodos centrados en el cuerpo de cada celda
//    cout << "[I] Creating Central Nodes..." << endl;
//    CreateNodesFromCellVerts();
//
//    this->inicie_nodes=true;
//    this->inicie_cells=true;
//
//    cout << "[I] Assigning Faces..." << endl;
//    Iniciar_Caras();
//
//    cout << "[I] Assigning Neighbours..." << endl;
//    AssignNeigboursCells();
//
//    cout << "[I] Calculating Volumes..." << endl;
//    CalcCellVolumes();
//
//    cout << "[I] Creating Patches..." << endl;
//    patch_name=patches_names(data);
//    vpatch=create_patches(data, patch_name);
//



}
