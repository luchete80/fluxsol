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
#include "Elementos.h"

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

    this->inicie_nodes=true;
    this->inicie_cells=true;

    cout << "[I] Assigning Faces..." << endl;
    Iniciar_Caras();

    cout << "[I] Assigning Neighbours..." << endl;
    AssignNeigboursCells();

    cout << "[I] Calculating Volumes..." << endl;
    CalcCellVolumes();

    map<vector <int> , int > facevertsmap=FaceVertsMap();

    cout << "[I] Creating Patches..." << endl;
//    patch_name=patches_names(data);
//    vpatch=create_patches(data, patch_name);

    set <int> util_pids;
    vector <set <int> > pid_els;    //Element per each pid
    vector<int> tempNodesSort;
    map<vector <int> , int > FaceVerts;
    myclass myobject;
    list<int> lista;
    vector<Elemento> vel=mod.Elementos;

    for (int e=0;e<mod.Elementos.size();e++)
    {
        if (!mod.Elementos[e].isBoundElem())
        {
            util_pids.insert(mod.Elementos[e].Pid());
            int faceid=FaceVerts[tempNodesSort];
            lista.push_back(faceid);
        }


    }

    for (int pid=0;pid<util_pids.size();pid++)
    {
        //int e=;
        tempNodesSort=mod.Elementos[e].Conect_int();
        sort (tempNodesSort.begin(), tempNodesSort.end(), myobject);

    }

    //            Patch p(pname[patch_index], lista);
    //            output.push_back(p);
    //            tempNodesSort.clear();



//    vector <Elemento> vel=mod.;
//
//vector<Patch> create_patches(const vector<string> &data, const vector<string> &pname) {
//    vector<Patch> output;
//    vector<int> tempNodesSort;
//    map<vector <int> , int > FaceVerts;
//    string patch_identifier="(13";
//    string interior="(1";
//    string fluid="(2";
//    string comment="(0";
//    string buff, str;
//    stringstream ss;
//    vector<string> vline, fline;
//    int patch_index=0;
//
//    int i=0;
//    while (i<data.size()) {
//        ss << data[i];
//
//        while (ss >> buff) {
//            vline.push_back(buff);
//        }
//        ss << "";
//        ss.clear();
//
//        if (vline[0]==patch_identifier && vline[1]!=interior && vline[1]!=fluid && vline[1]!=comment) {
//            ss << hex << vline[2];
//            int x;
//            ss >> x;
//            ss << "";
//            ss.clear();
//            ss << hex << vline[3];
//            int y;
//            ss >> y;
//            ss << "";
//            ss.clear();
//            int faces_qty=y-x+1;
//
//            list<int> lista;
//            for (int j=1; j<=faces_qty; j++) {
//                ss << data[i+j];
//
//                while (ss >> buff) {
//                    fline.push_back(buff);
//                }
//                ss << "";
//                ss.clear();
//
//                for (int k=0; k<4; k++) {
//                    ss << hex << fline[k];
//                    int z;
//                    ss >> z;
//                    ss << "";
//                    ss.clear();
//                    tempNodesSort.push_back(z);
//                }
//
//                sort (tempNodesSort.begin(), tempNodesSort.end(), myobject);
//                int faceid=FaceVerts[tempNodesSort];
//                lista.push_back(faceid);
//
//                fline.clear();
//
//            }
//
//            Patch p(pname[patch_index], lista);
//            output.push_back(p);
//            tempNodesSort.clear();
//
//            i=i+j;
//            patch_index++;
//        }
//
//        vline.clear();
//        str.clear();
//        i++;
//    }
//
//    return output;
//}


}
