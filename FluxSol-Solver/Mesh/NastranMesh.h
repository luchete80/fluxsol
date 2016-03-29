#ifndef NASTRANMESH_H
#define NASTRANMESH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "FvGrid.h"

using namespace std;

namespace FluxSol {

class NastranMesh:public  Fv_CC_Grid {
    NastranMesh() {}

private:
    vector< vector<double> > nodes;
    vector< vector<int> > faces;
    vector<string> patch_name;
    vector< vector<int> > connectivity;
    vector<Patch> vpatch;

public:
    NastranMesh(const string &file);
    ~NastranMesh() {}

    //void init_patches();

};
}

#endif
