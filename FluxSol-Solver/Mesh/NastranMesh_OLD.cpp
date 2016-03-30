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

    map < int , int> pidloc;       //at left is id, right is position
    map <int, int >::iterator pidloc_it;
    vector < vector <int> > patchelem;  //Element (position per patch
    set<int> belem;                     //Element number per patch

    int found_pids=0;
    for (int idcell=0; idcell<mod.Elementos.size();idcell++)
    {
        if (mod.Elementos[idcell].Type()!="CQUAD4" && mod.Elementos[idcell].Type()!="CTRIA3")
        {Cell_CC scell(idcell, mod.Elementos[idcell].Conect_int());
        this->cell.push_back(scell);}
        else {  //Boundary element
            int pid=mod.Elementos[idcell].Pid();
            pidloc_it=pidloc.find(pid);
            if (pidloc_it!=pidloc.end()) //pid already created
            {
                cout << "Existent pid "<< pid << ", element" <<idcell<<endl;
                patchelem[pidloc_it->second].push_back(idcell);
            }
            else
            {
                cout << "Found new pid "<< pid << ", element" <<idcell<<endl;
                pidloc.insert(std::pair <int,int> (pid,found_pids));
                patchelem.push_back(vector<int>(1,idcell));
                found_pids++;
            }}
            //belem.insert(idcell);
    }
    this->num_cells=cell.size();
    //cout << "[I] Created " << mod.Elementos.size() - belem.size() << " internal cells." << endl;

    cout << "[I] Creating Central Nodes..." << endl;
    CreateNodesFromCellVerts();

    this->inicie_nodes=true;
    this->inicie_cells=true;

    cout << "[I] Assigning Faces..." << endl;
    Init_Faces();

    cout << "[I] Assigning Neighbours..." << endl;
    AssignNeigboursCells();

    cout << "[I] Calculating Volumes..." << endl;
    CalcCellVolumes();

    cout << "[I] Creating Patches..." << endl;

    myclass myobject;

    //TO MODIFY: THIS MUST BE COMBINED WITH FvGrid::ReadCGNS AT FVGRID.CPP
    map<vector <int> , int >::iterator sortfacemapit;
    vector <Patch> vpatch;
    std::vector<std::list <int> >bpfaces;
    vector <int> faceverts;
    //SAME OF FvGrid::ReadCGNS
    //Looking through raw elements (faces in Grid)
    //Looks faces in sortbfacemap defined in Init_Faces

    for (int bp=0;bp<found_pids;bp++)
    {
        list <int> temp;
        cout << "[I] Patch defined via Elems..."<<endl;
        for (int el=0;el<patchelem[bp].size();el++)
        {
            //Adding element vertices
            //for (int iv=0;iv<this->Cell(bpelem[bp][el]).Num_Vertex();iv++)
            //for (int iv=0;iv<vboundcell[bcell].Num_Vertex();iv++)   faceverts.push_back(vboundcell[bcell].Vert(iv));
            int bcell=patchelem[bp][el];
            faceverts=mod.Elementos[bcell].Conect_int();
            //for (int iv=0;iv<mod.Elementos[bcell].NumNodes();iv++)   faceverts.push_back(vboundcell[bcell].Vert(iv));

            vector <int> sortfaceverts(faceverts);
            std::sort (sortfaceverts.begin(), sortfaceverts.end(), myobject);

            //NEW FORM
            //// /*COULD BE LIKE THIS:*/int faceid=sortbfacemap[sortfaceverts];

            //or sortfacemapit=sortbfacemap.find(sortfaceverts);
            sortfacemapit=sortbfacemap.find(sortfaceverts);
            int faceid=sortfacemapit->second;
            if (sortfacemapit!=sortbfacemap.end()) //Found
                temp.push_back(faceid);
            faceverts.clear();
            bcell++;

        }//End element

        bpfaces.push_back(temp);
        stringstream convert;
        int pid=mod.Elementos[patchelem[bp][0]].Pid();
        convert <<pid;
        cout << "[I] Created Patch id"<<convert.str() <<", Face Count: " <<temp.size()<<endl;
        //cout << "[I] Created new Patch "<<pname<<", Face Count: " <<tempnew.size()<<endl;

        Patch p(convert.str(),bpfaces[bp]);
        vpatch.push_back(p);

    }//End patch

//    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
//    cout << "[I] Boundary Patches Creation Time: " <<scientific <<ittime_spent <<" " <<endl;

    this->SetFaceLocalCellNeighbours(); //New, 20150518

    Boundary bound(vpatch);
    this->AddBoundary(bound);
    cout << "[I] Creating internal faces..." <<endl;
    this->Create_IntFaces();
    this->boundary.AddGridPtr(*this);

    cout << "[I] Mesh created ..." << endl;
}
