#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>

#include "FluentMesh.h"

using namespace std;

vector< vector<double> > process_nodes(const vector<string> &data);
vector< vector<int> > process_cells(const vector<string> &data);
vector< vector<int> > cells_connectivity(const vector< vector<int> > &raw_data, const int &cellqty);
void process_patches(const vector<string> &data);

FluentMesh::FluentMesh(const string &file) {
    vector<string> data;
    fstream mesh_input(file.c_str());
    string line;

    cout << "\nReading Fluent Mesh..." << endl;

    while (!mesh_input.eof()) {
        getline(mesh_input, line);
        data.push_back(line);
    }

    nodes=process_nodes(data);
    connectivity=process_cells(data);
    //patches=process_patches(data);

}

vector< vector<double> > process_nodes(const vector<string> &data) {
    string identifier="(10";
    string one="(0";
    string buff;
    vector< vector<double> > out;
    vector<string> vline, vcoord;
    stringstream ss;
    int nodeqty, y=0;

    cout << "\nProcessing Nodes..." << endl;

    int i=0;
    while (i<data.size()) {
        ss << data[i];

        while (ss >> buff) {
            vline.push_back(buff);
        }

        ss << "";
        ss.clear();

        if (vline[0]==identifier && vline[1]!=one) {
            ss << hex << vline[3];
            int x;
            ss >> x;
            ss << "";
            ss.clear();

            for (int j=1; j<=(x-y); j++) {
                ss << data[i+j];

                while (ss >> buff) {
                    vcoord.push_back(buff);
                }

                vector<double> coords (3);
                for (int k=0; k<3; k++) {
                    stringstream sscoord(vcoord[k]);
                    sscoord >> coords[k];
                    sscoord << "";
                    sscoord.clear();
                }

                out.push_back(coords);
                ss << "";
                ss.clear();
                vcoord.clear();

            }

            i=i+x;
            y=out.size();

        }

        ss << "";
        ss.clear();
        vline.clear();
        i++;
    }

    nodeqty=out.size();
    cout << "\nNodes: " << nodeqty << endl;

    /*for (int i=0; i<out.size(); i++) {
        for (int j=0; j<out[0].size(); j++) {
            cout << out[i][j] << " ";
        }
        cout << endl;
    }*/

    return out;

}

vector< vector<int> > process_cells(const vector<string> &data) {
    string face_identifier="(13";
    string cell_identifier="(12";
    string one="(0";
    string buff;
    stringstream ss;
    vector<string> vline, vconnect;
    vector< vector<int> > out_connect, raw_connect;
    int cellqty, faceqty, patchqty;

    cout << "\nProcessing Cells..." << endl;

    int i=0;
    while (i<data.size()) {
        ss << data[i];

        while (ss >> buff) {
            vline.push_back(buff);
        }

        ss << "";
        ss.clear();

        if (vline[0]==cell_identifier && vline[1]==one) {
            ss << hex << vline[3];
            ss >> cellqty;
            ss << "";
            ss.clear();
        }
        else if (vline[0]==face_identifier && vline[1]==one) {
            ss << hex << vline[3];
            ss >> faceqty;
            ss << "";
            ss.clear();
        }
        else if (vline[0]==face_identifier && vline[1]!=one) {
            ss << hex << vline[2];
            int start_id;
            ss >> start_id;
            ss << "";
            ss.clear();
            ss << hex << vline[3];
            int end_id;
            ss >> end_id;
            ss << "";
            ss.clear();

            for (int j=1; j<(end_id-start_id+2); j++) {
                ss << data[i+j];

                while (ss >> buff) {
                    vconnect.push_back(buff);
                }

                ss << "";
                ss.clear();

                vector<int> temp_connect (vconnect.size());
                for (int k=0; k<vconnect.size(); k++) {
                    ss << hex << vconnect[k];
                    ss >> temp_connect[k];
                    ss << "";
                    ss.clear();
                }

                raw_connect.push_back(temp_connect);
                ss << "";
                ss.clear();
                vconnect.clear();
            }

            i=i+end_id-start_id+2;

        }

        vline.clear();
        i++;

    }

    cout << "\nCells: " << cellqty << endl;

    out_connect=cells_connectivity(raw_connect, cellqty);

    return out_connect;

}

vector< vector<int> > cells_connectivity(const vector< vector<int> > &raw_connect, const int &cellqty) {
    vector< vector<int> > connectivity;

    for (int i=0; i<cellqty; i++) {
        int face_one_id, face_two_id;
        vector<int> face_one (4);
        vector<int> face_two (4);
        vector<int> face_two_prime (4);
        vector<int> face_index, temp_connect;

        int j=0;
        while (j<raw_connect.size()) {

            if (raw_connect[j][4]==i+1) {

                for (int k=0; k<4; k++) {
                    face_one[k]=raw_connect[j][k];
                }
                face_one_id=j;
                break;

            }
            else if (raw_connect[j][5]==i+1) {

                for (int k=0; k<4; k++) {
                    face_one[k]=raw_connect[j][3-k];
                }
                face_one_id=j;
                break;

            }

            j++;
        }

        int m=0;
        while (m<raw_connect.size()) {

            if (raw_connect[m][4]==i+1 || raw_connect[m][5]==i+1) {

                if (raw_connect[m][0]!=face_one[0] && raw_connect[m][0]!=face_one[1] &&
                    raw_connect[m][0]!=face_one[2] && raw_connect[m][0]!=face_one[3] &&
                    raw_connect[m][1]!=face_one[0] && raw_connect[m][1]!=face_one[1] &&
                    raw_connect[m][1]!=face_one[2] && raw_connect[m][1]!=face_one[3] &&
                    raw_connect[m][2]!=face_one[0] && raw_connect[m][2]!=face_one[1] &&
                    raw_connect[m][2]!=face_one[2] && raw_connect[m][2]!=face_one[3] &&
                    raw_connect[m][3]!=face_one[0] && raw_connect[m][3]!=face_one[1] &&
                    raw_connect[m][3]!=face_one[2] && raw_connect[m][3]!=face_one[3]) {

                    if (raw_connect[m][4]==i+1) {

                        for (int k=0; k<4; k++) {
                            face_two_prime[k]=raw_connect[m][k];
                        }
                        face_two_id=m;

                    }
                    else if (raw_connect[m][5]==i+1) {

                        for (int k=0; k<4; k++) {
                            face_two_prime[k]=raw_connect[m][3-k];
                        }
                        face_two_id=m;

                    }

                }

                if (m!=face_one_id && m!=face_two_id) {
                    face_index.push_back(m);
                }

            }

            m++;
        }

        int n1_one=face_one[0];
        int k_index, f_index, n1_two_index;
        for (int j=0; j<face_index.size(); j++) {
            for (int k=0; k<4; k++) {

                if (raw_connect[face_index[j]][k]==n1_one) {
                    int up, down;

                    if (k==0) {
                        up=raw_connect[face_index[j]][3];
                        down=raw_connect[face_index[j]][1];
                        k_index=k;
                        f_index=face_index[j];
                    }
                    else if (k==1) {
                        up=raw_connect[face_index[j]][0];
                        down=raw_connect[face_index[j]][2];
                        k_index=k;
                        f_index=face_index[j];
                    }
                    else if (k==2) {
                        up=raw_connect[face_index[j]][1];
                        down=raw_connect[face_index[j]][3];
                        k_index=k;
                        f_index=face_index[j];
                    }
                    else if (k==3) {
                        up=raw_connect[face_index[j]][2];
                        down=raw_connect[face_index[j]][0];
                        k_index=k;
                        f_index=face_index[j];
                    }

                    for (int m=0; m<4; m++) {

                        if (up==face_two_prime[m] || down==face_two_prime[m]) {
                            n1_two_index=m;
                            break;
                        }
                    }
                    break;
                }

            }
        }

        if (n1_two_index==0) {
            face_two=face_two_prime;
        }
        else {

            for (int k=n1_two_index; k<4; k++) {
                face_two[k-n1_two_index]=face_two_prime[k];
            }

            for (int k=0; k<n1_two_index; k++) {
                face_two[k+(4-n1_two_index)]=face_two_prime[k];
            }
        }

        temp_connect=face_one;
        for (int k=0; k<4; k++) {
            temp_connect.push_back(face_two[k]);
        }

        connectivity.push_back(temp_connect);

    }

    /*for (int r=0; r<connectivity.size(); r++) {
        for (int s=0; s<connectivity[0].size(); s++) {
            cout << connectivity[r][s] << " ";
        }
        cout << endl;
    }*/

    return connectivity;

}
