#ifndef FLUENTMESH_H
#define FLUENTMESH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>

using namespace std;

class FluentMesh {
    FluentMesh() {}

public:
    FluentMesh(const string &file);
    vector< vector<double> > nodes;
    vector< vector<int> > faces;
    //vector< vector<int> > patches;
    vector< vector<int> > connectivity;

    ~FluentMesh() {}

};

#endif
