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

    cout << "\nReading Nastran Mesh..." << endl;

    while (!mesh_input.eof()) {
        getline(mesh_input, line);
        data.push_back(line);
    }

    FluxSol::Nastran::Modelo mod(file);
    cout << "Nastran model created."<<endl;
    cout << "[I] Creating vertices ..."<<endl;
    this->vert.assign(mod.NumNodes(),_Vertex());
    for (int nv=0;nv<mod.NumNodes();nv++)
        this->vert[nv]=_Vertex(mod.Nodos[nv].VerId_Nastran(),
                                mod.Nodos[nv].Sc_int(),
                                mod.Nodos[nv].Coords());

    cout << "[I] " <<mod.NumNodes() << " vertices created. "<<endl;


//    nodes=process_nodes(data);
//    connectivity=process_cells(data);
//
    cout << "[I] Creating cells..." << endl;
    //this->cell.assign(mod.Elementos.size(),scell);
    cout << "[I] Cell number "<<mod.Elementos.size()<<endl;
    vector <int> v=mod.Elementos[140].Conect_int();
    cout << "Element vector conect size: "<<v.size()<<endl;
//    cout << "Element type: "<<mod.Elementos[0].Type()<<endl;

    cout <<endl;

    for (int idcell=0; idcell<mod.Elementos.size();idcell++)
    {
        Cell_CC scell(idcell, mod.Elementos[idcell].Conect_int());
        this->cell.push_back(scell);
    }
    this->num_cells=cell.size();

    cout << "[I] Creating Central Nodes..." << endl;
    CreateNodesFromCellVerts();
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
