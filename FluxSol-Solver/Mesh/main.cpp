#include <iostream>
#include "FluentMesh.h"

using namespace std;

int main()
{
    FluentMesh malla("test.msh");

    //cout << "Nodes: " << malla.nodes.size() << endl;

    //cout << "Cells: " << malla.connectivity.size() << endl;

    cout << "Faces: " << malla.faces.size() << endl;

    return 0;
}
