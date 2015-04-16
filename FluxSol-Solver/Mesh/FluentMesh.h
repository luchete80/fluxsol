#ifndef FLUENTMESH_H
#define FLUENTMESH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "FvGrid.h"

using namespace std;

namespace FluxSol {

class FluentMesh:public  Fv_CC_Grid {
    FluentMesh() {}

    vector< vector<double> > nodes;
    vector< vector<int> > faces;
    //vector< vector<int> > patches;
    vector< vector<int> > connectivity;

public:
    FluentMesh(const string &file);
    ~FluentMesh() {}

};
}

#endif
