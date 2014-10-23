/************************************************************************

	Copyright 2012-2014 Luciano Buglioni - Pablo Zitelli

	Contacts:
        Luciano Buglioni: luciano.buglioni@gmail.com
        Pablo Zitelli:    zitelli.pablo@gmail.com
	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    FluxSol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Materials.h"

using namespace std;

vector<Materials> SetMaterials()
{
    vector<Materials> material;                 // Vector of materials
    vector<string> vline;
    fstream in_stream("DataBase.dat");          // File containing materials definitions
    string line,buff,name,Density,Viscosity,Diffusivity,Name;
    stringstream ss;
    int mat_count=0;

    while (!in_stream.eof())
        {
        getline(in_stream, line);
        ss << line;
        while (ss >> buff)
            vline.push_back(buff);

        if (vline[0]==Name)
            {
                name=vline[1];
                material.push_back(Materials());
                material[mat_count].SetName(name);
                mat_count++;
            }

        while (vline[0]!=Name)
            {
                if (vline[0]==Density)
                    {
                        stringstream ssd(vline[1]);
                        double d;
                        ssd >> d;
                        material[mat_count-1].SetDensity(d);
                        ssd << "";
                        ssd.clear();
                    }
                else if (vline[0]==Viscosity)
                {
                    stringstream ssv(vline[1]);
                    double v;
                    ssv >> v;
                    material[mat_count-1].SetViscosity(v);
                    ssv << "";
                    ssv.clear();
                }
                else if (vline[0]==Diffusivity)
                {
                    stringstream ssD(vline[1]);
                    double D;
                    ssD >> D;
                    material[mat_count-1].SetDiffusivity(D);
                    ssD << "";
                    ssD.clear();
                }
            }
        ss << "";
        ss.clear();
        }

        for (int i=0; i<mat_count-1; i++)
        {
            material[i].Print();
        }

    return material;
}
