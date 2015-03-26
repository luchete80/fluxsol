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
#ifndef MATERIALS_H_
#define MATERIALS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

namespace FluxSol
{


class Materials             // Material class definition
{
public:
    double rho;            // Density
    double mu;             // Viscosity
    double k;              // Diffusivity
    string name;

//public:
    Materials(){};

    ~Materials(){};

    void SetDensity(const double &user_rho)
    {
        rho=user_rho;
    }

    void SetViscosity(const double &user_mu)
    {
        mu=user_mu;
    }

    void SetDiffusivity(const double &user_k)
    {
        k=user_k;
    }

    void SetName(const string &user_name)
    {
        name=user_name;
    }

    void Print()
    {
        cout << "Name: " << name << endl;
        cout << "Density = " << rho << endl;
        cout << "Viscosity = " << mu << endl;
        cout << "Diffusivity = " << k << endl;
        cout << "" << endl;
    }
};

vector<Materials> SetMaterials();

class MaterialsDB
{

protected:
    vector <Materials> material;

public:
    MaterialsDB(){}
    MaterialsDB(const string &file);
    const Materials& Material(const string &name)const;
    ~MaterialsDB(){}

};

}//FluxSol
#endif
