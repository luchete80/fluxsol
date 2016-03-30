#ifndef FLUENTMESH_H
#define FLUENTMESH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "FvGrid.h"
#include "Utils.h"

using namespace std;

namespace FluxSol {

class FluentMesh:public  Fv_CC_Grid {
    FluentMesh() {}

private:
    vector< vector<double> > nodes;
    vector< vector<int> > faces;
    vector<string> patch_name;
    vector< vector<int> > connectivity;
    vector<Patch> vpatch;

public:
    FluentMesh(const string &file);
    ~FluentMesh() {}

    //void init_patches();

};
}

#endif
