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

#ifndef _INPUT_H_
#define _INPUT_H_

#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <string>
#include <iostream>
#include <fstream>


#include "BoundaryCond.h"
#include "Utils.h"
#include "Vec3d.h"
#include "Field.h"
#include "Scalar.h"
#include "SistEcuac.h"
#include "FvImp.h"
#include "FvField.h"
#include "Laplacian.h"
#include "Laplacian.h"

#include "UDO.h"

#include "FvGrid.h"
//#include <mpi.h>

//Based on Free-CFD Input source Code

using namespace std;

namespace FluxSol
{

template <typename T> class entry {
public:
	T value;
	T default_value;
	bool is_found;
	bool is_required;
	operator T() { return value; }
	bool operator== (const T &right) { return (value==right); }
	bool operator!= (const T &right) { return (value!=right); }
};


template <typename T>
ostream &operator<< (ostream &output,const entry<T> &right) {
	output << right.value ;
	return output;
}

class InputBaseContainer {
public:
	string rawData;
	string name,parentName;
	int index,parentIndex;
	bool is_required, is_found;
	map<string,entry<int> > ints;
	map<string,entry<double> > doubles;
	map<string,entry<string> > strings;
	map<string,entry<Vec3D> > Vec3Ds;
	map<string,entry<vector<int> > > intLists;
	map<string,entry<vector<double> > > doubleLists;
	map<string,entry<vector<string> > > stringLists;


	void register_int(string varName,bool is_required=true,int default_value=0) {
		entry<int>  temp; temp.is_required=is_required; temp.default_value=default_value; ints.insert(pair<string,entry<int> > (varName,temp));
	}
	void register_double(string varName,bool is_required=true,double default_value=0) {
		entry<double> temp; temp.is_required=is_required; temp.default_value=default_value; doubles.insert(pair<string,entry<double> > (varName,temp));
	}
	void register_string(string varName,bool is_required=true,string default_value="") {
		entry<string> temp; temp.is_required=is_required; temp.default_value=default_value; strings.insert(pair<string,entry<string> > (varName,temp));
	}
	void register_Vec3D(string varName,bool is_required=true,Vec3D default_value=0) {
		entry<Vec3D> temp;  temp.is_required=is_required;
		default_value=default_value[0];
		temp.default_value=default_value;
		Vec3Ds.insert(pair<string,entry<Vec3D> > (varName,temp));
	}
	void register_intList(string varName,bool is_required=true,int default_value=1) {
		entry<vector<int> > temp;  temp.is_required=is_required;
		temp.default_value.push_back(default_value);
		intLists.insert(pair<string,entry<vector<int> > > (varName,temp));
	}
	void register_doubleList(string varName,bool is_required=true,double default_value=0) {
		entry<vector<double> > temp;  temp.is_required=is_required;
		temp.default_value.push_back(default_value);
		doubleLists.insert(pair<string,entry<vector<double> > > (varName,temp));
	}
	void register_stringList(string varName,bool is_required=true,string default_value=" ") {
		entry<vector<string> > temp;  temp.is_required=is_required;
		temp.default_value.push_back(default_value);
		stringLists.insert(pair<string,entry<vector<string> > > (varName,temp));
	}

	void readEntries(void);
	void entry_not_found(string varName) {
		int Rank;
	//	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
		string path="";
		if (name!="") path=name+" -> ";
		if (parentName!="") path=parentName+" -> ";
		if (Rank==0) cerr << "[E] Required input entry " << path  <<  varName << " could not be found!!" << endl;
//		exit(1);
	}

	entry<int> get_int(string varName) {return ints[varName];}
	entry<double> get_double(string varName) {return doubles[varName];}
	entry<string> get_string(string varName) {return strings[varName];}
	entry<Vec3D> get_Vec3D(string varName) {return Vec3Ds[varName];}
	vector<int> get_intList(string varName) {return intLists[varName].value;}
	vector<double> get_doubleList(string varName) {return doubleLists[varName].value;}
	vector<string> get_stringList(string varName) {return stringLists[varName].value;}
};


class Subsection: public InputBaseContainer {
	public:
		bool numbered;
		int count;
};

class Section: public InputBaseContainer {
public:
	map<string,Subsection> subsections;
	map<string,vector<Subsection> > numberedSubsections;
	bool numbered;
	int count;
	void registerSubsection(string subsectionName, bool is_numbered=false, bool is_required=true) {
		if (is_numbered) {
			Subsection temp; temp.numbered=true; temp.count=0; temp.is_required=is_required;
			temp.name=subsectionName; temp.parentName=name;
			vector<Subsection> temp2;
			temp2.push_back(temp);
			numberedSubsections.insert(pair<string,vector<Subsection> > (subsectionName,temp2));
		} else {
			Subsection temp; temp.numbered=false; temp.is_required=is_required;
			temp.name=subsectionName; temp.parentName=name;
			subsections.insert(pair<string,Subsection> (subsectionName,temp));
		}
	}

	Subsection &subsection(string subsectionName, int number=-1) {
		if (number>=0) { return numberedSubsections[subsectionName][number]; }
		else { return subsections[subsectionName]; }
	}
};


class InputFile: public InputBaseContainer {

protected:
    	Fv_CC_Grid *GridPtr;

    	_CC_Fv_Field <Vec3D> * ufield;
    	_CC_Fv_Field <Scalar> * pfield;
    	int test;


public:
    UDOLib udolib;
	string fileName;
	map<string,Section> sections;
	map<string,vector<Section> > numberedSections;

	AssignGridPtr(const Fv_CC_Grid& gp){this->GridPtr=&gp;}

	InputFile(void);
	InputFile(string filename);
	void setFile(string);
	void read(string sectionName, int number=-1); // Reads all components of section if numbered
	void readSection(string sectionName, int number=-1); // Reads a single section
	void readSubsection(Subsection &sub);
	string ReadEqnType();
	EqnSystem <Scalar> ReadEqnSys(const Fv_CC_Grid &mesh);

	void registerSection(string sectionName, bool is_numbered=false, bool is_required=true) {
		if (is_numbered) {
			Section temp; temp.numbered=true; temp.count=0; temp.is_required=is_required;
			temp.name=sectionName;
			vector<Section> temp2;
			temp2.push_back(temp);
			numberedSections.insert(pair<string,vector<Section> > (sectionName,temp2));
		} else {
			Section temp; temp.is_required=is_required;  temp.name=sectionName;
			sections.insert(pair<string,Section> (sectionName,temp));
		}
	}

	Section &section(string sectionName, int number=-1) {
		if (number>=0) { return numberedSections[sectionName][number]; }
		else { return sections[sectionName]; }
	}

    void AssignFieldPtrs(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar> *p)
    {
        this->ufield=u;this->pfield=p;
    }
	void stripComments(string &data);
	void strip_white_spaces(string &data);
	void read_inputs(void);

	template <typename T>
	const _BoundaryField<T> BoundaryField(const string &name)const;

	template <typename T>
	_BoundaryField<T> UBoundaryField();

    _CC_Fv_Field <Vec3D> UField();
    _CC_Fv_Field <Scalar> pField();

    std::vector <BoundaryCond *> ReadBCs();

};

bool extract_in_between(string &data, string begin, string end, string &result,bool check_char_before=false, string acceptList="");
int number_of_occurances(string haystack, string needle);
void StringExplode(string str, string separator, vector<string>* results);

//
//ReadGridBoundary()
//{
//    vector <Patch> vpatch;
//
//    std::vector<std::list <int> >bpfaces;
//    bcell=0;
//    cout << "[I] Number of Patches: " << this->raw.bc_elem_list.size() <<endl;
//    cout << "[I] Creating Patches ..."<<endl;
//    for (int bp=0;bp<this->raw.bc_elem_list.size();bp++)
//    {
//        list <int> temp;
//        //Looking through raw elements (faces in Grid)
//        for (int el=0;el<bpelem[bp].size();el++)
//        {
//            vector<int> faceverts;
//            //Adding element vertices
//            //for (int iv=0;iv<this->Cell(bpelem[bp][el]).Num_Vertex();iv++)
//            for (int iv=0;iv<vboundcell[bcell].Num_Vertex();iv++)
//                faceverts.push_back(vboundcell[bcell].Vert(iv));
//
//            for (int idf=0;idf<this->Num_Faces();idf++)
//            {
//                bool enc=FindAllVals(faceverts,this->Face(idf).Vert());
//                if (enc)
//                {
//                    //Add face idf to Boundary patch - Agrego idf al Patch
//                    temp.push_back(idf);
//                }
//            }
//            bcell++;
//        }//End element
//        bpfaces.push_back(temp);
//        Patch p(bpfaces[bp]);
//        vpatch.push_back(p);
//    }//boundary patch
//
//    Boundary bound(vpatch);
//    this->AddBoundary(bound);
//
//}

}//End of FluxSol
#endif
